#include "StdAfx.h"
#include "ImageEditor.h"

#include "Resource.h"
#include "..\nui\ui\implement\Gdi\GdiUtil.h"

CImageEditor::CImageEditor(void)
{
}

CImageEditor::~CImageEditor(void)
{
}

void CImageEditor::Show()
{
    // Create Window
    NInstPtr<NWindow> window(MemToolParam);
    window->Create(NULL, WindowStyle::Top | WindowStyle::Sizable);
    window->SetSize(520, 420);
    window->CenterWindow(NULL);
    window->SetText(_T("Test Window"));
    window->SetVisible(true);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    shape_ = loader->CreateShape(MemToolParam);
    shape_->SetBorderWidth(2)->SetFillColor(::GetSysColor(CTLCOLOR_DLG) | 0xFF000000)->SetStyle(NShapeDraw::Rect)->SetBorderColor(MakeArgb(255, 0, 0, 0));

    window->SetMsgFilterCallback(MakeDelegate(this, &CImageEditor::MsgCallback));
    window->SetDrawCallback(MakeDelegate(this, &CImageEditor::DrawCallback));

    // Create controls
    NRect rcTmp;
    HWND hWnd = window->GetNative();

    // Path
    rcTmp.SetPos(10, 10).SetSize(80, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Image Path: "), rcTmp, 0, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 10).SetSize(250, 20);
    CreateRealControl(hWnd, IDC_EDIT_PATH, WC_EDIT, _T(""), rcTmp, ES_AUTOHSCROLL | ES_AUTOVSCROLL, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 9).SetSize(88, 22);
    CreateRealControl(hWnd, IDC_BTN_BROWSE, WC_BUTTON, _T("Browse..."), rcTmp, 0, 0);

    // DrawType
    rcTmp.SetPos(10, 35).SetSize(80, 20);
    CreateRealControl(hWnd, IDC_RADIO_STRETCH, WC_BUTTON, _T("Stretch"), rcTmp, BS_AUTORADIOBUTTON | WS_GROUP, 0);
    ::CheckDlgButton(hWnd, IDC_RADIO_STRETCH, BST_CHECKED);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 35).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_RADIO_TILE, WC_BUTTON, _T("Tile"), rcTmp, BS_AUTORADIOBUTTON, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 35).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_RADIO_NINESTRETCH, WC_BUTTON, _T("NineStretch"), rcTmp, BS_AUTORADIOBUTTON, 0);

    // Param
    rcTmp.SetPos(10, 60).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Stretch Param: "), rcTmp, 0, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(50, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_LEFT, WC_EDIT, _T("0"), rcTmp, ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(50, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_TOP, WC_EDIT, _T("0"), rcTmp, ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(50, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_RIGHT, WC_EDIT, _T("0"), rcTmp, ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(50, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_BOTTOM, WC_EDIT, _T("0"), rcTmp, ES_NUMBER, 0);

    // Count
    rcTmp.SetPos(10, 85).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Image Count: "), rcTmp, 0, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 85).SetSize(50, 20);
    CreateRealControl(hWnd, IDC_EDIT_HORZ_COUNT, WC_EDIT, _T("1"), rcTmp, ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 85).SetSize(50, 20);
    CreateRealControl(hWnd, IDC_EDIT_VERT_COUNT, WC_EDIT, _T("1"), rcTmp, ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 85).SetSize(88, 22);
    CreateRealControl(hWnd, IDC_BTN_SAVE, WC_BUTTON, _T("Save"), rcTmp, 0, 0);

    // Loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window->GetNative());

    // Destroy
    shape_ = NULL;
    image_ = NULL;
    window = NULL;
}

bool CImageEditor::DrawCallback(NWindow* window, NRender* render, const NRect& clipRect)
{
    NRect dstRect;
    window->GetRect(dstRect);
    dstRect.Offset(-dstRect.Left, -dstRect.Top);
    render->DrawShape(shape_, dstRect);
    if(image_ != NULL)
    {
        dstRect.Inflate(-10, -10);
        dstRect.Top += 100;
        render->DrawImage(image_, 0, 0, dstRect, 0);
    }
    return true;
}

