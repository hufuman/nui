#pragma once


namespace nui
{
    namespace Base
    {

        typedef std::basic_string<TCHAR> tstring;

        class NPoint
        {
        public:
            NPoint() : X(0), Y(0)
            {}
            NPoint(INT x, INT y) : X(x), Y(y)
            {}
            NPoint(const NPoint& p) : X(p.X), Y(p.Y)
            {}
            NPoint& operator = (const NPoint& p)
            {
                X = p.X;
                Y = p.Y;
                return (*this);
            }
            NPoint& SetPoint(INT x, INT y)
            {
                X = x;
                Y = y;
                return (*this);
            }
            NPoint& Offset(INT xOffset, int yOffset)
            {
                X += xOffset;
                Y += yOffset;
                return (*this);
            }
            bool operator == (const NPoint& point) const
            {
                return X == point.X && Y == point.Y;
            }

            operator POINT*()
            {
                return reinterpret_cast<POINT*>(this);
            }

            operator const POINT*() const
            {
                return reinterpret_cast<const POINT*>(this);
            }

            static const NPoint g_EmptyPoint;
        public:
            INT X;
            INT Y;
        };

        class NSize
        {
        public:
            NSize() : Width(0), Height(0)
            {}
            NSize(INT width, INT height) : Width(width), Height(height)
            {}
            NSize(const NSize& p) : Width(p.Width), Height(p.Height)
            {}
            NSize& operator = (const NSize& p)
            {
                Width = p.Width;
                Height = p.Height;
                return (*this);
            }
            NSize& SetSize(INT width, INT height)
            {
                Width = width;
                Height = height;
                return (*this);
            }
            bool operator == (const NSize& size) const
            {
                return Width == size.Width && Height == size.Height;
            }

            operator SIZE*()
            {
                return reinterpret_cast<SIZE*>(this);
            }

            operator const SIZE*() const
            {
                return reinterpret_cast<const SIZE*>(this);
            }

            static const NSize g_EmptySize;

        public:
            INT Width;
            INT Height;
        };

        class NRect
        {
        public:
            NRect()
            {
                Left = Top = Right = Bottom = 0;
            }
            NRect(const NRect& rect)
            {
                Left = rect.Left;
                Top = rect.Top;
                Right = rect.Right;
                Bottom = rect.Bottom;
            }
            NRect& operator = (const NRect& rect)
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
            NRect(INT left, INT top, INT right, INT bottom)
            {
                Left = left;
                Top = top;
                Right = right;
                Bottom = bottom;
            }
            NRect(const NPoint& topLeft, const NPoint& rightBottom)
            {
                Left = topLeft.X;
                Top = topLeft.Y;
                Right = rightBottom.X;
                Bottom = rightBottom.Y;
            }
            NRect(const NPoint& topLeft, const NSize& size)
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

            NRect& Offset(int cx, int cy)
            {
                Left += cx;
                Right += cx;
                Top += cy;
                Bottom += cy;
                return *this;
            }

            NRect& Inflate(int cx, int cy)
            {
                Left -= cx;
                Right += cx;
                Top -= cy;
                Bottom += cy;
                return *this;
            }

            bool Overlap(const NRect& rect) const
            {
                // center to center
                int w1 = (Left + Right);
                int h1 = (Top + Bottom);
                int w2 = (rect.Left + rect.Right);
                int h2 = (rect.Top + rect.Bottom);
                int x1 = w1 / 2 + Left;
                int y1 = h1 / 2 + Top;
                int x2 = w2 / 2 + rect.Left;
                int y2 = h2 / 2 + rect.Top;
                return (std::abs(x1 - x2) < (w1 + w2) / 2)
                    && (std::abs(y1 - y2) < (h1 + h2) / 2);
            }

            bool IsEmpty() const
            {
                return Left == Right && Top == Bottom;
            }

            bool IsZero() const
            {
                return Left == 0 && Right == 0 && Top == 0 && Bottom == 0;
            }

            void Intersect(const NRect& rect)
            {
                ::IntersectRect(reinterpret_cast<LPRECT>(this), reinterpret_cast<const RECT*>(this), reinterpret_cast<const RECT*>(&rect));
            }

            NRect& SetRect(INT left, INT top, INT right, INT bottom)
            {
                Left = left;
                Top = top;
                Right = right;
                Bottom = bottom;
                return (*this);
            }
            NRect& SetWidth(INT width)
            {
                Right = Left + width;
                return (*this);
            }
            NRect& SetHeight(INT height)
            {
                Bottom = Top + height;
                return (*this);
            }
            NRect& SetPos(INT left, INT top)
            {
                Right = left + Right - Left;
                Bottom = top + Bottom - Top;
                Left = left;
                Top = top;
                return (*this);
            }
            NRect& SetSize(INT width, INT height)
            {
                Right = Left + width;
                Bottom = Top + height;
                return (*this);
            }
            bool Contains(INT x, INT y) const
            {
                return x >= Left && x <= Right
                    && y >= Top && y <= Bottom;
            }
            bool Contains(const NPoint& pt) const
            {
                return pt.X >= Left && pt.X <= Right
                    && pt.Y >= Top && pt.Y <= Bottom;
            }

            operator RECT*()
            {
                return reinterpret_cast<RECT*>(this);
            }

            operator const RECT*() const
            {
                return reinterpret_cast<const RECT*>(this);
            }

            NPoint& GetLeftTop()
            {
                return *reinterpret_cast<NPoint*>(this);
            }

            const NPoint& GetLeftTop() const
            {
                return *reinterpret_cast<NPoint*>(const_cast<NRect*>(this));
            }

            NSize GetSize() const
            {
                NSize size;
                size.SetSize(Right - Left, Bottom - Top);
                return size;
            }

            bool operator == (const NRect& rect) const
            {
                return Left == rect.Left
                    && Top == rect.Top
                    && Right == rect.Right
                    && Bottom == rect.Bottom;
            }

            static const NRect g_EmptyRect;
        public:
            INT     Left;
            INT     Top;
            INT     Right;
            INT     Bottom;
        };

    }
}