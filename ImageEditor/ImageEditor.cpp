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

    // Drop Files
    ::DragAcceptFiles(window->GetNative(), TRUE);
    Shell::FilterWindowMessage(0x0049 /*WM_COPYGLOBALDATA*/, 1);
    Shell::FilterWindowMessage(WM_DROPFILES, 1);
    Shell::FilterWindowMessage(WM_COPYDATA, 1);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    shape_ = loader->CreateShape(MemToolParam);
    shape_->SetBorderWidth(2)->SetFillColor(::GetSysColor(CTLCOLOR_DLG) | 0xFF000000)->SetStyle(NShapeDraw::Rect)->SetBorderColor(MakeArgb(255, 0, 0, 0));

    window->SetMsgFilterCallback(MakeDelegate(this, &CImageEditor::MsgCallback));
    window->SetDrawCallback(MakeDelegate(this, &CImageEditor::DrawCallback));

    // Create controls
    NRect rcTmp;
    HWND hWnd = window->GetNative();

    // Path
    rcTmp.SetPos(10, 13).SetSize(80, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Image Path: "), rcTmp, 0, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 10).SetSize(250, 20);
    CreateRealControl(hWnd, IDC_EDIT_PATH, WC_EDIT, _T(""), rcTmp, ES_AUTOHSCROLL | ES_AUTOVSCROLL, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 9).SetSize(88, 22);
    CreateRealControl(hWnd, IDC_BTN_BROWSE, WC_BUTTON, _T("Browse..."), rcTmp, 0, 0);

    // Image Index
    rcTmp.SetPos(10, 38).SetSize(90, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Image Index: "), rcTmp, 0, 0);
    rcTmp.SetPos(rcTmp.Right + 8, 35).SetSize(20, 22);
    CreateRealControl(hWnd, IDC_BTN_HORZ_PREV_FRAME, WC_BUTTON, _T("<"), rcTmp, 0, 0);
    rcTmp.SetPos(rcTmp.Right + 2, 38).SetSize(25, 20);
    CreateRealControl(hWnd, IDC_LABEL_HORZ_INDEX, WC_STATIC, _T("0"), rcTmp, SS_CENTER, 0);
    rcTmp.SetPos(rcTmp.Right + 2, 35).SetSize(20, 22);
    CreateRealControl(hWnd, IDC_BTN_HORZ_NEXT_FRAME, WC_BUTTON, _T(">"), rcTmp, 0, 0);
    rcTmp.SetPos(rcTmp.Right + 22, 35).SetSize(20, 22);
    CreateRealControl(hWnd, IDC_BTN_VERT_PREV_FRAME, WC_BUTTON, _T("<"), rcTmp, 0, 0);
    rcTmp.SetPos(rcTmp.Right + 2, 38).SetSize(25, 20);
    CreateRealControl(hWnd, IDC_LABEL_VERT_INDEX, WC_STATIC, _T("0"), rcTmp, SS_CENTER, 0);
    rcTmp.SetPos(rcTmp.Right + 2, 35).SetSize(20, 22);
    CreateRealControl(hWnd, IDC_BTN_VERT_NEXT_FRAME, WC_BUTTON, _T(">"), rcTmp, 0, 0);

    // DrawType
    int left = 370;
    rcTmp.SetPos(left, 35).SetSize(80, 20);
    CreateRealControl(hWnd, IDC_RADIO_STRETCH, WC_BUTTON, _T("Stretch"), rcTmp, BS_AUTORADIOBUTTON | WS_GROUP, 0);
    ::CheckDlgButton(hWnd, IDC_RADIO_STRETCH, BST_CHECKED);
    rcTmp.SetPos(left, 60).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_RADIO_TILE, WC_BUTTON, _T("Tile"), rcTmp, BS_AUTORADIOBUTTON, 0);
    rcTmp.SetPos(left, 85).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_RADIO_NINESTRETCH, WC_BUTTON, _T("NineStretch"), rcTmp, BS_AUTORADIOBUTTON, 0);

    // Param
    rcTmp.SetPos(10, 63).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Stretch Param: "), rcTmp, 0, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(30, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_LEFT, WC_EDIT, _T("0"), rcTmp, ES_CENTER | ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(30, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_TOP, WC_EDIT, _T("0"), rcTmp, ES_CENTER | ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(30, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_RIGHT, WC_EDIT, _T("0"), rcTmp, ES_CENTER | ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 60).SetSize(30, 20);
    CreateRealControl(hWnd, IDC_EDIT_PARAM_BOTTOM, WC_EDIT, _T("0"), rcTmp, ES_CENTER | ES_NUMBER, 0);

    // Count
    rcTmp.SetPos(10, 88).SetSize(110, 20);
    CreateRealControl(hWnd, IDC_STATIC, WC_STATIC, _T("Image Count: "), rcTmp, 0, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 85).SetSize(30, 20);
    CreateRealControl(hWnd, IDC_EDIT_HORZ_COUNT, WC_EDIT, _T("1"), rcTmp, ES_CENTER | ES_NUMBER, 0);
    rcTmp.SetPos(10 + rcTmp.Right + 8, 85).SetSize(30, 20);
    CreateRealControl(hWnd, IDC_EDIT_VERT_COUNT, WC_EDIT, _T("1"), rcTmp, ES_CENTER | ES_NUMBER, 0);
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

        int curHorzIndex = ::GetDlgItemInt(window->GetNative(), IDC_LABEL_HORZ_INDEX, NULL, FALSE);
        int curVertIndex = ::GetDlgItemInt(window->GetNative(), IDC_LABEL_VERT_INDEX, NULL, FALSE);

        render->DrawImage(image_, curHorzIndex, curVertIndex, dstRect, 0);
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
    case WM_DROPFILES:
    {
        HDROP hDrop = (HDROP)wParam;
        if(hDrop != NULL)
        {
            OnDropFiles(window->GetNative(), hDrop);
            return true;
        }
        break;
    }
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
                        UpdateDraw(window->GetNative(), TRUE);
                    }
                }
                else if(uId == IDC_BTN_SAVE)
                {
                    if(image_ != NULL)
                    {
                        SaveExtInfo(window->GetNative(), filePath_);
                    }
                }
                else if(uId == IDC_RADIO_STRETCH || uId == IDC_RADIO_TILE || uId == IDC_RADIO_NINESTRETCH)
                {
                    UpdateDraw(window->GetNative(), FALSE);
                }
                else if(uId == IDC_BTN_HORZ_PREV_FRAME)
                {
                    OffsetImageIndex(window->GetNative(), -1, 0);
                }
                else if(uId == IDC_BTN_HORZ_NEXT_FRAME)
                {
                    OffsetImageIndex(window->GetNative(), 1, 0);
                }
                else if(uId == IDC_BTN_VERT_PREV_FRAME)
                {
                    OffsetImageIndex(window->GetNative(), 0, -1);
                }
                else if(uId == IDC_BTN_VERT_NEXT_FRAME)
                {
                    OffsetImageIndex(window->GetNative(), 0, 1);
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
                    UpdateDraw(window->GetNative(), FALSE);
                }
            }
            break;
        }
    }
    return false;
}

