#pragma once



#include "./NDraw.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        class ImageDrawType
        {
        public:
            enum Type
            {
                Stretch,
                Tile,
                NineStretch
            };
        };

        struct stImageExtInfo
        {
            ImageDrawType::Type drawType;
            int horzCount;
            int vertCount;
            int leftParam;
            int topParam;
            int rightParam;
            int bottomParam;
        };

        class NUI_INTF NImageDraw : public NDraw
        {
        public:
            virtual const Base::NSize& GetSize() const  = 0;

            virtual int NextDelayValue(int index) = 0;
            virtual NImageDraw* NextFrame() = 0;
            virtual int GetFrameCount() const = 0;

            virtual bool SetCount(int horzCount, int vertCount) = 0;
            virtual void GetCount(int& horzCount, int& vertCount) const = 0;

            virtual void SetDrawType(ImageDrawType::Type drawType) = 0;
            virtual void SetStretchParam(int left, int top, int right, int bottom) = 0;
        };
    }
}
