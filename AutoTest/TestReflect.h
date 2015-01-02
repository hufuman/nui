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
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TestReflect, Basic)
{
    ASSERT_TRUE(NReflect::GetInstance().AddReflect<TestReflectData::ReflectClass>(NReflect::None));

    TestReflectData::ReflectClass* pData = NULL;
    EXPECT_TRUE(NReflectCreate(pData));
    EXPECT_EQ(pData->Release(), 0);

    ASSERT_TRUE(NReflect::GetInstance().RemoveReflect<TestReflectData::ReflectClass>());
}

TEST_F(TestReflect, Singleton)
{
    ASSERT_TRUE(NReflect::GetInstance().AddReflect<TestReflectData::ReflectClass>(NReflect::Singleton));

    TestReflectData::ReflectClass* pData1 = NULL;
    TestReflectData::ReflectClass* pData2 = NULL;
    TestReflectData::ReflectClass* pData3 = NULL;
    TestReflectData::ReflectClass* pData4 = NULL;
    EXPECT_TRUE(NReflectCreate(pData1));
    EXPECT_EQ(pData1->Release(), 1);
    EXPECT_TRUE(NReflectCreate(pData2));
    EXPECT_TRUE(NReflectCreate(pData3));
    EXPECT_EQ(pData2->Release(), 2);
    EXPECT_EQ(pData3->Release(), 1);
    EXPECT_TRUE(NReflectCreate(pData4));
    EXPECT_EQ(pData4->Release(), 1);

    ASSERT_TRUE(NReflect::GetInstance().RemoveReflect<TestReflectData::ReflectClass>());
}
