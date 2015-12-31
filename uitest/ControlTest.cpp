#include "StdAfx.h"
#include "ControlTest.h"

CControlTest::CControlTest(void)
{
}

CControlTest::~CControlTest(void)
{
}

const int g_ListColumnCount = 3;

void CControlTest::Test()
{
    NString tmp;

    // create window
    window_.Create(MemToolParam);
    // window_->Create(NULL, WindowStyle::Layered);
    window_->Create(NULL, WindowStyle::Sizable | WindowStyle::Top);
    window_->SetSize(520, 420);
    window_->CenterWindow(NULL);
    window_->SetText(_T("Test Window"));
    window_->SetVisible(true);
    window_->PostDrawEvent.AddHandler(MakeDelegate(this, &CControlTest::PaintTest));

    // setup controls
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    NAutoPtr<NShapeDraw> pBkgDraw = loader->CreateShape(MemToolParam);
    pBkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 255, 255, 0));
    window_->GetRootFrame()->SetBkgDraw(pBkgDraw);

    // NList
    //*
    NInstPtr<NList> pList(MemToolParam);
    list_ = pList;
    pList->Create(window_->GetRootFrame(), _T("list1"), NFrame::LayoutHFill | NFrame::LayoutVFill, NULL);
    pList->SetMargin(10, 10, 10, 10);
    pList->SetBkgDraw(CreateRectDraw(MakeArgb(255, 255, 255, 255)));
    const int ListLineCount = 1;
    for(int i=0; i<g_ListColumnCount; ++ i)
    {
        tmp.Format(_T("Header%d"), i);
        pList->AddColumn(i, tmp.GetData(), (i + 1) * 40, 0);
    }
    for(int lineIndex=0; lineIndex<ListLineCount; ++ lineIndex)
    {
        AddLine(pList);
    }

    NInstPtr<NButton> btnAdd(MemToolParam);
    btnAdd->Create(window_->GetRootFrame(), _T("btnAdd"), NFrame::LayoutLeft | NFrame::LayoutBottom, _T("Click To Add "));
    btnAdd->GetTextAttr()->SetColor(MakeArgb(255, 255, 255, 0));
    btnAdd->ClickEvent.AddHandler(MakeDelegate(this, &CControlTest::OnButtonClicked));

    NInstPtr<NButton> btnRemove(MemToolParam);
    btnRemove->Create(window_->GetRootFrame(), _T("btnRemove"), NFrame::LayoutLeft | NFrame::LayoutBottom, _T("Click To Remove"));
    btnRemove->ClickEvent.AddHandler(MakeDelegate(this, &CControlTest::OnButtonClicked));
    btnRemove->GetTextAttr()->SetColor(MakeArgb(255, 255, 255, 0));
    btnRemove->SetMargin(90, 0, 0,0);
    //*/

    /*
    // Layout
    NInstPtr<NLayout> pLayout1(MemToolParam);
    pLayout1->SetId(_T("TestLayout"));
    pLayout1->SetPos(150, 50);
    pLayout1->SetAutoSize(false);
    pLayout1->SetSize(310, 300);
    pLayout1->SetLayoutType(NLayout::LayoutVert);

    pBkgDraw = loader->CreateShape(MemToolParam);
    pBkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 0, 255, 255));
    pLayout1->SetBkgDraw(pBkgDraw);

    NString msg;
    pLayout1->SetLayoutable(false);
    for(int i=0; i<4; ++ i)
    {
        NInstPtr<NButton> pChild(MemToolParam);
        pChild->GetTextAttr()->SetColor(MakeArgb(255, 255, 255, 0));
        pChild->SetId(_T("child"));
        pChild->SetAutoSize(false);
        pChild->SetSize(100, 20);
        pChild->SetMargin(5, 10, 15, 20);
        msg.Format(_T("Item %d"), pLayout1->GetChildById(_NUI_INNER_FRAME_ID_, false)->GetChildCount());
        pChild->SetText(msg.GetData());
        if(i == 8)
            printf("");
        pLayout1->AddChild(pChild);
    }
    pLayout1->SetLayoutable(true);
    window_->GetRootFrame()->AddChild(pLayout1);
    // */

    /*
    // VertScroll
    NInstPtr<NScroll> pVertScroll(MemToolParam);
    pVertScroll->SetPos(100, 200);
    pVertScroll->SetSize(16, 200);
    pVertScroll->SetScrollRange(4);
    pVertScroll->SetScrollPos(3);
    pVertScroll->SetScrollPage(1);
    pVertScroll->SetLayout(NFrame::LayoutVFill);
    window_->GetRootFrame()->AddChild(pVertScroll);
    vertScroll_ = pVertScroll;
    vertScroll_->ScrollEvent.AddHandler(MakeDelegate(this, &CControlTest::OnScrollEvent));

    // HorzScroll
    NInstPtr<NScroll> pHorzScroll(MemToolParam);
    pHorzScroll->SetPos(100, 180);
    pHorzScroll->SetSize(200, 16);
    pHorzScroll->SetScrollType(true);
    pHorzScroll->SetScrollRange(4);
    pHorzScroll->SetScrollPos(3);
    pHorzScroll->SetScrollPage(1);
    pHorzScroll->SetLayout(NFrame::LayoutHFill);
    window_->GetRootFrame()->AddChild(pHorzScroll);
    horzScroll_ = pHorzScroll;
    horzScroll_->ScrollEvent.AddHandler(MakeDelegate(this, &CControlTest::OnScrollEvent));

    NInstPtr<NFrame> posLabel(MemToolParam);
    posLabel->SetPos(10, 100);
    posLabel->SetText(_T("Pos: "));
    window_->GetRootFrame()->AddChild(posLabel);

    NInstPtr<NFrame> rangeLabel(MemToolParam);
    rangeLabel->SetPos(10, 130);
    rangeLabel->SetText(_T("Range: "));
    window_->GetRootFrame()->AddChild(rangeLabel);

    NInstPtr<NFrame> pageLabel(MemToolParam);
    pageLabel->SetPos(10, 160);
    pageLabel->SetText(_T("Page: "));
    window_->GetRootFrame()->AddChild(pageLabel);

    NInstPtr<NEdit> pEdit1(MemToolParam);
    pEdit1->SetLayout(NFrame::LayoutLeft | NFrame::LayoutTop);
    pEdit1->SetMargin(60, 100, 0, 0);
    pEdit1->SetAutoSize(false);
    pEdit1->SetSize(100, 18);
    pEdit1->SetText(_T("3"));
    pEdit1->TextChangeEvent.AddHandler(MakeDelegate(this, &CControlTest::OnEditTextChanged));
    window_->GetRootFrame()->AddChild(pEdit1);
    editPos_ = pEdit1;

    NInstPtr<NEdit> pEdit2(MemToolParam);
    pEdit2->SetLayout(NFrame::LayoutLeft | NFrame::LayoutTop);
    pEdit2->SetMargin(60, 130, 0, 0);
    pEdit2->SetAutoSize(false);
    pEdit2->SetSize(100, 18);
    pEdit2->SetText(_T("4"));
    pEdit2->TextChangeEvent.AddHandler(MakeDelegate(this, &CControlTest::OnEditTextChanged));
    window_->GetRootFrame()->AddChild(pEdit2);
    editRange_ = pEdit2;

    NInstPtr<NEdit> pEdit3(MemToolParam);
    pEdit3->SetLayout(NFrame::LayoutLeft | NFrame::LayoutTop);
    pEdit3->SetMargin(60, 160, 0, 0);
    pEdit3->SetAutoSize(false);
    pEdit3->SetSize(100, 18);
    pEdit3->SetText(_T("1"));
    pEdit3->TextChangeEvent.AddHandler(MakeDelegate(this, &CControlTest::OnEditTextChanged));
    window_->GetRootFrame()->AddChild(pEdit3);
    editPage_ = pEdit3;

    /*
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
    */

    // loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window_->GetNative());

    // destroy
    editPos_ = NULL;
    editRange_ = NULL;
    horzScroll_ = NULL;
    vertScroll_ = NULL;
    list_ = NULL;
    window_ = NULL;
}

