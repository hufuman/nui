#pragma once

#include "FrameParserImpl.h"


class ProgressParserImpl : public FrameParserImpl
{
	DECLARE_REFLECTION(TEXT("nui"), TEXT("progressparser"))
public:
	ProgressParserImpl();

	virtual bool IsCorrectObj(nui::Base::NBaseObj* targetObj);
	virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
