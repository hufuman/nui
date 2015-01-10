#pragma once


class TestCore : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TestCore, Basic)
{
    NInstPtr<NCore> core(InstPtrParam);
    ASSERT_NE(core, static_cast<NCore*>(0));
    NString filePath = TestUtil::GetTestFile(_T("resource"));
    core->InitCore(filePath.GetData(), _T("2052"));
}
