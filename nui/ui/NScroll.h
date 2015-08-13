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
            virtual void DrawFore(NRender* render, const Base::NRect& rect) const;

        private:
            void requireDraws();

        private:
            int scrollRange_;
            int scrollPos_;
            bool horzScroll_;
        };
    }
}
