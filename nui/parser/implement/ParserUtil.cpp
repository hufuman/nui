#include "stdafx.h"
#include "ParserUtil.h"

#include "../../Ui/NUiBus.h"

using namespace nui;
using namespace Base;
using namespace Data;

namespace ParserUtil
{
    Base::NAutoPtr<Ui::NDraw> ParseShapeDraw(Base::NAutoPtr<NDataReader> styleNode)
    {
        Base::NAutoPtr<Ui::NShapeDraw> shape = Ui::NUiBus::Instance().GetResourceLoader()->CreateShape(MemToolParam);

        INT32 tmpInt;
        Ui::ArgbColor tmpColor;
        NString tmpString;

        if(styleNode->ReadValue(_T("style"), tmpString))
        {
            if(tmpString == _T("rect"))
                shape->SetStyle(Ui::NShapeDraw::Rect);
            else if(tmpString == _T("roundRect"))
                shape->SetStyle(Ui::NShapeDraw::RoundRect);
            else if(tmpString == _T("line"))
                shape->SetStyle(Ui::NShapeDraw::Line);
        }

        if(styleNode->ReadValue(_T("borderColor"), tmpColor))
            shape->SetBorderColor(tmpColor);

        if(styleNode->ReadValue(_T("borderWidth"), tmpInt))
            shape->SetBorderWidth(tmpInt);

        if(styleNode->ReadValue(_T("fillColor"), tmpColor))
            shape->SetFillColor(tmpColor);

        Base::NAutoPtr<Ui::NDraw> result = dynamic_cast<Ui::NDraw*>((Ui::NShapeDraw*)shape);
        return result;
    }

    bool FillUiStyile(nui::Base::NBaseObj* targetObj, nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode)
    {
        Base::NString styleName;
        if(!styleNode->ReadValue(_T("UiStyle"), styleName))
            return false;

        Base::NInstPtr<Parser::NParser> parser(MemToolParam);

        NAutoPtr<NDataReader> node = parser->FindStyleNode(styleName);
        return ApplyStyle(targetObj, node);
    }

    NAutoPtr<NBaseObj> LoadObj(bool newObj, NBaseObj* parentObj, Base::NAutoPtr<Data::NDataReader> styleNode)
    {
        NString objName = styleNode->GetNodeName();
        objName.MakeLower();

        NString parserName = objName + _T("parser");
        NAutoPtr<NBaseParser> parser = dynamic_cast<NBaseParser*>(NReflect::GetInstance().Create(parserName, MemToolParam));
        NAssertError(parser != NULL, _T("Parser not found, Name: %s"), parserName.GetData());
        if(parser == NULL)
            return NULL;

        parser->SetNewObj(newObj);

        NString frameId;
        Base::NAutoPtr<NBaseObj> targetObj;
        bool needAllocObj = true;
        if(!newObj && styleNode->ReadValue(_T("id"), frameId))
        {
            nui::Ui::NFrame* frame = dynamic_cast<nui::Ui::NFrame*>(parentObj);
            nui::Ui::NFrame* targetFrame = frame->GetChildById(frameId, false);
            if(targetFrame != NULL)
            {
                targetObj = targetFrame;
                bool correctObj = parser->IsCorrectObj(targetFrame);
                NForceAssertError(correctObj, _T("Wrong Type for FrameId: %s"), frameId.GetData());
                if(!correctObj)
                    return NULL;
                needAllocObj = false;
            }
        }

        if(!targetObj)
        {
            targetObj = parser->Alloc();
        }

        parser->PreParse(targetObj, styleNode);
        if(needAllocObj)
            parser->Create(parentObj, targetObj);
        FillUiStyile(targetObj, styleNode);
        parser->FillAttr(targetObj, styleNode);
        parser->PostParse(targetObj, styleNode);
        NDelayedRelease(parser);
        if(needAllocObj)
            targetObj->Release();

        return targetObj;
    }