bool CImageEditor::MsgCallback(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(lResult);
    UINT uId;
    UINT uCode;
    switch(message)
    {
    case WM_COMMAND:
        {
            uId = LOWORD(wParam);
            uCode  = HIWORD(wParam);
            if(uCode == BN_CLICKED)
            {
                if(uId == IDC_BTN_BROWSE)
                {
                    NString strPath = Shell::BrowseForFile(window->GetNative(), TRUE, _T("All Files|*.gif;*.bmp;*.ico;*.png;*.jpg;*.jpeg||"));
                    if(!strPath.IsEmpty())
                    {
                        filePath_ = strPath;
                        ::SetDlgItemText(window->GetNative(), IDC_EDIT_PATH, strPath);
                        UpdateDraw(window, TRUE);
                    }
                }
                else if(uId == IDC_BTN_SAVE)
                {
                    if(image_ != NULL)
                    {
                        NString strPath = Shell::BrowseForFile(window->GetNative(), FALSE, _T("All Files|*.gif;*.bmp;*.ico;*.png;*.jpg;*.jpeg||"));
                        if(!strPath.IsEmpty())
                        {
                            SaveExtInfo(window->GetNative(), strPath);
                        }
                    }
                }
                else if(uId == IDC_RADIO_STRETCH || uId == IDC_RADIO_TILE || uId == IDC_RADIO_NINESTRETCH)
                {
                    UpdateDraw(window, FALSE);
                }
            }
            else if(uCode == EN_UPDATE)
            {
                if(uId == IDC_EDIT_PARAM_LEFT
                    || uId == IDC_EDIT_PARAM_TOP
                    || uId == IDC_EDIT_PARAM_RIGHT
                    || uId == IDC_EDIT_PARAM_BOTTOM
                    || uId == IDC_EDIT_HORZ_COUNT
                    || uId == IDC_EDIT_VERT_COUNT)
                {
                    UpdateDraw(window, FALSE);
                }
            }
            break;
        }
    }
    return false;
}

void CImageEditor::UpdateDraw(NWindowBase* window, BOOL bReload)
{
    if(filePath_.IsEmpty())
        return;

    if(bReload)
    {
        NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
        // set to NULL first, to reload current image, because loader caches images
        image_ = NULL;
        image_ = loader->LoadImage(filePath_);

        ImageDrawType::Type drawType = image_->GetDrawType();
        ::CheckDlgButton(window->GetNative(), IDC_RADIO_STRETCH, drawType == ImageDrawType::Stretch ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(window->GetNative(), IDC_RADIO_TILE, drawType == ImageDrawType::Tile ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(window->GetNative(), IDC_RADIO_NINESTRETCH, drawType == ImageDrawType::NineStretch ? BST_CHECKED : BST_UNCHECKED);

        NRect rcParam = image_->GetStretchParam();
        ::SetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_LEFT, rcParam.Left, FALSE);
        ::SetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_TOP, rcParam.Top, FALSE);
        ::SetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_RIGHT, rcParam.Right, FALSE);
        ::SetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_BOTTOM, rcParam.Bottom, FALSE);

        int horzCount, vertCount;
        image_->GetCount(horzCount, vertCount);
        ::SetDlgItemInt(window->GetNative(), IDC_EDIT_HORZ_COUNT, horzCount, FALSE);
        ::SetDlgItemInt(window->GetNative(), IDC_EDIT_VERT_COUNT, vertCount, FALSE);
    }

    ImageDrawType::Type drawType = ImageDrawType::Stretch;
    if(::IsDlgButtonChecked(window->GetNative(), IDC_RADIO_STRETCH) == BST_CHECKED)
        drawType = ImageDrawType::Stretch;
    else if(::IsDlgButtonChecked(window->GetNative(), IDC_RADIO_TILE) == BST_CHECKED)
        drawType = ImageDrawType::Tile;
    else if(::IsDlgButtonChecked(window->GetNative(), IDC_RADIO_NINESTRETCH) == BST_CHECKED)
        drawType = ImageDrawType::NineStretch;

    int left = ::GetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_LEFT, NULL, FALSE);
    int top = ::GetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_TOP, NULL, FALSE);
    int right = ::GetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_RIGHT, NULL, FALSE);
    int bottom = ::GetDlgItemInt(window->GetNative(), IDC_EDIT_PARAM_BOTTOM, NULL, FALSE);

    int horzCount = ::GetDlgItemInt(window->GetNative(), IDC_EDIT_HORZ_COUNT, NULL, FALSE);
    int vertCount = ::GetDlgItemInt(window->GetNative(), IDC_EDIT_VERT_COUNT, NULL, FALSE);

    image_->SetDrawType(drawType)->SetStretchParam(left, top, right, bottom)->SetCount(horzCount, vertCount);
    window->Invalidate();
}

