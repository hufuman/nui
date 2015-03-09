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
        ASSERT_TRUE(NReflect::GetInstance().AddReflect<TestReflectData::ReflectClass>(NReflect::Singleton));
    }

    virtual void TearDown()
    {
        ASSERT_TRUE(NReflect::GetInstance().RemoveReflect<TestReflectData::ReflectClass>());
    }
};

TEST_F(TestReflect, Basic)
{
    nui::Base::NInstPtr<TestReflectData::ReflectClass> pData(MemToolParam);
    EXPECT_TRUE(pData != NULL);
    EXPECT_EQ(pData->RefCount(), 2);
}

TEST_F(TestReflect, Singleton)
{
    nui::Base::NInstPtr<TestReflectData::ReflectClass> pData1(MemToolParam);
    EXPECT_TRUE(pData1 != NULL);
    EXPECT_EQ(pData1->RefCount(), 2);

    {
        nui::Base::NInstPtr<TestReflectData::ReflectClass> pData2(MemToolParam);
        EXPECT_TRUE(pData2 != NULL);
        EXPECT_EQ(pData2->RefCount(), 3);
    }
    EXPECT_EQ(pData1->RefCount(), 2);

    {
        nui::Base::NInstPtr<TestReflectData::ReflectClass> pData2(MemToolParam);
        EXPECT_TRUE(pData2 != NULL);
        EXPECT_EQ(pData2->RefCount(), 3);
    }
    EXPECT_EQ(pData1->RefCount(), 2);
}
