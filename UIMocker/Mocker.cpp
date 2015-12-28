#include "StdAfx.h"
#include "Mocker.h"

CMocker::CMocker(void)
{
}

CMocker::~CMocker(void)
{
}

bool CMocker::mock(const NString& styleName, NString& error)
{
    NInstPtr<NParser> parser(MemToolParam);
    frame_ = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(NULL, styleName.GetData()));
    if(!frame_)
    {
        error = _T("failed to parse style");
        return false;
    }

    NInstPtr<NWindow> window(MemToolParam);
    window->WindowCreatedEvent += MakeDelegate(this, &CMocker::onWindowCreated);
    window->SetVisible(true);
    window->DoModal(NULL);
    return true;
}

bool CMocker::onWindowCreated(NBaseObj* baseObj, NEventData*)
{
    NAutoPtr<NWindow> window = dynamic_cast<NWindow*>(baseObj);
    NFrame* rootFrame = window->GetRootFrame();
    rootFrame->SetSize(frame_->GetRect().Width(), frame_->GetRect().Height());
    rootFrame->AddChild(frame_);
    window->CenterWindow(NULL);
    return true;
}
