#include "StdAfx.h"
#include "UiUtil.h"


namespace UiUtil
{
    NFrame* SetFrameText(NFrame* parentFrame, LPCTSTR frameId, LPCTSTR text)
    {
        if(parentFrame == NULL)
            return NULL;
        NFrame* child = parentFrame->GetChildById(frameId);
        if(child != NULL)
            child->SetText(text);
        return child;
    }

    NImage* SetFrameImage(NFrame* parentFrame, LPCTSTR frameId, LPCTSTR imagePath)
    {
        if(parentFrame == NULL)
            return NULL;
        NImage* child = parentFrame->GetChildById<NImage*>(frameId);
        if(child != NULL)
            child->LoadImage(imagePath);
        return child;
    }
};
