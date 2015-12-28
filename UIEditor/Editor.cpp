#include "StdAfx.h"
#include "Editor.h"


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
    window_->DoModal(NULL);
    window_ = NULL;

    return true;
}

bool CEditor::createWindow()
{
    NInstPtr<NParser> parser(MemToolParam);
    NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(NULL, _T("@MainUI:MainUI")));
    window_.Create(MemToolParam);
    window_->SetSize(frame->GetRect().Width(), frame->GetRect().Height());
    window_->SetVisible(true);
    window_->CenterWindow(NULL);
    window_->SetText(_T("Editor"));
    window_->GetRootFrame()->AddChild(frame);
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
    return result;
}

void CEditor::listenEvents()
{
    NFrame* rootFrame = window_->GetRootFrame();
    rootFrame->GetChildById(_T("btnBrowse"), true)->ClickEvent.AddHandler(this, &CEditor::onBtnBrowseClicked);
}

bool CEditor::openFile(const NString& filePath)
{
    return true;
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
    NString filePath = Util::Shell::BrowseForFile(window_->GetNative(), true, _T("Style files(*.xml)|*.xml||"));
    if(filePath.IsEmpty())
        return false;

    openFile(filePath);

    return true;
}

bool CEditor::onWindowMsg(NWindowBase*, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    if(message == WM_NOTIFY && scintillaWnd_.OnNotify(wParam, lParam, lResult))
        return true;
    return false;
}