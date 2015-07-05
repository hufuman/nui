#pragma once



#include "NWindowBase.h"
#include "NRender.h"
#include "NRichFrame.h"
#include "NRenderStatus.h"
#include "../base/NInstPtr.h"
#include "../base/NAutoPtr.h"

namespace nui
{
    namespace Ui
    {
        class NWindow;
        typedef FastDelegate3<NWindow*, NRender*, HRGN, bool> WindowDrawCallback;

        class NUI_CLASS NWindow : public Base::NBaseObj, public NWindowBase
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("window"))

        public:
            NWindow();
            ~NWindow();

            NRichFrame* GetRootFrame();
            NRender* GetRender() const;

            void SetDrawCallback(WindowDrawCallback callback);

        protected:
            virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            virtual void OnSize(int width, int height);
            virtual void OnDraw(NRender* render, HRGN clipRgn);

            virtual void Draw(HDC hDc);
            void SetHoverItem(NFrame* frame);
            void RefreshHoverItem(const Base::NPoint& point);

        protected:

        BEGIN_USE_UNEXPORT_TEMPLATE()
            Base::NAutoPtr<NRichFrame> rootFrame_;
            Base::NAutoPtr<NRender> render_;
            WindowDrawCallback drawCallback_;

            Base::NAutoPtr<NFrame> hoverFrame_;
            Base::NAutoPtr<NFrame> selectedFrame_;

            NRenderStatus renderStatus_;

            Base::NRect sizableBorder_;
        END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
