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
    if(!parser->FindStyleNode(styleName))
    {
        error = _T("failed to parse style");
        return false;
    }

    NInstPtr<NWindow> window(MemToolParam);
    window->SetVisible(true);
    window->DoModalWithStyle(NULL, styleName);
    return true;
}
