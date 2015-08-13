#pragma once


#include "NRenderDef.h"


namespace nui
{
    namespace Ui
    {
        class NRender;
        class NFrameBase;
        class NResourceLoader;

        class NUI_CLASS NUiBus
        {
            NUiBus();
            NUiBus(const NUiBus&);
            NUiBus& operator = (const NUiBus&);
        public:
            ~NUiBus();
            void Init(NRenderType::Type type);
            void Destroy();

            static NUiBus& Instance();

            NRender* CreateRender();
            NResourceLoader* GetResourceLoader();

            NFrameBase* GetCaptureFrame();
            void SetCaptureFrame(NFrameBase* frame);

        private:
            NRenderType::Type renderType_;
            NResourceLoader* loader_;
            NFrameBase* captureFrame_;
        };
    }
}
