#pragma once


#include "./NRender.h"
#include "../base/BaseObj.h"
#include "../base/NString.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NFrame;
        typedef FastDelegate2<NFrame*, LPARAM, bool> UiContainerEnumCallback;
        END_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NFrame : public nui::Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("frame"))
        public:
            // childs manipulations
            virtual bool AddChild(NFrame* child) = 0;
            virtual bool RemoveChild(NFrame* child) = 0;
            virtual void RemoveAllChilds() = 0;
            virtual size_t SetChildZOrder(NFrame* child, size_t zorder) = 0;
            virtual size_t GetChildZOrder(NFrame* child) const = 0;
            virtual void SetChildTopmost(NFrame* child) = 0;
            virtual void SetChildBottommost(NFrame* child) = 0;
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const = 0;
            virtual NFrame* GetChildById(const Base::NString& id) = 0;

            virtual NFrame* GetParent() const = 0;

            // flags
            virtual void SetVisible(bool visible) = 0;
            virtual bool IsVisible() const = 0;
            virtual void SetEnabled(bool enabled) = 0;
            virtual bool IsEnabled() const = 0;
            virtual void SetAutoSize(bool autosize) = 0;
            virtual bool IsAutoSize() const = 0;
            virtual void SetValid(bool valid) = 0;
            virtual bool IsValid() const = 0;

            // data
            virtual void SetText(const Base::NString& text) = 0;
            virtual Base::NString GetText() const = 0;
            virtual void SetId(const Base::NString& id) = 0;
            virtual Base::NString GetId() const = 0;

            // pos / size
            virtual const Base::NRect& GetRect() const = 0;
            virtual Base::NRect GetRootRect() const = 0;
            virtual void AutoSize() = 0;
            virtual void SetPos(int left, int top) = 0;
            virtual void SetSize(int width, int height) = 0;
            virtual void SetMinSize(int minWidth, int minHeight) = 0;

            // draw
            virtual void Invalidate() = 0;
            virtual void Draw(NRender* render, const Base::NRect& clipRect) = 0;
        };
    }
}
