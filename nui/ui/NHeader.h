#pragma once


#include "NFrame.h"


namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NHeader : public NFrame
        {
        public:
            enum NHeaderFlag
            {
                HeaderFlagNone      = 0x00,
                HeaderFlagSortAsc   = 0x01,
                HeaderFlagSortDesc  = 0x02,
                HeaderFlagNoSort    = 0x04,
                HeaderFlagFixedWidth = 0x08,
            };

        private:
            class NHeaderItem
            {
            public:
                UINT    flags_;
                int     width_;
                DWORD   data_;
                Base::NString text_;
                Base::NAutoPtr<NTextAttr>   textAttr_;

                NHeaderItem()
                {
                    flags_ = HeaderFlagNone;
                    width_ = 0;
                    data_ = 0;
                    textAttr_ = NULL;
                }

                NHeaderItem(const NHeaderItem& item)
                {
                    flags_ = item.flags_;
                    width_ = item.width_;
                    data_ = item.data_;
                    text_ = item.text_;
                    textAttr_ = item.textAttr_;
                }

                NHeaderItem& operator = (const NHeaderItem& item)
                {
                    flags_ = item.flags_;
                    width_ = item.width_;
                    data_ = item.data_;
                    text_ = item.text_;
                    textAttr_ = item.textAttr_;
                    return *this;
                }
            };

            DECLARE_REFLECTION(TEXT("nui"), TEXT("header"))
        public:
            NHeader();
            ~NHeader();

            virtual int AddItem(int index, LPCTSTR text, int width);
            virtual int AddItem(int index, LPCTSTR text, int width, UINT flags);
            virtual void RemoveItem(int index);
            virtual int GetItemCount() const;
            virtual int GetItemWidth(int index) const;
            virtual int GetTotalWidth() const;
            virtual void ClearAllItems();

            virtual void SetItemData(int index, DWORD data);
            virtual DWORD GetItemData(int index) const;

        protected:

            virtual void OnCreate();

            virtual void DrawBkg(NRender* render, const Base::NRect& rect) const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;
            virtual Base::NSize GetAutoSize() const;

            virtual void OnMouseDown(short x, short y);
            virtual void OnMouseUp();
            virtual void OnMouseLeave();
            virtual void OnMouseMove(short x, short y);

            virtual NCursor* GetCursor() const;

        public:
            class ItemWidthChangeEventData : public NEventData
            {
            public:
                int itemOldWidth;
                int itemWidth;
                int itemIndex;
            };
            NEvent ItemWidthChangeEvent;

            class SortEventData : public NEventData
            {
            public:
                int itemIndex;
                bool sortAsc;
            };
            NEvent SortEvent;

        private:
            void RequireDraws();
            int RefreshCurrentItem(short x, short y, bool& inDragArea) const;

        private:
            Data::NArrayT<NHeaderItem> listItems_;
            Base::NAutoPtr<NDraw> headerDraw_;
            Base::NAutoPtr<NDraw> arrowDraw_;
            int hoverItemIndex_;
            bool headerDragging_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
