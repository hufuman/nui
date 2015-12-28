#include "StdAfx.h"
#include "ScintillaWnd.h"

#include "../thirdparty/Scintilla/Scintilla.h"
#include "../thirdparty/Scintilla/SciLexer.h"

#define SCINTILLA_MODULE_NAME _T("SciLexer.dll")
#define SCINTILLA_WINDOW_NAME _T("Scintilla")

namespace
{
    const COLORREF ColorBlack = RGB(0, 0, 0);
    const COLORREF ColorWhite = RGB(255, 255, 255);
    const COLORREF ColorRed = RGB(255, 0, 0);
    const COLORREF ColorDarkBlue = RGB(0, 0, 128);
    const COLORREF ColorDarkGreen = RGB(0, 128, 0);
}

CScintillaWnd::CScintillaWnd(void)
{
}

CScintillaWnd::~CScintillaWnd(void)
{
}

bool CScintillaWnd::Create(HWND hWndParent, const RECT& rcWnd, DWORD dwStyle)
{
    if(!LoadModule())
        return false;

    m_hWnd = ::CreateWindowEx(WS_EX_CLIENTEDGE,
        SCINTILLA_WINDOW_NAME,
        SCINTILLA_WINDOW_NAME,
        dwStyle,
        rcWnd.left, rcWnd.top,
        rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top,
        hWndParent,
        0,
        ::GetModuleHandle(NULL),
        NULL);
    if(m_hWnd == NULL)
        return false;

    if(!InitEditor())
        return false;

    return true;
}

bool CScintillaWnd::OnNotify(WPARAM, LPARAM lParam, LRESULT& lResult)
{
    LPNMHDR pnmh = (LPNMHDR)lParam;
    if(pnmh->hwndFrom != m_hWnd)
        return false;
    bool handled = false;
    lResult = 0;
    SCNotification *pSCNotification = (SCNotification*)pnmh;
    switch(pnmh->code)
    {
    case SCN_MARGINCLICK:
        {
            handled = true;
            int nLine = SendEditorMsg(SCI_LINEFROMPOSITION, (WPARAM)pSCNotification->position);
            SendEditorMsg(SCI_TOGGLEFOLD, (WPARAM)nLine);
        }
        break;
    default:
        break;
    }
    return handled;
}

void CScintillaWnd::SetKeywords(const char* keywords)
{
    SendEditorMsg(SCI_SETKEYWORDS, 0, reinterpret_cast<LPARAM>(keywords));
}

bool CScintillaWnd::LoadModule()
{
    HMODULE hModule = ::GetModuleHandle(SCINTILLA_MODULE_NAME);
    if(hModule != NULL)
        return true;
    hModule = ::LoadLibrary(SCINTILLA_MODULE_NAME);
    return hModule != NULL;
}

bool CScintillaWnd::InitEditor()
{
    SendEditorMsg(SCI_SETCODEPAGE, SC_CP_UTF8);
    SendEditorMsg(SCI_USEPOPUP, 1, 0);

    // default style
    SetAStyle(STYLE_DEFAULT, ColorBlack, ColorWhite, 11, "Courier New");
    SendEditorMsg(SCI_STYLECLEARALL);

    // current line color
    SendEditorMsg(SCI_SETCARETLINEVISIBLE, 1);
    SendEditorMsg(SCI_SETCARETLINEVISIBLEALWAYS, 1);
    SendEditorMsg(SCI_SETCARETLINEBACK, RGB(255, 255, 0));
    SendEditorMsg(SCI_SETCARETLINEBACKALPHA, 100);

    // set lexer
    SendEditorMsg(SCI_SETLEXER, SCLEX_XML);
    SendEditorMsg(SCI_SETSTYLEBITS, 7);

    SendEditorMsg(SCI_STYLESETBOLD, STYLE_CONTROLCHAR, 1);
    SetAStyle(SCE_H_DEFAULT, ColorBlack, ColorWhite, 11, "Courier New");

    // set colors
    SetAStyle(SCE_H_TAG, 128);
    SetAStyle(SCE_H_TAGEND, 128);
    SetAStyle(SCE_H_ATTRIBUTE, ColorRed);
    SetAStyle(SCE_H_NUMBER, RGB(0x80,0,0x80));
    SetAStyle(SCE_H_DOUBLESTRING, RGB(0,0,0x80));
    SetAStyle(SCE_H_SINGLESTRING, RGB(0,0,0x80));
    SetAStyle(SCE_H_OTHER, RGB(0x80,0,0x80));
    SetAStyle(SCE_H_COMMENT, RGB(0,0x80,0));
    SetAStyle(SCE_H_ENTITY, RGB(0x80,0,0x80));

    SetAStyle(SCE_H_XMLSTART, ColorDarkBlue);	// <?
    SetAStyle(SCE_H_XMLEND, ColorDarkBlue);		// ?>

    SetAStyle(SCE_HB_DEFAULT, ColorBlack);
    SetAStyle(SCE_HB_COMMENTLINE, ColorDarkGreen);
    SetAStyle(SCE_HB_NUMBER, RGB(0,0x80,0x80));
    SetAStyle(SCE_HB_WORD, ColorDarkBlue);
    SendEditorMsg(SCI_STYLESETBOLD, SCE_HB_WORD, 1);
    SetAStyle(SCE_HB_STRING, RGB(0x80,0,0x80));
    SetAStyle(SCE_HB_IDENTIFIER, ColorBlack);

    SendEditorMsg(SCI_STYLESETBACK, SCE_HB_STRINGEOL, RGB(0x7F,0x7F,0xFF));
    SendEditorMsg(SCI_STYLESETFONT, SCE_HB_COMMENTLINE, reinterpret_cast<LPARAM>("Courier New"));

    // folding
    SendEditorMsg(SCI_SETPROPERTY, (WPARAM)"fold", (LPARAM)"1");
    SendEditorMsg(SCI_SETPROPERTY, (WPARAM)"fold.compact", (LPARAM)"0");
    SendEditorMsg(SCI_SETPROPERTY, (WPARAM)"fold.html", (LPARAM)"1");
    SendEditorMsg(SCI_SETPROPERTY, (WPARAM)"fold.html.preprocessor", (LPARAM)"1");
    SendEditorMsg(SCI_SETMARGINWIDTHN, 1, 0);
    SendEditorMsg(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
    SendEditorMsg(SCI_SETMARGINMASKN, 1, SC_MASK_FOLDERS);
    SendEditorMsg(SCI_SETMARGINWIDTHN, 1, 20);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
    SendEditorMsg(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);

    SendEditorMsg(SCI_SETFOLDFLAGS, 16, 0);

    SendEditorMsg(SCI_SETMARGINSENSITIVEN, 1, 1);

    // ending of folding

    // line width
    {
        SendEditorMsg(SCI_SETMARGINWIDTHN, 0, SendEditorMsg(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"9999"));
    }

    return true;
}

LRESULT CScintillaWnd::SendEditorMsg(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return ::SendMessage(m_hWnd, Msg, wParam, lParam);
}

void CScintillaWnd::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char *face)
{
    SendEditorMsg(SCI_STYLESETFORE, style, fore);
    SendEditorMsg(SCI_STYLESETBACK, style, back);
    if (size >= 1)
        SendEditorMsg(SCI_STYLESETSIZE, style, size);
    if (face) 
        SendEditorMsg(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}
