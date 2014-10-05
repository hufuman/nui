#pragma once


class TestFileMapping : public testing::Test
{
public:
    TestFileMapping()
    {
    }
    virtual void SetUp()
    {
        fileMapping_ = NULL;
    }
    virtual void TearDown()
    {
        if(fileMapping_ != NULL)
        {
            fileMapping_->Release();
            fileMapping_ = NULL;
        }
    }

protected:
    NFileMapping* fileMapping_;
};

TEST_F(TestFileMapping, Basic)
{
    NString filePath = TestUtil::GetTestFile(_T("FileMapping\\1.txt"));
    ASSERT_TRUE(NReflect::GetInstance().Create(fileMapping_));
    ASSERT_TRUE(fileMapping_->OpenForRead(filePath.GetData()));
    EXPECT_TRUE(fileMapping_->GetSize() == 10);
    EXPECT_TRUE(memcmp(fileMapping_->GetData(), "1234567890", 10) == 0);
    EXPECT_TRUE(!NUI::Util::File::CanWrite(filePath.GetData()));
    EXPECT_TRUE(NUI::Util::File::CanRead(filePath.GetData()));
    fileMapping_->Close();
    EXPECT_TRUE(NUI::Util::File::CanWrite(filePath.GetData()));
    EXPECT_TRUE(NUI::Util::File::CanRead(filePath.GetData()));
    fileMapping_->Release();
    fileMapping_ = NULL;
}
