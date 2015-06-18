#pragma once


#include "./NRender.h"
#include "./NDraw.h"
#include "../base/NString.h"
#include "../base/DataTypes.h"
#include "../base/NAutoPtr.h"
#include "../data/NArrayT.h"

namespace nui
{
    namespace Ui
    {

        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NRichFrame : public nui::Ui::NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("richframe"))
        public:
            NRichFrame();
            ~NRichFrame();

        public:
            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText() const;

            // draw
            virtual void SetBkgDraw(NDraw* bkgDraw);
            virtual NDraw* GetBkgDraw() const;

            // Normal, Hover, Down, Disabled
            // Checked
            // HalfChecked
            virtual void SetForeDraw(NDraw* foreDraw);
            virtual NDraw* GetForeDraw() const;

            // Pos & Size
            virtual void SetAutoSize(bool autosize);
            virtual void AutoSize();
            virtual Base::NSize GetAutoSize() const;
            virtual bool IsAutoSize() const;

        protected:
            virtual void OnParentChanged();

            // Draw
            virtual void DrawBkg(NRender* render, const Base::NRect& rect) const;
            virtual void DrawFore(NRender* render, const Base::NRect& rect) const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

            virtual void SetSizeImpl(int width, int height, bool force);

        private:
            Base::NAutoPtr<NDraw> bkgDraw_;
            Base::NAutoPtr<NDraw> foreDraw_;

            Base::NAutoPtr<NText> text_;
            Base::NAutoPtr<NFont> font_;
         };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
