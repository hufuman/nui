#pragma once


namespace nui
{
    namespace Ui
    {
        typedef FastDelegate1<void*, LRESULT> NUiProcHandler;

        void NUI_API PostUiProc(NUiProcHandler handler, void* data);
        LRESULT NUI_API SendUiProc(NUiProcHandler handler, void* data);
    }
}
