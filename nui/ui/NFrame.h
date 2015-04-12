#pragma once


#include "./NRender.h"
#include "./NDraw.h"
#include "../base/BaseObj.h"
#include "../base/NString.h"
#include "../base/DataTypes.h"
#include "../base/NAutoPtr.h"
#include "../data/NArrayT.h"

namespace nui
{
    namespace Ui
    {
        /**
         *note: child don't increase refCount of parent
         * consider such situation:
         *  NInstPtr<NFrame> parent;
         *  NInstPtr<NFrame> child;
         *  parent->AddChild(child);
         *  parent = NULL;
         *  Expect: these two NFrames are released.
         *  In fact, they are not, because refCount of them are 1 at last.
         */

        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NUI_CLASS NFrame;
        class NUI_CLASS NWindow;
        typedef FastDelegate2<NFrame*, LPARAM, bool> UiContainerEnumCallback;
        typedef FastDelegate2<NFrame*, const Base::NPoint&, bool> ClickEventCallback;

        class NUI_CLASS NFrame : public nui::Base::NBaseObj
        {
            typedef std::list<NFrame*> FrameList;

            friend class NWindow;

            enum Flag
            {
                FlagNone        = 0x0000,
                FlagVisible     = 0x0001,
                FlagEnabled     = 0x0002,
                FlagAutoSize    = 0x0004,
                FlagValid       = 0x0008,
                FlagCanHover    = 0x0010,
            };

            enum Status
            {
                StatusNormal  = 0x0001,
                StatusHover   = 0x0002,
                StatusPressed = 0x0004,
                StatusDisabled= 0x0008,
                StatusSelected= 0x0010,
                StatusChecked = 0x0020,
            };

            DECLARE_REFLECTION(TEXT("nui"), TEXT("frame"))
        public:
            NFrame();
            ~NFrame();

        public:
            // Event
            virtual void SetClickCallback(ClickEventCallback callback);

            // childs manipulations
            virtual bool AddChild(NFrame* child);
            virtual bool RemoveChild(NFrame* child);
            virtual void RemoveAllChilds();
            virtual size_t SetChildZOrder(NFrame* child, size_t zorder);
            virtual size_t GetChildZOrder(NFrame* child) const;
            virtual void SetChildTopmost(NFrame* child);
            virtual void SetChildBottommost(NFrame* child);
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const;
            virtual NFrame* GetChildById(const Base::NString& id, bool recursive);
            virtual NFrame* GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags);

            virtual NFrame* GetParent() const;

            // flags
            virtual void SetVisible(bool visible);
            virtual bool IsVisible() const;
            virtual void SetEnabled(bool enabled);
            virtual bool IsEnabled() const;
            virtual void SetAutoSize(bool autosize);
            virtual bool IsAutoSize() const;
            virtual void SetValid(bool valid);
            virtual bool IsValid() const;

            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText() const;
            virtual void SetId(const Base::NString& id);
            virtual Base::NString GetId() const;

            // pos / size
            virtual const Base::NRect& GetRect() const;
            virtual Base::NRect GetRootRect() const;
            virtual void AutoSize();
            virtual void SetPos(int left, int top);
            virtual void SetSize(int width, int height);
            virtual void SetMinSize(int minWidth, int minHeight);

            // draw
            virtual void SetBkgDraw(NDraw* bkgDraw);
            virtual NDraw* GetBkgDraw() const;
            virtual void Invalidate();
            virtual void Draw(NRender* render, Base::NPoint& ptOffset, const Base::NRect& clipRect);

        protected:
            virtual void OnParentChanged();
            virtual void OnWindowChanged(NWindow* window);

            virtual void OnClicked(const nui::Base::NPoint& point);

            // called by NWindow
            virtual void CancelHover();
            virtual void BeginHover();
            virtual bool CanHover() const;

        private:
            static void SetParentHelper(NFrame* child, NFrame* newParent);
            FrameList::const_iterator GetChildHelper(NFrame* child, size_t& zorder) const;

        private:
            size_t topMostCount_;
            size_t bottomMostCount_;
            NFrame* parentFrame_;
            FrameList childs_;

            Base::NAutoPtr<NWindow> window_;

            // NFrame::Flag
            DWORD frameFlags_;

            // NFrame::Status
            DWORD frameStatus_;

            // Background
            Base::NAutoPtr<NDraw> bkgDraw_;

            Base::NString frameId_;
            Base::NAutoPtr<NText> text_;
            Base::NAutoPtr<NFont> font_;
            Base::NRect frameRect_;
            Base::NSize minSize_;

            ClickEventCallback clickCallback_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
