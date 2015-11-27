#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NScroll : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("scroll"))
        public:
            NScroll();
            ~NScroll();

            virtual void SetScrollRange(int range);
            virtual void SetScrollPos(int pos);
            virtual void SetScrollType(bool horzScroll);

        protected:
            // Pos & Size
            virtual Base::NSize GetAutoSize() const;

            // Draw
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;
        private:
            void requireDraws();

            Base::NSize GetHorzAutoSize() const;
            Base::NSize GetVertAutoSize() const;

            void DrawHorzContent(NRender* render, const Base::NRect& rect) const;
            void DrawVertContent(NRender* render, const Base::NRect& rect) const;

        private:
            int scrollRange_;
            int scrollPos_;
            bool horzScroll_;

            BEGIN_USE_UNEXPORT_TEMPLATE()
            Base::NAutoPtr<NImageDraw> blockDraw_;
            Base::NAutoPtr<NImageDraw> sliderDraw_;
            Base::NAutoPtr<NImageDraw> scrollBkgDraw_;
            END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
