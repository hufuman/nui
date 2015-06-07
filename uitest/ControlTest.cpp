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
    NAutoPtr<NShapeDraw> pBgkDraw = loader->CreateShape(MemToolParam);
    pBgkDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 255, 255, 0));
    window->GetRootFrame()->SetBkgDraw(pBgkDraw);

    NInstPtr<NButton> pButton(MemToolParam);
    window->GetRootFrame()->AddChild(pButton);

    // loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window->GetNative());

    // destroy
    window = NULL;
}
