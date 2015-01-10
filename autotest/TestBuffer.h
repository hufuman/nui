#pragma once


class TestBuffer : public testing::Test
{
public:
    TestBuffer()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestBuffer, Basic)
{
    NInstPtr<NBuffer> buffer(InstPtrParam);
    ASSERT_TRUE(buffer != NULL);
    ASSERT_TRUE(buffer != NULL);

    DWORD size = rand() % 65535;
    EXPECT_TRUE(buffer->SetSize(size));
    EXPECT_TRUE(buffer->GetSize() == size);
    ASSERT_TRUE(buffer->GetBuffer() != NULL);
    memset(buffer->GetBuffer(), 0, buffer->GetSize());
    memset(buffer->GetBuffer(), 125, buffer->GetSize());

    buffer->ReleaseBuffer();
    EXPECT_TRUE(buffer->GetSize() == 0);
    EXPECT_TRUE(buffer->GetBuffer() == NULL);

    size += 20;
    LPVOID buf = buffer->GetBuffer(size);
    memset(buf, 0, buffer->GetSize());
    memset(buf, 125, buffer->GetSize());
}
