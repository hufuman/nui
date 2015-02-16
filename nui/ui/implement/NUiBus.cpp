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
            render_ = NULL;
            loader_ = NULL;
        }

        NUiBus::~NUiBus()
        {
        }

        void NUiBus::Init(NRenderType::Type type)
        {
            Destroy();

            render_ = NULL;
            loader_ = NULL;

            switch(type)
            {
            case NRenderType::GdiRender:
                render_ = dynamic_cast<NRender*>(NNew(GdiRender));
                render_ ->AddRef();
                loader_ = dynamic_cast<NResourceLoader*>(NNew(GdiResourceLoader));
                loader_->AddRef();
                break;
            }
            NAssertError(render_ != NULL, TEXT("unknown type of RenderType: %d"), type);
            NAssertError(loader_ != NULL, TEXT("unknown type of LoaderType: %d"), type);
        }

        void NUiBus::Destroy()
        {
            if(render_)
                render_->Release();
            if(loader_)
                loader_->Release();

            render_ = NULL;
            loader_ = NULL;
        }

        NUiBus& NUiBus::Instance()
        {
            static NUiBus nuibus_instance;
            return nuibus_instance;
        }


        NRender* NUiBus::GetRender()
        {
            NAssertError(render_ != NULL, _T("render_ is NULL in NUiBus::GetRender"));
            return render_;
        }

        NResourceLoader* NUiBus::GetResourceLoader()
        {
            NAssertError(loader_ != NULL, _T("loader_ is NULL in NUiBus::GetLoader"));
            return loader_;
        }

    }
}