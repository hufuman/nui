#pragma once



class TestFrameParser : public testing::Test
{
public:
    TestFrameParser()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }
};

TEST_F(TestFrameParser, Basic)
{
    // Parse BaseObj
    NInstPtr<NParser> parser(MemToolParam);
    NAutoPtr<NBaseObj> obj = parser->Parse(NULL, _T("@TestUI:TestFrame"));
    ASSERT_TRUE(obj != NULL);

    // Frame
    NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)obj);
    ASSERT_TRUE(frame != NULL);

    EXPECT_EQ(frame->GetId(), _T("TestFrame"));
    EXPECT_TRUE(frame->IsVisible());
    EXPECT_TRUE(frame->IsEnabled());

    const NRect& rect = frame->GetRect();
    EXPECT_EQ(rect.Left, 123);
    EXPECT_EQ(rect.Top, 34);
    EXPECT_EQ(rect.Width(), 220);
    EXPECT_EQ(rect.Height(), 330);
    EXPECT_TRUE(frame->IsAutoSize());

    // Text
    NText* text = frame->GetRichText();
    TCHAR szText[100] = {0x6211, 0x53EA, 0x662F, 0x4E00, 0x4E2A, 0x6D4B, 0x8BD5};
    EXPECT_EQ(frame->GetText(), szText);
    EXPECT_EQ(text->GetColor(), 0x426594);
    EXPECT_EQ(text->GetBgColor(), 0x726892);
    EXPECT_EQ(text->GetSingleLine(), false);
    EXPECT_EQ(text->GetVertCenter(), true);
    EXPECT_EQ(text->GetHorzCenter(), true);
}
