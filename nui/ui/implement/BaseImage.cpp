#include "stdafx.h"
#include "BaseImage.h"


namespace nui
{
    namespace Ui
    {
        BaseImage::BaseImage(NResourceLoader* loader)
        {
            frameCount_ = 0;
            horzCount_ = 1;
            vertCount_ = 1;
            loader_ = loader;
        }

        BaseImage::~BaseImage()
        {
            ;
        }

        INT BaseImage::Release()
        {
            if(RefCount() == 1)
            {
                Destroy();
            }
            return NImage::Release();
        }

        int BaseImage::GetFrameCount() const
        {
            return frameCount_;
        }

        const Base::NSize& BaseImage::GetSize() const
        {
            return size_;
        }

        bool BaseImage::SetCount(int horzCount, int vertCount)
        {
            if(horzCount_ == horzCount && vertCount_ == vertCount)
                return false;

            horzCount_ = horzCount;
            vertCount_ = vertCount;
            return true;
        }

        void BaseImage::GetCount(int& horzCount, int& vertCount) const
        {
            horzCount = horzCount_;
            vertCount = vertCount_;
        }

        void BaseImage::InitForDynamicImage(int frameCount, const Base::NSize& size)
        {
            horzCount_ = 1;
            vertCount_ = 1;
            frameCount_ = frameCount;
            size_ = size;
        }

        void BaseImage::InitForStaticImage(int horzCount, int vertCount, const Base::NSize& size)
        {
            horzCount_ = horzCount;
            vertCount_ = vertCount;
            frameCount_ = 1;
            size_ = size;
        }
    }
}
