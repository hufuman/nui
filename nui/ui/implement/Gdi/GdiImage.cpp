#include "stdafx.h"
#include "GdiImage.h"

#include "GdiResourceLoader.h"

namespace nui
{
    namespace Ui
    {
        GdiImage::GdiImage(NResourceLoader* loader)
            : BaseImage(loader)
        {
            vctBitmaps_ = NULL;
            vctDelayCount_ = NULL;
        }

        GdiImage::~GdiImage()
        {
            Destroy();
        }

        LPVOID GdiImage::LockBits()
        {
            return NULL;
        }

        void GdiImage::UnlockBits(LPVOID bits)
        {
            UNREFERENCED_PARAMETER(bits);
        }

        int GdiImage::NextDelayValue(int index)
        {
            NAssertError(index < vctDelayCount_->Count(), _T("Out of bound"));
            return (*vctDelayCount_)[index];
        }

        void GdiImage::SetBitmaps(const Base::NString& imagePath, const Data::NArrayT<HBITMAP>& vctBitmaps, const Data::NArrayT<int>& vctDelayCount)
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

        HBITMAP GdiImage::GetHBitmap(int index) const
        {
            NAssertError(index < frameCount_, _T("index out of bound"));
            return (*vctBitmaps_)[index];
        }

        void GdiImage::Destroy()
        {
            if(loader_ == NULL || imagePath_.IsEmpty())
                return;
            GdiResourceLoader* gdiLoader = dynamic_cast<GdiResourceLoader*>(loader_);
            gdiLoader->ReleaseImage(imagePath_);
            imagePath_ = _T("");
        }
    }
}
