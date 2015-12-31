#include "stdafx.h"
#include "FrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NFrame.h"

IMPLEMENT_REFLECTION(FrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

FrameParserImpl::FrameParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NFrame>;
}

void FrameParserImpl::PreParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    bool tmpBool;
    NString tmpString;
    NPoint tmpPoint;
    NSize tmpSize;
    NRect tmpRect;

    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj);
    NAssertError(targetFrame != NULL, _T("Invalid obj in FrameParser"));
    targetFrame->SetLayoutable(false);

    // parser autosize first, or size=xxx will fail
    if(styleNode->ReadValue(_T("autoSize"), tmpBool))
        targetFrame->SetAutoSize(tmpBool);

    if(styleNode->ReadValue(_T("minsize"), tmpSize))
        targetFrame->SetMinSize(tmpSize.Width, tmpSize.Height);

    if(styleNode->ReadValue(_T("margin"), tmpRect))
        targetFrame->SetMargin(tmpRect.Left, tmpRect.Top, tmpRect.Right, tmpRect.Bottom);

    if(styleNode->ReadValue(_T("text"), tmpString))
        targetFrame->SetText(tmpString);

    if(styleNode->ReadValue(_T("id"), tmpString))
        targetFrame->SetId(tmpString);

    if(styleNode->ReadValue(_T("pos"), tmpPoint))
        targetFrame->SetPos(tmpPoint.X, tmpPoint.Y);

    if(styleNode->ReadValue(_T("size"), tmpSize))
        targetFrame->SetSize(tmpSize.Width, tmpSize.Height);

    if(styleNode->ReadValue(_T("layout"), tmpString))
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
        for(int position=0; tmpString.Tokenize(position, _T(","), false, token);)
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

void FrameParserImpl::Create(nui::Base::NBaseObj* parentObj, nui::Base::NBaseObj* targetObj)
{
    NAutoPtr<NFrame> targetFrame = dynamic_cast<NFrame*>(targetObj);
    NAutoPtr<NFrame> parentFrame = dynamic_cast<NFrame*>(parentObj);
    targetFrame->Create(parentFrame);
}

void FrameParserImpl::PostParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    UNREFERENCED_PARAMETER(styleNode);
    NAutoPtr<NFrame> targetFrame = dynamic_cast<NFrame*>(targetObj);

    NFrame* parentFrame = targetFrame->GetParent();
    if(parentFrame)
    {
        bool tmpBool;
        int tmpInt;
        if(styleNode->ReadValue(_T("topMost"), tmpBool) && tmpBool)
            parentFrame->SetChildTopmost(targetFrame);
        else if(styleNode->ReadValue(_T("bottomMost"), tmpBool) && tmpBool)
            parentFrame->SetChildBottommost(targetFrame);
        else if(styleNode->ReadValue(_T("zorder"), tmpInt))
            parentFrame->SetChildZOrder(targetFrame, tmpInt);
    }

    bool result = false;
    for(int i=0; ; ++ i)
    {
        NAutoPtr<NDataReader> childNode;
        if(!styleNode->ReadNode(i, childNode))
            break;

        if(childNode->GetNodeName() == _T("ForeDraw"))
        {
            targetFrame->SetForeDraw(ParserUtil::ParseDraw(childNode));
        }
        else if(childNode->GetNodeName() == _T("BkgDraw"))
        {
            targetFrame->SetBkgDraw(ParserUtil::ParseDraw(childNode));
        }
        else if(childNode->GetNodeName() == _T("Font"))
        {
            targetFrame->SetFont(ParserUtil::ParseFont(childNode));
        }
        else
        {
            result = (ParserUtil::LoadObj(targetObj, childNode) != NULL);
            UNREFERENCED_PARAMETER(result);
            NAssertError(result, _T("Failed to LoadObj in FrameParser"));
        }
    }

    targetFrame->SetLayoutable(true);
}

void FrameParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    NString tmpString;
    ArgbColor color;
    bool bFlag;
    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj);

    if(styleNode->ReadValue(_T("visible"), bFlag))
        targetFrame->SetVisible(bFlag);

    if(styleNode->ReadValue(_T("enabled"), bFlag))
        targetFrame->SetEnabled(bFlag);

    if(styleNode->ReadValue(_T("text"), tmpString))
        targetFrame->SetText(tmpString);

    if(styleNode->ReadValue(_T("textColor"), color))
        targetFrame->GetTextAttr()->SetColor(color);

    if(styleNode->ReadValue(_T("textBgColor"), color))
        targetFrame->GetTextAttr()->SetBgColor(color);

    if(styleNode->ReadValue(_T("textSingleLine"), bFlag))
        targetFrame->GetTextAttr()->SetSingleLine(bFlag);

    if(styleNode->ReadValue(_T("textAlign"), tmpString))
    {
        NString token;
        UINT alignFlag = NTextAttr::TextAlignLeft | NTextAttr::TextAlignTop;
        struct
        {
            LPCTSTR name;
            NTextAttr::TextAlign alignFlag;
        } TextAlignData[] =
        {
            {   _T("left"), NTextAttr::TextAlignLeft          },
            {   _T("top"), NTextAttr::TextAlignTop            },
            {   _T("right"), NTextAttr::TextAlignRight        },
            {   _T("bottom"), NTextAttr::TextAlignBottom      },

            {   _T("center"), NTextAttr::TextAlignCenter      },
            {   _T("hcenter"), NTextAttr::TextAlignHCenter    },
            {   _T("vcenter"), NTextAttr::TextAlignVCenter    },
        };
        for(int position=0; tmpString.Tokenize(position, _T(","), false, token);)
        {
            for(int j=0; j<_countof(TextAlignData); ++ j)
            {
                if(token == TextAlignData[j].name)
                {
                    alignFlag |= TextAlignData[j].alignFlag;
                    break;
                }
            }
        }
        targetFrame->GetTextAttr()->SetAlignFlags(alignFlag);
    }

    if(styleNode->ReadValue(_T("foreDraw"), tmpString))
    {
        targetFrame->SetForeDraw(tmpString);
    }

    if(styleNode->ReadValue(_T("bkgDraw"), tmpString))
    {
        targetFrame->SetBkgDraw(tmpString);
    }

    if(styleNode->ReadValue(_T("font"), tmpString))
    {
        targetFrame->SetFont(tmpString);
    }

}
