#include "stdafx.h"
#include "FrameParserImpl.h"

#include "ParserUtil.h"

IMPLEMENT_REFLECTION(FrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

FrameParserImpl::FrameParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NFrame>;
}

bool FrameParserImpl::IsCorrectObj(nui::Base::NBaseObj* targetObj)
{
    return targetObj != NULL && dynamic_cast<NFrame*>(targetObj) != NULL;
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

    if(styleNode->ReadValue(_T("minSize"), tmpSize))
        targetFrame->SetMinSize(tmpSize.Width, tmpSize.Height);

    if(styleNode->ReadValue(_T("maxSize"), tmpSize))
        targetFrame->SetMaxSize(tmpSize.Width, tmpSize.Height);

    if(styleNode->ReadValue(_T("margin"), tmpRect))
        targetFrame->SetMargin(tmpRect.Left, tmpRect.Top, tmpRect.Right, tmpRect.Bottom);

    if(styleNode->ReadValue(_T("padding"), tmpRect))
        targetFrame->SetPadding(tmpRect.Left, tmpRect.Top, tmpRect.Right, tmpRect.Bottom);

    if(styleNode->ReadValue(_T("text"), tmpString))
        targetFrame->SetText(tmpString);

    if(styleNode->ReadValue(_T("tooltip"), tmpString))
        targetFrame->SetTooltip(tmpString);

    if(styleNode->ReadValue(_T("id"), tmpString))
        targetFrame->SetId(tmpString);

    if(styleNode->ReadValue(_T("pos"), tmpPoint))
        targetFrame->SetPos(tmpPoint.X, tmpPoint.Y);

    if(styleNode->ReadValue(_T("size"), tmpSize))
    {
        if(targetFrame->IsAutoSize())
            targetFrame->SetAutoSize(false);
        targetFrame->SetSize(tmpSize.Width, tmpSize.Height);
    }

    if(styleNode->ReadValue(_T("font"), tmpString))
    {
        targetFrame->SetFont(tmpString);
    }

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
        if(tmpString == _T("center"))
        {
            layout = NFrameBase::LayoutHCenter | NFrameBase::LayoutVCenter;
        }
        else
        {
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
        }
        targetFrame->SetLayout(layout);
    }
}

void FrameParserImpl::Create(nui::Base::NBaseObj* parentObj, nui::Base::NBaseObj* targetObj)
{
    NAutoPtr<NFrame> targetFrame = dynamic_cast<NFrame*>(targetObj);
    NAutoPtr<NFrame> parentFrame = dynamic_cast<NFrame*>(parentObj);
    targetFrame->Create(parentFrame);

    targetFrame->SetLayoutable(false);
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
            result = (ParserUtil::LoadObj(newObj_, targetObj, childNode) != NULL);
            UNREFERENCED_PARAMETER(result);
            NAssertError(result, _T("Failed to LoadObj in FrameParser"));
        }
    }

    targetFrame->SetLayoutable(true);
}

void FrameParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    NString tmpString;
    bool bFlag;
    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj);

    if(styleNode->ReadValue(_T("visible"), bFlag))
        targetFrame->SetVisible(bFlag);

    if(styleNode->ReadValue(_T("checked"), bFlag))
        targetFrame->SetCheck(bFlag);

    if(styleNode->ReadValue(_T("enabled"), bFlag))
        targetFrame->SetEnabled(bFlag);

    if(styleNode->ReadValue(_T("text"), tmpString))
        targetFrame->SetText(tmpString);

    if(styleNode->ReadValue(_T("foreDraw"), tmpString))
    {
        targetFrame->SetForeDraw(tmpString);
    }

    if(styleNode->ReadValue(_T("bkgDraw"), tmpString))
    {
        targetFrame->SetBkgDraw(tmpString);
    }

    if(styleNode->ReadValue(_T("cursor"), tmpString))
    {
        struct
        {
            LPCTSTR cursorName;
            NCursor::CursorType cursorType;
        } cursorDatas[] = 
        {
            {   _T("arrow"), NCursor::CursorArrow },
            {   _T("hand"), NCursor::CursorHand },
            {   _T("upDown"), NCursor::CursorUpDown },
            {   _T("leftRight"), NCursor::CursorLeftRight },
            {   _T("cross"), NCursor::CursorCross },
            {   _T("slash"), NCursor::CursorSlash },
            {   _T("backSlash"), NCursor::CursorBackSlash },
            {   _T("beam"), NCursor::CursorBeam },
        };
        for(int i=0; i<_countof(cursorDatas); ++ i)
        {
            if(tmpString == cursorDatas[i].cursorName)
            {
                targetFrame->SetCursor(cursorDatas[i].cursorType);
                break;
            }
        }
    }

    nui::Base::NAutoPtr<nui::Ui::NFont> font = ParserUtil::ParseFont(styleNode);
    if(font)
        targetFrame->SetFont(font);

    FillTextAttr(targetFrame, styleNode);
}

void FrameParserImpl::FillTextAttr(nui::Ui::NFrame* targetFrame, nui::Data::NDataReader* styleNode)
{
    NString tmpString;
    ArgbColor color;
    bool bFlag;

    NString name;

    struct
    {
        LPCTSTR prefix;
        UINT status;
    } textAttrData[] =
    {
        { _T("text"), NFrame::StatusNormal },
        { _T("hoverText"), NFrame::StatusHover },
        { _T("pressedText"), NFrame::StatusHover | NFrame::StatusPressed },
        { _T("disabledText"), NFrame::StatusDisabled },

        { _T("chkText"), NFrame::StatusChecked },
        { _T("chkHoverText"), NFrame::StatusHover | NFrame::StatusChecked },
        { _T("chkPressedText"), NFrame::StatusHover | NFrame::StatusPressed | NFrame::StatusChecked },
        { _T("chkDisabledText"), NFrame::StatusDisabled | NFrame::StatusChecked },
    };

    for(int i=0; i<_countof(textAttrData); ++ i)
    {
        name.Format(_T("%sColor"), textAttrData[i].prefix);
        if(styleNode->ReadValue(name, color))
            targetFrame->GetTextAttr(textAttrData[i].status, true)->SetColor(color);

        name.Format(_T("%sBgColor"), textAttrData[i].prefix);
        if(styleNode->ReadValue(name, color))
            targetFrame->GetTextAttr(textAttrData[i].status, true)->SetBgColor(color);

        name.Format(_T("%sSingleLine"), textAttrData[i].prefix);
        if(styleNode->ReadValue(name, bFlag))
            targetFrame->GetTextAttr(textAttrData[i].status, true)->SetSingleLine(bFlag);

        name.Format(_T("%sAlign"), textAttrData[i].prefix);
        if(styleNode->ReadValue(name, tmpString))
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
            targetFrame->GetTextAttr(textAttrData[i].status, true)->SetAlignFlags(alignFlag);
        }
    }
}
