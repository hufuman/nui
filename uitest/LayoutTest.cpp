#include "StdAfx.h"
#include "LayoutTest.h"

CLayoutTest::CLayoutTest(void)
{
    count_ = 0;
}

CLayoutTest::~CLayoutTest(void)
{
}

void CLayoutTest::Test()
{
    window_.Create(MemToolParam);
    window_->WindowCreatedEvent.AddHandler(MakeDelegate(this, &CLayoutTest::OnWindowCreated));
    window_->DoModalWithStyle(NULL, _T("@LayoutTest:TestUI"));
    window_ = NULL;
}

bool CLayoutTest::OnWindowCreated(NBaseObj*, NEventData* eventData)
{
    window_->GetRootFrame()->GetChildById(_T("btnAdd"))->ClickEvent.AddHandler(MakeDelegate(this, &CLayoutTest::OnBtnAddClicked));
    window_->GetRootFrame()->GetChildById(_T("btnRemove"))->ClickEvent.AddHandler(MakeDelegate(this, &CLayoutTest::OnBtnRemoveClicked));
    return true;
}

bool CLayoutTest::OnBtnAddClicked(NBaseObj* pButton, NEventData* eventData)
{
    NButton* pBtnTest = dynamic_cast<NButton*>(pButton);
    NLayout* pLayout = window_->GetRootFrame()->GetChildById<NLayout*>(_T("TestLayout"), false);

    NInstPtr<NParser> parser(MemToolParam);
    NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(pLayout, _T("@LayoutTest:TestChild")));
    frame->SetId(_T("child"));

    return true;
}

bool CLayoutTest::OnBtnRemoveClicked(NBaseObj* pButton, NEventData* eventData)
{
    NButton* pBtnTest = dynamic_cast<NButton*>(pButton);
    NLayout* pLayout = window_->GetRootFrame()->GetChildById<NLayout*>(_T("TestLayout"), false);

    NButton* pChild = pLayout->GetChildById<NButton*>(_T("child"), true);
    if(pChild)
        pLayout->RemoveChild(pChild);

    return true;
}
