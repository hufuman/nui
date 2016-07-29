#include "StdAfx.h"
#include "ImageEditor.h"

#include "Resource.h"
#include "..\nui\ui\implement\Gdi\GdiUtil.h"

#undef min
#undef max

CImageEditor::CImageEditor(void)
{
}

CImageEditor::~CImageEditor(void)
{
}

void CImageEditor::Show()
{
    // Create Window
    window_.Create(MemToolParam);
    window_->Create(NULL, WindowStyle::Top | WindowStyle::Sizable);
    window_->SetSize(520, 420);
    window_->CenterWindow(NULL);
    window_->SetText(_T("Image Editor"));
    window_->GetRootFrame()->ApplyStyle(_T("@MainUI"));
    window_->SetVisible(true);

    // Drop Files
    ::DragAcceptFiles(window_->GetNative(), TRUE);
    Shell::FilterWindowMessage(0x0049 /*WM_COPYGLOBALDATA*/, 1);
    Shell::FilterWindowMessage(WM_DROPFILES, 1);
    Shell::FilterWindowMessage(WM_COPYDATA, 1);

    ListenEvents();

    window_->SetMsgFilterCallback(MakeDelegate(this, &CImageEditor::MsgCallback));
    window_->PostDrawEvent.AddHandler(MakeDelegate(this, &CImageEditor::PostDrawCallback));

    // Loop
    nui::Ui::NMsgLoop loop;
    loop.Loop(window_->GetNative());

    // Destroy
    image_ = NULL;
    window_ = NULL;
}

bool CImageEditor::PostDrawCallback(NBaseObj* baseObj, NEventData* eventData)
{
    NWindow::WindowDrawEventData* data = static_cast<NWindow::WindowDrawEventData*>(eventData);
    if(image_ != NULL)
    {
        NFrame* rootFrame = window_->GetRootFrame();
        NFrame* targetFrame = rootFrame->GetChildById(_T("canvas"), true);
        NRect dstRect = targetFrame->GetRootRect();

        int curHorzIndex = _ttoi(rootFrame->GetChildById(_T("editHorzCount"), true)->GetText());
        int curVertIndex = _ttoi(rootFrame->GetChildById(_T("editVertCount"), true)->GetText());

        data->render->DrawImage(image_, curHorzIndex, curVertIndex, dstRect, 0);
    }
    return false;
}

void CImageEditor::ListenEvents()
{
    NFrame* rootFrame = window_->GetRootFrame();

    rootFrame->GetChildById(_T("btnBrowse"), true)->ClickEvent.AddHandler(this, &CImageEditor::OnBtnBrowse);
    rootFrame->GetChildById(_T("btnSave"), true)->ClickEvent.AddHandler(this, &CImageEditor::OnBtnSave);

    rootFrame->GetChildById(_T("btnPrevHorzIndex"), true)->ClickEvent.AddHandler(this, &CImageEditor::OnImageIndexChanged);
    rootFrame->GetChildById(_T("btnNextHorzIndex"), true)->ClickEvent.AddHandler(this, &CImageEditor::OnImageIndexChanged);
    rootFrame->GetChildById(_T("btnPrevVertIndex"), true)->ClickEvent.AddHandler(this, &CImageEditor::OnImageIndexChanged);
    rootFrame->GetChildById(_T("btnNextVertIndex"), true)->ClickEvent.AddHandler(this, &CImageEditor::OnImageIndexChanged);

    rootFrame->GetChildById<NEdit*>(_T("editLeft"))->TextChangedEvent.AddHandler(this, &CImageEditor::OnParamChanged);
    rootFrame->GetChildById<NEdit*>(_T("editTop"))->TextChangedEvent.AddHandler(this, &CImageEditor::OnParamChanged);
    rootFrame->GetChildById<NEdit*>(_T("editRight"))->TextChangedEvent.AddHandler(this, &CImageEditor::OnParamChanged);
    rootFrame->GetChildById<NEdit*>(_T("editBottom"))->TextChangedEvent.AddHandler(this, &CImageEditor::OnParamChanged);
    rootFrame->GetChildById<NEdit*>(_T("editHorzCount"))->TextChangedEvent.AddHandler(this, &CImageEditor::OnParamChanged);
    rootFrame->GetChildById<NEdit*>(_T("editVertCount"))->TextChangedEvent.AddHandler(this, &CImageEditor::OnParamChanged);

    rootFrame->GetChildById<NFrame*>(_T("radioStretch"))->ClickEvent.AddHandler(this, &CImageEditor::OnDrawTypeChanged);
    rootFrame->GetChildById<NFrame*>(_T("radioTile"))->ClickEvent.AddHandler(this, &CImageEditor::OnDrawTypeChanged);
    rootFrame->GetChildById<NFrame*>(_T("radioNineStretch"))->ClickEvent.AddHandler(this, &CImageEditor::OnDrawTypeChanged);
}

