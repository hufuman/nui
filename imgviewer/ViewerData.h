#pragma once

#include "MapFile.h"

struct ViewerData
{
    HWND hWnd;
};

class ViewerDataMgr
{
    ViewerDataMgr()
    {
    }
    ViewerDataMgr& operator = (const ViewerDataMgr&);
    ViewerDataMgr(const ViewerDataMgr&);
public:
    ~ViewerDataMgr()
    {
        mapFile_.Close();
    }

    static ViewerDataMgr& Instance()
    {
        static ViewerDataMgr inst;
        return inst;
    }

    ViewerData* Hold(LPCTSTR filePath, HWND hWnd, BOOL needCreate)
    {
        NString name;
        name = _T("Global\\ImgViewer_1"); // only one instance is allowed
        /*
        name.Format(_T("Global\\ImgViewer%s_%d"), filePath, 1);  // TODO: Version
        name.Replace(_T("\\"), _T("_"));
        name.Replace(_T("/"), _T("_"));
        name.Replace(_T(":"), _T("_"));
        name.Replace(_T("*"), _T("_"));
        name.Replace(_T("?"), _T("_"));
        name.Replace(_T("\""), _T("_"));
        name.Replace(_T("<"), _T("_"));
        name.Replace(_T(">"), _T("_"));
        name.Replace(_T("|"), _T("_"));
        name.Replace(_T("$"), _T("_"));
        */
        if(!mapFile_.Create(name, sizeof(ViewerData), needCreate))
            return NULL;
        ViewerData* data = reinterpret_cast<ViewerData*>(mapFile_.GetData());
        if(hWnd != NULL)
            data->hWnd = hWnd;
        return data;
    }

private:
    CMapFile mapFile_;
};
