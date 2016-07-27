#pragma once


class TestMsgLoop : public testing::Test
{
public:
    virtual void SetUp()
    {
        count_ = 0;
        if(testMsgId_ == 0)
            testMsgId_ = ::RegisterWindowMessage(_T("_test_msg_id_"));
    }

    void OnIdle(int idleCount)
    {
        ++ count_;
        UNREFERENCED_PARAMETER(idleCount);
    }

    bool TestWndProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
    {
        UNREFERENCED_PARAMETER(message);
        UNREFERENCED_PARAMETER(wParam);
        UNREFERENCED_PARAMETER(lParam);
        UNREFERENCED_PARAMETER(lResult);
        if(message == testMsgId_)
        {
            ::DestroyWindow(window->GetNative());
        }
        return false;
    }

    static unsigned int WINAPI TestThreadProc(void* param)
    {
        DWORD testMsgId = ::RegisterWindowMessage(_T("_test_msg_id_"));
        HWND hWnd = reinterpret_cast<HWND>(param);
        BOOL isWindow = ::IsWindow(hWnd);
        DWORD dwThreadId = isWindow ? ::GetWindowThreadProcessId(hWnd, NULL) : reinterpret_cast<DWORD>(param);
        ::Sleep(500);
        ::PostThreadMessage(dwThreadId, testMsgId, 0, 0);
        ::Sleep(500);
        ::PostThreadMessage(dwThreadId, testMsgId, 0, 0);
        ::Sleep(500);
        if(isWindow)
            ::PostMessage(hWnd, WM_CLOSE, 0, 0);
        else
            ::PostThreadMessage(dwThreadId, WM_QUIT, 0, 0);
        return 0;
    }

protected:
    int count_;
    DWORD testMsgId_;
};

TEST_F(TestMsgLoop, NoWindow)
{
    TestUtil::EatQuitMsg();
    nui::Ui::NMsgLoop loop;
    loop.AddIdleHandler(MakeDelegate(this, &TestMsgLoop::OnIdle));

    ::CloseHandle(reinterpret_cast<HANDLE>(::_beginthreadex(0, 0, &TestMsgLoop::TestThreadProc, reinterpret_cast<void*>(::GetCurrentThreadId()),0, 0)));
    ASSERT_TRUE(loop.Loop());

    ASSERT_TRUE(count_ >= 3);
}

TEST_F(TestMsgLoop, Window)
{
    TestUtil::EatQuitMsg();
    nui::Ui::NMsgLoop loop;
    loop.AddIdleHandler(MakeDelegate(this, &TestMsgLoop::OnIdle));

    nui::Ui::NWindowBase window;
    nui::Base::NRect rect(0, 0, 20, 20);
    window.SetMsgFilterCallback(MakeDelegate(this, &TestMsgLoop::TestWndProc));
    ASSERT_TRUE(window.Create(NULL));

    ::CloseHandle(reinterpret_cast<HANDLE>(::_beginthreadex(0, 0, &TestMsgLoop::TestThreadProc, reinterpret_cast<void*>(window.GetNative()),0, 0)));
    ASSERT_TRUE(loop.Loop(window.GetNative()));

    ASSERT_TRUE(count_ >= 3);
}
