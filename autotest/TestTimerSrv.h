#pragma once


class TestTimerSrv : public testing::Test
{
public:
    virtual void SetUp()
    {
        TestUtil::EatQuitMsg();
        count_ = 0;
    }

    virtual void TearDown()
    {
        TestUtil::EatQuitMsg();
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
    nui::Base::NHolder timer_;
    nui::UI::NMsgLoop loop_;
    bool onceTimer_;
};

TEST_F(TestTimerSrv, Timer)
{
    NInstPtr<nui::UI::NTimerSrv> timerSrv(InstPtrParam);
    ASSERT_TRUE(timerSrv != NULL);

    onceTimer_ = false;
    DWORD dwTickCount = ::GetTickCount();
    timer_ = timerSrv->startTimer(100, MakeDelegate(this, &TestTimerSrv::TimerProc));
    loop_.Loop();
    DWORD dwElapse = ::GetTickCount() - dwTickCount;
    ASSERT_TRUE(dwElapse > 500) << dwElapse;
}

TEST_F(TestTimerSrv, OnceTimer)
{
    NInstPtr<nui::UI::NTimerSrv> timerSrv(InstPtrParam);
    ASSERT_TRUE(timerSrv != NULL);

    onceTimer_ = true;
    DWORD dwTickCount = ::GetTickCount();
    timer_ = timerSrv->startOnceTimer(100, MakeDelegate(this, &TestTimerSrv::TimerProc));
    loop_.Loop();
    DWORD dwElapse = ::GetTickCount() - dwTickCount;
    ASSERT_TRUE(dwElapse <= 200 && dwElapse >= 100) << dwElapse;
}
