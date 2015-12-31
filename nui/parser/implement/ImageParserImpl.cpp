#include "stdafx.h"
#include "ImageParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NImage.h"

IMPLEMENT_REFLECTION(ImageParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

ImageParserImpl::ImageParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NImage>;
}

void ImageParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    FrameParserImpl::FillAttr(targetObj, styleNode);

    NString tmpString;
    NSize tmpSize;
    NImage* targetImage = dynamic_cast<NImage*>(targetObj);

    if(styleNode->ReadValue(_T("imagePath"), tmpString))
        targetImage->LoadImage(tmpString);

    if(styleNode->ReadValue(_T("imageIndex"), tmpSize))
        targetImage->SetIndex(tmpSize.Width, tmpSize.Height);
}
