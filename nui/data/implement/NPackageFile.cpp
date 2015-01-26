#include "stdafx.h"
#include "NPackageFile.h"


using namespace nui;
using namespace Data;
using namespace Base;

NPackageFile::NPackageFile()
{
    ;
}

NPackageFile::~NPackageFile()
{
    ;
}

void NPackageFile::Init(LPCTSTR filePath, bool useRealFS)
{
    useRealFS_ = useRealFS;
    mapPaths_.clear();
    rootPath_ = filePath;

    nui::Base::NString path;
    if(useRealFS_)
        path = rootPath_.ToLower();
    else
        path = _T("");
    mapPaths_.insert(std::make_pair(_T("root"), path));
}

bool NPackageFile::LoadConfig(nui::Base::NInstPtr<nui::Data::NBuffer> fileBuffer)
{
    nui::Base::NAutoPtr<nui::Data::NDataReader> reader = nui::Data::CreateDataReader(ReaderXml);
    if(!reader->ParseUtf8(static_cast<const char*>(fileBuffer->GetBuffer()), fileBuffer->GetSize()))
        return false;

    if(!LoadConfigForNode(reader, _T("xnc"), _T("root"), _T("name"), mapPaths_)
        || !LoadConfigForNode(reader, _T("string"), _T("bundle"), _T("lang"), mapStrings_))
    {
        return false;
    }
    return true;
}

bool NPackageFile::LoadConfigForNode(nui::Base::NAutoPtr<nui::Data::NDataReader> reader, LPCTSTR xmlPath, LPCTSTR nodeName, LPCTSTR keyName, PathMap& pathMap)
{
    nui::Base::NAutoPtr<nui::Data::NDataReader> nodes;
    if(!reader->ReadNodeByPath(xmlPath, nodes))
        return false;

    nui::Base::NString key, path;
    nui::Base::NString realPath;
    for(int i=0; ; ++ i)
    {
        nui::Base::NAutoPtr<nui::Data::NDataReader> node;
        if(!nodes->ReadNode(i, nodeName, node))
            break;

        if(!node->ReadValue(keyName, key)
            || !node->ReadValue(_T("path"), path))
        {
            continue;
        }

        key.MakeLower();
        realPath = GetRealPath(path.GetData());
        pathMap.insert(std::make_pair(key, realPath));
    }

    return pathMap.size() > 0;
}

nui::Base::NString NPackageFile::GetRealPath(LPCTSTR xncPath)
{
    if(xncPath == NULL || xncPath[0] != _T('@'))
        return xncPath;

    nui::Base::NString path(xncPath);
    path.MakeLower();

    nui::Base::NString prefix = GetXNCPrefix(path);
    PathMap::iterator ite = mapPaths_.find(prefix);
    NAssertError(ite != mapPaths_.end(), _T("Invalid XNC"));
    if(ite == mapPaths_.end())
        return xncPath;

    nui::Base::NString folder = ite->second;
    nui::Base::NString result;
    result = nui::Util::File::CombinePath(folder, path.SubString(prefix.GetLength() + 2));
    return result;
}

nui::Base::NString NPackageFile::GetXNCPrefix(const nui::Base::NString& path)
{
    if(path.GetLength() < 1 || path[0] != _T('@'))
        return path;

    nui::Base::NString prefix;
    size_t pos = path.IndexOf(_T(':'));
    if(pos == -1)
        prefix = path.SubString(1);
    else
        prefix = path.SubString(1, pos - 1);
    return prefix;
}
