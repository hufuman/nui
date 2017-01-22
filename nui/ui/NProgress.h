#pragma once


#include "NFrame.h"

namespace nui
{
	namespace Ui
	{
		class NUI_CLASS NProgress : public NFrame
		{
			DECLARE_REFLECTION(TEXT("nui"), TEXT("Progress"))

		public:
			NProgress();
			~NProgress();

			virtual void SetProgressRange(int range);
			virtual void SetProgressPos(int pos);

			virtual int GetProgressPos() const;
			virtual int GetProgressRange() const;

			virtual bool SetVisible(bool visible);

		protected:
			virtual void OnCreate();

			virtual Base::NSize GetAutoSize() const;

			// Draw
			virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

		private:
			void RequireDraws();

		public:
			// Event
			class ProgressEventData : public NEventData
			{
			public:
				int progressPos;
			};
			NEvent ProgressEvent;

		private:
			int progressRange_;
			int progressPos_;
		};
	}
}
