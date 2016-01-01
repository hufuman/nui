#include "stdafx.h"
#include "LayoutParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NLayout.h"

IMPLEMENT_REFLECTION(LayoutParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

LayoutParserImpl::LayoutParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NLayout>;
}

void LayoutParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    FrameParserImpl::FillAttr(targetObj, styleNode);

    NLayout* targetLayout = dynamic_cast<NLayout*>(targetObj);

    bool tmpBool;
    NString tmpString;

    if(styleNode->ReadValue(_T("scrollAllowed"), tmpBool))
    {
        targetLayout->SetScrollAllowed(tmpBool);
    }

    if(styleNode->ReadValue(_T("layoutType"), tmpString))
    {
        NLayout::NLayoutType layoutType = NLayout::LayoutHorz;
        struct
        {
            LPCTSTR name;
            NLayout::NLayoutType layoutType;
        } LayoutTypeData[] =
        {
            {   _T("horz"), NLayout::LayoutHorz     },
            {   _T("vert"), NLayout::LayoutVert     },
            {   _T("tile"), NLayout::LayoutTile     },
        };
        for(int j=0; j<_countof(LayoutTypeData); ++ j)
        {
            if(tmpString == LayoutTypeData[j].name)
            {
                layoutType = LayoutTypeData[j].layoutType;
                break;
            }
        }
        targetLayout->SetLayoutType(layoutType);
    }
}
