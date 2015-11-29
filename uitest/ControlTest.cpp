#include "StdAfx.h"
#include "ControlTest.h"

#include "F:\VCProjects\nui\nui\ui\NLayout.h"

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
    window_->Create(NULL, WindowStyle::Sizable | WindowStyle::Top);
    window_->SetSize(520, 420);
    window_->CenterWindow(NULL);
    window_->SetText(_T("Test Window"));
    window_->SetVisible(true);
    window_->SetPostDrawCallback(MakeDelegate(this, &CControlTest::PaintTest));

    // setup controls
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    NAutoPtr<NShapeDraw> pBkgDraw = loader->CreateShape(MemToolParam);
    pBkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 255, 255, 0));
    window_->GetRootFrame()->SetBkgDraw(pBkgDraw);

    // Layout
    NInstPtr<NLayout> pLayout1(MemToolParam);
    pLayout1->SetPos(150, 50);
    pLayout1->SetAutoSize(false);
    pLayout1->SetSize(310, 300);
    pLayout1->SetLayoutType(NLayout::LayoutTile);

    pBkgDraw = loader->CreateShape(MemToolParam);
    pBkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 0, 255, 255));
    pLayout1->SetBkgDraw(pBkgDraw);

    NString msg;
    pLayout1->SetLayoutable(false);
    for(int i=0; i<200; ++ i)
    {
        NInstPtr<NButton> pChild(MemToolParam);
        pChild->SetAutoSize(false);
        pChild->SetSize(100, 20);
        pChild->SetMargin(5, 10, 15, 20);
        msg.Format(_T("Item %d"), i);
        pChild->SetText(msg.GetData());
        pChild->GetRichText()->SetColor(MakeArgb(255, 255, 255, 0));
        if(i == 8)
            printf("");
        pLayout1->AddChild(pChild);
    }
    pLayout1->SetLayoutable(true);
    window_->GetRootFrame()->AddChild(pLayout1);

    //*
    // VertScroll
    NInstPtr<NScroll> pVertScroll(MemToolParam);
    pVertScroll->SetPos(100, 200);
    pVertScroll->SetSize(16, 200);
    pVertScroll->SetScrollRange(4);
    pVertScroll->SetScrollPos(3);
    pVertScroll->SetLayout(NFrame::LayoutVFill);
    window_->GetRootFrame()->AddChild(pVertScroll);
    vertScroll_ = pVertScroll;
    vertScroll_->SetScrollCallback(MakeDelegate(this, &CControlTest::OnScrollEvent));

    // HorzScroll
    NInstPtr<NScroll> pHorzScroll(MemToolParam);
    pHorzScroll->SetPos(100, 180);
    pHorzScroll->SetSize(200, 16);
    pHorzScroll->SetScrollType(true);
    pHorzScroll->SetScrollRange(4);
    pHorzScroll->SetScrollPos(3);
    pHorzScroll->SetLayout(NFrame::LayoutHFill);
    window_->GetRootFrame()->AddChild(pHorzScroll);
    horzScroll_ = pHorzScroll;
    horzScroll_->SetScrollCallback(MakeDelegate(this, &CControlTest::OnScrollEvent));

    NInstPtr<NFrame> posLabel(MemToolParam);
    posLabel->SetPos(10, 100);
    posLabel->SetText(_T("Pos: "));
    window_->GetRootFrame()->AddChild(posLabel);

    NInstPtr<NFrame> rangeLabel(MemToolParam);
    rangeLabel->SetPos(10, 130);
    rangeLabel->SetText(_T("Range: "));
    window_->GetRootFrame()->AddChild(rangeLabel);

    NInstPtr<NEdit> pEdit1(MemToolParam);
    pEdit1->SetLayout(NFrame::LayoutLeft | NFrame::LayoutTop);
    pEdit1->SetMargin(60, 100, 0, 0);
    pEdit1->SetAutoSize(false);
    pEdit1->SetSize(100, 18);
    pEdit1->SetText(_T("3"));
    pEdit1->SetTextChangeCallback(MakeDelegate(this, &CControlTest::OnEditTextChanged));
    window_->GetRootFrame()->AddChild(pEdit1);
    editPos_ = pEdit1;

    NInstPtr<NEdit> pEdit2(MemToolParam);
    pEdit2->SetLayout(NFrame::LayoutLeft | NFrame::LayoutTop);
    pEdit2->SetMargin(60, 130, 0, 0);
    pEdit2->SetAutoSize(false);
    pEdit2->SetSize(100, 18);
    pEdit2->SetText(_T("4"));
    pEdit2->SetTextChangeCallback(MakeDelegate(this, &CControlTest::OnEditTextChanged));
    window_->GetRootFrame()->AddChild(pEdit2);
    editRange_ = pEdit2;

    // Gif Image
    for(int i=0; i<10; ++ i)
    {
        NInstPtr<NImage> pImg2(MemToolParam);
        pImg2->LoadImage(_T("@skin:images\\3.gif"));
        pImg2->SetMargin(i * 8, 0, 0, 0);
        pImg2->SetLayout(NFrame::LayoutLeft | NFrame::LayoutBottom);
        window_->GetRootFrame()->AddChild(pImg2);
    }

    // Static Image
    NInstPtr<NImage> pImg1(MemToolParam);
    pImg1->LoadImage(_T("@skin:images\\514540469.png"));
    pImg1->SetLayout(NFrame::LayoutRight | NFrame::LayoutBottom);
    window_->GetRootFrame()->AddChild(pImg1);

    // loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window_->GetNative());

    // destroy
    editPos_ = NULL;
    editRange_ = NULL;
    horzScroll_ = NULL;
    vertScroll_ = NULL;
    window_ = NULL;
}

bool CControlTest::OnButtonClicked(NFrame* pButton, const Base::NPoint& point)
{
    // MessageBox(pButton->GetWindow()->GetNative(), _T("Button Clicked"), _T("Test"), MB_OK | MB_ICONINFORMATION);
    return true;
}

void CControlTest::OnEditTextChanged(NEdit* pEdit)
{
    NString strPos = editPos_->GetText();
    NString strRange = editRange_->GetText();

    int pos = _ttoi(strPos);
    int range = _ttoi(strRange);

    horzScroll_->SetScrollRange(range);
    horzScroll_->SetScrollPos(pos);

    vertScroll_->SetScrollRange(range);
    vertScroll_->SetScrollPos(pos);
}

void CControlTest::OnScrollEvent(NScroll* pScroll, int scrollPos)
{
    horzScroll_->SetScrollPos(scrollPos);
    vertScroll_->SetScrollPos(scrollPos);

    NString posText;
    posText.Format(_T("%d"), scrollPos);
    editPos_->SetText(posText.GetData());
}

bool CControlTest::PaintTest(NWindow* window, NRender* render, HRGN clipRgn)
{
    return false;
}
