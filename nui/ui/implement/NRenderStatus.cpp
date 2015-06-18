#include "stdafx.h"
#include "../NRenderStatus.h"

namespace nui
{
    namespace Ui
    {
        NRenderStatus::NRenderStatus()
        {
            lastTime_ = 0;
            text_ = NUiBus::Instance().GetResourceLoader()->CreateText(_T(""), MemToolParam);
        }

        NRenderStatus::~NRenderStatus()
        {
            ;
        }

        void NRenderStatus::BeforeDraw()
        {
            lastTime_ = ::GetTickCount();
        }

        void NRenderStatus::DrawStatus(NRender* render, const Base::NRect& rcClient)
        {
            DWORD currentTime = ::GetTickCount();
            Base::NString status;

            if(currentTime == lastTime_)
            {
                status = _T("FPS: max");
            }
            else
            {
                status.Format(_T("FPS: %.2f"), (double)1000 / (currentTime - lastTime_));
            }
            text_->SetText(status.GetData());
            Base::NSize size;
            render->GetTextSize(text_, NULL, size);
            Base::NRect rcText;
            rcText.SetPos(rcClient.Right - size.Width - 20, 20);
            rcText.SetSize(size.Width, size.Height);

            render->AddDrawRegion(rcText);
            render->DrawText(text_, NULL, rcText);
        }
    }
}
