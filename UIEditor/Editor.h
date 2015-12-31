#pragma once

#include "ScintillaWnd.h"

class CEditor
{
public:
    CEditor(void);
    ~CEditor(void);

    bool create();

private:
    bool createWindow();
    bool initEditor();
    void listenEvents();
    bool openFile(const NString& filePath);
    bool previewUi(const NString& styleName, NString& error);

    bool onWindowMsg(NWindowBase*, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

    // events
    bool onWindowCreated(NBaseObj*, NEventData*);
    bool onBtnBrowseClicked(NBaseObj*, NEventData*);
    bool onBtnSaveAndPreviewClicked(NBaseObj*, NEventData*);


private:
    NAutoPtr<NWindow> window_;
    CScintillaWnd   scintillaWnd_;

    NString styleFilePath_;
};
