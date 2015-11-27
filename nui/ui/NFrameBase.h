#pragma once


#include "./NRender.h"
#include "../base/BaseObj.h"
#include "../base/NString.h"
#include "../base/DataTypes.h"
#include "../base/NAutoPtr.h"
#include "../data/NArrayT.h"
#include "NWindow.h"

namespace nui
{
    namespace Ui
    {
        /**
        *note: child don't increase refCount of parent
        * consider such situation:
        *  NInstPtr<NFrameBase> parent;
        *  NInstPtr<NFrameBase> child;
        *  parent->AddChild(child);
        *  parent = NULL;
        *  Expect: these two NFrameBases are released.
        *  In fact, they are not, because refCount of them are 1 at last.
        */

        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NUI_CLASS NWindow;
        class NUI_CLASS NFrameBase;
        typedef FastDelegate2<NFrameBase*, LPARAM, bool> UiContainerEnumCallback;

        class NUI_CLASS NFrameBase : public nui::Base::NBaseObj
        {
            typedef std::list<NFrameBase*> FrameList;
            friend class NWindow;

        protected:
            enum Flag
            {
                FlagNone        = 0x0000,
                FlagVisible     = 0x0001,
                FlagEnabled     = 0x0002,
                FlagAutoSize    = 0x0004,
                FlagValid       = 0x0008,
                FlagCanHover    = 0x0010,
            };

        public:
            enum Status
            {
                StatusNormal  = 0x0001,
                StatusHover   = 0x0002,
                StatusPressed = 0x0004,
                StatusDisabled= 0x0008,
                StatusSelected= 0x0010,
                StatusChecked = 0x0020,
            };

            enum Layout
            {
                LayoutNone      = 0x0000,
                LayoutLeft      = 0x0001,
                LayoutTop       = 0x0002,
                LayoutRight     = 0x0004,
                LayoutBottom    = 0x0008,

                LayoutHCenter   = 0x0010,
                LayoutVCenter   = 0x0020,
                LayoutPosEnd    = 0x0020,

                LayoutSizeStart = 0x0040,
                LayoutHFill     = 0x0040,
                LayoutVFill     = 0x0080,
            };

            NFrameBase();

        public:
            ~NFrameBase();

        public:

            // childs manipulations
            virtual bool AddChild(NFrameBase* child);
            virtual bool RemoveChild(NFrameBase* child);
            virtual void RemoveAllChilds();
            virtual size_t SetChildZOrder(NFrameBase* child, size_t zorder);
            virtual size_t GetChildZOrder(NFrameBase* child) const;
            virtual void SetChildTopmost(NFrameBase* child);
            virtual void SetChildBottommost(NFrameBase* child);
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const;
            virtual NFrameBase* GetChildById(const Base::NString& id, bool recursive);
            virtual NFrameBase* GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags);

            virtual NFrameBase* GetParent() const;
            virtual void Invalidate() const;
            virtual void Draw(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn);

            // flags
            virtual void SetVisible(bool visible);
            virtual bool IsVisible() const;
            virtual void SetEnabled(bool enabled);
            virtual bool IsEnabled() const;
            virtual void SetValid(bool valid);
            virtual bool IsValid() const;
            virtual bool IsInStatus(Status status) const;

            // data
            virtual void SetId(const Base::NString& id);
            virtual const Base::NString& GetId() const;
            virtual NWindow* GetWindow();

            // pos / size
            virtual const Base::NRect& GetRect() const;
            virtual Base::NRect GetRootRect() const;
            virtual Base::NRect GetScreenRect() const;
            virtual void SetPos(int left, int top);
            virtual void SetSize(int width, int height);
            virtual void SetMinSize(int minWidth, int minHeight);

            virtual void SetAutoSize(bool autosize);
            virtual void AutoSize();
            virtual Base::NSize GetAutoSize() const;
            virtual bool IsAutoSize() const;

            virtual void SetMargin(int left, int top, int right, int bottom);
            virtual const Base::NRect& GetMargin() const;
            virtual void SetLayout(UINT layout);
            virtual void ReLayout();

            virtual void OnMouseDown(int x, int y);
            virtual void OnMouseUp();
            virtual void OnMouseMove(int x, int y);
            virtual void OnMouseHover();
            virtual void OnMouseLeave();

        protected:
            virtual void OnParentChanged();
            virtual void OnWindowChanged(NWindow* window);

            virtual bool CanHover() const;

            // Draw
            virtual void GetDrawIndex(int& horzIndex, int& vertIndex) const;
            virtual void DrawBkg(NRender* render, const Base::NRect& rect) const;
            virtual void DrawFore(NRender* render, const Base::NRect& rect) const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;
            virtual void DrawChilds(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn);

            virtual bool SetPosImpl(int left, int top, bool force);
            virtual bool SetSizeImpl(int width, int height, bool force);

        private:
            static void SetParentHelper(NFrameBase* child, NFrameBase* newParent);
            FrameList::const_iterator GetChildHelper(NFrameBase* child, size_t& zorder) const;

        protected:
            size_t topMostCount_;
            size_t bottomMostCount_;
            NFrameBase* parentFrame_;
            FrameList childs_;

            Base::NAutoPtr<NWindow> window_;

            // NFrameBase::Flag
            DWORD frameFlags_;

            // NFrameBase::Status
            DWORD frameStatus_;

            Base::NString frameId_;
            Base::NRect frameRect_;
            Base::NSize minSize_;
            Base::NRect margin_;
            UINT layout_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
