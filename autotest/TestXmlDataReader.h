#pragma once


namespace TestXmlDataReaderData
{
    const char g_SingleNode[] = "<node id=\"id1\" name=\"name2\"/>";
    const char g_WrongSingleNode[] = "<node id=\"id1\" name=\"name2\"";

    const char g_Node[] = "<root><node id=\"id10\" name=\"name11\"/></root>";
    const char g_PathNode[] = "<root><base><node><child id=\"id8\" name=\"name9\"/></node></base></root>";
    const char g_MultiplePathNode[] = "<root><base><node><child id=\"id12\" name=\"name13\"/><child id=\"id14\" name=\"name15\"/><child id=\"id16\" name=\"name17\"/></node></base></root>";
    const char g_MultipleNode[] = "<root><node id=\"id2\" name=\"name4\" /><node id=\"id3\" name=\"name5\" /><node id=\"id4\" name=\"name6\" /></root>";

    const char g_AllTypes[] = "<node int32=\"1235\" int64=\"595882823415132\" bool1=\"true\" bool2=\"false\" tpoint=\"234,8897\" tsize=\"378,234\" trect=\"356,325,23,123\"/>";
}

class TestXmlDataReader : public testing::Test
{
public:
    TestXmlDataReader()
    {
        reader_ = NULL;
    }
    virtual void SetUp()
    {
        reader_ = CreateDataReader(ReaderXml);
        reader_->AddRef();
    }
    virtual void TearDown()
    {
        NSafeRelease(reader_);
    }

protected:
    NDataReader* reader_;
};

TEST_F(TestXmlDataReader, CreateDataReader)
{
    ASSERT_NE(reader_, static_cast<NDataReader*>(0));
}

TEST_F(TestXmlDataReader, ParseUtf8)
{
    using namespace TestXmlDataReaderData;
    EXPECT_TRUE(reader_->ParseUtf8(g_SingleNode));
    EXPECT_FALSE(reader_->ParseUtf8(g_WrongSingleNode));
}

TEST_F(TestXmlDataReader, SingleNode)
{
    NString value;

    using namespace TestXmlDataReaderData;
    ASSERT_TRUE(reader_->ParseUtf8(g_SingleNode));

    EXPECT_TRUE(reader_->ReadValue(TEXT("id"), value));
    EXPECT_STREQ(value, TEXT("id1"));

    EXPECT_TRUE(reader_->ReadValue(TEXT("name"), value));
    EXPECT_STREQ(value, TEXT("name2"));
}

TEST_F(TestXmlDataReader, ReadPath)
{
    using namespace TestXmlDataReaderData;
    ASSERT_TRUE(reader_->ParseUtf8(g_PathNode));

    NString value;
    EXPECT_TRUE(reader_->ReadPath(TEXT("base\\node\\child"), TEXT("id"), value));
    EXPECT_STREQ(value, TEXT("id8"));

    EXPECT_TRUE(reader_->ReadPath(TEXT("base\\node\\child"), TEXT("name"), value));
    EXPECT_STREQ(value, TEXT("name9"));

    ASSERT_TRUE(reader_->ParseUtf8(g_Node));
    NAutoPtr<NDataReader> data;
    ASSERT_TRUE(reader_->ReadNode(TEXT("node"), data));

    ASSERT_EQ(data->GetNodeName(), _T("node"));

    EXPECT_TRUE(data->ReadValue(TEXT("id"), value));
    EXPECT_STREQ(value, TEXT("id10"));

    EXPECT_TRUE(data->ReadValue(TEXT("name"), value));
    EXPECT_STREQ(value, TEXT("name11"));
}

TEST_F(TestXmlDataReader, MultipleReadPath)
{
    using namespace TestXmlDataReaderData;
    ASSERT_TRUE(reader_->ParseUtf8(g_MultiplePathNode));

    NString value, id, name;
    for(int i=0; i<3; ++ i)
    {
        id.Format(TEXT("id%d"), i * 2 + 12);
        EXPECT_TRUE(reader_->ReadPath(i, TEXT("base\\node\\child"), TEXT("id"), value));
        EXPECT_STREQ(value, id);

        name.Format(TEXT("name%d"), i * 2 + 13);
        EXPECT_TRUE(reader_->ReadPath(i, TEXT("base\\node\\child"), TEXT("name"), value));
        EXPECT_STREQ(value, name);
    }
}

TEST_F(TestXmlDataReader, MultipleReadNode)
{
    NString tmp1, tmp2, value;

    using namespace TestXmlDataReaderData;
    ASSERT_TRUE(reader_->ParseUtf8(g_MultipleNode));

    for(int i=0; i<3; ++ i)
    {
        NAutoPtr<NDataReader> node;
        ASSERT_TRUE(reader_->ReadNode(i, TEXT("node"), node));

        ASSERT_EQ(node->GetNodeName(), _T("node"));

        tmp1.Format(TEXT("id%d"), 2 + i);
        EXPECT_TRUE(node->ReadValue(TEXT("id"), value));
        EXPECT_STREQ(value, tmp1);

        tmp2.Format(TEXT("name%d"), 4 + i);
        EXPECT_TRUE(node->ReadValue(TEXT("name"), value));
        EXPECT_STREQ(value, tmp2);
    }
}

