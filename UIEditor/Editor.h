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

    bool onWindowMsg(NWindowBase*, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

    // events
    bool onWindowCreated(NBaseObj*, NEventData*);
    bool onBtnBrowseClicked(NBaseObj*, NEventData*);


private:
    NAutoPtr<NWindow> window_;
    CScintillaWnd   scintillaWnd_;
};
