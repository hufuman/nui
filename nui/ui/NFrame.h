#pragma once


#include "./NRender.h"
#include "./NDraw.h"
#include "../base/NString.h"
#include "../base/DataTypes.h"
#include "../base/NAutoPtr.h"
#include "../data/NArrayT.h"

namespace nui
{
    namespace Ui
    {

        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NFrame;
        class NUI_CLASS NWindow;
        typedef FastDelegate2<NFrame*, const Base::NPoint&, bool> ClickEventCallback;

        class NUI_CLASS NFrame : public NFrameBase
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("frame"))

            friend class NWindow;

        public:
            NFrame();
            ~NFrame();

        public:

            virtual void Create(NFrame* parentFrame, LPCTSTR frameId, UINT layout, LPCTSTR frameText);
            virtual void Create(NFrame* parentFrame, LPCTSTR frameId, const Base::NPoint& pos, LPCTSTR frameText);
            virtual void Create(NFrame* parentFrame, LPCTSTR frameId, const Base::NRect& rect, LPCTSTR frameText);

            // Event
            virtual void SetClickCallback(ClickEventCallback callback);

            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText();

            // draw
            virtual void SetBkgDraw(NDraw* bkgDraw);
            virtual NDraw* GetBkgDraw() const;

            // Normal, Hover, Down, Disabled
            // Checked
            // HalfChecked
            virtual void SetForeDraw(NDraw* foreDraw);
            virtual NDraw* GetForeDraw() const;

            // Pos & Size
            virtual Base::NSize GetAutoSize() const;

            // Childs
            virtual NFrame* GetChildById(const Base::NString& id, bool recursive);
            virtual NFrame* GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags);
            virtual NFrame* GetParent() const;

        protected:
            virtual void OnCreate();
            virtual void OnParentChanged();

            virtual void OnClicked(const nui::Base::NPoint& point);

            // Draw
            virtual void DrawBkg(NRender* render, const Base::NRect& rect) const;
            virtual void DrawFore(NRender* render, const Base::NRect& rect) const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

        private:
            Base::NAutoPtr<NDraw> bkgDraw_;
            Base::NAutoPtr<NDraw> foreDraw_;

            Base::NAutoPtr<NText> text_;
            Base::NAutoPtr<NFont> font_;

            ClickEventCallback clickCallback_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
