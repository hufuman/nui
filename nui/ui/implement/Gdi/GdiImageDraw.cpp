#include "stdafx.h"
#include "GdiImageDraw.h"

#include "GdiResourceLoader.h"

namespace nui
{
    namespace Ui
    {
        GdiImageDraw::GdiImageDraw(NResourceLoader* loader)
            : BaseImageDraw(loader)
        {
            vctBitmaps_ = NULL;
            vctDelayCount_ = NULL;
        }

        GdiImageDraw::~GdiImageDraw()
        {
            Destroy();
        }

        int GdiImageDraw::NextDelayValue(int index)
        {
            NAssertError(index < vctDelayCount_->Count(), _T("Out of bound"));
            int value = (*vctDelayCount_)[index];
            return value < 100 ? 100 : value;
        }

        void GdiImageDraw::SetBitmaps(const Base::NString& imagePath, const Data::NArrayT<HBITMAP>& vctBitmaps, const Data::NArrayT<int>& vctDelayCount)
        {
            Destroy();

            imagePath_ = imagePath;
            NAssertError(vctBitmaps.Count() == vctDelayCount.Count(), _T("Wrong size"));

            vctBitmaps_ = &vctBitmaps;
            vctDelayCount_ = &vctDelayCount;

            BITMAP bmp;
            ::GetObject(vctBitmaps[0], sizeof(bmp), &bmp);

            Base::NSize size(bmp.bmWidth, bmp.bmHeight);
            if(vctBitmaps_->Count() == 1)
            {
                InitForStaticImage(1, 1, size);
            }
            else
            {
                InitForDynamicImage(vctBitmaps.Count(), size);
            }
        }

        HBITMAP GdiImageDraw::GetHBitmap(int index) const
        {
            NAssertError(index < frameCount_, _T("index out of bound"));
            return (*vctBitmaps_)[index];
        }

        void GdiImageDraw::Destroy()
        {
            if(loader_ == NULL || imagePath_.IsEmpty())
                return;
            GdiResourceLoader* gdiLoader = dynamic_cast<GdiResourceLoader*>(loader_);
            gdiLoader->ReleaseImage(imagePath_);
            imagePath_ = _T("");
            vctBitmaps_ = NULL;
            vctDelayCount_ = NULL;
        }
    }
}
