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

            virtual int GetFrameCount() const;

            virtual const Base::NSize& GetSize() const ;
            virtual bool SetCount(int horzCount, int vertCount);
            virtual void GetCount(int& horzCount, int& vertCount) const;

            void InitForDynamicImage(int frameCount, const Base::NSize& size);
            void InitForStaticImage(int horzCount, int vertCount, const Base::NSize& size);
            virtual void Destroy() = 0;

        protected:
            Base::NSize size_;
            int frameCount_;
            int horzCount_;
            int vertCount_;
            NResourceLoader* loader_;
        };
    }
}
