#pragma once


class TestEvent : public testing::Test
{
public:
    void Init()
    {
        event_.Clear();
        result_ = "";
    }

    bool TestFun1(NBaseObj*, NEventData*)
    {
        result_ += "Func1;";
        return true;
    }
    bool TestAdd1(NBaseObj*, NEventData*)
    {
        result_ += "Add1;";
        event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
        return true;
    }
    bool TestRemove1(NBaseObj*, NEventData*)
    {
        result_ += "Remove1;";
        event_.RemoveHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
        return true;
    }
    bool TestFun2(NBaseObj*, NEventData*)
    {
        result_ += "Func2;";
        return true;
    }

    std::string GetResult() const
    {
        return result_;
    }

protected:
    std::string result_;
    NEvent event_;
};

TEST_F(TestEvent, Basic)
{
    NEventData data;
    NBaseObj* baseObj = NULL;

    this->Init();
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun2));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
    event_.Invoke(baseObj, &data);
    EXPECT_EQ(GetResult(), "Func1;Func2;Func1;");

    this->Init();
    event_.RemoveHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
    event_.RemoveHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun2));
    event_.RemoveHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
    event_.Invoke(baseObj, &data);
    EXPECT_EQ(GetResult(), "");

    Init();
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestRemove1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestAdd1));
    event_.Invoke(baseObj, &data);
    EXPECT_EQ(GetResult(), "Func1;Remove1;Add1;Func1;");

    Init();
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestFun1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestAdd1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestAdd1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestRemove1));
    event_.AddHandler(fastdelegate::MakeDelegate(this, &TestEvent::TestRemove1));
    event_.Invoke(baseObj, &data);
    EXPECT_EQ(GetResult(), "Func1;Add1;Add1;Remove1;Remove1;Func1;Func1;");

}