bool CControlTest::OnButtonClicked(NBaseObj* pButton, NEventData* eventData)
{
    NButton* pBtnTest = dynamic_cast<NButton*>(pButton);
    if(pBtnTest->GetId() == _T("btnAdd"))
    {
        AddLine(list_);
    }
    else if(pBtnTest->GetId() == _T("btnRemove"))
    {
        int count = list_->GetItemCount();
        if(count > 0)
            list_->RemoveItem(rand() % count);
    }

    /*
    NButton* pBtnTest = dynamic_cast<NButton*>(pButton);
    NLayout* pLayout = window_->GetRootFrame()->GetChildById<NLayout*>(_T("TestLayout"), false);
    if(pBtnTest->GetId() == _T("btnAdd"))
    {
        NButton* pChild = pLayout->GetChildById<NButton*>(_T("child"), true);
        if(pChild)
            pChild->SetSize(pChild->GetRect().Width() + 20, pChild->GetRect().Height());

        NInstPtr<NButton> pChild(MemToolParam);
        pChild->GetTextAttr()->SetColor(MakeArgb(255, 255, 255, 0));
        pChild->SetId(_T("child"));
        pChild->SetAutoSize(false);
        pChild->SetSize(100, 20);
        pChild->SetMargin(5, 10, 15, 20);
        NString msg;
        msg.Format(_T("Item %d"), pLayout->GetChildById(_NUI_INNER_FRAME_ID_, false)->GetChildCount());
        pChild->SetText(msg);
        pLayout->AddChild(pChild);
    }
    else if(pBtnTest->GetId() == _T("btnRemove"))
    {
        NButton* pChild = pLayout->GetChildById<NButton*>(_T("child"), true);
        if(pChild)
            pLayout->RemoveChild(pChild);
        NButton* pChild = pLayout->GetChildById<NButton*>(_T("child"), true);
        if(pChild)
            pChild->SetSize(pChild->GetRect().Width() - 20, pChild->GetRect().Height());
    }
    */

    return true;
}

