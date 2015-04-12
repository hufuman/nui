#pragma once



namespace nui
{
    namespace Ui
    {
        class NUI_INTF NRender;
        class NUI_INTF NDraw : public nui::Base::NBaseObj
        {
        public:
            virtual void Draw(NRender* render, const Base::NRect& rect) = 0;
            virtual bool IsDrawValid() const = 0;
        };
    }
}
