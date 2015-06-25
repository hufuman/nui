#pragma once

#include "noncopyable.h"
#include "BaseObj.h"
#include "NMemTool.h"

namespace nui
{
    namespace Base
    {
        class NCore;

        template < typename T >
        NBaseObj* BasicObjectCreator(LPCSTR filePath, int line)
        {
            // return NNew(T);
            return NNewImpl<T>(nui::Base::MemTypeNew, new T(), 1, filePath, line);
        }

#define NReflectCreate(ptr) nui::Base::NReflect::GetInstance().Create(ptr, MemToolParam)

#define DECLARE_REFLECTION(namespaceName, className)        \
    public:                                                 \
    static LPCTSTR GetNamespace()                           \
        { return namespaceName; }                           \
        static LPCTSTR GetClassName()                       \
        { return className; }                               \
    private:



#define IMPLEMENT_REFLECTION_EX(implementClass, flag)       \
    namespace   {                                           \
        class MERGE_MACRO(ReflectClass, implementClass)     \
        {                                                   \
        public:                                             \
        MERGE_MACRO(ReflectClass, implementClass)(){ nui::Base::NReflect::GetInstance().AddReflect<implementClass>(flag); }      \
        ~MERGE_MACRO(ReflectClass, implementClass)(){}  \
        } MERGE_MACRO(g_ReflectClassObj, implementClass);}

#define IMPLEMENT_REFLECTION(implementClass)    IMPLEMENT_REFLECTION_EX(implementClass, (nui::Base::NReflect::None))


        BEGIN_USE_UNEXPORT_TEMPLATE()
        // filePath
        // line
        // return value
        typedef FastDelegate2<LPCSTR, int, NBaseObj*> ObjectCreator;

        class NUI_CLASS NReflect : protected Noncopyable
        {
            NReflect();

        public:
            enum ReflectFlag
            {
                None,
                Singleton,
            };

            struct ClassData
            {
                ObjectCreator creator;
                ReflectFlag flag;
                NBaseObj* existObj;
            };

        public:
            ~NReflect();

            static NReflect& GetInstance();

            bool AddReflect(LPCTSTR szNamespace, LPCTSTR szClassName, ObjectCreator objCreator, ReflectFlag flag);
            bool RemoveReflect(LPCTSTR szNamespace, LPCTSTR szClassName);
            bool RemoveAllReflect(LPCTSTR szNamespace);
            bool IsClassExists(LPCTSTR szNamespace, LPCTSTR szClassName);

            template < typename T>
            bool AddReflect(LPCTSTR szNamespace, LPCTSTR szClassName, ReflectFlag flag)
            {
                ObjectCreator objCreator(BasicObjectCreator<T>);
                return AddReflect(szNamespace, szClassName, objCreator, flag);
            }

            template < typename T>
            bool AddReflect(ReflectFlag flag)
            {
                ObjectCreator objCreator(BasicObjectCreator<T>);
                return AddReflect(T::GetNamespace(), T::GetClassName(), objCreator, flag);
            }

            template < typename T>
            bool RemoveReflect()
            {
                return RemoveReflect(T::GetNamespace(), T::GetClassName());
            }

            template < typename T>
            bool Create(NAutoPtr<T>& ptr, LPCSTR filePath, int line)
            {
                T* p = ptr;
                NAssertError(p == NULL, _T("Not Null AutoPtr"));
                bool result = Create<T>(p, filePath, line);
                ptr = p;
                ptr->Release();
                return result;
            }

            template < typename T>
            bool Create(T*& data, LPCSTR filePath, int line)
            {
                NBaseObj* obj = Create(T::GetNamespace(), T::GetClassName(), filePath, line);
                if(obj == NULL)
                    return NULL;
                data = dynamic_cast<T*>(obj);
                if(data)
                    return true;
                obj->Release();
                return false;
            }

            NBaseObj* Create(LPCTSTR szFullName, LPCSTR filePath, int line)
            {
                tstring namespaceName;
                tstring className;
                LPCTSTR pos = _tcsstr(szFullName, _T("::"));
                if(pos == NULL)
                {
                    namespaceName = _T("nui");
                    className = szFullName;
                }
                else
                {
                    namespaceName.assign(szFullName, pos - szFullName);
                    className = pos + 2;
                }
                return Create(namespaceName.c_str(), className.c_str(), filePath, line);
            }

            NBaseObj* Create(LPCTSTR szNamespace, LPCTSTR szClassName, LPCSTR filePath, int line);

            void ReleaseData(NCore* core);
        private:
            bool GetClassData(LPCTSTR szNamespace, LPCTSTR szClassName, ClassData*& pClassData);

        private:
            typedef std::map<tstring, ClassData> ClassDataMap;
            typedef std::map<tstring, ClassDataMap> NamespaceInfoMap;
            NamespaceInfoMap   m_mapNamespaceInfo;
        };

        END_USE_UNEXPORT_TEMPLATE()
    }
}