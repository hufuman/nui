#include "stdafx.h"
#include "../NReflect.h"



namespace NUI

{
    namespace Base
    {
        NReflect::NReflect()
        {
            ;
        }

        NReflect::~NReflect()
        {
            ;
        }

        NReflect& NReflect::GetInstance()
        {
            static NReflect instance;
            return instance;
        }

        bool NReflect::AddReflect(LPCTSTR szNamespace, LPCTSTR szClassName, ObjectCreator objCreator)
        {
#ifdef _DEBUG
            NAssertError(!IsClassExists(szNamespace, szClassName), TEXT("Class has been added already"));
#endif

            NamespaceInfoMap::iterator iteInfo = m_mapNamespaceInfo.find(szNamespace);
            if(iteInfo == m_mapNamespaceInfo.end())
            {
                ObjCreatorMap objCreatorMap;
                objCreatorMap.insert(std::make_pair(szClassName, objCreator));
                m_mapNamespaceInfo.insert(std::make_pair(szNamespace, objCreatorMap));
            }
            else
            {
                iteInfo->second.insert(std::make_pair(szClassName, objCreator));
            }
            return true;
        }

        bool NReflect::RemoveReflect(LPCTSTR szNamespace, LPCTSTR szClassName)
        {
            NamespaceInfoMap::iterator iteInfo = m_mapNamespaceInfo.find(szNamespace);
            if(iteInfo == m_mapNamespaceInfo.end())
                return false;

            ObjCreatorMap::iterator iteCreator = iteInfo->second.find(szClassName);
            if(iteCreator == iteInfo->second.end())
                return false;

            iteInfo->second.erase(iteCreator);
            return true;
        }

        bool NReflect::RemoveAllReflect(LPCTSTR szNamespace)
        {
            NamespaceInfoMap::iterator iteInfo = m_mapNamespaceInfo.find(szNamespace);
            if(iteInfo == m_mapNamespaceInfo.end())
                return false;
            m_mapNamespaceInfo.erase(iteInfo);
            return true;
        }

        bool NReflect::IsClassExists(LPCTSTR szNamespace, LPCTSTR szClassName) const
        {
            ObjectCreator creator;
            return GetClassCreator(szNamespace, szClassName, creator);
        }

        NBaseObj* NReflect::Create(LPCTSTR szNamespace, LPCTSTR szClassName) const
        {
            ObjectCreator creator;
            if(!GetClassCreator(szNamespace, szClassName, creator))
                return NULL;
            return creator();
        }

        bool NReflect::GetClassCreator(LPCTSTR szNamespace, LPCTSTR szClassName, ObjectCreator& creator) const
        {
            NamespaceInfoMap::const_iterator iteInfo = m_mapNamespaceInfo.find(szNamespace);
            if(iteInfo == m_mapNamespaceInfo.end())
                return false;

            const ObjCreatorMap& creatorMap = iteInfo->second;
            ObjCreatorMap::const_iterator iteCreator = creatorMap.find(szClassName);
            if(iteCreator == iteInfo->second.end())
                return false;

            creator = iteCreator->second;
            return true;
        }
    }
}