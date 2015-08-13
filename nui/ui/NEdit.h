#pragma once


#include "NRichFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NUI_CLASS NEdit;
        typedef FastDelegate1<NEdit*, void> EditTextChangeEventCallback;

        class NUI_CLASS NEdit : public NWndUi<NRichFrame>
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

            // Event
            virtual void SetTextChangeCallback(EditTextChangeEventCallback callback);

            // NRichFrame
            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText();

        protected:
            virtual bool OnWndMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            virtual void OnWindowChanged(NWindow* window);
            bool IsEditValid() const;

            // NFrame
            virtual bool SetPosImpl(int left, int top, bool force);
            virtual bool SetSizeImpl(int width, int height, bool force);

        protected:
            HWND editWindow_;
            EditTextChangeEventCallback textChangeCallback_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
