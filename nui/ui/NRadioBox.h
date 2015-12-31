#pragma once


#include "NCheckBox.h"

namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NRadioBox : public NCheckBox
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("radiobox"))
        public:
            NRadioBox();
            ~NRadioBox();

            virtual void SetGroup(const Base::NString& groupName);

        public:
            virtual bool SetCheck(bool checked);

        protected:
            virtual void OnCreate();
            virtual void OnClicked(const nui::Base::NPoint& point);

            bool SyncRadioGroups(NFrameBase* frameBase, LPARAM lParam);

        private:
            Base::NString groupName_;
        };
    }
}
