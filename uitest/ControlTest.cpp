#include "StdAfx.h"
#include "ControlTest.h"

CControlTest::CControlTest(void)
{
}

CControlTest::~CControlTest(void)
{
}

void CControlTest::Test()
{
    // create window
    window_.Create(MemToolParam);
    // window_->Create(NULL, WindowStyle::Layered);
    window_->Create(NULL, WindowStyle::Sizable);
    window_->SetSize(520, 420);
    window_->CenterWindow(NULL);
    window_->SetText(_T("Test Window"));
    window_->SetVisible(true);

    NInstPtr<NTimerSrv> timerSrv(MemToolParam);
    timerSrv->startTimer(100, MakeDelegate(this, &CControlTest::PaintTest));

    // setup controls
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    NAutoPtr<NShapeDraw> pBkgDraw = loader->CreateShape(MemToolParam);
    pBkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 255, 255, 0));
    window_->GetRootFrame()->SetBkgDraw(pBkgDraw);
//*
    // VertScroll
    NInstPtr<NScroll> pVertScroll(MemToolParam);
    pVertScroll->SetPos(100, 100);
    pVertScroll->SetSize(16, 200);
    pVertScroll->SetScrollRange(4);
    pVertScroll->SetScrollPos(3);
    window_->GetRootFrame()->AddChild(pVertScroll);

    // HorzScroll
    NInstPtr<NScroll> pHorzScroll(MemToolParam);
    pHorzScroll->SetPos(100, 80);
    pHorzScroll->SetSize(200, 16);
    pHorzScroll->SetScrollType(true);
    pHorzScroll->SetScrollRange(1);
    pHorzScroll->SetScrollPos(1);
    window_->GetRootFrame()->AddChild(pHorzScroll);

    // Button
    NInstPtr<NButton> pButton1(MemToolParam);
    pButton1->SetLayout(NFrame::LayoutHCenter | NFrame::LayoutTop);
    pButton1->SetMargin(10, 10, 20, 40);
    window_->GetRootFrame()->AddChild(pButton1);
    pButton1->SetClickCallback(MakeDelegate(this, &CControlTest::OnButtonClicked));
//*/
    /*
    // Static Image
    NInstPtr<NImage> pImg1(MemToolParam);
    pImg1->LoadImage(_T("@skin:images\\514540469.png"));
    window_->GetRootFrame()->AddChild(pImg1);
    */

    ::CreateWindowEx(0, WC_EDIT, _T("TEST"), WS_VISIBLE | WS_CHILD, 0, 0, 100, 18, window_->GetNative(), NULL, NULL, 0);
    ::CreateWindowEx(0, WC_EDIT, _T("TEST"), WS_VISIBLE | WS_CHILD, 80, 80, 100, 18, window_->GetNative(), NULL, NULL, 0);

    // Gif Image
    for(int i=0; i<0; ++ i)
    {
        NInstPtr<NImage> pImg2(MemToolParam);
        pImg2->LoadImage(_T("@skin:images\\3.gif"));
        pImg2->SetPos(i * 10, i % 5 * 50);
        window_->GetRootFrame()->AddChild(pImg2);
    }

    // Label
    NInstPtr<NRichFrame> label(MemToolParam);
    label->SetText(_T("1111111111\r\n2222222222\r\n33333333333333\r\n444444444\r\n55555555555555555\r\n"));
    label->GetRichText()->SetSingleLine(false);
    window_->GetRootFrame()->AddChild(label);

    // loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window_->GetNative());

    // destroy
    window_ = NULL;
}

bool CControlTest::OnButtonClicked(NFrame* pButton, const Base::NPoint& point)
{
    MessageBox(pButton->GetWindow()->GetNative(), _T("Button Clicked"), _T("Test"), MB_OK | MB_ICONINFORMATION);
    return true;
}

void CControlTest::PaintTest()
{
    /*
    HWND hWnd = window_->GetNative();
    HDC hDc = ::GetDC(hWnd);

    NRect rcTest(0, 0, 200, 200);
    ::FillRect(hDc, rcTest, (HBRUSH)::GetStockObject(BLACK_BRUSH));

    ::ReleaseDC(hWnd, hDc);

    */
}
