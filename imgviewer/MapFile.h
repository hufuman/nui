#pragma once


class CMapFile
{
    CMapFile(const CMapFile&);
    CMapFile& operator = (const CMapFile&);
public:
    CMapFile()
    {
        m_hMap = NULL;
        m_pData = NULL;
        m_dwSize = 0;
    }
    ~CMapFile()
    {
        Close();
    }

    BOOL Create(LPCTSTR dataName, DWORD dwSize, BOOL needCreate)
    {
        Close();

        for(; ;)
        {
            if(needCreate)
                m_hMap = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, dwSize, dataName);
            else
                m_hMap = ::OpenFileMapping(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, dataName);
            if(m_hMap == NULL)
                break;

            m_pData = (LPBYTE)::MapViewOfFile(m_hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, dwSize);
            if(m_pData == NULL)
                break;

            m_dwSize = dwSize;
            break;
        }
        if(m_dwSize <= 0)
            Close();

        return (m_dwSize > 0);
    }

    void Close()
    {
        if(m_pData != NULL)
        {
            ::UnmapViewOfFile(m_pData);
            m_pData = NULL;
            m_dwSize = 0;
        }
        if(m_hMap != NULL)
        {
            ::CloseHandle(m_hMap);
            m_hMap = NULL;
        }
    }

    LPBYTE GetData()
    {
        return m_pData;
    }
    DWORD GetSize()
    {
        return m_dwSize;
    }

private:
    HANDLE m_hMap;
    LPBYTE m_pData;
    DWORD  m_dwSize;
};