bool CControlTest::OnEditTextChanged(NBaseObj* pButton, NEventData* eventData)
{
    UNREFERENCED_PARAMETER(pButton);
    UNREFERENCED_PARAMETER(eventData);
    NString strPos = editPos_->GetText();
    NString strRange = editRange_->GetText();
    NString strPage = editPage_->GetText();

    int pos = _ttoi(strPos);
    int range = _ttoi(strRange);
    int page = _ttoi(strPage);

    horzScroll_->SetScrollPos(pos);
    horzScroll_->SetScrollRange(range);
    horzScroll_->SetScrollPage(page);

    vertScroll_->SetScrollPos(pos);
    vertScroll_->SetScrollRange(range);
    vertScroll_->SetScrollPage(page);
    return true;
}

bool CControlTest::OnScrollEvent(NBaseObj* baseObj, NEventData* eventData)
{
    NScroll::ScrollEventData* data = static_cast<NScroll::ScrollEventData*>(eventData);
    horzScroll_->SetScrollPos(data->scrollPos);
    vertScroll_->SetScrollPos(data->scrollPos);

    NString posText;
    posText.Format(_T("%d"), data->scrollPos);
    editPos_->SetText(posText.GetData());
    return true;
}

bool CControlTest::PaintTest(NBaseObj* baseObj, NEventData* eventData)
{
    return false;
}

void CControlTest::AddLine(NList* pList)
{
    int index = pList->AddItem(0);
    NFrame* lineFrame = pList->GetLineFrame(index);
    lineFrame->SetBkgDraw(CreateRectDraw(MakeArgb(255, (BYTE)rand(), (BYTE)rand(), (BYTE)rand())));

    NString tmp;
    for(int columnIndex=0; columnIndex<g_ListColumnCount; ++ columnIndex)
    {
        NFrame* subFrame = pList->GetSubFrame(index, columnIndex);
        subFrame->SetBkgDraw(CreateRectDraw(MakeArgb(255, (BYTE)rand(), (BYTE)rand(), (BYTE)rand())));
        tmp.Format(_T("||%d, %d||"), index, columnIndex);
        subFrame->SetText(tmp);
    }
}
