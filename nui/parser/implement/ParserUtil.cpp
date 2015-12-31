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

    NAutoPtr<NBaseObj> LoadObj(NBaseObj* parentObj, Base::NAutoPtr<Data::NDataReader> styleNode)
    {
        NString objName = styleNode->GetNodeName();
        objName.MakeLower();

        NString parserName = objName + _T("parser");
        NAutoPtr<NBaseParser> parser = dynamic_cast<NBaseParser*>(NReflect::GetInstance().Create(parserName, MemToolParam));
        NAssertError(parser != NULL, _T("Parser not found, Name: %s"), parserName.GetData());
        if(parser == NULL)
            return NULL;

        Base::NAutoPtr<NBaseObj> targetObj = parser->Alloc();
        parser->PreParse(targetObj, styleNode);
        parser->Create(parentObj, targetObj);
        parser->FillAttr(targetObj, styleNode);
        parser->PostParse(targetObj, styleNode);
        parser->Release();
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

        parser->PreParse(targetObj, styleNode);
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

        if(prefix == _T("image"))
        {
            return Ui::NUiBus::Instance().GetResourceLoader()->LoadImage(stylePath);
        }
        else if(prefix == _T("shape"))
        {
            Base::NInstPtr<Parser::NParser> parser(MemToolParam);
            NAutoPtr<NDataReader> styleNode = parser->FindStyleNode(stylePath);
            return ParseShapeDraw(styleNode);
        }
        NAssertError(false, _T("Invalid styleName in ParseDraw: %s"), styleName.GetData());
        return NULL;
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
        Base::NAutoPtr<Ui::NFont> font = Ui::NUiBus::Instance().GetResourceLoader()->CreateFont(MemToolParam);

        INT32 tmpLong;
        NString tmpString;
        bool tmpBool;

        if(styleNode->ReadValue(_T("fontSize"), tmpLong))
            font->SetFontSize(tmpLong);

        if(styleNode->ReadValue(_T("bold"), tmpBool))
            font->SetBold(tmpBool);

        if(styleNode->ReadValue(_T("italic"), tmpBool))
            font->SetItalic(tmpBool);

        if(styleNode->ReadValue(_T("underline"), tmpBool))
            font->SetUnderline(tmpBool);

        if(styleNode->ReadValue(_T("strikeOut"), tmpBool))
            font->SetStrikeOut(tmpBool);

        if(styleNode->ReadValue(_T("fontName"), tmpString))
            font->SetFontName(tmpString);

        return font;
    }
}