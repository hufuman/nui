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
            text_->SetSingleLine(false);
        }

        NRenderStatus::~NRenderStatus()
        {
            ;
        }

        void NRenderStatus::BeforeDraw()
        {
            lastTime_ = ::GetTickCount();
        }

        void NRenderStatus::DrawStatus(NRender* render, const Base::NRect& rcClient, HRGN clipRgn)
        {
            DWORD currentTime = ::GetTickCount();

            Base::NRect invalidRect;
            ::GetRgnBox(clipRgn, invalidRect);

            Base::NString status;
            if(currentTime <= lastTime_)
                status.Format(_T("FPS: MAX\r\nClipRect: (%d, %d) - (%d, %d)"), invalidRect.Left, invalidRect.Top, invalidRect.Right, invalidRect.Bottom);
            else
                status.Format(_T("FPS: %.2f\r\nClipRect: (%d, %d) - (%d, %d)"), (double)1000 / (currentTime - lastTime_), invalidRect.Left, invalidRect.Top, invalidRect.Right, invalidRect.Bottom);

            lastTime_ = currentTime;
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
