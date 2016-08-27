#include "stdafx.h"
#include "NParserImpl.h"

#include "ParserUtil.h"
#include "../../Ui/NTimerSrv.h"

using namespace nui;
using namespace Base;
using namespace Data;

IMPLEMENT_REFLECTION_EX(NParserImpl, NReflect::Singleton);

NAutoPtr<NBaseObj> NParserImpl::Parse(Base::NBaseObj* parentObj, LPCTSTR stylePath)
{
    NAutoPtr<NBaseObj> result;
    if(stylePath != NULL)
    {
        NAutoPtr<NDataReader> styleNode = FindStyleNode(stylePath);
        if(styleNode)
        {
            result = ParserUtil::LoadObj(true, parentObj, styleNode);
        }
    }
    NAssertError(result, _T("StyleNotFound: %s"), stylePath);
    return result;
}

bool NParserImpl::ApplyStyle(nui::Base::NBaseObj* targetObj, LPCTSTR stylePath)
{
    if(targetObj == NULL)
        return false;
    bool result = false;
    nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode = FindStyleNode(stylePath);
    if(styleNode)
    {
        result = ParserUtil::ApplyStyle(targetObj, styleNode);
    }
    NAssertError(result, _T("StyleNotFound: %s"), stylePath);
    return result;
}

nui::Base::NAutoPtr<nui::Data::NDataReader> NParserImpl::FindStyleNode(LPCTSTR stylePathParam)
{
    NString filePath;
    NString styleName;
    NAutoPtr<NDataReader> styleNode;

    bool innerBreak = false;
    NString stylePath(stylePathParam);
    nui::Base::NAutoPtr<nui::Data::NDataReader> result;
    // in case of cycle reference
    for(int refCount=0; refCount<5; ++ refCount)
    {
        if(!GetStyleParam(stylePath, filePath, styleName) || filePath.IsEmpty())
            break;

        nui::Base::NAutoPtr<nui::Data::NDataReader> dataReader = LoadPackFile(filePath);
        if(!dataReader)
            break;

        if(styleName.IsEmpty())
        {
            NString tmpString;
            if(dataReader->ReadValue(_T("default"), tmpString) && !tmpString.IsEmpty())
            {
                stylePath = tmpString;
                continue;
            }
        }

        innerBreak = false;
        for(int i=0;; ++i)
        {
            if(!dataReader->ReadNode(i, styleNode))
            {
                innerBreak = true;
                break;
            }
            if(styleName.IsEmpty())
                break;
            Base::NString name;
            if(styleNode->ReadValue(_T("name"), name) && name == styleName)
                break;
            styleNode = NULL;
        }
        if(innerBreak)
            break;
    }

    NAssertError(styleNode, _T("StyleNotFound: %s"), stylePathParam);

    return styleNode;
}

bool NParserImpl::GetStyleParam(LPCTSTR stylePath, NString& filePath, NString& styleName)
{
    // Normalize path
    LPCTSTR separator = _tcschr(stylePath, _T(':'));

    if(separator == NULL)
    {
        if(stylePath[0] == _T('@'))
        {
            filePath.Format(_T("@style:%s.xml"), stylePath + 1);
        }
        else
        {
            styleName = stylePath;
        }
        return true;
    }

    if(stylePath[0] != _T('@'))
        return false;

    styleName = separator + 1;

    filePath.Assign(stylePath, separator - stylePath);
    filePath.Format(_T("@style:%s.xml"), filePath.GetData() + 1);
    return true;
}

NAutoPtr<NDataReader> NParserImpl::LoadPackFile(LPCTSTR filePath)
{
    NString lowerFilePath(filePath);
    lowerFilePath.ToLower();
    StyleMap::iterator ite = styleMap_.find(lowerFilePath);
    if(ite != styleMap_.end())
    {
        StyleData& data = ite->second;
        data.lastAccessTime = ::GetTickCount();
        return data.styleReader;
    }

    NAutoPtr<NDataReader> reader;

    for(;;)
    {
        // Find Resource
        NInstPtr<NBuffer> buffer(MemToolParam);
        NInstPtr<NFileSystem> fs(MemToolParam);
        if(!fs->LoadFile(filePath, buffer))
        {
            NAssertError(false, _T("failed to load file: %s"), filePath);
            reader = NULL;
            break;
        }

        // Load Xml
        reader = CreateDataReader(ReaderXml);
        if(!reader->ParseUtf8(static_cast<const char*>(buffer->GetBuffer()), buffer->GetSize()))
        {
            NAssertError(false, _T("failed to parse file: %s"), filePath);
            reader = NULL;
            break;
        }

        break;
    }

    StyleData data;
    data.lastAccessTime = ::GetTickCount();
    data.styleReader = reader;
    styleMap_.insert(std::make_pair(lowerFilePath, data));

    if(!releaseHolder_)
    {
        nui::Base::NInstPtr<nui::Ui::NTimerSrv> timerSrv(MemToolParam);
        releaseHolder_ = timerSrv->startTimer(1000, MakeDelegate(this, &NParserImpl::OnReleaseTimer));
    }

    return reader;
}

void NParserImpl::OnReleaseTimer()
{
    DWORD nowTickCount = ::GetTickCount();
    StyleMap::iterator ite = styleMap_.begin();
    while(ite != styleMap_.end())
    {
        const nui::Base::NString& styleName = ite->first;
        UNREFERENCED_PARAMETER(styleName);
        StyleData& data = ite->second;
        if(nowTickCount - data.lastAccessTime > 1000)
        {
            data.styleReader = NULL;
            ite = styleMap_.erase(ite);
        }
        else
        {
            ++ ite;
        }
    }
    if(styleMap_.empty())
    {
        releaseHolder_.Release();
    }
}
