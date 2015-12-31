#include "StdAfx.h"
#include "Editor.h"

#include "PromptUI.h"
#include "EditorUtil.h"

CEditor::CEditor(void)
{
}

CEditor::~CEditor(void)
{
}

bool CEditor::create()
{
    if(!createWindow())
        return false;

    window_->WindowCreatedEvent.AddHandler(this, &CEditor::onWindowCreated);
    window_->DoModalWithStyle(NULL, _T("@MainUI:MainUI"));
    window_ = NULL;

    return true;
}

bool CEditor::createWindow()
{
    window_.Create(MemToolParam);
    window_->SetText(_T("Editor"));
    window_->SetMsgFilterCallback(MakeDelegate(this, &CEditor::onWindowMsg));

    return true;
}

bool CEditor::initEditor()
{
    NFrame* scintllaFrame = window_->GetRootFrame()->GetChildById(_T("scintilla"), true);
    if(scintllaFrame == NULL)
        return false;

    const NRect& rcFrame = scintllaFrame->GetRect();
    RECT rect = {rcFrame.Left, rcFrame.Top, rcFrame.Right, rcFrame.Bottom};
    bool result = scintillaWnd_.Create(window_->GetNative(), rect);
    NAssertError(result, _T("failed to create scintilla window"));
    if(result)
    {
        NInstPtr<NNative> nativeFrame(MemToolParam);
        nativeFrame->Attach(scintllaFrame, scintillaWnd_.GetHandle());
        nativeFrame->SetLayout(NFrame::LayoutHFill | NFrame::LayoutVFill);
    }
    return result;
}

void CEditor::listenEvents()
{
    NFrame* rootFrame = window_->GetRootFrame();
    rootFrame->GetChildById(_T("btnBrowse"), true)->ClickEvent.AddHandler(this, &CEditor::onBtnBrowseClicked);
    rootFrame->GetChildById(_T("btnPreview"), true)->ClickEvent.AddHandler(this, &CEditor::onBtnSaveAndPreviewClicked);
}

bool CEditor::openFile(const NString& filePath)
{
    styleFilePath_ = filePath;
    if(!scintillaWnd_.Open(filePath))
        return false;

    NFrame* rootFrame = window_->GetRootFrame();
    rootFrame->GetChildById(_T("btnPreview"), true)->SetEnabled(true);
    return true;
}

void CEditor::previewUi(const NString& styleName)
{
    NString resourcePath = File::GetParentFolder(styleFilePath_);
    while(!resourcePath.IsEmpty())
    {
        if(File::IsFileExists(File::CombinePath(resourcePath, _T("package.xml"))))
            break;
        resourcePath = File::GetParentFolder(resourcePath);
    }
    if(resourcePath.IsEmpty())
    {
        return;
    }

    NString commandLine;
    commandLine.Format(_T("UIMocker.exe /res \"%s\" /style %s"), resourcePath.GetData(), styleName.GetData());
    commandLine.Replace(_T("\\\""), _T("\""));
    if(!EditorUtil::Execute(commandLine.GetData()))
    {
        return;
    }
}

bool CEditor::onWindowCreated(NBaseObj*, NEventData*)
{
    if(!initEditor())
        return false;

    listenEvents();

    return true;
}

bool CEditor::onBtnBrowseClicked(NBaseObj*, NEventData*)
{
    NString filePath;
    if(!Shell::BrowseForFile(filePath, window_->GetNative(), true, _T("Style File(*.xml)|*.xml||")) || filePath.IsEmpty())
        return false;

    openFile(filePath);

    return true;
}

bool CEditor::onBtnSaveAndPreviewClicked(NBaseObj*, NEventData*)
{
    if(!scintillaWnd_.Save(styleFilePath_))
        return false;

    CPromptUI promptUi;
    NString styleName = promptUi.DoModal(window_->GetNative());
    if(styleName.IsEmpty())
        return false;

    previewUi(styleName);
    return true;
}

bool CEditor::onWindowMsg(NWindowBase*, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    if(message == WM_NOTIFY && scintillaWnd_.OnNotify(wParam, lParam, lResult))
        return true;
    return false;
}