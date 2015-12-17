#pragma once



#include "NWindowBase.h"
#include "NRender.h"
#include "NRenderStatus.h"
#include "NEvent.h"
#include "../base/NInstPtr.h"
#include "../base/NAutoPtr.h"

namespace nui
{
    namespace Ui
    {
        class NFrame;

        class NUI_CLASS NWindow : public Base::NBaseObj, public NWindowBase
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("window"))

        public:
            NWindow();
            ~NWindow();

            NFrame* GetRootFrame();
            NRender* GetRender() const;

        protected:
            virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            virtual bool OnWndCmd(WPARAM wParam, LPARAM lParam);

            virtual void OnSize(int width, int height);
            virtual void OnDraw(NRender* render, HRGN clipRgn);

            virtual void Draw(HDC hDc);
            void SetHoverItem(NFrame* frame);
            NFrame* RefreshHoverItem(const Base::NPoint& point);

        public:
            // Event
            class WindowDrawEventData : public NEventData
            {
            public:
                HRGN region;
                NRender* render;
            };
            NEvent PreDrawEvent;
            NEvent PostDrawEvent;

        protected:
            BEGIN_USE_UNEXPORT_TEMPLATE()
            Base::NAutoPtr<NFrame> rootFrame_;
            Base::NAutoPtr<NRender> render_;

            Base::NAutoPtr<NFrame> hoverFrame_;

            NRenderStatus renderStatus_;

            Base::NRect sizableBorder_;
            END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
