#pragma once


#include "../NFrame.h"
#include "../../base/NAutoPtr.h"
#include "../../data/NArrayT.h"

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
        class NFrameImpl : public nui::Ui::NFrame
        {
            typedef std::list<NFrameImpl*> FrameList;

            enum Flag
            {
                FlagNone        = 0x0000,
                FlagVisible     = 0x0001,
                FlagEnabled     = 0x0002,
                FlagAutoSize    = 0x0004,
                FlagValid       = 0x0008,
            };

        public:
            NFrameImpl();
            ~NFrameImpl();

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
            NText* GetRichText() const;
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
            virtual void Invalidate();
            virtual void Draw(NRender* render, Base::NPoint& ptOffset, const Base::NRect& clipRect);

        protected:
            virtual void OnParentChanged();

        private:
            static void SetParentHelper(NFrameImpl* childImpl, NFrameImpl* newParent);
            FrameList::const_iterator GetChildHelper(NFrameImpl* childImpl, size_t& zorder) const;

        private:
            size_t topMostCount_;
            size_t bottomMostCount_;
            NFrameImpl* parentFrame_;
            FrameList childs_;

            Base::NAutoPtr<NWindow> window_;

            // NFrameImpl::Flag
            DWORD frameFlags_;

            Base::NString frameId_;
            Base::NAutoPtr<NText> text_;
            Base::NAutoPtr<NFont> font_;
            Base::NRect frameRect_;
            Base::NSize minSize_;
        };
    }
}
