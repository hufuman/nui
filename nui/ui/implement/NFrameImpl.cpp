#include "stdafx.h"

#include "../NFrame.h"

#include "../NRenderClip.h"
#include "../../base/NInstPtr.h"

#ifndef _NO_NUI_PARSER_
#include "../../parser/NParser.h"
#include "../../parser/implement/ParserUtil.h"
#endif // _NO_NUI_PARSER_

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NFrame);

        NFrame::NFrame()
        {
            cursorType_ = NCursor::CursorDefault;
            bkgDraw_ = NULL;
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagAutoSize, true);
        }

        NFrame::~NFrame()
        {
            bkgDraw_ = NULL;
            foreDraw_ = NULL;
        }

        void NFrame::Create(NFrame* parentFrame, LPCTSTR frameId, UINT layout, LPCTSTR frameText)
        {
            NAssertError(parentFrame != NULL, _T("Create twice"));
            SetLayoutable(false);
            frameId = frameId == NULL ? _T("") : frameId;
            frameText = frameText == NULL ? _T("") : frameText;
            SetId(frameId);
            SetText(frameText);
            SetLayout(layout);
            if(parentFrame != NULL)
                parentFrame->AddChild(this);
            SetLayoutable(true);
            OnCreate();
        }

        void NFrame::Create(NFrame* parentFrame)
        {
            Create(parentFrame, frameId_, frameRect_, GetText());
        }

        void NFrame::Create(NFrame* parentFrame, LPCTSTR frameId, const Base::NPoint& pos, LPCTSTR frameText)
        {
            SetPos(pos.X, pos.Y);
            Create(parentFrame, frameId, frameRect_, frameText);
        }

        void NFrame::Create(NFrame* parentFrame, LPCTSTR frameId, const Base::NRect& rect, LPCTSTR frameText)
        {
            SetLayoutable(false);
            if(frameId && frameId[0])
                SetId(frameId);
            if(frameText && frameText[0])
                SetText(frameText);
            SetPos(rect.Left, rect.Top);
            SetSize(rect.Width(), rect.Height());
            if(parentFrame != NULL)
                parentFrame->AddChild(this);
            SetLayoutable(true);
            OnCreate();
        }

#ifndef _NO_NUI_PARSER_
        bool NFrame::ApplyStyle(const Base::NString& style)
        {
            Base::NInstPtr<Parser::NParser> parser(MemToolParam);
            return parser->ApplyStyle(this, style);
        }
#endif // _NO_NUI_PARSER_

        void NFrame::OnCreate()
        {
            ;
        }

        void NFrame::OnParentChanged()
        {
            __super::OnParentChanged();
            if(GetParent() != NULL)
                AutoSize();
        }

        void NFrame::OnClicked(const nui::Base::NPoint& point)
        {
            // sometimes, you need to destroy window in button's click event
            //  add ref to avoid crash because of freed this pointer
            AddRef();
            ClickEventData eventData(point);
            ClickEvent.Invoke(this, &eventData);
            OnMouseUp();
            Release();
        }

        void NFrame::OnFocus()
        {
            ;
        }

        void NFrame::OnLostFocus()
        {
            ;
        }

        void NFrame::SetText(const Base::NString& text)
        {
            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            nui::Base::NString readText = stringBundle->GetString(text);

            if(text_ == readText)
                return;

            text_ = readText;
            TextChangedEvent.Invoke(this, NULL);
            AutoSize();
            Invalidate();
        }

        Base::NString NFrame::GetText() const
        {
            return text_;
        }

        NTextAttr* NFrame::GetTextAttr() const
        {
            NTextAttr* result = NULL;
            TextAttrMap::const_iterator ite = textAttrMap_.find(frameStatus_);
            if(ite == textAttrMap_.end())
                ite = textAttrMap_.find(NFrame::StatusNormal);

            if(ite != textAttrMap_.end())
                result = ite->second;
            return result;
        }

        NTextAttr* NFrame::GetTextAttr(UINT status, bool create)
        {
            NTextAttr* result = NULL;
            TextAttrMap::iterator ite = textAttrMap_.find(status);
            if(ite == textAttrMap_.end())
            {
                if(create)
                {
                    result = NUiBus::Instance().GetResourceLoader()->CreateText(MemToolParam);
                    textAttrMap_.insert(std::make_pair(status, result));
                }
            }
            else
            {
                result = ite->second;
            }
            return result;
        }

        void NFrame::SetCursor(NCursor::CursorType type)
        {
            cursorType_ = type;
        }

        NCursor* NFrame::GetCursor() const
        {
            if(cursorType_ == NCursor::CursorDefault)
                return NULL;
            return GetCursorByType(cursorType_);
        }

        void NFrame::SetTooltip(const Base::NString &tooltip)
        {
            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            nui::Base::NString realTooltip = stringBundle->GetString(tooltip);

            if(tooltip_ == realTooltip)
                return;

            tooltip_ = realTooltip;
        }

        Base::NString NFrame::GetTooltip() const
        {
            return tooltip_;
        }

        NFont* NFrame::GetFont(bool needCreate)
        {
            if(font_ == NULL && needCreate)
            {
                font_ = NUiBus::Instance().GetResourceLoader()->CreateFont(MemToolParam);
            }
            return font_;
        }

        NFont* NFrame::GetFont() const
        {
            return font_;
        }

        void NFrame::SetFont(NFont* font)
        {
            font_ = font;
        }

