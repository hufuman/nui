#pragma once


#include "../../NResourceLoader.h"


namespace nui
{
    namespace Ui
    {
        class GdiResourceLoader : public NResourceLoader
        {
        public:
            GdiResourceLoader();
            ~GdiResourceLoader();

            virtual NImage* LoadImage(LPCTSTR filePath);
            virtual NShape* CreateShape(LPCSTR filePath, int line);
            virtual NText* CreateText(LPCTSTR text, LPCSTR filePath, int line);

            void ReleaseImage(const Base::NString& path);
        private:
            Gdiplus::Bitmap* LoadImageFromBuffer(Data::NBuffer* buffer);
            void ReleaseAllImages();

        private:
            struct ImageData
            {
                int refCount;
                Data::NArrayT<HBITMAP> vctBitmaps;
                Data::NArrayT<int> vctDelayTicks;
            };
            typedef std::map<Base::NString, ImageData> ImageMap;
            ImageMap    imageMap_;
        };
    }
}
