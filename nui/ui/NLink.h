#pragma once


#include "NLabel.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NLink : public NLabel
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("link"))
        public:
            NLink();
            ~NLink();

            virtual NCursor* GetCursor() const;
            virtual void SetUrl(const Base::NString& url);

        protected:
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

            virtual void OnCreate();
            virtual void OnClicked(const nui::Base::NPoint& point);

        private:
            bool isUrl_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
