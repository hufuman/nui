#pragma once


class TestFileUtil : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TestFileUtil, Basic)
{
    LPCTSTR testCases[] = 
    {
        // Src                  Expected
        _T("C:"),               _T("C:\\"),
        _T("C:\\"),             _T("C:\\"),
        _T("C:\\a.txt"),        _T("C:\\"),
        _T("C:\\aaaa"),         _T("C:\\"),
        _T("C:\\aaaa\\"),       _T("C:\\"),
    };
    for(int i=0; i<_countof(testCases) / 2; ++ i)
    {
        LPCTSTR src = testCases[i * 2];
        LPCTSTR expected = testCases[i * 2 + 1];
        NString result = NUI::Util::File::GetParentFolder(src);
        EXPECT_TRUE(result == expected) << L"\r\n\tSrc: " << src << L"\r\n\tExpected: " << expected << L"\r\n\tResult: " << result.GetData() << "\r\n";
    }
}

TEST_F(TestFileUtil, Right)
{
    NString filePath = TestUtil::GetTestFile(_T("FileUtil\\1.txt"));

    // Read
    HANDLE hFile = ::CreateFile(filePath.GetData(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    ASSERT_TRUE(hFile != INVALID_HANDLE_VALUE);
    EXPECT_TRUE(NUI::Util::File::CanRead(filePath.GetData()));
    EXPECT_TRUE(!NUI::Util::File::CanWrite(filePath.GetData()));
    ::CloseHandle(hFile);

    // Write
    hFile = ::CreateFile(filePath.GetData(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    ASSERT_TRUE(hFile != INVALID_HANDLE_VALUE);
    EXPECT_TRUE(NUI::Util::File::CanWrite(filePath.GetData()));
    EXPECT_TRUE(!NUI::Util::File::CanRead(filePath.GetData()));
    ::CloseHandle(hFile);

    // Read & Write
    hFile = ::CreateFile(filePath.GetData(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    ASSERT_TRUE(hFile != INVALID_HANDLE_VALUE);
    EXPECT_TRUE(NUI::Util::File::CanRead(filePath.GetData()));
    EXPECT_TRUE(NUI::Util::File::CanWrite(filePath.GetData()));
    ::CloseHandle(hFile);

    // None
    hFile = ::CreateFile(filePath.GetData(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    ASSERT_TRUE(hFile != INVALID_HANDLE_VALUE);
    EXPECT_TRUE(!NUI::Util::File::CanRead(filePath.GetData()));
    EXPECT_TRUE(!NUI::Util::File::CanWrite(filePath.GetData()));
    ::CloseHandle(hFile);
}
