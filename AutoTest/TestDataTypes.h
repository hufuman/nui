#pragma once




class TestDataTypes : public testing::Test
{
public:
};

TEST_F(TestDataTypes, Point)
{
    TPoint p1;
    {
        EXPECT_EQ(p1.X, 0);
        EXPECT_EQ(p1.Y, 0);
    }

    const INT testX = rand();
    const INT testY = rand();
    TPoint p2(testX, testY);
    {
        EXPECT_EQ(p2.X, testX);
        EXPECT_EQ(p2.Y, testY);
    }
    TPoint p3(p2);
    {
        EXPECT_EQ(p3.X, p2.X);
        EXPECT_EQ(p3.Y, p2.Y);
    }
    TPoint p4;
    p4 = p3;
    {
        EXPECT_EQ(p3.X, p4.X);
        EXPECT_EQ(p3.Y, p4.Y);
    }
}

TEST_F(TestDataTypes, Size)
{
    TSize size1;
    {
        EXPECT_EQ(size1.Width, 0);
        EXPECT_EQ(size1.Height, 0);
    }
    const INT testX = rand();
    const INT testY = rand();
    TSize size2(testX, testY);
    {
        EXPECT_EQ(size2.Width, testX);
        EXPECT_EQ(size2.Height, testY);
    }
    TSize size3(size2);
    {
        EXPECT_EQ(size3.Width, size2.Width);
        EXPECT_EQ(size3.Height, size2.Height);
    }
    TSize size4;
    size4 = size3;
    {
        EXPECT_EQ(size3.Width, size4.Width);
        EXPECT_EQ(size3.Height, size4.Height);
    }
}

TEST_F(TestDataTypes, Rect)
{
    const INT testX = rand();
    const INT testY = rand();
    const INT testWidth = rand();
    const INT testHeight = rand();

    TRect rect1;
    {
        EXPECT_EQ(rect1.Left, 0);
        EXPECT_EQ(rect1.Top, 0);
        EXPECT_EQ(rect1.Right, 0);
        EXPECT_EQ(rect1.Bottom, 0);
        EXPECT_EQ(rect1.Width(), 0);
        EXPECT_EQ(rect1.Height(), 0);
    }
    TRect rect2(testX, testY, testX + testWidth, testY + testHeight);
    {
        EXPECT_EQ(rect2.Left, testX);
        EXPECT_EQ(rect2.Top, testY);
        EXPECT_EQ(rect2.Right, testX + testWidth);
        EXPECT_EQ(rect2.Bottom, testY + testHeight);
        EXPECT_EQ(rect2.Width(), testWidth);
        EXPECT_EQ(rect2.Height(), testHeight);
    }
    TRect rect3(rect2);
    EXPECT_EQ(rect2, rect3);

    TRect rect4;
    rect4 = rect3;
    EXPECT_EQ(rect3, rect4);

    TRect rect5;
    rect5.Left = testX;
    rect5.Top = testY;
    rect5.Right = testX + testWidth;
    rect5.Bottom = testY + testHeight;
    EXPECT_EQ(rect5, rect4);

    TRect rect6;
    rect6.Left = testX;
    rect6.Top = testY;
    rect6.SetWidth(testWidth);
    rect6.SetHeight(testHeight);
    EXPECT_EQ(rect6, rect5);

    TRect rect7;
    rect7.SetRect(testX, testY, testX + testWidth, testY + testHeight);
    EXPECT_EQ(rect7, rect6);

    TRect rect8;
    rect8.Left = testX;
    rect8.Top = testY;
    rect8.SetSize(testWidth, testHeight);
    EXPECT_EQ(rect8, rect7);
}
