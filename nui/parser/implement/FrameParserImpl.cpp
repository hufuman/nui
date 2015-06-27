#include "stdafx.h"
#include "FrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NFrame.h"

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

bool FrameParserImpl::SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue)
{
    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj_);

    if(attrName == _T("id"))
    {
        targetFrame->SetId(attrValue.GetData());
    }
    else if(attrName == _T("visible"))
    {
        targetFrame->SetVisible(ParserUtil::ParseBool(attrValue.GetData()));
    }
    else if(attrName == _T("enabled"))
    {
        targetFrame->SetEnabled(ParserUtil::ParseBool(attrValue.GetData()));
    }
    else if(attrName == _T("pos"))
    {
        NPoint point;
        if(ParserUtil::ParsePoint(attrValue.GetData(), point))
            targetFrame->SetPos(point.X, point.Y);
    }
    else if(attrName == _T("size"))
    {
        NSize size;
        if(ParserUtil::ParseSize(attrValue.GetData(), size))
            targetFrame->SetSize(size.Width, size.Height);
    }
    else if(attrName == _T("minSize"))
    {
        NSize size;
        if(ParserUtil::ParseSize(attrValue.GetData(), size))
            targetFrame->SetMinSize(size.Width, size.Height);
    }
    else if(attrName == _T("autoSize"))
    {
        targetFrame->SetAutoSize(ParserUtil::ParseBool(attrValue.GetData()));
    }
    else if(attrName == _T("margin"))
    {
        NRect rect;
        if(ParserUtil::ParseRect(attrValue.GetData(), rect))
            targetFrame->SetMargin(rect.Left, rect.Top, rect.Right, rect.Bottom);
    }
    else if(attrName == _T("layout"))
    {
        NString token;
        UINT layout = NFrame::LayoutNone;
        struct
        {
            LPCTSTR layoutName;
            NFrame::Layout layoutValue;
        } layoutData[] =
        {
            {   _T("left"), NFrame::LayoutLeft          },
            {   _T("top"), NFrame::LayoutTop            },
            {   _T("right"), NFrame::LayoutRight        },
            {   _T("bottom"), NFrame::LayoutBottom      },

            {   _T("hfill"), NFrame::LayoutHFill        },
            {   _T("vfill"), NFrame::LayoutVFill        },
            {   _T("hcenter"), NFrame::LayoutHCenter    },
            {   _T("vcenter"), NFrame::LayoutVCenter    },
        };
        for(int i=0; attrValue.Tokenize(i, _T(","), false, token) ; ++i)
        {
            for(int j=0; j<_countof(layoutData); ++ j)
            {
                if(token == layoutData[j].layoutName)
                {
                    layout |= layoutData[j].layoutValue;
                    break;
                }
            }
        }
        targetFrame->SetLayout(layout);
    }
    else
    {
        return false;
    }

    return true;
}

void FrameParserImpl::PostParse()
{
    NAutoPtr<NFrame> targetFrame = dynamic_cast<NFrame*>(targetObj_);

    for(int i=0; ; ++ i)
    {
        NAutoPtr<NDataReader> childNode;
        if(!styleNode_->ReadNode(i, childNode))
            break;
        NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)ParserUtil::LoadObj(childNode));
        targetFrame->AddChild(frame);
    }
}