bool CImageEditor::MsgCallback(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(lResult);
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
    }
    return false;
}

void CImageEditor::UpdateDraw(HWND hWnd, BOOL bReload)
{
    if(filePath_.IsEmpty())
        return;

    // trick: cause this function is none-reentrant, set filePath_ to empty to avoid this
    NFrame* rootFrame = window_->GetRootFrame();
    NString strPath = filePath_;
    filePath_ = _T("");
    if(bReload)
    {
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
        rootFrame->GetChildById<NRadioBox*>(_T("radioStretch"))->SetCheck(drawType == ImageDrawType::Stretch);
        rootFrame->GetChildById<NRadioBox*>(_T("radioTile"))->SetCheck(drawType == ImageDrawType::Tile);
        rootFrame->GetChildById<NRadioBox*>(_T("radioNineStretch"))->SetCheck(drawType == ImageDrawType::NineStretch);

        NString tmp;
        NRect rcParam = image_->GetStretchParam();
        tmp.Format(_T("%d"), rcParam.Left);
        rootFrame->GetChildById<NFrame*>(_T("editLeft"))->SetText(tmp);
        tmp.Format(_T("%d"), rcParam.Top);
        rootFrame->GetChildById<NFrame*>(_T("editTop"))->SetText(tmp);
        tmp.Format(_T("%d"), rcParam.Right);
        rootFrame->GetChildById<NFrame*>(_T("editRight"))->SetText(tmp);
        tmp.Format(_T("%d"), rcParam.Bottom);
        rootFrame->GetChildById<NFrame*>(_T("editBottom"))->SetText(tmp);

        rootFrame->GetChildById<NFrame*>(_T("labelHorzIndex"))->SetText(_T("0"));
        rootFrame->GetChildById<NFrame*>(_T("labelVertIndex"))->SetText(_T("0"));

        int horzCount, vertCount;
        image_->GetCount(horzCount, vertCount);
        tmp.Format(_T("%d"), horzCount);
        rootFrame->GetChildById<NFrame*>(_T("editHorzCount"))->SetText(tmp);
        tmp.Format(_T("%d"), vertCount);
        rootFrame->GetChildById<NFrame*>(_T("editVertCount"))->SetText(tmp);
    }

    ImageDrawType::Type drawType = ImageDrawType::Stretch;
    if(rootFrame->GetChildById<NFrame*>(_T("radioStretch"))->IsChecked())
        drawType = ImageDrawType::Stretch;
    else if(rootFrame->GetChildById<NFrame*>(_T("radioTile"))->IsChecked())
        drawType = ImageDrawType::Tile;
    else if(rootFrame->GetChildById<NFrame*>(_T("radioNineStretch"))->IsChecked())
        drawType = ImageDrawType::NineStretch;

    int left = _ttoi(rootFrame->GetChildById<NFrame*>(_T("editLeft"))->GetText());
    int top = _ttoi(rootFrame->GetChildById<NFrame*>(_T("editTop"))->GetText());
    int right = _ttoi(rootFrame->GetChildById<NFrame*>(_T("editRight"))->GetText());
    int bottom = _ttoi(rootFrame->GetChildById<NFrame*>(_T("editBottom"))->GetText());

    int horzCount = _ttoi(rootFrame->GetChildById<NFrame*>(_T("editHorzCount"))->GetText());
    int vertCount = _ttoi(rootFrame->GetChildById<NFrame*>(_T("editVertCount"))->GetText());

    image_->SetDrawType(drawType)->SetStretchParam(left, top, right, bottom)->SetCount(horzCount, vertCount);

    filePath_ = strPath;
    window_->Invalidate();
    ::InvalidateRect(window_->GetNative(), NULL, TRUE);
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
            window_->GetRootFrame()->GetChildById<NFrame*>(_T("editPath"))->SetText(filePath_);
            UpdateDraw(hWnd, TRUE);
        }
    }
    DragFinish(hDrop);
}

