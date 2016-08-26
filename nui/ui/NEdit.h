#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NEdit : public NNative
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("edit"))
        public:

            enum TooltipIconType
            {
                IconNone    = 0,
                IconError   = 3,
                IconInfo    = 1,
                IconWarning = 2,
            };

            NEdit();
            ~NEdit();

            void SetHintText(LPCTSTR szHintText);
            void SetReadOnly(bool readOnly);
            void SelectAll();
            void ShowTooltip(TooltipIconType iconType, LPCTSTR szTitle, LPCTSTR szText);

            virtual Base::NSize GetAutoSize() const;

        protected:
            virtual void OnCreate();
            virtual NCursor* GetCursor() const;

            virtual bool OnParentCommand(WORD notifyCode);
            virtual bool GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle);
            virtual void OnRealWindowCreated();

        private:
            Base::NString hintText_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
