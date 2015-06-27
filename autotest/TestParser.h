#pragma once



class TestParser : public testing::Test
{
public:
    TestParser()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestParser, Frame)
{
    // Parse BaseObj
    NInstPtr<NParser> parser(MemToolParam);
    NAutoPtr<NBaseObj> obj = parser->Parse(_T("@AboutUI:MainUI"));
    ASSERT_TRUE(obj != NULL);

    // Frame
    NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)obj);
    ASSERT_TRUE(frame != NULL);

    EXPECT_EQ(frame->GetId(), _T("TestFrame"));
    EXPECT_FALSE(frame->IsVisible());
    EXPECT_FALSE(frame->IsEnabled());

    const NRect& rect = frame->GetRect();
    EXPECT_EQ(rect.Left, 123);
    EXPECT_EQ(rect.Top, 34);
    EXPECT_EQ(rect.Width(), 110);
    EXPECT_EQ(rect.Height(), 120);
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
