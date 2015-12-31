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

        class NUI_CLASS NWindow;

        class NUI_CLASS NFrame : public NFrameBase
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("frame"))

            friend class NWindow;

        public:
            NFrame();
            ~NFrame();

        public:

            virtual void Create(NFrame* parentFrame);
            virtual void Create(NFrame* parentFrame, LPCTSTR frameId, UINT layout, LPCTSTR frameText);
            virtual void Create(NFrame* parentFrame, LPCTSTR frameId, const Base::NPoint& pos, LPCTSTR frameText);
            virtual void Create(NFrame* parentFrame, LPCTSTR frameId, const Base::NRect& rect, LPCTSTR frameText);

#ifndef _NO_NUI_PARSER_
            virtual bool ApplyStyle(const Base::NString& style);
#endif  // _NO_NUI_PARSER_

            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText();
            virtual NCursor* GetCursor() const;

            virtual NFont* GetFont();
            virtual void SetFont(NFont* font);
#ifndef _NO_NUI_PARSER_
            virtual void SetFont(const Base::NString& fontName);
#endif  // _NO_NUI_PARSER_

            // draw
            virtual void SetBkgDraw(NDraw* bkgDraw);
#ifndef _NO_NUI_PARSER_
            virtual void SetBkgDraw(const Base::NString& drawName);
#endif  // _NO_NUI_PARSER_
            virtual NDraw* GetBkgDraw() const;

            // Normal, Hover, Down, Disabled
            // Checked
            // HalfChecked
            virtual void SetForeDraw(NDraw* foreDraw);
#ifndef _NO_NUI_PARSER_
            virtual void SetForeDraw(const Base::NString& drawName);
#endif  // _NO_NUI_PARSER_
            virtual NDraw* GetForeDraw() const;

            // Pos & Size
            virtual Base::NSize GetAutoSize() const;

            // Childs
            template < typename T >
            T GetChildById(const Base::NString& id)
            {
                return dynamic_cast<T>(GetChildById(id, true));
            }

            template < typename T >
            T GetChildById(const Base::NString& id, bool recursive)
            {
                return dynamic_cast<T>(GetChildById(id, recursive));
            }

            NFrame* GetChildById(const Base::NString& id)
            {
                return GetChildById(id, true);
            }

            virtual NFrame* GetChildById(const Base::NString& id, bool recursive);
            virtual NFrame* GetChildAtIndex(size_t index);
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

        public:
            // Event
            class ClickEventData : public NEventData
            {
            public:
                ClickEventData(const Base::NPoint& p) : point(p)
                {
                }
                ClickEventData& operator = (const ClickEventData& d);
                const Base::NPoint& point;;
            };
            NEvent ClickEvent;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
