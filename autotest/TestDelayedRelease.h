#pragma once


int g_DelayedReleaseIndex = 0;

class DelayedReleaseObj : public NBaseObj
{
public:
    DelayedReleaseObj()
    {
        curIndex_ = ++ g_DelayedReleaseIndex;
    }
    ~DelayedReleaseObj()
    {
        if(curIndex_ < 5)
        {
            DelayedReleaseObj* obj = NNew(DelayedReleaseObj);
            obj->AddRef();

            NDelayedRelease(obj);
        }
        else
        {
            ::PostQuitMessage(0);
        }
    }
private:
    int curIndex_;
};

class TestDelayedRelease : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(TestDelayedRelease, Basic)
{
    DWORD tickCount = ::GetTickCount();
    DelayedReleaseObj* obj = NNew(DelayedReleaseObj);
    obj->AddRef();

    NDelayedRelease(obj);

    NMsgLoop looop;
    looop.Loop();

    tickCount = ::GetTickCount() - tickCount;
    EXPECT_GT(tickCount, static_cast<DWORD>(4999));
}
