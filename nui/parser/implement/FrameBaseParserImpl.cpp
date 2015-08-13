#include "stdafx.h"
#include "FrameBaseParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NFrameBase.h"

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

IMPLEMENT_REFLECTION(FrameBaseParserImpl);

void FrameBaseParserImpl::PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target)
{
    NBaseParser::PreParse(styleNode, target);
}

bool FrameBaseParserImpl::SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue)
{
    NFrameBase* targetFrame = dynamic_cast<NFrameBase*>(targetObj_);

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
        UINT layout = NFrameBase::LayoutNone;
        struct
        {
            LPCTSTR layoutName;
            NFrameBase::Layout layoutValue;
        } layoutData[] =
        {
            {   _T("left"), NFrameBase::LayoutLeft          },
            {   _T("top"), NFrameBase::LayoutTop            },
            {   _T("right"), NFrameBase::LayoutRight        },
            {   _T("bottom"), NFrameBase::LayoutBottom      },

            {   _T("hfill"), NFrameBase::LayoutHFill        },
            {   _T("vfill"), NFrameBase::LayoutVFill        },
            {   _T("hcenter"), NFrameBase::LayoutHCenter    },
            {   _T("vcenter"), NFrameBase::LayoutVCenter    },
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

void FrameBaseParserImpl::PostParse()
{
    NAutoPtr<NFrameBase> targetFrame = dynamic_cast<NFrameBase*>(targetObj_);

    for(int i=0; ; ++ i)
    {
        NAutoPtr<NDataReader> childNode;
        if(!styleNode_->ReadNode(i, childNode))
            break;
        NAutoPtr<NFrameBase> frame = dynamic_cast<NFrameBase*>((NBaseObj*)ParserUtil::LoadObj(childNode));
        targetFrame->AddChild(frame);
    }
    targetFrame->Invalidate();
}
