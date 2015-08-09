#include "stdafx.h"
#include "NStringBundleImpl.h"

#include "../../base/NMemTool.h"
#include "../../data/NFileSystem.h"
#include "../../util/NFileUtil.h"

IMPLEMENT_REFLECTION_EX(NStringBundleImpl, nui::Base::NReflect::Singleton);


using namespace nui;
using namespace Base;
using namespace Data;

NStringBundleImpl::NStringBundleImpl(void)
{
    ;
}

NStringBundleImpl::~NStringBundleImpl(void)
{
    ;
}

bool NStringBundleImpl::InitStringBundle(LPCTSTR stringLang)
{
    // Load config
    NAutoPtr<NDataReader> reader = CreateDataReader(ReaderXml);
    if(!reader->ParseFile(_T("@root:package.xml")))
        return false;
    NString bundleLang;
    for(int i=0; reader->ReadPath(i, _T("string\\bundle"), _T("lang"), bundleLang); ++ i)
    {
        if(bundleLang == stringLang)
        {
            if(reader->ReadPath(i, _T("string\\bundle"), _T("path"), basePath_))
                break;
        }
    }
    if(basePath_.IsEmpty())
    {
        if(!reader->ReadPath(0, _T("string\\bundle"), _T("path"), basePath_))
            return false;
    }
    return !basePath_.IsEmpty();
}

const NString& NStringBundleImpl::GetString(const NString& name)
{
    if(name.GetLength() < 1 || name[0] != _T('@'))
        return name;

    // Find BundleName and stringId
    int pos = name.IndexOf(_T(':'));
    if(pos <= 1 || pos >= name.GetLength() - 1)
        return name;

    NString bundleName = name.SubString(1, pos - 1);
    NString stringId = name.SubString(pos + 1);

    // Find if existed
    BundleMap::const_iterator iteBundle = bundleMap_.find(bundleName);
    if(iteBundle != bundleMap_.end())
    {
        // existed
        const StringBundle& stringBundle = iteBundle->second;
        StringBundle::const_iterator iteString = stringBundle.find(stringId);
        if(iteString == stringBundle.end())
            return name;
        return iteString->second;
    }

    // Load Bundle Data
    NString bundlePath = nui::Util::File::CombinePath(basePath_, bundleName);
    bundlePath += _T(".xml");
    NAutoPtr<NDataReader> reader = CreateDataReader(ReaderXml);
    if(!reader->ParseFile(bundlePath.GetData()))
        return name;

    StringBundle tmpBundle;
    std::pair<BundleMap::iterator, bool> result = bundleMap_.insert(std::make_pair(bundleName, tmpBundle));
    if(!result.second)
        return name;

    StringBundle& bundleData = result.first->second;

    NString id;
    NString value;
    bool hasId = false;
    NAutoPtr<NDataReader> node;
    for(int i=0; reader->ReadNode(i, node); ++ i)
    {
        if(node->ReadValue(_T("id"), id)
            && node->ReadValue(_T("value"), value))
        {
            if(id == stringId)
                hasId = true;
            bundleData.insert(std::make_pair(id, value));
        }
    }

    if(hasId)
        return bundleData[stringId];
    return name;
}

