#pragma once



class TestParser : public testing::Test
{
public:
    TestParser()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestParser, Basic)
{
    NInstPtr<NParser> parser(MemToolParam);
    Base::NAutoPtr<Base::NBaseObj> obj = parser->Parse(_T("@AboutUI:MainUI"));
    ASSERT_TRUE(obj != NULL);
}
