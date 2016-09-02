#pragma once


#include <map>


namespace HttpUtil
{
    void InitHttpUtil();
    void UnInitHttpUtil();

    class HttpResult
    {
    public:
        HttpResult();
        HttpResult(const HttpResult&);
        HttpResult& operator = (const HttpResult&);
        ~HttpResult();

        void Reset();
        void NeedHeader(LPCTSTR headerName);
        NString GetHeader(LPCTSTR headerName) const;

        DWORD   statusCode;
        NString text;
        char*   buffer;
        DWORD   bufferLength;
        NString cookie;

        typedef std::map<NString, NString> HeaderMap;
        HeaderMap headerMap_;
    };

    bool PostString(LPCTSTR url, LPVOID data, DWORD dwDataLength, HttpResult& httpResult);
    bool GetString(LPCTSTR url, HttpResult& httpResult, DWORD dwFlags);
    bool GetFile(LPCTSTR url, LPCTSTR fileName);
    void StopAllRequest();
};
