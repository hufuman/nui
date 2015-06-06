#pragma once


#include "../NImageDraw.h"


namespace nui
{
    namespace Ui
    {
        class BaseImageDraw : public NImageDraw
        {
        public:
            BaseImageDraw(NResourceLoader* loader);
            ~BaseImageDraw();

            virtual INT Release();
            virtual const Base::NSize& GetSize() const ;

            virtual NImageDraw* NextFrame();
            virtual int GetFrameCount() const;

            virtual bool SetCount(int horzCount, int vertCount);
            virtual void GetCount(int& horzCount, int& vertCount) const;

            virtual NImageDraw* SetDrawType(ImageDrawType::Type drawType);
            virtual ImageDrawType::Type GetDrawType() const;

            virtual NImageDraw* SetStretchParam(int left, int top, int right, int bottom);
            virtual Base::NRect GetStretchParam() const;

        public:
            void InitForDynamicImage(int frameCount, const Base::NSize& size);
            void InitForStaticImage(int horzCount, int vertCount, const Base::NSize& size);
            virtual void Destroy() = 0;

            bool IsValid() const;

            // NDraw
            virtual NDraw* Draw(NRender* render, int horzIndex, int vertIndex, const Base::NRect& rect);

        protected:
            Base::NSize size_;
            int frameIndex_;
            int frameCount_;
            NResourceLoader* loader_;

            int horzCount_;
            int vertCount_;
            ImageDrawType::Type imageDrawType_;
            Base::NRect drawParam_;
        };
    }
}
