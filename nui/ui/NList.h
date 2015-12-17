#pragma once


#include "NFrame.h"
#include "NHeader.h"
#include "NLayout.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NList : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("list"))

        public:
            enum NListFlags
            {
                ListFlagNone = 0x00,
                ListFlagNoSort = 0x01,
                ListFlagFixedWidth = 0x02,
            };

        public:
            NList();
            ~NList();

        public:
            virtual int AddColumn(int columnIndex, LPCTSTR text, int width);
            virtual int AddColumn(int columnIndex, LPCTSTR text, int width, UINT flags);
            virtual void RemoveColumn(int columnIndex);

            virtual int AddItem(int lineIndex);
            virtual void RemoveItem(int lineIndex);
            virtual void RemoveAllItems();

            virtual NFrame* GetLineFrame(int lineIndex);
            virtual NFrame* GetSubFrame(int lineIndex, int columnIndex);
            virtual int GetItemCount() const;

        public:
            class SortEventData : public NEventData
            {
            public:
                int itemIndex;
                bool sortAsc;
            };
            NEvent SortEvent;

        protected:
            virtual void OnCreate();
            virtual void OnSize(int width, int height);

            void GetSubFrameId(int columnIndex, Base::NString& subFrameId) const;

            bool OnAdjustLinewWidths(NFrameBase* child, LPARAM lParam);

            bool OnHeaderItemWidthChanged(Base::NBaseObj*, NEventData* eventData);
            bool OnHorzScrollPosChanged(Base::NBaseObj*, NEventData* eventData);
            bool OnHeaderSort(Base::NBaseObj*, NEventData* eventData);

            void ResetLineFrameWidth();
            int GetLineFrameWidth() const;

        private:
            Base::NAutoPtr<NHeader> header_;
            Base::NAutoPtr<NLayout> layout_;

            DWORD columnId_;
            int normalLineHeight_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
