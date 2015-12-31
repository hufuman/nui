#pragma once


#include "NFrame.h"
#include "NLayoutArranger.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NLayout : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("layout"))

        public:
            enum NLayoutType
            {
                LayoutNone,
                LayoutHorz,
                LayoutVert,
                LayoutTile,
            };

        public:
            NLayout();
            ~NLayout();

            virtual void SetLayoutType(NLayoutType layoutType);

            virtual bool AddChild(NFrameBase* child);
            virtual bool RemoveChild(NFrameBase* child);
            virtual void RemoveAllChilds();

        public:
            virtual void SetLayoutable(bool layoutable);

        protected:
            virtual void OnCreate();

            virtual Base::NSize GetAutoSize() const;
            virtual void OnSizeChanged(int width, int height);

        private:
            void RelayoutChilds();
            bool OnEnumChild(NFrameBase* child, LPARAM lParam);
            bool OnHorzScrolled(Base::NBaseObj*, NEventData* eventData);
            bool OnVertScrolled(Base::NBaseObj*, NEventData* eventData);
            bool OnChildSizeChanged(Base::NBaseObj*, NEventData* eventData);

            NScroll* GetHorzScroll();
            NScroll* GetVertScroll();

            bool IsLayoutChild(NFrameBase* child) const;

        private:
            NLayoutType currentLayoutType_;
            Base::NInstPtr<NFrame> innerFrame_;
            Base::NAutoPtr<NLayoutArranger> layoutArranger_;

            Base::NAutoPtr<NScroll> horzScroll_;
            Base::NAutoPtr<NScroll> vertScroll_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