TEST_F(TestXmlDataReader, MultipleAllTypes)
{
    using namespace TestXmlDataReaderData;
    ASSERT_TRUE(reader_->ParseUtf8(g_AllTypes));

    int int32Data;
    DWORD dwordData;
    INT32 int32Data2;

    EXPECT_TRUE(reader_->ReadValue(TEXT("int32"), int32Data));
    EXPECT_EQ(int32Data, 1235);
    EXPECT_TRUE(reader_->ReadValue(TEXT("int32"), dwordData));
    EXPECT_EQ(dwordData, 1235);
    EXPECT_TRUE(reader_->ReadValue(TEXT("int32"), int32Data2));
    EXPECT_EQ(int32Data2, 1235);

    INT64 int64Data;
    EXPECT_TRUE(reader_->ReadValue(TEXT("int64"), int64Data));
    EXPECT_EQ(int64Data, 595882823415132);

    bool bool1Data;
    EXPECT_TRUE(reader_->ReadValue(TEXT("bool1"), bool1Data));
    EXPECT_EQ(bool1Data, true);

    bool bool2Data;
    EXPECT_TRUE(reader_->ReadValue(TEXT("bool2"), bool2Data));
    EXPECT_EQ(bool2Data, false);

    NPoint pt;
    EXPECT_TRUE(reader_->ReadValue(TEXT("tpoint"), pt));
    EXPECT_TRUE(pt.X == 234 && pt.Y == 8897);

    NSize size;
    EXPECT_TRUE(reader_->ReadValue(TEXT("tsize"), size));
    EXPECT_TRUE(size.Width == 378 && size.Height == 234);

    NRect rect;
    EXPECT_TRUE(reader_->ReadValue(TEXT("trect"), rect));
    EXPECT_TRUE(rect.Left == 356 && rect.Top == 325
        && rect.Right == 23 && rect.Bottom == 123);
}

TEST_F(TestXmlDataReader, MultipleAllTypesByIndex)
{
    // const char g_AllTypes[] = "<node int32=\"1235\" int64=\"595882823415132\" bool1=\"true\" bool2=\"false\" tpoint=\"234,8897\" tsize=\"378,234\" trect=\"356,325,23,123\"/>";
    using namespace TestXmlDataReaderData;
    ASSERT_TRUE(reader_->ParseUtf8(g_AllTypes));

    NString name;
    int int32Data;
    DWORD dwordData;
    INT32 int32Data2;

    EXPECT_TRUE(reader_->ReadValue(0, name, int32Data));
    EXPECT_TRUE(name == _T("int32"));
    EXPECT_EQ(int32Data, 1235);
    EXPECT_TRUE(reader_->ReadValue(0, name, dwordData));
    EXPECT_TRUE(name == _T("int32"));
    EXPECT_EQ(dwordData, 1235);
    EXPECT_TRUE(reader_->ReadValue(0, name, int32Data2));
    EXPECT_TRUE(name == _T("int32"));
    EXPECT_EQ(int32Data2, 1235);

    INT64 int64Data;
    EXPECT_TRUE(reader_->ReadValue(1, name, int64Data));
    EXPECT_TRUE(name == _T("int64"));
    EXPECT_EQ(int64Data, 595882823415132);

    bool bool1Data;
    EXPECT_TRUE(reader_->ReadValue(2, name, bool1Data));
    EXPECT_TRUE(name == _T("bool1"));
    EXPECT_EQ(bool1Data, true);

    bool bool2Data;
    EXPECT_TRUE(reader_->ReadValue(3, name, bool2Data));
    EXPECT_TRUE(name == _T("bool2"));
    EXPECT_EQ(bool2Data, false);

    NPoint pt;
    EXPECT_TRUE(reader_->ReadValue(4, name, pt));
    EXPECT_TRUE(name == _T("tpoint"));
    EXPECT_TRUE(pt.X == 234 && pt.Y == 8897);

    NSize size;
    EXPECT_TRUE(reader_->ReadValue(5, name, size));
    EXPECT_TRUE(name == _T("tsize"));
    EXPECT_TRUE(size.Width == 378 && size.Height == 234);

    NRect rect;
    EXPECT_TRUE(reader_->ReadValue(6, name, rect));
    EXPECT_TRUE(name == _T("trect"));
    EXPECT_TRUE(rect.Left == 356 && rect.Top == 325
        && rect.Right == 23 && rect.Bottom == 123);

    EXPECT_FALSE(reader_->ReadValue(7, name, int32Data));
}

