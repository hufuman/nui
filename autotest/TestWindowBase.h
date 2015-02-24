#pragma once


class TestWindowBase : public testing::Test
{
public:
    virtual void SetUp()
    {
        count_ = 0;
        timerId_ = 2024;
    }

    bool TestWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
    {
        switch(message)
        {
        case WM_CREATE:
            ::SetTimer(hWnd, timerId_, 10, NULL);
            break;
        case WM_NCDESTROY:
            timerId_ = 0;
            ::PostQuitMessage(0);
            break;
        case WM_TIMER:
            ++ count_;
            if(count_ > 10)
            {
                ::KillTimer(hWnd, timerId_);
                ::DestroyWindow(hWnd);
                return true;
            }
            break;
        }
        return false;
    }

protected:
    int count_;
    DWORD timerId_;
};


TEST_F(TestWindowBase, Modaless)
{
    nui::Ui::NWindowBase window;
    window.SetMsgFilterCallback(MakeDelegate(this, &TestWindowBase::TestWndProc));
    NRect rect(0, 0, 100, 100);
    ASSERT_TRUE(window.Create(NULL));
    MSG msg;
    while(::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    ASSERT_EQ(window.GetNative(), static_cast<HWND>(NULL));
    ASSERT_EQ(count_, 11);
    ASSERT_EQ(timerId_, 0);
}


TEST_F(TestWindowBase, Modal)
{
    nui::Ui::NWindowBase window;
    window.SetMsgFilterCallback(MakeDelegate(this, &TestWindowBase::TestWndProc));
    NRect rect(0, 0, 100, 100);
    ASSERT_TRUE(window.DoModal(NULL));
    ASSERT_EQ(window.GetNative(), static_cast<HWND>(NULL));
    ASSERT_EQ(count_, 11);
    ASSERT_EQ(timerId_, 0);
}
