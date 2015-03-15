#pragma once



#include "NWindowBase.h"
#include "NRender.h"
#include "NFrame.h"
#include "../base/NInstPtr.h"
#include "../base/NAutoPtr.h"

namespace nui
{
    namespace Ui
    {
        class NWindow;
        typedef FastDelegate3<NWindow*, NRender*, const Base::NRect&, bool> WindowDrawCallback;

        class NUI_CLASS NWindow : public Base::NBaseObj, public NWindowBase
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("window"))
        public:
            NWindow();
            ~NWindow();

            NFrame* GetRootFrame();
            NRender* GetRender() const;

            void SetDrawCallback(WindowDrawCallback callback);

        protected:
            virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            virtual void OnSize(int width, int height);
            virtual void OnDraw(NRender* render, const Base::NRect& clipRect);

            void Draw(HDC hDc);

        protected:

        BEGIN_USE_UNEXPORT_TEMPLATE()
            Base::NAutoPtr<NFrame> rootFrame_;
            Base::NAutoPtr<NRender> render_;
            WindowDrawCallback drawCallback_;
        END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
