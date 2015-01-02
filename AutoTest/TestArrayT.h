#pragma once

class TestArrayT : public testing::Test
{
public:
    bool ValidateArray(NArrayT<int>& array, int n1, ...)
    {
        va_list args;
        va_start(args, n1);
        int arg = n1;
        int count = 0;
        for(int i=0; i<array.Count(); ++ i)
        {
            int data1 = array.GetAt(i);
            int data2 = array[i];
            if(data1 != arg || data1 != data2)
                return false;
            arg = va_arg(args, int);
            ++ count;
        }
        return count == array.Count();
    }

    NString GetArrayData(NArrayT<int>& array)
    {
        NString result;
        for(int i=0; i<array.Count(); ++ i)
        {
            int value = array[i];
            result += NString(_T("%1,")).arg(value);
        }
        return result;
    }
};

TEST_F(TestArrayT, Basic)
{
    NArrayT<int> array;
    EXPECT_TRUE(array.Count() == 0);

    EXPECT_TRUE(array.AddItem(12));
    EXPECT_TRUE(array.Count() == 1);
    EXPECT_TRUE(ValidateArray(array, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.AddItem(23));
    EXPECT_TRUE(array.Count() == 2);
    EXPECT_TRUE(ValidateArray(array, 12, 23)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.AddItem(34));
    EXPECT_TRUE(array.Count() == 3);
    EXPECT_TRUE(ValidateArray(array, 12, 23, 34)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.AddItem(45));
    EXPECT_TRUE(array.Count() == 4);
    EXPECT_TRUE(ValidateArray(array, 12, 23, 34, 45)) << GetArrayData(array).GetData();
    array.RemoveAllItems();
    EXPECT_TRUE(array.Count() == 0);

    EXPECT_TRUE(array.InsertAt(0, 12));
    EXPECT_TRUE(array.Count() == 1);
    EXPECT_TRUE(ValidateArray(array, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(0, 11));
    EXPECT_TRUE(array.Count() == 2);
    EXPECT_TRUE(ValidateArray(array, 11, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(0, 10));
    EXPECT_TRUE(array.Count() == 3);
    EXPECT_TRUE(ValidateArray(array, 10, 11, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(1, 22));
    EXPECT_TRUE(array.Count() == 4);
    EXPECT_TRUE(ValidateArray(array, 10, 22, 11, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(1, 33));
    EXPECT_TRUE(array.Count() == 5);
    EXPECT_TRUE(ValidateArray(array, 10, 33, 22, 11, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(array.Count(), 44));
    EXPECT_TRUE(array.Count() == 6);
    EXPECT_TRUE(ValidateArray(array, 10, 33, 22, 11, 12, 44)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(array.Count(), 55));
    EXPECT_TRUE(array.Count() == 7);
    EXPECT_TRUE(ValidateArray(array, 10, 33, 22, 11, 12, 44, 55)) << GetArrayData(array).GetData();
    array.RemoveAllItems();
    EXPECT_TRUE(array.Count() == 0);

    EXPECT_TRUE(array.InsertAt(0, 12));
    EXPECT_TRUE(array.Count() == 1);
    EXPECT_TRUE(ValidateArray(array, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.AddItem(13));
    EXPECT_TRUE(array.Count() == 2);
    EXPECT_TRUE(ValidateArray(array, 12, 13)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.InsertAt(0, 12));
    EXPECT_TRUE(array.Count() == 3);
    EXPECT_TRUE(ValidateArray(array, 12, 12, 13)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.AddItem(13));
    EXPECT_TRUE(array.Count() == 4);
    EXPECT_TRUE(ValidateArray(array, 12, 12, 13, 13)) << GetArrayData(array).GetData();
    array.RemoveAllItems();
    EXPECT_TRUE(array.Count() == 0);

    EXPECT_TRUE(array.AddItem(11));
    EXPECT_TRUE(array.AddItem(12));
    EXPECT_TRUE(array.AddItem(13));
    EXPECT_TRUE(array.GetItemPos(11) == 0);
    EXPECT_TRUE(array.GetItemPos(12) == 1);
    EXPECT_TRUE(array.GetItemPos(13) == 2);
    array.ChangeItemPos(0, 1);
    EXPECT_TRUE(ValidateArray(array, 12, 11, 13)) << GetArrayData(array).GetData();
    array.ChangeItemPos(1, 0);
    EXPECT_TRUE(ValidateArray(array, 11, 12, 13)) << GetArrayData(array).GetData();
    array.ChangeItemPos(2, 1);
    EXPECT_TRUE(ValidateArray(array, 11, 13, 12)) << GetArrayData(array).GetData();
    array.ChangeItemPos(1, 2);
    EXPECT_TRUE(ValidateArray(array, 11, 12, 13)) << GetArrayData(array).GetData();
    array.ChangeItemPos(2, 0);
    EXPECT_TRUE(ValidateArray(array, 13, 11, 12)) << GetArrayData(array).GetData();
    array.ChangeItemPos(0, 2);
    EXPECT_TRUE(ValidateArray(array, 11, 12, 13)) << GetArrayData(array).GetData();

    EXPECT_TRUE(!array.RemoveItem(14));
    EXPECT_TRUE(array.RemoveItem(11));
    EXPECT_TRUE(ValidateArray(array, 12, 13)) << GetArrayData(array).GetData();
    EXPECT_TRUE(!array.RemoveItem(14));
    EXPECT_TRUE(array.RemoveItem(12));
    EXPECT_TRUE(ValidateArray(array, 13)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.RemoveItem(13));
    EXPECT_TRUE(array.Count() == 0);

    EXPECT_TRUE(array.AddItem(11));
    EXPECT_TRUE(array.AddItem(12));
    EXPECT_TRUE(array.AddItem(13));

    EXPECT_TRUE(!array.RemoveItem(14));
    EXPECT_TRUE(array.RemoveItem(13));
    EXPECT_TRUE(ValidateArray(array, 11, 12)) << GetArrayData(array).GetData();
    EXPECT_TRUE(!array.RemoveItem(14));
    EXPECT_TRUE(array.RemoveItem(12));
    EXPECT_TRUE(ValidateArray(array, 11)) << GetArrayData(array).GetData();
    EXPECT_TRUE(array.RemoveItem(11));
    EXPECT_TRUE(array.Count() == 0);
}
