#include "stdafx.h"
#include "../NUiBus.h"

#include "./Gdi/GdiRender.h"
#include "./Gdi/GdiResourceLoader.h"

namespace nui
{
    namespace Ui
    {
        NUiBus::NUiBus()
        {
            loader_ = NULL;
        }

        NUiBus::~NUiBus()
        {
        }

        void NUiBus::Init(NRenderType::Type type)
        {
            Destroy();

            renderType_ = type;

            loader_ = NULL;

            switch(renderType_)
            {
            case NRenderType::GdiRender:
                loader_ = dynamic_cast<NResourceLoader*>(NNew(GdiResourceLoader));
                loader_->AddRef();
                break;
            }
            NAssertError(loader_ != NULL, TEXT("unknown type of LoaderType: %d"), renderType_);
        }

        void NUiBus::Destroy()
        {
            if(loader_)
                loader_->Release();

            loader_ = NULL;
        }

        NUiBus& NUiBus::Instance()
        {
            static NUiBus nuibus_instance;
            return nuibus_instance;
        }


        NRender* NUiBus::CreateRender()
        {
            NRender* render = NULL;
            switch(renderType_)
            {
            case NRenderType::GdiRender:
                render = dynamic_cast<GdiRender*>(NNew(GdiRender));
                break;
            }
            NAssertError(render != NULL, TEXT("unknown type of RenderType: %d"), renderType_);
            return render;
        }

        NResourceLoader* NUiBus::GetResourceLoader()
        {
            NAssertError(loader_ != NULL, _T("loader_ is NULL in NUiBus::GetLoader"));
            return loader_;
        }

    }
}