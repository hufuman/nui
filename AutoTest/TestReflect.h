#pragma once


namespace TestReflectData
{
    class ReflectClass : public NBaseObj
    {
        DECLARE_REFLECTION(TEXT("test"), TEXT("reflect"))
    };
}

class TestReflect : public testing::Test
{
public:
    virtual void SetUp()
    {
        NReflect::GetInstance().AddReflect<TestReflectData::ReflectClass>();
    }

    virtual void TearDown()
    {
        NReflect::GetInstance().RemoveReflect<TestReflectData::ReflectClass>();
    }
};

TEST_F(TestReflect, Basic)
{
    TestReflectData::ReflectClass* pData = NULL;
    EXPECT_TRUE(NReflect::GetInstance().Create(pData));
    EXPECT_EQ(pData->Release(), 0);
}
