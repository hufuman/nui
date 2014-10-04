#include "stdafx.h"

#include "../../Base/NString.h"
#include "../NFileUtil.h"


namespace NUI
{
    namespace Util
    {
        namespace File
        {
            Base::NString GetParentFolder(LPCTSTR path)
            {
                if(path == NULL || path[0] == 0)
                    return Base::NString();

                Base::NString folder(path);
                bool endWithSlash = (folder[folder.GetLength()-1] == _T('\\'));
                size_t pos = endWithSlash ?
                    folder.LastIndexOf(_T('\\'), folder.GetLength()-2)
                    : folder.LastIndexOf(_T('\\'));

                if(pos == -1)
                    return endWithSlash ? path : (folder + _T('\\'));
                return folder.SubString(0, pos + 1);
            }
        }
    }
}