    bool ApplyStyle(nui::Base::NBaseObj* targetObj, nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode)
    {
        NString objName = styleNode->GetNodeName();
        objName.MakeLower();

        NString parserName = objName + _T("parser");
        NAutoPtr<NBaseParser> parser = dynamic_cast<NBaseParser*>(NReflect::GetInstance().Create(parserName, MemToolParam));
        NAssertError(parser != NULL, _T("Parser not found, Name: %s"), parserName.GetData());
        if(parser == NULL)
            return NULL;

        parser->SetNewObj(false);

        NString frameId;
        if(styleNode->ReadValue(_T("id"), frameId))
        {
            nui::Ui::NFrame* targetFrame = dynamic_cast<nui::Ui::NFrame*>(targetObj);
            if(targetFrame != NULL)
            {
                targetObj = targetFrame;
                bool correctObj = parser->IsCorrectObj(targetFrame);
                NForceAssertError(correctObj, _T("Wrong Type for FrameId: %s"), frameId.GetData());
                if(!correctObj)
                    return NULL;
            }
        }

        parser->PreParse(targetObj, styleNode);
        FillUiStyile(targetObj, styleNode);
        parser->FillAttr(targetObj, styleNode);
        parser->PostParse(targetObj, styleNode);
        parser->Release();
        return true;
    }

    NAutoPtr<nui::Ui::NDraw> ParseDraw(const NString& styleName)
    {
        int pos = styleName.IndexOf(_T(':'));
        NAssertError(pos > 0, _T("Invalid styleName in ParseDraw: %s"), styleName.GetData());
        if(pos <= 0)
            return NULL;

        NString prefix = styleName.SubString(0, pos);
        NString stylePath = styleName.SubString(pos + 1);

        NAutoPtr<nui::Ui::NDraw> result;
        if(prefix == _T("image"))
        {
            result = Ui::NUiBus::Instance().GetResourceLoader()->LoadImage(stylePath);
        }
        else if(prefix == _T("shape"))
        {
            Base::NInstPtr<Parser::NParser> parser(MemToolParam);
            NAutoPtr<NDataReader> styleNode = parser->FindStyleNode(stylePath);
            result = ParseShapeDraw(styleNode);
        }
        NAssertError(result != NULL, _T("Invalid styleName in ParseDraw: %s"), styleName.GetData());
        return result;
    }

    NAutoPtr<nui::Ui::NDraw> ParseDraw(NAutoPtr<NDataReader> styleNode)
    {
        NString drawType;
        if(!styleNode->ReadValue(_T("drawType"), drawType))
        {
            NAssertError(false, _T("Invalid styleName in ParseDraw for Node"));
            return NULL;
        }

        if(drawType == _T("image"))
        {
            NString imagePath;
            if(styleNode->ReadValue(_T("imagePath"), imagePath))
                return Ui::NUiBus::Instance().GetResourceLoader()->LoadImage(imagePath);
        }
        else if(drawType == _T("shape"))
        {
            return ParseShapeDraw(styleNode);
        }

        NAssertError(false, _T("Invalid styleName in ParseDraw for Node"));
        return NULL;
    }

    NAutoPtr<nui::Ui::NFont> ParseFont(NAutoPtr<NDataReader> styleNode)
    {
        INT32 fontSize = -1;
        Base::NString fontName;
        bool bold = false;
        bool italic = false;
        bool underline = false;
        bool strikeOut = false;

        bool hasFontSize = styleNode->ReadValue(_T("fontSize"), fontSize);
        bool hasBold = styleNode->ReadValue(_T("bold"), bold);
        bool hasItalic = styleNode->ReadValue(_T("italic"), italic);
        bool hasUnderline = styleNode->ReadValue(_T("underline"), underline);
        bool hasStrikeOut = styleNode->ReadValue(_T("strikeOut"), strikeOut);
        bool hasFontName = styleNode->ReadValue(_T("fontName"), fontName);
        if(!hasFontSize && !hasBold && !hasItalic && !hasUnderline && !hasStrikeOut && !hasFontName)
        {
            return NULL;
        }

        Base::NAutoPtr<Ui::NFont> font = Ui::NUiBus::Instance().GetResourceLoader()->CreateFont(MemToolParam);
        if(!fontName.IsEmpty() && hasFontName)
            font->SetFontName(fontName);

        if(hasStrikeOut)
            font->SetStrikeOut(strikeOut);
        if(hasUnderline)
            font->SetUnderline(underline);
        if(hasItalic)
            font->SetItalic(italic);
        if(hasBold)
            font->SetBold(bold);
        if(fontSize > 0 && hasFontSize)
            font->SetFontSize(fontSize);

        return font;
    }
}