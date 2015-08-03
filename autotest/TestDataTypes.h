#pragma once




class TestDataTypes : public testing::Test
{
public:
};

TEST_F(TestDataTypes, Point)
{
    NPoint p1;
    {
        EXPECT_EQ(p1.X, 0);
        EXPECT_EQ(p1.Y, 0);
    }

    const INT testX = rand();
    const INT testY = rand();
    NPoint p2(testX, testY);
    {
        EXPECT_EQ(p2.X, testX);
        EXPECT_EQ(p2.Y, testY);
    }
    NPoint p3(p2);
    {
        EXPECT_EQ(p3.X, p2.X);
        EXPECT_EQ(p3.Y, p2.Y);
    }
    NPoint p4;
    p4 = p3;
    {
        EXPECT_EQ(p3.X, p4.X);
        EXPECT_EQ(p3.Y, p4.Y);
    }
}

TEST_F(TestDataTypes, Size)
{
    NSize size1;
    {
        EXPECT_EQ(size1.Width, 0);
        EXPECT_EQ(size1.Height, 0);
    }
    const INT testX = rand();
    const INT testY = rand();
    NSize size2(testX, testY);
    {
        EXPECT_EQ(size2.Width, testX);
        EXPECT_EQ(size2.Height, testY);
    }
    NSize size3(size2);
    {
        EXPECT_EQ(size3.Width, size2.Width);
        EXPECT_EQ(size3.Height, size2.Height);
    }
    NSize size4;
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

    NRect rect1;
    {
        EXPECT_EQ(rect1.Left, 0);
        EXPECT_EQ(rect1.Top, 0);
        EXPECT_EQ(rect1.Right, 0);
        EXPECT_EQ(rect1.Bottom, 0);
        EXPECT_EQ(rect1.Width(), 0);
        EXPECT_EQ(rect1.Height(), 0);
    }
    NRect rect2(testX, testY, testX + testWidth, testY + testHeight);
    {
        EXPECT_EQ(rect2.Left, testX);
        EXPECT_EQ(rect2.Top, testY);
        EXPECT_EQ(rect2.Right, testX + testWidth);
        EXPECT_EQ(rect2.Bottom, testY + testHeight);
        EXPECT_EQ(rect2.Width(), testWidth);
        EXPECT_EQ(rect2.Height(), testHeight);
    }
    NRect rect3(rect2);
    EXPECT_EQ(rect2, rect3);

    NRect rect4;
    rect4 = rect3;
    EXPECT_EQ(rect3, rect4);

    NRect rect5;
    rect5.Left = testX;
    rect5.Top = testY;
    rect5.Right = testX + testWidth;
    rect5.Bottom = testY + testHeight;
    EXPECT_EQ(rect5, rect4);

    NRect rect6;
    rect6.Left = testX;
    rect6.Top = testY;
    rect6.SetWidth(testWidth);
    rect6.SetHeight(testHeight);
    EXPECT_EQ(rect6, rect5);

    NRect rect7;
    rect7.SetRect(testX, testY, testX + testWidth, testY + testHeight);
    EXPECT_EQ(rect7, rect6);

    NRect rect8;
    rect8.Left = testX;
    rect8.Top = testY;
    rect8.SetSize(testWidth, testHeight);
    EXPECT_EQ(rect8, rect7);

    NRect rect9;
    rect9.SetRect(100, 120, 140, 180);
    EXPECT_EQ(rect9.GetLeftTop().X, 100);
    EXPECT_EQ(rect9.GetLeftTop().Y, 120);
    EXPECT_EQ(rect9.GetSize().Width, 40);
    EXPECT_EQ(rect9.GetSize().Height, 60);

    const NRect rect10(130, 180, 234, 334);
    EXPECT_EQ(rect10.GetLeftTop().X, 130);
    EXPECT_EQ(rect10.GetLeftTop().Y, 180);
    EXPECT_EQ(rect10.GetSize().Width, 234 - 130);
    EXPECT_EQ(rect10.GetSize().Height, 334 - 180);
}
