#pragma once


#include "../NImage.h"


namespace nui
{
    namespace Ui
    {
        class BaseImage : public NImage
        {
        public:
            BaseImage(NResourceLoader* loader);
            ~BaseImage();

            virtual INT Release();

            virtual bool NextFrame();
            virtual int GetFrameCount() const;

            virtual const Base::NSize& GetSize() const ;
            virtual bool SetCount(int horzCount, int vertCount);
            virtual void GetCount(int& horzCount, int& vertCount) const;
            virtual bool SetIndex(int horzIndex, int vertIndex);
            virtual void GetIndex(int& horzIndex, int& vertIndex) const;

            void InitForDynamicImage(int frameCount, const Base::NSize& size);
            void InitForStaticImage(int horzCount, int vertCount, const Base::NSize& size);
            virtual void Destroy() = 0;

        protected:
            Base::NSize size_;
            int frameCount_;
            int frameIndex_;
            int horzCount_;
            int vertCount_;
            int horzIndex_;
            int vertIndex_;
            NResourceLoader* loader_;
        };
    }
}
