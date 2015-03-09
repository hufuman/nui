#pragma once


class TestFileMapping : public testing::Test
{
public:
    TestFileMapping()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestFileMapping, Basic)
{
    NInstPtr<NFileMapping> fileMapping(MemToolParam);
    NString filePath = TestUtil::GetTestFile(_T("FileMapping\\1.txt"));
    ASSERT_TRUE(fileMapping->OpenForRead(filePath.GetData()));
    EXPECT_TRUE(fileMapping->GetSize() == 10);
    EXPECT_TRUE(memcmp(fileMapping->GetData(), "1234567890", 10) == 0);
    EXPECT_TRUE(!nui::Util::File::CanWrite(filePath.GetData()));
    EXPECT_TRUE(nui::Util::File::CanRead(filePath.GetData()));
    fileMapping->Close();
    EXPECT_TRUE(nui::Util::File::CanWrite(filePath.GetData()));
    EXPECT_TRUE(nui::Util::File::CanRead(filePath.GetData()));
}
