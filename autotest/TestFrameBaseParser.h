#pragma once



class TestFrameBaseParser : public testing::Test
{
public:
    TestFrameBaseParser()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestFrameBaseParser, Basic)
{
    // Parse BaseObj
    NInstPtr<NParser> parser(MemToolParam);
    NAutoPtr<NBaseObj> obj = parser->Parse(NULL, _T("@TestUI:TestFrameBase"));
    ASSERT_TRUE(obj != NULL);

    // Frame
    NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)obj);
    ASSERT_TRUE(frame != NULL);

    EXPECT_EQ(frame->GetId(), _T("TestFrameBase"));
    EXPECT_FALSE(frame->IsVisible());
    EXPECT_FALSE(frame->IsEnabled());

    const NRect& rect = frame->GetRect();
    EXPECT_EQ(rect.Left, 123);
    EXPECT_EQ(rect.Top, 34);
    EXPECT_EQ(rect.Width(), 220);
    EXPECT_EQ(rect.Height(), 330);
    EXPECT_FALSE(frame->IsAutoSize());

    const NRect& margin = frame->GetMargin();
    EXPECT_EQ(margin.Left, 6);
    EXPECT_EQ(margin.Top, 9);
    EXPECT_EQ(margin.Right, 3);
    EXPECT_EQ(margin.Bottom, 2);

    // Child
    NFrame* btnOkFrame = frame->GetChildById(_T("btnOk"), false);
    EXPECT_TRUE(btnOkFrame != NULL);
    NButton* btnOk = dynamic_cast<NButton*>(btnOkFrame);
    EXPECT_TRUE(btnOk != NULL);
}