#ifndef _NO_NUI_PARSER_
        void NFrame::SetFont(const Base::NString& fontName)
        {
            Base::NInstPtr<Parser::NParser> parser(MemToolParam);
            nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode = parser->FindStyleNode(fontName);
            SetFont(ParserUtil::ParseFont(styleNode));
        }
#endif // _NO_NUI_PARSER_

        Base::NSize NFrame::GetAutoSize() const
        {
            Base::NSize autoSize;
            if(!IsLayoutable() || window_ == NULL || window_->GetRender() == NULL)
                return autoSize;

            if(text_.IsEmpty() && foreDraw_ == NULL && bkgDraw_ == NULL)
            {
                return GetAutoSize(autoSize);
            }

#undef max

            if(!text_.IsEmpty())
                window_->GetRender()->GetTextSize(text_, GetTextAttr(), font_, autoSize);

            if(foreDraw_ != NULL)
            {
                Base::NSize foreSize = foreDraw_->GetPreferSize();
                autoSize.Width = std::max(autoSize.Width, foreSize.Width);
                autoSize.Height = std::max(autoSize.Height, foreSize.Height);
            }

            if(bkgDraw_ != NULL)
            {
                Base::NSize bkgSize = bkgDraw_->GetPreferSize();
                autoSize.Width = std::max(autoSize.Width, bkgSize.Width);
                autoSize.Height = std::max(autoSize.Height, bkgSize.Height);
            }
            return GetAutoSize(autoSize);
        }

        Base::NSize NFrame::GetAutoSize(const Base::NSize& size) const
        {
#undef min
#undef max
            Base::NSize result(size);
            if(maxSize_.Width != 0)
                result.Width = std::min(result.Width, maxSize_.Width);
            if(maxSize_.Height != 0)
                result.Height = std::min(result.Height, maxSize_.Height);
            if(minSize_.Width != 0)
                result.Width = std::max(result.Width, minSize_.Width);
            if(minSize_.Height != 0)
                result.Height = std::max(result.Height, minSize_.Height);
            return result;
        }

        NFrame* NFrame::GetChildById(const Base::NString& id, bool recursive)
        {
            return dynamic_cast<NFrame*>(__super::GetChildById(id, recursive));
        }

        NFrame* NFrame::GetChildAtIndex(size_t index)
        {
            if(index >= childs_.size())
                return NULL;
            FrameList::iterator ite = childs_.begin();
            for(size_t i=0; i<index; ++ i)
                ++ ite;
            return dynamic_cast<NFrame*>(*ite);
        }

        NFrame* NFrame::GetParent() const
        {
            return dynamic_cast<NFrame*>(__super::GetParent());
        }

        void NFrame::SetBkgDraw(NDraw* bkgDraw)
        {
            if(bkgDraw == bkgDraw_)
                return;
            bkgDraw_ = bkgDraw;
            AutoSize();
            Invalidate();
        }

#ifndef _NO_NUI_PARSER_
        void NFrame::SetBkgDraw(const Base::NString& drawName)
        {
            SetBkgDraw(ParserUtil::ParseDraw(drawName));
        }
#endif  // _NO_NUI_PARSER_

        NDraw* NFrame::GetBkgDraw() const
        {
            return bkgDraw_;
        }

        void NFrame::SetForeDraw(NDraw* foreDraw)
        {
            if(foreDraw == foreDraw_)
                return;
            foreDraw_ = foreDraw;
            AutoSize();
            Invalidate();
        }

#ifndef _NO_NUI_PARSER_
        void NFrame::SetForeDraw(const Base::NString& drawName)
        {
            SetForeDraw(ParserUtil::ParseDraw(drawName));
        }
#endif // _NO_NUI_PARSER_

        NDraw* NFrame::GetForeDraw() const
        {
            return foreDraw_;
        }

        // Draw
        void NFrame::DrawBkg(NRender* render, const Base::NRect& rect) const
        {
            if(bkgDraw_ == NULL)
                return;
            int horzIndex, vertIndex;
            GetDrawIndex(horzIndex, vertIndex);
            bkgDraw_->Draw(render, horzIndex, vertIndex, rect);
        }

        void NFrame::DrawFore(NRender* render, const Base::NRect& rect) const
        {
            if(foreDraw_ == NULL)
                return;
            int horzIndex, vertIndex;
            GetDrawIndex(horzIndex, vertIndex);
            foreDraw_->Draw(render, horzIndex, vertIndex, rect);
        }

        void NFrame::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            if(!text_.IsEmpty())
                render->DrawText(text_, GetTextAttr(), font_, rect);
        }

    }
}
