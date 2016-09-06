#include "StdAfx.h"
#include "HttpUtil.h"

#include <winsock2.h>
#include <wininet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")

#pragma warning(disable: 4996)

namespace
{
    HINTERNET g_InterHandles[1024] = {};

    void AddIntHandle(HINTERNET handle)
    {
        if(handle == NULL)
            return;
        for(int i=0; i<_countof(g_InterHandles); ++ i)
        {
            if(g_InterHandles[i] != NULL)
                continue;
            g_InterHandles[i] = handle;
            break;
        }
    }

    void RemoveIntHandle(HINTERNET handle)
    {
        if(handle == NULL)
            return;
        for(int i=0; i<_countof(g_InterHandles); ++ i)
        {
            if(g_InterHandles[i] != handle)
                continue;
            g_InterHandles[i] = NULL;
            break;
        }
    }
}

namespace HttpUtil
{
    namespace
    {
        HINTERNET g_hIntOpen = NULL;
    }

    HttpResult::HttpResult()
    {
        statusCode = 0;
        buffer = NULL;
        bufferLength = 0;
        text = _T("");
    }

    HttpResult::~HttpResult()
    {
        Reset();
    }

    void HttpResult::Reset()
    {
        if(buffer)
            free(buffer);
        statusCode = 0;
        buffer = NULL;
        bufferLength = 0;
        text = _T("");
    }

    void HttpResult::NeedHeader(LPCTSTR headerName)
    {
        headerMap_.insert(std::make_pair(headerName, _T("")));
    }

    NString HttpResult::GetHeader(LPCTSTR headerName) const
    {
        HeaderMap::const_iterator ite = headerMap_.find(headerName);
        if(ite == headerMap_.end())
            return _T("");
        return ite->second;
    }

    void InitHttpUtil()
    {
        WSAData data = {0};
        ::WSAStartup(MAKEWORD(2, 2), &data);

        LPCTSTR szAgent = _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; WOW64; Trident/4.0; QQDownload 717; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; .NET4.0E)");
        g_hIntOpen = ::InternetOpen(szAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
        DWORD timeout = 1000 * 15;
        ::InternetSetOption(g_hIntOpen, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
        ::InternetSetOption(g_hIntOpen, INTERNET_OPTION_SEND_TIMEOUT, &timeout, sizeof(timeout));
        ::InternetSetOption(g_hIntOpen, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));
    }

    void UnInitHttpUtil()
    {
        if(g_hIntOpen != NULL)
        {
            ::InternetCloseHandle(g_hIntOpen);
            g_hIntOpen = NULL;
        }
        ::WSACleanup();
    }

