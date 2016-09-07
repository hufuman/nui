#include "stdafx.h"
#include "../NImage.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NImage);

        NImage::NImage()
        {
            horzIndex_ = 0;
            vertIndex_ = 0;
            currentDelayValue_ = 0;
        }
        NImage::~NImage()
        {
            movieTimer_.Release();
        }

        bool NImage::LoadImage(LPCTSTR filePath)
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            imageDraw_ = loader->LoadImage(filePath);
            NAssertError(imageDraw_ != NULL, _T("Load Image Failed: %s"), filePath);
            if(imageDraw_ == NULL)
                return false;

            AutoSize();

            Start();

            Invalidate();

            return true;
        }

        void NImage::Start()
        {
            Reset();
            Stop();
            if(imageDraw_->GetFrameCount() <= 1)
                return;
            Base::NInstPtr<NTimerSrv> timerSrv(MemToolParam);
            currentDelayValue_ = imageDraw_->NextDelayValue(horzIndex_);
            movieTimer_ = timerSrv->startTimer(currentDelayValue_, MakeDelegate(this, &NImage::MovieTimerProc));
        }

        void NImage::Stop()
        {
            movieTimer_.Release();
        }

        void NImage::SetIndex(int horzIndex, int vertIndex)
        {
            horzIndex_ = horzIndex;
            vertIndex_ = vertIndex;
            Invalidate();
        }

        Base::NSize NImage::GetAutoSize() const
        {
            Base::NSize autoSize = __super::GetAutoSize();
            if(imageDraw_ != NULL)
            {
                Base::NSize preferSize = imageDraw_->GetPreferSize();
#undef max
                autoSize.Width = std::max(preferSize.Width, autoSize.Width);
                autoSize.Height = std::max(preferSize.Height, autoSize.Height);
            }
            return __super::GetAutoSize(autoSize);
        }

        void NImage::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            __super::DrawContent(render, rect);
            if(imageDraw_ != NULL)
                imageDraw_->Draw(render, horzIndex_, vertIndex_, rect);
        }

        void NImage::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:image"));
#endif  // _NO_NUI_PARSER_
        }

        void NImage::Reset()
        {
            horzIndex_ = 0;
            vertIndex_ = 0;
        }

        void NImage::MovieTimerProc()
        {
            imageDraw_->NextFrame();
            Invalidate();

            ++ horzIndex_;
            int horzCount, vertCount;
            imageDraw_->GetCount(horzCount, vertCount);
            horzIndex_= (horzIndex_ >= horzCount) ? 0 : horzIndex_;

            int delayValue = imageDraw_->NextDelayValue(horzIndex_);
            if(currentDelayValue_ == delayValue)
                return;

            currentDelayValue_ = delayValue;
            movieTimer_.Release();
            Base::NInstPtr<NTimerSrv> timerSrv(MemToolParam);
            movieTimer_ = timerSrv->startTimer(currentDelayValue_, MakeDelegate(this, &NImage::MovieTimerProc));
        }
    }
}
