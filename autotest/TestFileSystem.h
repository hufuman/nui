#pragma once


class TestFileSystem : public testing::Test
{
public:
    TestFileSystem()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestFileSystem, RealFS)
{
    printf("");
}

TEST_F(TestFileSystem, PackFS)
{
    printf("");
}
