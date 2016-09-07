#include "StdAfx.h"
#include "../NUiProc.h"

namespace nui
{
    namespace Ui
    {
        namespace Impl
        {
            class NUiProc
            {
                NUiProc(void);
            public:
                ~NUiProc(void);

            public:
                static NUiProc& Get();
                void Init();
                void Destroy();

                void PostUiProc(const NUiProcHandler& handler, void* data);
                LRESULT SendUiProc(const NUiProcHandler& handler, void* data);

            private:
                bool WindowProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            private:

                NWindowBase window_;
            };
        }
    }
}
