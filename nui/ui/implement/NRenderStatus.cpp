#include "stdafx.h"
#include "NRenderStatus.h"

NRenderStatus::NRenderStatus()
{
    lastTime_ = 0;
}

NRenderStatus::~NRenderStatus()
{
    ;
}

void NRenderStatus::BeforeDraw()
{
    lastTime_ = ::GetTickCount();
}

void NRenderStatus::DrawStatus(NRender* render, r)
{
    DWORD currentTime = ::GetTickCount();
    NString status;

    if(currentTime == lastTime_)
    {
        status = _T("max");
    }
    else
    {
        status.Format(_T("FPS: %.2f"), 1000 / (currentTime - lastTime_));
    }
    text_->SetText(status.GetData());
    render->DrawText(text_, NULL, 
}
