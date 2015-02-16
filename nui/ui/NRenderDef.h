#pragma once



namespace nui
{
    namespace Ui
    {
        class NRenderType
        {
        public:
            enum Type
            {
                GdiRender,
                D2DRender,
                SkiaRender,
                GdiplusRender,
            };
        };

        typedef DWORD ArgbColor;

        __inline BYTE GetAlpha(ArgbColor color)
        {
            return (((DWORD)color) & 0xFF000000) >> 24;
        }

        __inline ArgbColor MakeArgb(BYTE alpha, BYTE red, BYTE green, BYTE blue)
        {
            return (((ArgbColor) (red) <<  0) |
                ((ArgbColor) (green) << 8) |
                ((ArgbColor) (blue) <<   16) |
                ((ArgbColor) (alpha) << 24));
        }
    }
}