#include "stdafx.h"
#include "../NProgress.h"

#include "../NRenderClip.h"
#include "../../util/NShellUtil.h"


#undef max
#undef min

namespace nui
{
	namespace Ui
	{
		const int g_minSliderWidth = 8;

		IMPLEMENT_REFLECTION(NProgress);

		NProgress::NProgress()
		{
			progressRange_ = 100;
			progressPos_ = 0;

			RequireDraws();
			ReLayout();
		}

		NProgress::~NProgress()
		{
		}

		void NProgress::SetProgressRange(int range)
		{
			if (progressRange_ == range)
				return;
			NAssertError(range > 0, _T("why you use progress since range is zero?"));
			if (range < 1)
				return;
			progressRange_ = range;
			if (progressPos_ >= progressRange_)
				SetProgressPos(progressRange_);
			Invalidate();
		}

		void NProgress::SetProgressPos(int pos)
		{
			NAssertError(pos >= 0 && pos <= progressRange_, _T("wrong pos in NProgress?"));
			if (pos < 0)
				pos = 0;
			if (pos >= progressRange_)
				pos = progressRange_;
			if (progressPos_ == pos)
				return;

			progressPos_ = pos;

			ProgressEventData eventData;
			eventData.progressPos = progressPos_;
			ProgressEvent.Invoke(this, &eventData);
			Invalidate();
		}

		int NProgress::GetProgressPos() const
		{
			return progressPos_;
		}

		int NProgress::GetProgressRange() const
		{
			return progressRange_;
		}

		bool NProgress::SetVisible(bool visible)
		{
			bool result = __super::SetVisible(visible);

			if (result && visible)
			{
				ProgressEventData eventData;
				eventData.progressPos = progressPos_;
				ProgressEvent.Invoke(this, &eventData);
			}

			return result;
		}

		void NProgress::OnCreate()
		{
			__super::OnCreate();

#ifndef _NO_NUI_PARSER_
			NAssertTempDisable();
			ApplyStyle(_T("@sys_default_style:progress"));
#endif  // _NO_NUI_PARSER_
		}

		Base::NSize NProgress::GetAutoSize() const
		{
			Base::NSize autoSize;

			if (GetBkgDraw())
				autoSize = GetBkgDraw()->GetPreferSize();

			return autoSize;
		}

		void NProgress::DrawContent(NRender* render, const Base::NRect& rect) const
		{		
			if (GetBkgDraw() == NULL)
				return;
			Base::NRect foreRect = rect;
			foreRect.SetWidth((INT)(foreRect.Width() * ((float)progressPos_ / (float)progressRange_)));
			GetBkgDraw()->Draw(render, 0, 1, foreRect);

			nui::Base::NString text;
			text.Format(_T("%d%%"), progressPos_);
			render->DrawText(text, GetTextAttr(), GetFont(), rect);

			__super::DrawContent(render, rect);
		}

		void NProgress::RequireDraws()
		{
			NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
			SetBkgDraw(loader->LoadImage(_T("@skin:common\\Progress.png")));
		}
	}
}
