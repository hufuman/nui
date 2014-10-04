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
