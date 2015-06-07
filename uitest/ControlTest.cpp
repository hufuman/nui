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

    NInstPtr<NButton> pButton1(MemToolParam);
    window->GetRootFrame()->AddChild(pButton1);
    pButton1->SetClickCallback(MakeDelegate(this, &CControlTest::OnButtonClicked));

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