    bool DoHttpRequest(LPCTSTR method, LPCTSTR url, LPVOID data, DWORD dwDataLength, HttpResult& httpResult, DWORD dwFlags)
    {
        bool result = false;
        HINTERNET hIntCnn = NULL;
        HINTERNET hIntReq = NULL;
        char* buffer = NULL;
        DWORD dwLength = 0;
        do 
        {
            if(g_hIntOpen == NULL)
                break;

            // get host
            URL_COMPONENTS comp = {sizeof(comp)};

            comp.dwSchemeLength = 1;
            comp.dwHostNameLength = 1;
            comp.dwUserNameLength = 1;
            comp.dwPasswordLength = 1;
            comp.dwUrlPathLength = 1;
            comp.dwExtraInfoLength = 1;

            if(!::InternetCrackUrl(url, _tcslen(url), 0, &comp) || !comp.lpszHostName)
            {
                break;
            }

            // connect
            NString host;
            host.Assign(comp.lpszHostName, comp.dwHostNameLength);
            hIntCnn = ::InternetConnect(g_hIntOpen, host, comp.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if(hIntCnn == NULL)
            {
                break;
            }
            AddIntHandle(hIntCnn);

            bool bIsPost = _tcsicmp(method, _T("post")) == 0;

            // ´´½¨ÇëÇó
            LPCTSTR szAcceptTypes[] =
            {
                _T("text/html"),
                _T("application/xhtml+xml"),
                _T("application/xml;q=0.9"),
                _T("*/*;q=0.8"),
                _T("*/*"),
                NULL,
            };
            NString strUrl;

            if(comp.lpszUrlPath)
            {
                NString strUrlPath;
                strUrlPath.Assign(comp.lpszUrlPath, comp.dwUrlPathLength);
                strUrl += strUrlPath;
            }

            if(comp.lpszExtraInfo)
            {
                NString strExtra;
                strExtra.Assign(comp.lpszExtraInfo, comp.dwExtraInfoLength);
                strUrl += strExtra;
            }
            dwFlags = (_tcsnicmp(url, _T("https"), 5) == 0 ? INTERNET_FLAG_SECURE : 0) | dwFlags;
            dwFlags = dwFlags | INTERNET_FLAG_RELOAD;
            hIntReq = ::HttpOpenRequest(hIntCnn, method, strUrl.GetData(), HTTP_VERSION, url, szAcceptTypes, dwFlags, 0);
            if(hIntReq == NULL)
            {
                break;
            }

            // add header
            // Connection:Close, to advoid waiting too long because of CDN
            NString strHeader;
            strHeader = _T("Connection:Close\r\n");
            if(bIsPost)
                strHeader += _T("Content-Type: application/x-www-form-urlencoded\r\n");
            if(!httpResult.cookie.IsEmpty())
            {
                strHeader += _T("Cookie: ");
                strHeader += httpResult.cookie;
            }
            if(!::HttpAddRequestHeaders(hIntReq,
                strHeader.GetData(),
                strHeader.GetLength(),
                HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE))
            {
                break;
            }

            // send request
            LPVOID pPostData = bIsPost ? data : NULL;
            DWORD dwPostDataLen = bIsPost ? dwDataLength : 0;
            if(!::HttpSendRequest(hIntReq, NULL, 0, pPostData, dwPostDataLen))
            {
                break;
            }

            // check statusCode
            DWORD dwStatusCode = 404;
            DWORD dwBufferLen = sizeof(dwStatusCode);
            if(::HttpQueryInfo(hIntReq, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&dwStatusCode, &dwBufferLen, 0))
            {
                httpResult.statusCode = dwStatusCode;
                dwStatusCode = dwStatusCode / 100;
                if(dwStatusCode != 2 && dwStatusCode != 3)
                {
                    break;
                }
            }

            if(httpResult.headerMap_.size() > 0)
            {
                TCHAR headerValue[4096];
                HttpResult::HeaderMap::iterator ite = httpResult.headerMap_.begin();
                for(; ite != httpResult.headerMap_.end(); ++ ite)
                {
                    DWORD dwLevel = 0;
                    if(ite->first == _T("Set-Cookie"))
                        dwLevel = HTTP_QUERY_SET_COOKIE;
                    else if(ite->first == _T("Location"))
                        dwLevel = HTTP_QUERY_LOCATION;
                    ite->second = _T("");
                    DWORD dwIndex = 0;
                    for(;;)
                    {
                        dwBufferLen = _countof(headerValue);
                        if(::HttpQueryInfo(hIntReq, dwLevel, (LPVOID)headerValue, &dwBufferLen, &dwIndex))
                        {
                            ite->second += headerValue;
                            ite->second += _T("<->");
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }

            // check length
            dwLength = 0;
            BOOL bLengthValid = FALSE;
            dwBufferLen = sizeof(dwLength);
            if(!::HttpQueryInfo(hIntReq, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&dwLength, &dwBufferLen, 0))
                break;

            // alloc buffer
            buffer = static_cast<char*>(malloc((dwLength + 1) * sizeof(TCHAR)));
            if(buffer == NULL)
                break;

            // read data
            result = true;
            DWORD dwRead = 0;
            DWORD dwTemp = 1;
            BOOL bReadOk = TRUE;
            while(result && bReadOk && dwTemp != 0)
            {
                DWORD dwReadLen = dwLength - dwRead;
                result = ::InternetReadFile(hIntReq, buffer + dwRead, dwReadLen, &dwTemp) && dwTemp >= 0;
                if(result && dwTemp == 0)
                    break;
                if(!result)
                    break;
                dwRead += dwTemp;
                if(dwRead >= dwLength)
                    break;
            }
            if(!result)
                break;
            result = result && dwRead == dwLength;

        } while (FALSE);

        if(hIntCnn != NULL)
            ::InternetCloseHandle(hIntCnn);
        if(hIntReq != NULL)
            ::InternetCloseHandle(hIntReq);

        RemoveIntHandle(hIntCnn);

        if(result)
        {
            httpResult.buffer = buffer;
            httpResult.buffer[dwLength] = 0;
            httpResult.buffer[dwLength + 1] = 0;
            httpResult.bufferLength = dwLength;
        }
        else
        {
            httpResult.buffer = NULL;
            httpResult.bufferLength = 0;
            free(buffer);
        }

        return result;
    }

    bool PostString(LPCTSTR url, LPVOID data, DWORD dwDataLength, HttpResult& httpResult)
    {
        httpResult.Reset();
        bool result = DoHttpRequest(_T("POST"), url, data, dwDataLength, httpResult, 0);
        if(result)
            httpResult.text = utf82t(httpResult.buffer, httpResult.bufferLength);
        return result;
    }

    bool GetString(LPCTSTR url, HttpResult& httpResult, DWORD dwFlags)
    {
        httpResult.Reset();
        bool result = DoHttpRequest(_T("GET"), url, NULL, 0, httpResult, dwFlags);
        if(result)
            httpResult.text = utf82t(httpResult.buffer, httpResult.bufferLength);
        return result;
    }

    bool GetFile(LPCTSTR url, LPCTSTR filePath)
    {
        HttpResult httpResult;
        bool result = DoHttpRequest(_T("GET"), url, NULL, 0, httpResult, 0);
        if(!result)
            return false;

        HANDLE file = ::CreateFile(filePath, GENERIC_WRITE, 0, NULL,  CREATE_ALWAYS, 0, 0);
        if(file == INVALID_HANDLE_VALUE)
            return false;
        DWORD dwWritten = 0;
        DWORD dwTemp = 0;
        while(httpResult.bufferLength > dwWritten)
        {
            if(!::WriteFile(file, httpResult.buffer + dwWritten, httpResult.bufferLength - dwWritten, &dwTemp, NULL))
                break;
            dwWritten += dwTemp;
        }
        ::CloseHandle(file);
        result = httpResult.bufferLength == dwWritten;
        return result;
    }

    void StopAllRequest()
    {
        for(int i=0; i<_countof(g_InterHandles); ++ i)
        {
            if(g_InterHandles[i] == NULL)
                continue;
            ::InternetCloseHandle(g_InterHandles[i]);
            g_InterHandles[i] = NULL;
        }
    }
};

