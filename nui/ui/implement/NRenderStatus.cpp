#include "stdafx.h"
#include "../NRenderStatus.h"

#include "./Gdi/AlphaDC.h"

namespace nui
{
    namespace Ui
    {
        NRenderStatus::NRenderStatus()
        {
            lastTime_ = 0;
            index_ = 0;
            hWnd_ = NULL;
        }

        NRenderStatus::~NRenderStatus()
        {
            ;
        }

        void NRenderStatus::BeforeDraw(HWND hWnd)
        {
            hWnd_ = hWnd;
            lastTime_ = ::GetTickCount();
        }

        void NRenderStatus::DrawStatus(NRender* render)
        {
            DWORD currentTime = ::GetTickCount();

            Base::NString status;
            if(currentTime <= lastTime_)
                status.Format(_T("Index: %d\r\nFPS: MAX"), ++ index_);
            else
                status.Format(_T("Index: %d\r\nFPS: %.2f"), ++ index_, (double)1000 / (currentTime - lastTime_));

            lastTime_ = currentTime;

            Base::NRect rcClient;
            ::GetClientRect(hWnd_, rcClient);

            HDC hDc = ::GetDC(hWnd_);

            Base::NRect rcText(rcClient);
            ::DrawText(hDc, status.GetData(), status.GetLength(), rcText, DT_EDITCONTROL | DT_CALCRECT);
            rcText.Offset(rcClient.Width() - rcText.Width() - 8, 8);

            CAlphaDC alphaDc;
            if(alphaDc.Init(hDc, rcText, rcClient.GetSize(), true))
            {
                ::SetBkColor(alphaDc, RGB(255, 255, 0));
                ::SetBkMode(alphaDc, OPAQUE);
                ::DrawText(alphaDc, status.GetData(), status.GetLength(), rcText, DT_EDITCONTROL);

                alphaDc.EndDraw(255);
            }
            ::ReleaseDC(hWnd_, hDc);
        }
    }
}
