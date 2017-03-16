#pragma once



#include "NWindowBase.h"
#include "NRender.h"
#include "NEvent.h"
#include "NCheckBox.h"
#include "../base/NInstPtr.h"
#include "../base/NAutoPtr.h"

namespace nui
{
    namespace Ui
    {
        class NFrame;

        class NUI_CLASS NWindow : public Base::NBaseObj, public NWindowBase
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("window"))

        public:
            NWindow();
            ~NWindow();

#ifndef _NO_NUI_PARSER_
            bool DoModalWithStyle(HWND parentWindow, LPCTSTR styleName);
			bool CreateWithStyle(HWND parentWindow, LPCTSTR styleName);
#endif  // _NO_NUI_PARSER_

            NFrame* GetRootFrame();
            NRender* GetRender() const;

            void ShowTooltip(const Base::NString& tooltip);

        protected:
            virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            virtual bool HandleKeyEvent(TCHAR key, bool isDownEvent);
            virtual bool OnWndCmd(WPARAM wParam, LPARAM lParam);
            virtual void OnCreate();
			virtual bool OnClose();

            virtual void OnSize(int width, int height);
            virtual void OnDraw(NRender* render, HRGN clipRgn);

            virtual void Draw(HDC hDc);

            void SetHoverItem(NFrame* frame);
            NFrame* RefreshHoverItem(const Base::NPoint& point);

            bool OnRootFrameSizeChanged(Base::NBaseObj* baseObj, NEventData* eventData);
            bool OnRootFrameTextChanged(Base::NBaseObj* baseObj, NEventData* eventData);

            void SyncSysButtonGroup();
			bool HasSysMaxButton();

            bool OnBtnMinClickedChanged(Base::NBaseObj* baseObj, NEventData* eventData);
            bool OnBtnMaxClickedChanged(Base::NBaseObj* baseObj, NEventData* eventData);
            bool OnBtnCloseClickedChanged(Base::NBaseObj* baseObj, NEventData* eventData);

            HWND GetTooltipWnd();
            void UpdateTooltipWnd();
            void DestroyTooltipWnd();
            void UpdateTooltipText(const Base::NString& tooltip);
            void HideTooltip();

        public:
            // Event
            class WindowDrawEventData : public NEventData
            {
            public:
                HRGN region;
                NRender* render;
            };
            NEvent PreDrawEvent;
            NEvent PostDrawEvent;

            class KeyEventData : public NEventData
            {
            public:
                TCHAR key;
                bool isDownEvent;
            };
            NEvent KeyEvent;

            NEvent WindowCreatedEvent;

			class WindowMoveEventData : public NEventData
			{
			public:
				Base::NPoint pos;
			};
			NEvent WindowMoveEvent;

			class WindowSizeEventData : public NEventData
			{
			public:
				int sizeType;
				Base::NSize size;
			};
			NEvent WindowSizeEvent;

			class WindowShowEventData : public NEventData
			{
			public:
				bool show;
			};
			NEvent WindowShowEvent;

			class WindowControlClickedEventData : public NEventData
			{
			public:
				NFrame * control;
			};
			NEvent WindowControlClickedEvent;

			class WindowEnableEventData : public NEventData
			{
			public:
				bool enable;
			};
			NEvent WindowEnableEvent;

			class WindowCloseEventData : public NEventData
			{
			public:
			};
			NEvent WindowCloseEvent;

			class WindowCommandEventData : public NEventData
			{
			public:
				int cmdSource;
				int cmdId;
			};
			NEvent WindowCommandEvent;

			class WindowHotkeyEventData : public NEventData
			{
			public:
				int hotKeyId;
				int modifiers;
				int virtKey;
			};
			NEvent WindowHotkeyEvent;

		protected:
            BEGIN_USE_UNEXPORT_TEMPLATE()
            Base::NAutoPtr<NFrame> rootFrame_;
            Base::NAutoPtr<NRender> render_;

            Base::NAutoPtr<NFrame> hoverFrame_;
            Base::NAutoPtr<NFrame> focusFrame_;

            Base::NAutoPtr<NFrame> btnSysMin_;
            Base::NAutoPtr<NCheckBox> btnSysMax_;
            Base::NAutoPtr<NFrame> btnSysClose_;

            Base::NString styleName_;
            Base::NRect sizableBorder_;

            HWND tooltipWnd_;
            Base::NString tooltip_;

            END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