void CImageEditor::SaveExtInfo(HWND hWnd, NString strPath)
{
    stImageExtInfo extInfo;

    NFrame* rootFrame = window_->GetRootFrame();
    ImageDrawType::Type drawType = ImageDrawType::Stretch;
    if(rootFrame->GetChildById<NFrame*>(_T("radioStretch"))->IsChecked())
        extInfo.drawType = ImageDrawType::Stretch;
    else if(rootFrame->GetChildById<NFrame*>(_T("radioTile"))->IsChecked())
        extInfo.drawType = ImageDrawType::Tile;
    else if(rootFrame->GetChildById<NFrame*>(_T("radioNineStretch"))->IsChecked())
        extInfo.drawType = ImageDrawType::NineStretch;

    extInfo.horzCount = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editHorzCount"))->GetText());
    extInfo.vertCount = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editVertCount"))->GetText());
    extInfo.leftParam = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editLeft"))->GetText());
    extInfo.topParam = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editTop"))->GetText());
    extInfo.rightParam = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editRight"))->GetText());
    extInfo.bottomParam = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editBottom"))->GetText());

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

bool CImageEditor::OnImageIndexChanged(NBaseObj* baseObj, NEventData*)
{
    NFrame* frame = dynamic_cast<NFrame*>(baseObj);
    NString frameId = frame->GetId();

    int horzCount = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editHorzCount"))->GetText());
    int vertCount = _ttoi(window_->GetRootFrame()->GetChildById<NFrame*>(_T("editVertCount"))->GetText());

    NString tmp;
    if(frameId == _T("btnPrevHorzIndex"))
    {
        NFrame* horzIndex = window_->GetRootFrame()->GetChildById<NFrame*>(_T("labelHorzIndex"));
        tmp.Format(_T("%d"), std::max(_ttoi(horzIndex->GetText()) - 1, 0));
        horzIndex->SetText(tmp);
    }
    else if(frameId == _T("btnNextHorzIndex"))
    {
        NFrame* horzIndex = window_->GetRootFrame()->GetChildById<NFrame*>(_T("labelHorzIndex"));
        tmp.Format(_T("%d"), std::min(_ttoi(horzIndex->GetText()) + 1, horzCount - 1));
        horzIndex->SetText(tmp);
    }
    else if(frameId == _T("btnPrevVertIndex"))
    {
        NFrame* vertIndex = window_->GetRootFrame()->GetChildById<NFrame*>(_T("labelVertIndex"));
        tmp.Format(_T("%d"), std::max(_ttoi(vertIndex->GetText()) - 1, 0));
        vertIndex->SetText(tmp);
    }
    else if(frameId == _T("btnNextVertIndex"))
    {
        NFrame* vertIndex = window_->GetRootFrame()->GetChildById<NFrame*>(_T("labelVertIndex"));
        tmp.Format(_T("%d"), std::min(_ttoi(vertIndex->GetText()) + 1, vertCount - 1));
        vertIndex->SetText(tmp);
    }
    window_->Invalidate();
    ::InvalidateRect(window_->GetNative(), NULL, TRUE);

    return false;
}

bool CImageEditor::OnBtnBrowse(NBaseObj*, NEventData*)
{
    NString strPath;
    if(Shell::BrowseForFile(strPath, window_->GetNative(), TRUE, _T("All Files|*.gif;*.bmp;*.ico;*.png;*.jpg;*.jpeg||")) && !strPath.IsEmpty())
    {
        filePath_ = strPath;
        window_->GetRootFrame()->GetChildById<NFrame*>(_T("editPath"))->SetText(strPath);
        UpdateDraw(window_->GetNative(), TRUE);
    }

    return false;
}

bool CImageEditor::OnBtnSave(NBaseObj*, NEventData*)
{
    if(image_ != NULL)
    {
        SaveExtInfo(window_->GetNative(), filePath_);
    }

    return false;
}
bool CImageEditor::OnParamChanged(NBaseObj*, NEventData*)
{
    UpdateDraw(window_->GetNative(), FALSE);
    return false;
}

bool CImageEditor::OnDrawTypeChanged(NBaseObj*, NEventData*)
{
    UpdateDraw(window_->GetNative(), FALSE);
    return false;
}
