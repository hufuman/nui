#pragma once


class TestBuffer : public testing::Test
{
public:
    TestBuffer()
    {
    }
    virtual void SetUp()
    {
        buffer_ = NULL;
    }
    virtual void TearDown()
    {
        if(buffer_ != NULL)
        {
            buffer_->Release();
            buffer_ = NULL;
        }
    }

protected:
    NBuffer* buffer_;
};

TEST_F(TestBuffer, Basic)
{
    ASSERT_TRUE(NReflect::GetInstance().Create(buffer_));
    ASSERT_TRUE(buffer_ != NULL);

    DWORD size = rand() % 65535;
    EXPECT_TRUE(buffer_->SetSize(size).GetSize() == size);
    ASSERT_TRUE(buffer_->GetBuffer() != NULL);
    memset(buffer_->GetBuffer(), 0, buffer_->GetSize());
    memset(buffer_->GetBuffer(), 125, buffer_->GetSize());

    buffer_->ReleaseBuffer();
    EXPECT_TRUE(buffer_->GetSize() == 0);
    EXPECT_TRUE(buffer_->GetBuffer() == NULL);

    size += 20;
    LPVOID buffer = buffer_->GetBuffer(size);
    memset(buffer, 0, buffer_->GetSize());
    memset(buffer, 125, buffer_->GetSize());

    buffer_->Release();
    buffer_ = NULL;
}
