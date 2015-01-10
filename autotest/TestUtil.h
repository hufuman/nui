#pragma once



namespace testing
{
    namespace internal
    {
        AssertionResult CmpHelperSTREQ(const char expected_expression[],
            const char actual_expression[],
            const nui::Base::NString& expected,
            const wchar_t actual[])
        {
            return CmpHelperSTREQ(expected_expression, actual_expression, expected.GetData(), actual);
        }

        AssertionResult CmpHelperSTREQ(const char expected_expression[],
            const char actual_expression[],
            const nui::Base::NString& expected,
            const nui::Base::NString& actual)
        {
            return CmpHelperSTREQ(expected_expression, actual_expression, expected.GetData(), actual.GetData());
        }

        AssertionResult CmpHelperSTRNE(const char expected_expression[],
            const char actual_expression[],
            const nui::Base::NString& expected,
            const wchar_t actual[])
        {
            return CmpHelperSTRNE(expected_expression, actual_expression, expected.GetData(), actual);
        }

        AssertionResult CmpHelperSTRNE(const char expected_expression[],
            const char actual_expression[],
            const nui::Base::NString& expected,
            const nui::Base::NString& actual)
        {
            return CmpHelperSTRNE(expected_expression, actual_expression, expected.GetData(), actual.GetData());
        }
    }
}

namespace TestUtil
{
    __inline NString GetTestFile(LPCTSTR szFilePath)
    {
        TCHAR szTmpBuffer[1024];
        ::GetModuleFileName(NULL, szTmpBuffer, 1024);
        *_tcsrchr(szTmpBuffer, _T('\\')) = 0;
        *_tcsrchr(szTmpBuffer, _T('\\')) = 0;
        *(_tcsrchr(szTmpBuffer, _T('\\')) + 1) = 0;
        _tcsncat(szTmpBuffer, _T("\\AutoTest\\TestData\\"), 1024);
        _tcsncat(szTmpBuffer, szFilePath, 1024);
        return szTmpBuffer;
    }

    __inline LPVOID GetFileContent(LPCTSTR filePath, DWORD& size)
    {
        HANDLE fileHandle = ::CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if(fileHandle == NULL)
            return NULL;
        size = ::GetFileSize(fileHandle, NULL);
        LPVOID data = ::malloc(size);
        if(data != NULL)
        {
            DWORD read;
            if(!::ReadFile(fileHandle, data, size, &read, NULL)
                || read != size)
            {
                data = NULL;
            }
        }
        ::CloseHandle(fileHandle);
        return data;
    }

    __inline bool CompareFile(LPCTSTR filePath1, LPCTSTR filePath2)
    {
        DWORD size1, size2;
        LPVOID data1 = GetFileContent(filePath1, size1);
        LPVOID data2 = GetFileContent(filePath2, size2);
        bool result = false;
        if(data1 != NULL && data2 != NULL && size1 == size2)
        {
            result = (::memcmp(data1, data2, size1) == 0);
        }
        if(data1 != NULL)
            ::free(data1);
        if(data2 != NULL)
            ::free(data2);
        return result;
    }

    __inline void EatQuitMsg()
    {
        MSG msg;
        while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)
            && msg.message == WM_QUIT)
        {
            ::GetMessage(&msg, NULL, 0, 0);
        }
    }
}
