#include "stdafx.h"
#include "BaseImageDraw.h"


namespace nui
{
    namespace Ui
    {
        BaseImageDraw::BaseImageDraw(NResourceLoader* loader)
        {
            frameIndex_ = 0;
            frameCount_ = 0;
            horzCount_ = 1;
            vertCount_ = 1;
            loader_ = loader;
        }

        BaseImageDraw::~BaseImageDraw()
        {
            ;
        }

        INT BaseImageDraw::Release()
        {
            if(RefCount() == 1)
            {
                Destroy();
            }
            return NImageDraw::Release();
        }

        void BaseImageDraw::NextFrame()
        {
            if(frameCount_ == 0)
                return;
            frameIndex_ = (++ frameIndex_) % frameCount_;
        }

        int BaseImageDraw::GetFrameCount() const
        {
            return frameCount_;
        }

        const Base::NSize& BaseImageDraw::GetSize() const
        {
            return size_;
        }

        bool BaseImageDraw::SetCount(int horzCount, int vertCount)
        {
            if(horzCount_ == horzCount && vertCount_ == vertCount)
                return false;

            horzCount_ = horzCount;
            vertCount_ = vertCount;
            return true;
        }

        void BaseImageDraw::GetCount(int& horzCount, int& vertCount) const
        {
            horzCount = horzCount_;
            vertCount = vertCount_;
        }

        void BaseImageDraw::InitForDynamicImage(int frameCount, const Base::NSize& size)
        {
            horzCount_ = 1;
            vertCount_ = 1;
            frameIndex_ = 0;
            frameCount_ = frameCount;
            size_ = size;
        }

        void BaseImageDraw::InitForStaticImage(int horzCount, int vertCount, const Base::NSize& size)
        {
            horzCount_ = horzCount;
            vertCount_ = vertCount;
            frameIndex_ = 0;
            frameCount_ = 1;
            size_ = size;
        }

        bool BaseImageDraw::IsValid() const
        {
            return frameCount_ > 0;
        }

        void BaseImageDraw::Draw(NRender* render, int horzIndex, int vertIndex, const Base::NRect& rect)
        {
            if(!IsValid())
                return;
            horzIndex = (horzIndex < 0) ? 0 : ((horzIndex >= horzCount_) ? horzCount_ - 1 : horzIndex);
            vertIndex = (vertIndex < 0) ? 0 : ((vertIndex >= vertCount_) ? vertCount_ - 1 : vertIndex);
            render->DrawImage(this, horzIndex, vertIndex, rect, frameIndex_);
        }
    }
}
