#pragma once


class TestModule : public testing::Test
{
public:
    TestModule()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }

protected:
};

TEST_F(TestModule, Init)
{
    HMODULE hNui = ::GetModuleHandle(_T("nui.dll"));
    EXPECT_EQ(hNui, NModule::GetInst().GetNUIModule());

    TCHAR szAppPath[1024];
    ::GetModuleFileName(NULL, szAppPath, 1024);
    *(_tcsrchr(szAppPath, _T('\\')) + 1) = 0;
    EXPECT_EQ(NModule::GetInst().GetAppPath(), szAppPath);
}
