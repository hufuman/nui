#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NImage : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("image"))
        public:
            NImage();
            ~NImage();

            bool LoadImage(LPCTSTR filePath);

            void Start();
            void Stop();

            void SetIndex(int horzIndex, int vertIndex);

            virtual Base::NSize GetAutoSize() const;
            virtual void DrawContent(NRender* render, const Base::NRect& rect) const;

        protected:
            void Reset();
            void MovieTimerProc();

        protected:
            Base::NHolder movieTimer_;
            int horzIndex_;
            int vertIndex_;
            int currentDelayValue_;
            Base::NAutoPtr<NImageDraw> imageDraw_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
