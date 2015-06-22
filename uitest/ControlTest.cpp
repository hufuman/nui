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
    NInstPtr<NWindow> window(MemToolParam);
    window->Create(NULL);
    window->SetSize(520, 420);
    window->CenterWindow(NULL);
    window->SetText(_T("Test Window"));
    window->SetVisible(true);

    // setup controls
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    NAutoPtr<NShapeDraw> pBkgDraw = loader->CreateShape(MemToolParam);
    pBkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 255, 255, 0));
    window->GetRootFrame()->SetBkgDraw(pBkgDraw);

    /*
    // Button
    NInstPtr<NButton> pButton1(MemToolParam);
    window->GetRootFrame()->AddChild(pButton1);
    pButton1->SetClickCallback(MakeDelegate(this, &CControlTest::OnButtonClicked));

    // Static Image
    NInstPtr<NImage> pImg1(MemToolParam);
    pImg1->LoadImage(_T("@skin:images\\514540469.png"));
    window->GetRootFrame()->AddChild(pImg1);
    */

    // Gif Image
    for(int i=0; i<50; ++ i)
    {
        NInstPtr<NImage> pImg2(MemToolParam);
        pImg2->LoadImage(_T("@skin:images\\3.gif"));
        pImg2->SetPos(i * 10, i % 5 * 50);
        window->GetRootFrame()->AddChild(pImg2);
    }

    // Label
    NInstPtr<NRichFrame> label(MemToolParam);
    label->SetText(_T("1111111111\r\n2222222222\r\n33333333333333\r\n444444444\r\n55555555555555555\r\n"));
    label->GetRichText()->SetSingleLine(false);
    window->GetRootFrame()->AddChild(label);

    // loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window->GetNative());

    // destroy
    window = NULL;
}

bool CControlTest::OnButtonClicked(NFrame* pButton, const Base::NPoint& point)
{
    MessageBox(pButton->GetWindow()->GetNative(), _T("Button Clicked"), _T("Test"), MB_OK | MB_ICONINFORMATION);
    return true;
}