void CImageEditor::OffsetImageIndex(HWND hWnd, int horzOffset, int vertOffset)
{
    if(image_ == NULL)
        return;
    int horzCount, vertCount;
    image_->GetCount(horzCount, vertCount);
    int curHorzIndex = ::GetDlgItemInt(hWnd, IDC_LABEL_HORZ_INDEX, NULL, FALSE);
    int curVertIndex = ::GetDlgItemInt(hWnd, IDC_LABEL_VERT_INDEX, NULL, FALSE);
    curHorzIndex += horzOffset;
    curVertIndex += vertOffset;
    if(curHorzIndex >= horzCount)
        curHorzIndex = horzCount - 1;
    if(curHorzIndex < 0)
        curHorzIndex = 0;
    if(curVertIndex >= vertCount)
        curVertIndex = vertCount - 1;
    if(curVertIndex < 0)
        curVertIndex = 0;
    ::SetDlgItemInt(hWnd, IDC_LABEL_HORZ_INDEX, curHorzIndex, FALSE);
    ::SetDlgItemInt(hWnd, IDC_LABEL_VERT_INDEX, curVertIndex, FALSE);

    ::InvalidateRect(hWnd, NULL, FALSE);
}

void CImageEditor::UpdateDraw(HWND hWnd, BOOL bReload)
{
    if(filePath_.IsEmpty())
        return;

    // trick: cause this function is none-reentrant, set filePath_ to empty to avoid this
    NString strPath = filePath_;
    filePath_ = _T("");
    if(bReload)
    {
        ::SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);

        NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
        // set to NULL first, to reload current image, because loader caches images
        image_ = NULL;
        image_ = loader->LoadImage(strPath);
        if(image_ == NULL)
        {
            ::MessageBox(hWnd, _T("Failed to load image"), _T("ImageEditor"), MB_OK | MB_ICONERROR);
            return;
        }

        ImageDrawType::Type drawType = image_->GetDrawType();
        ::CheckDlgButton(hWnd, IDC_RADIO_STRETCH, drawType == ImageDrawType::Stretch ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(hWnd, IDC_RADIO_TILE, drawType == ImageDrawType::Tile ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(hWnd, IDC_RADIO_NINESTRETCH, drawType == ImageDrawType::NineStretch ? BST_CHECKED : BST_UNCHECKED);

        NRect rcParam = image_->GetStretchParam();
        ::SetDlgItemInt(hWnd, IDC_EDIT_PARAM_LEFT, rcParam.Left, FALSE);
        ::SetDlgItemInt(hWnd, IDC_EDIT_PARAM_TOP, rcParam.Top, FALSE);
        ::SetDlgItemInt(hWnd, IDC_EDIT_PARAM_RIGHT, rcParam.Right, FALSE);
        ::SetDlgItemInt(hWnd, IDC_EDIT_PARAM_BOTTOM, rcParam.Bottom, FALSE);

        ::SetDlgItemInt(hWnd, IDC_LABEL_HORZ_INDEX, 0, FALSE);
        ::SetDlgItemInt(hWnd, IDC_LABEL_VERT_INDEX, 0, FALSE);

        int horzCount, vertCount;
        image_->GetCount(horzCount, vertCount);
        ::SetDlgItemInt(hWnd, IDC_EDIT_HORZ_COUNT, horzCount, FALSE);
        ::SetDlgItemInt(hWnd, IDC_EDIT_VERT_COUNT, vertCount, FALSE);

        ::SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
    }

    ImageDrawType::Type drawType = ImageDrawType::Stretch;
    if(::IsDlgButtonChecked(hWnd, IDC_RADIO_STRETCH) == BST_CHECKED)
        drawType = ImageDrawType::Stretch;
    else if(::IsDlgButtonChecked(hWnd, IDC_RADIO_TILE) == BST_CHECKED)
        drawType = ImageDrawType::Tile;
    else if(::IsDlgButtonChecked(hWnd, IDC_RADIO_NINESTRETCH) == BST_CHECKED)
        drawType = ImageDrawType::NineStretch;

    int left = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_LEFT, NULL, FALSE);
    int top = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_TOP, NULL, FALSE);
    int right = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_RIGHT, NULL, FALSE);
    int bottom = ::GetDlgItemInt(hWnd, IDC_EDIT_PARAM_BOTTOM, NULL, FALSE);

    int horzCount = ::GetDlgItemInt(hWnd, IDC_EDIT_HORZ_COUNT, NULL, FALSE);
    int vertCount = ::GetDlgItemInt(hWnd, IDC_EDIT_VERT_COUNT, NULL, FALSE);

    image_->SetDrawType(drawType)->SetStretchParam(left, top, right, bottom)->SetCount(horzCount, vertCount);

    filePath_ = strPath;
    ::InvalidateRect(hWnd, NULL, FALSE);
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

void CImageEditor::OnDropFiles(HWND hWnd, HDROP hDrop)
{
    UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
    if(uNumFiles > 0 && uNumFiles != 0xFFFFFFFF)
    {
        TCHAR buffer[1024];
        if(::DragQueryFile(hDrop, 0, buffer, MAX_PATH) > 0)
        {
            filePath_ = buffer;
            ::SetDlgItemText(hWnd, IDC_EDIT_PATH, buffer);
            UpdateDraw(hWnd, TRUE);
        }
    }
    DragFinish(hDrop);
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

    NSize size = image_->GetPreferSize();
    if(extInfo.horzCount <= 0 || extInfo.horzCount >= 20
        || extInfo.vertCount <= 0 || extInfo.vertCount >= 20
        || extInfo.leftParam >= size.Width
        || extInfo.topParam >= size.Height
        || extInfo.rightParam >= size.Width
        || extInfo.bottomParam >= size.Height)
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