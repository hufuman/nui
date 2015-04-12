#pragma once



#include "./NDraw.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NImage : public NDraw
        {
        public:
            virtual const Base::NSize& GetSize() const  = 0;

            virtual int NextDelayValue(int index) = 0;
            virtual void NextFrame() = 0;
            virtual int GetFrameCount() const = 0;

            virtual bool SetCount(int horzCount, int vertCount) = 0;
            virtual void GetCount(int& horzCount, int& vertCount) const = 0;

            virtual bool SetIndex(int horzIndex, int vertIndex) = 0;
            virtual void GetIndex(int& horzIndex, int& vertIndex) const = 0;
        };
    }
}
