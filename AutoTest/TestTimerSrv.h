#pragma once


class TestTimerSrv : public testing::Test
{
public:
    virtual void SetUp()
    {
        TestUtil::EatQuitMsg();
        count_ = 0;
        timerSrv_ = NULL;
    }

    virtual void TearDown()
    {
        TestUtil::EatQuitMsg();
        NSafeRelease(timerSrv_);
    }

    void TimerProc()
    {
        ++ count_;
        if(count_ >= 5 || onceTimer_)
        {
            timer_.Release();
            loop_.Stop();
        }
    }

protected:
    int count_;
    NUI::UI::NTimerSrv* timerSrv_;
    NUI::Base::NHolder timer_;
    NUI::UI::NMsgLoop loop_;
    bool onceTimer_;
};

TEST_F(TestTimerSrv, Timer)
{
    NReflectCreate(timerSrv_);
    ASSERT_TRUE(timerSrv_ != NULL);

    onceTimer_ = false;
    DWORD dwTickCount = ::GetTickCount();
    timer_ = timerSrv_->startTimer(100, MakeDelegate(this, &TestTimerSrv::TimerProc));
    loop_.Loop();
    DWORD dwElapse = ::GetTickCount() - dwTickCount;
    ASSERT_TRUE(dwElapse > 500) << dwElapse;
}

TEST_F(TestTimerSrv, OnceTimer)
{
    NReflectCreate(timerSrv_);
    ASSERT_TRUE(timerSrv_ != NULL);

    onceTimer_ = true;
    DWORD dwTickCount = ::GetTickCount();
    timer_ = timerSrv_->startOnceTimer(100, MakeDelegate(this, &TestTimerSrv::TimerProc));
    loop_.Loop();
    DWORD dwElapse = ::GetTickCount() - dwTickCount;
    ASSERT_TRUE(dwElapse <= 200 && dwElapse >= 100) << dwElapse;
}
