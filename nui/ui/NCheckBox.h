#pragma once


#include "NButton.h"

namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NCheckBox : public NButton
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("checkbox"))
        public:
            NCheckBox();
            ~NCheckBox();

        public:
            virtual void SetScaleImage(bool scaleImage);

        public:
            virtual Base::NSize GetAutoSize() const;

        protected:
            virtual void DrawFore(NRender* render, const Base::NRect& rect) const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

        protected:
            virtual void OnCreate();
            virtual void OnClicked(const nui::Base::NPoint& point);

        private:
            bool scaleImage_;
        };
    }
}
