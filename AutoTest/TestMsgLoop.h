#pragma once


class TestMsgLoop : public testing::Test, public NUI::UI::NIdleHandler
{
public:
    virtual void SetUp()
    {
        count_ = 0;
    }

    virtual bool OnIdle(int idleCount)
    {
        ++ count_;
        return count_ != 2;
    }

    bool TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
    {
        if(message == WM_NULL)
        {
            ::DestroyWindow(hWnd);
        }
        return false;
    }

    static unsigned int WINAPI TestThreadProc(void* param)
    {
        HWND hWnd = reinterpret_cast<HWND>(param);
        BOOL isWindow = ::IsWindow(hWnd);
        DWORD dwThreadId = isWindow ? ::GetWindowThreadProcessId(hWnd, NULL) : reinterpret_cast<DWORD>(param);
        ::Sleep(500);
        ::PostThreadMessage(dwThreadId, WM_NULL, 0, 0);
        ::Sleep(500);
        ::PostThreadMessage(dwThreadId, WM_NULL, 0, 0);
        ::Sleep(500);
        if(isWindow)
            ::PostMessage(hWnd, WM_CLOSE, 0, 0);
        else
            ::PostThreadMessage(dwThreadId, WM_QUIT, 0, 0);
        return 0;
    }

protected:
    int count_;
};

TEST_F(TestMsgLoop, NoWindow)
{
    TestUtil::EatQuitMsg();
    NUI::UI::NMsgLoop loop;
    loop.AddIdleHandler(this);

    ::CloseHandle(reinterpret_cast<HANDLE>(::_beginthreadex(0, 0, &TestMsgLoop::TestThreadProc, reinterpret_cast<void*>(::GetCurrentThreadId()),0, 0)));
    ASSERT_TRUE(loop.Loop());

    ASSERT_EQ(count_, 4);
}

TEST_F(TestMsgLoop, Window)
{
    TestUtil::EatQuitMsg();
    NUI::UI::NMsgLoop loop;
    loop.AddIdleHandler(this);

    NUI::UI::NWindowBase window;
    NUI::Base::NRect rect(0, 0, 20, 20);
    window.SetMsgFilterCallback(MakeDelegate(this, &TestMsgLoop::TestWndProc));
    ASSERT_TRUE(window.Create(NULL, _T("TestText"), rect));

    ::CloseHandle(reinterpret_cast<HANDLE>(::_beginthreadex(0, 0, &TestMsgLoop::TestThreadProc, reinterpret_cast<void*>(window.GetNative()),0, 0)));
    ASSERT_TRUE(loop.Loop(window.GetNative()));

    ASSERT_EQ(count_, 4);
}
