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

protected:
    NCore* core_;
};

TEST_F(TestCore, Basic)
{
    ASSERT_TRUE(NReflect::GetInstance().Create(core_));
    ASSERT_NE(core_, static_cast<NCore*>(0));
    EXPECT_EQ(core_->Release(), 0);
    core_ = NULL;
}