HWND CImageEditor::CreateRealControl(HWND hWndParent, UINT uId, LPCTSTR szClassName, LPCTSTR szText, const NRect& rcEdit, DWORD dwStyle, DWORD dwExStyle)
{
    HWND hWnd = ::CreateWindowEx(dwExStyle,
        szClassName, szText,
        dwStyle | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        rcEdit.Left, rcEdit.Top, rcEdit.Width(), rcEdit.Height(),
        hWndParent, (HMENU)uId, NULL, 0);
    ::SendMessage(hWnd, WM_SETFONT, (WPARAM)::SendMessage(hWndParent, WM_GETFONT, 0, 0), TRUE);
    HGDIOBJ hFont = GetStockObject(DEFAULT_GUI_FONT);
    LOGFONT logFont;
    ::GetObject(hFont, sizeof(logFont), &logFont);
    hFont = (HGDIOBJ)::SendMessage(hWnd, WM_GETFONT, 0, 0);
        ::GetObject(hFont, sizeof(logFont), &logFont);
    return hWnd;
}

void CImageEditor::SaveExtInfo(HWND hWnd, NString strPath)
{
    stImageExtInfo extInfo;

    ImageDrawType::Type drawType = ImageDrawType::Stretch;
    if(::IsDlgButtonChecked(hWnd, IDC_RADIO_STRETCH) == BST_CHECKED)
        extInfo.drawType = ImageDrawType::Stretch;
    else if(::IsDlgButtonChecked(hWnd, IDC_RADIO_TILE) == BST_CHECKED)
        extInfo.drawType = ImageDrawType::Tile;
    else if(::IsDlgButtonChecked(hWnd, IDC_RADIO_NINESTRETCH) == BST_CHECKED)
        extInfo.drawType = ImageDrawType::NineStretch;

    extInfo.horzCount = ::GetDlgItemInt(hWnd, IDC_EDIT_HORZ_COUNT, NULL, FALSE);
    extInfo.vertCount = ::GetDlgItemInt(hWnd, IDC_EDIT_VERT_COUNT, NULL, FALSE);
    extInfo.leftParam = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_LEFT, NULL, FALSE);
    extInfo.topParam = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_TOP, NULL, FALSE);
    extInfo.rightParam = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_RIGHT, NULL, FALSE);
    extInfo.bottomParam = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_BOTTOM, NULL, FALSE);

    NSize size = image_->GetSize();
    if(extInfo.horzCount <= 0 || extInfo.horzCount >= 20
        || extInfo.vertCount <= 0 || extInfo.vertCount >= 20
        || extInfo.leftParam <= 0 || extInfo.leftParam >= size.Width
        || extInfo.topParam <= 0 || extInfo.topParam >= size.Height
        || extInfo.rightParam <= 0 || extInfo.rightParam >= size.Width
        || extInfo.bottomParam <= 0 || extInfo.bottomParam >= size.Height)
    {
        ::MessageBox(hWnd, _T("Wrong param, please recorrect it"), _T("ImageEditor"), MB_OK | MB_ICONERROR);
        return;
    }

    if(!strPath.CompareNoCases(filePath_))
    {
        if(!::CopyFile(filePath_, strPath, FALSE))
        {
            ::MessageBox(hWnd, _T("Failed to access target file"), _T("ImageEditor"), MB_OK | MB_ICONERROR);
            return;
        }
    }

    if(!Gdi::UpdateImageData(strPath, extInfo))
    {
        ::MessageBox(hWnd, _T("Failed to save extInfo of image"), _T("ImageEditor"), MB_OK | MB_ICONERROR);
        return;
    }
}