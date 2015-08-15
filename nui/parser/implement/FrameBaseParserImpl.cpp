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

    NString strTmp;
    NPoint pos;
    NSize size;
    NRect rect;
    bool bFlag;
    NFrameBase* targetFrame = dynamic_cast<NFrameBase*>(target);

    // parser autosize first, or size=xxx will fail
    if(styleNode->ReadValue(_T("autoSize"), bFlag))
        targetFrame->SetAutoSize(bFlag);

    if(styleNode->ReadValue(_T("id"), strTmp))
        targetFrame->SetId(strTmp.GetData());

    if(styleNode->ReadValue(_T("pos"), pos))
        targetFrame->SetPos(pos.X, pos.Y);

    if(styleNode->ReadValue(_T("size"), size))
        targetFrame->SetSize(size.Width, size.Height);

    if(styleNode->ReadValue(_T("size"), size))
        targetFrame->SetMinSize(size.Width, size.Height);

    if(styleNode->ReadValue(_T("margin"), rect))
        targetFrame->SetMargin(rect.Left, rect.Top, rect.Right, rect.Bottom);

    if(styleNode->ReadValue(_T("layout"), strTmp))
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
        for(int i=0; strTmp.Tokenize(i, _T(","), false, token) ; ++i)
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
}

void FrameBaseParserImpl::FillAttr()
{
    NFrameBase* targetFrame = dynamic_cast<NFrameBase*>(targetObj_);

    bool bFlag;
    if(styleNode_->ReadValue(_T("visible"), bFlag))
        targetFrame->SetVisible(bFlag);

    if(styleNode_->ReadValue(_T("enabled"), bFlag))
        targetFrame->SetEnabled(bFlag);
}

void FrameBaseParserImpl::PostParse()
{
    NAutoPtr<NFrameBase> targetFrame = dynamic_cast<NFrameBase*>(targetObj_);

    bool result = false;
    for(int i=0; ; ++ i)
    {
        NAutoPtr<NDataReader> childNode;
        if(!styleNode_->ReadNode(i, childNode))
            break;
        result = (ParserUtil::LoadObj(targetObj_, childNode) != NULL);
        UNREFERENCED_PARAMETER(result);
        NAssertError(result, _T("Failed to LoadObj in FrameBaseParser"));
    }
    targetFrame->Invalidate();
}
