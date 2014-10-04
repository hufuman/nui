#pragma once


namespace NUI
{
    namespace Base
    {

        typedef std::basic_string<TCHAR> tstring;

        class TPoint
        {
        public:
            TPoint() : X(0), Y(0)
            {}
            TPoint(INT x, INT y) : X(x), Y(y)
            {}
            TPoint(const TPoint& p) : X(p.X), Y(p.Y)
            {}
            TPoint& operator = (const TPoint& p)
            {
                X = p.X;
                Y = p.Y;
                return (*this);
            }
            TPoint& SetPoint(INT x, INT y)
            {
                X = x;
                Y = y;
                return (*this);
            }
            bool operator == (const TPoint& point) const
            {
                return X == point.X && Y == point.Y;
            }
        public:
            INT X;
            INT Y;
        };

        class TSize
        {
        public:
            TSize() : Width(0), Height(0)
            {}
            TSize(INT width, INT height) : Width(width), Height(height)
            {}
            TSize(const TSize& p) : Width(p.Width), Height(p.Height)
            {}
            TSize& operator = (const TSize& p)
            {
                Width = p.Width;
                Height = p.Height;
                return (*this);
            }
            TSize& SetSize(INT width, INT height)
            {
                Width = width;
                Height = height;
                return (*this);
            }
            bool operator == (const TSize& size) const
            {
                return Width == size.Width && Height == size.Height;
            }
        public:
            INT Width;
            INT Height;
        };

        class TRect
        {
        public:
            TRect()
            {
                Left = Top = Right = Bottom = 0;
            }
            TRect(const TRect& rect)
            {
                Left = rect.Left;
                Top = rect.Top;
                Right = rect.Right;
                Bottom = rect.Bottom;
            }
            TRect& operator = (const TRect& rect)
            {
                if(this != &rect)
                {
                    Left = rect.Left;
                    Top = rect.Top;
                    Right = rect.Right;
                    Bottom = rect.Bottom;
                }
                return (*this);
            }
            TRect(INT left, INT top, INT right, INT bottom)
            {
                Left = left;
                Top = top;
                Right = right;
                Bottom = bottom;
            }
            TRect(const TPoint& topLeft, const TPoint& rightBottom)
            {
                Left = topLeft.X;
                Top = topLeft.Y;
                Right = rightBottom.X;
                Bottom = rightBottom.Y;
            }
            TRect(const TPoint& topLeft, const TSize& size)
            {
                Left = topLeft.X;
                Top = topLeft.Y;
                Right = Left + size.Width;
                Bottom = Top + size.Height;
            }

            const INT Width() const
            {
                return Right - Left;
            }
            const INT Height() const
            {
                return Bottom - Top;
            }

            TRect& SetRect(INT left, INT top, INT right, INT bottom)
            {
                Left = left;
                Top = top;
                Right = right;
                Bottom = bottom;
                return (*this);
            }
            TRect& SetWidth(INT width)
            {
                Right = Left + width;
                return (*this);
            }
            TRect& SetHeight(INT height)
            {
                Bottom = Top + height;
                return (*this);
            }
            TRect& SetSize(INT width, INT height)
            {
                Right = Left + width;
                Bottom = Top + height;
                return (*this);
            }

            bool operator == (const TRect& rect) const
            {
                return Left == rect.Left
                    && Top == rect.Top
                    && Right == rect.Right
                    && Bottom == rect.Bottom;
            }
        public:
            INT     Left;
            INT     Top;
            INT     Right;
            INT     Bottom;
        };

    }
}