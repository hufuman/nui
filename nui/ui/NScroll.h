#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
            class NUI_CLASS NScroll;
            typedef FastDelegate2<NScroll*, int, void> ScrollEventCallback;
        END_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NScroll : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("scroll"))

            enum ScrollPart
            {
                ScrollPartNone,
                ScrollPartLeftBlock,
                ScrollPartRightBlock,
                ScrollPartSlider,
                ScrollPartLeftBkg,
                ScrollPartRightBkg,
            };

        public:
            NScroll();
            ~NScroll();

            // Event
            virtual void SetScrollCallback(ScrollEventCallback callback);

            // Data
            virtual void SetScrollType(bool horzScroll);

            virtual void SetScrollRange(int range);
            virtual void SetScrollPos(int pos);
            virtual void SetScrollLine(int line);
            virtual void SetScrollPage(int page);

            virtual int GetScrollPos() const;
            virtual int GetScrollRange() const;
            virtual int GetScrollLine() const;
            virtual int GetScrollPage() const;

        protected:
            // Pos & Size
            virtual Base::NSize GetAutoSize() const;

            // Draw
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

            virtual void OnMouseDown(int x, int y);
            virtual void OnMouseUp();
            virtual void OnMouseLeave();
            virtual void OnMouseMove(int x, int y);

            virtual void SetBlockDraw(LPCTSTR filePath);
            virtual void SetSliderDraw(LPCTSTR filePath);
            virtual void SetBkgDraw(LPCTSTR filePath);

            void PosChangeTimerProc();

        private:
            void RequireDraws();

            Base::NSize GetHorzAutoSize() const;
            void GetHorzPartRect(const Base::NRect& rect, Base::NRect& leftBlock, Base::NRect& rightBlock, Base::NRect& bkg, Base::NRect& slider) const;

            Base::NSize GetVertAutoSize() const;
            void GetVertPartRect(const Base::NRect& rect, Base::NRect& topBlock, Base::NRect& bottomBlock, Base::NRect& bkg, Base::NRect& slider) const;

            void RefreshCapturePart(int x, int y);
            void ResetCapturePart();
            void RefreshHoverPart(int x, int y);
            void ResetHoverPart();
            ScrollPart FindPart(int x, int y);

        private:
            int scrollRange_;
            int scrollPos_;
            int scrollLine_;
            int scrollPage_;
            bool horzScroll_;

            ScrollPart hoverPart_;
            ScrollPart capturedPart_;

            nui::Base::NHolder firstTimer_;
            nui::Base::NHolder continousTimer_;

            BEGIN_USE_UNEXPORT_TEMPLATE()

            ScrollEventCallback scrollCallback_;
            Base::NPoint startPoint_;
            Base::NAutoPtr<NImageDraw> blockDraw_;
            Base::NAutoPtr<NImageDraw> sliderDraw_;
            Base::NAutoPtr<NImageDraw> scrollBkgDraw_;

            END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
