#pragma once


class TestListT : public testing::Test
{
public:
    TestListT()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }

    bool ValidateList(NListT<int>& list, int n1, ...)
    {
        va_list args;
        va_start(args, n1);
        NListT<int>::Position pos = list.GetHeadPosition();
        int arg = n1;
        int count = 0;
        for(; pos != NULL ;)
        {
            int data = list.GetNextItem(pos);
            if(data != arg)
                return false;
            arg = va_arg(args, int);
            ++ count;
        }
        return count == list.Count();
    }

    NString GetListData(NListT<int>& list)
    {
        NString result;
        NListT<int>::Position pos = list.GetHeadPosition();
        while(pos != NULL)
        {
            int value = list.GetNextItem(pos);
            result += NString(_T("%1,")).arg(value);
        }
        return result;
    }

protected:
};

TEST_F(TestListT, Basic)
{
    NListT<int> list;
    EXPECT_TRUE(list.Count() == 0);
    NListT<int>::Position addPos = list.AddItem(12);
    EXPECT_TRUE(list.Count() == 1);
    NListT<int>::Position addPos1 = addPos;
    EXPECT_TRUE(addPos != NULL);
    EXPECT_TRUE(ValidateList(list, 12));
    int value = list.GetNextItem(addPos);
    EXPECT_TRUE(value == 12 && addPos == NULL);

    NListT<int>::Position insertPos = list.InsertItem(NULL, 11);
    EXPECT_TRUE(list.Count() == 2);
    EXPECT_TRUE(insertPos != NULL);
    EXPECT_TRUE(ValidateList(list, 11, 12));
    value = list.GetNextItem(insertPos);
    EXPECT_TRUE(value == 11 && insertPos != NULL);

    insertPos = list.InsertItem(NULL, 10);
    EXPECT_TRUE(list.Count() == 3);
    EXPECT_TRUE(insertPos != NULL);
    EXPECT_TRUE(ValidateList(list, 10, 11, 12));
    value = list.GetNextItem(insertPos);
    EXPECT_TRUE(value == 10 && insertPos != NULL);
    value = list.GetNextItem(insertPos);
    EXPECT_TRUE(value == 11 && insertPos != NULL);

    insertPos = list.InsertItem(addPos1, 22);
    EXPECT_TRUE(list.Count() == 4);
    EXPECT_TRUE(insertPos != NULL);
    EXPECT_TRUE(ValidateList(list, 10, 11, 22, 12)) << GetListData(list).GetData();

    list.RemoveItem(addPos1);
    EXPECT_TRUE(list.Count() == 3);
    EXPECT_TRUE(ValidateList(list, 10, 11, 22));
    list.RemoveItem(insertPos);
    EXPECT_TRUE(list.Count() == 2);
    EXPECT_TRUE(ValidateList(list, 10, 11));
    list.RemoveItem(list.GetHeadPosition());
    EXPECT_TRUE(list.Count() == 1);
    EXPECT_TRUE(ValidateList(list, 11));
    list.RemoveItem(list.GetHeadPosition());
    EXPECT_TRUE(list.Count() == 0 && list.GetHeadPosition() == NULL);

    list.AddItem(12);
    EXPECT_TRUE(list.Count() == 1);
    list.AddItem(23);
    EXPECT_TRUE(list.Count() == 2);
    list.AddItem(34);
    EXPECT_TRUE(list.Count() == 3);
    list.AddItem(45);
    EXPECT_TRUE(list.Count() == 4);
    EXPECT_TRUE(ValidateList(list, 12, 23, 34, 45));
    list.Clear();
    EXPECT_TRUE(list.Count() == 0);

    list.InsertItem(NULL, 12);
    EXPECT_TRUE(list.Count() == 1);
    list.InsertItem(NULL, 23);
    EXPECT_TRUE(list.Count() == 2);
    list.InsertItem(NULL, 34);
    EXPECT_TRUE(list.Count() == 3);
    list.InsertItem(NULL, 45);
    EXPECT_TRUE(list.Count() == 4);
    EXPECT_TRUE(ValidateList(list, 45, 34, 23, 12));
    list.Clear();
    EXPECT_TRUE(list.Count() == 0);

    list.InsertItem(NULL, 12);
    EXPECT_TRUE(list.Count() == 1);
    addPos = list.AddItem(22);
    EXPECT_TRUE(list.Count() == 2);
    EXPECT_TRUE(ValidateList(list, 12, 22));
}
