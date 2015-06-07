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

            void Init();

            virtual NImageDraw* LoadImage(LPCTSTR filePath);
            virtual NShapeDraw* CreateShape(LPCSTR filePath, int line);
            virtual NText* CreateText(LPCTSTR text, LPCSTR filePath, int line);
            virtual NFont* CreateFont(int fontSize, LPCSTR filePath, int line);

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
                bool hasExtInfo;
                stImageExtInfo extInfo;
            };
            typedef std::map<Base::NString, ImageData> ImageMap;
            ImageMap    imageMap_;
        };
    }
}
