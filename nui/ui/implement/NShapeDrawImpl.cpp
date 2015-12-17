#include "stdafx.h"
#include "../NShapeDraw.h"


namespace nui
{
    namespace Ui
    {
        NUI_API Base::NAutoPtr<NShapeDraw> CreateRectDraw(ArgbColor fillColor)
        {
            Base::NInstPtr<NShapeDraw> shape(MemToolParam);
            shape->SetStyle(NShapeDraw::Rect)->SetFillColor(fillColor);
            return shape;
        }
    }
}
