#pragma once

#include "noncopyable.h"
#include "BaseObj.h"
#include "NMemTool.h"

namespace NUI
{
    namespace Base
    {
        template < typename T >
        NBaseObj* BasicObjectCreator(LPCSTR filePath, int line)
        {
            // return NNew(T);
            return NUI::Base::NNewImpl<T>(NUI::Base::MemTypeNew, new T(), 1, filePath, line);
        }

#define NReflectCreate(ptr) NUI::Base::NReflect::GetInstance().Create(ptr, __FILE__, __LINE__)

#define DECLARE_REFLECTION(namespaceName, className)        \
    public:                                                 \
    static LPCTSTR GetNamespace()                           \
        { return namespaceName; }                           \
        static LPCTSTR GetClassName()                       \
        { return className; }



#define IMPLEMENT_REFLECTION_EX(implementClass, flag)       \
    namespace   {                                           \
        class ReflectClass                                  \
        {                                                   \
        public:                                             \
        ReflectClass(){ NUI::Base::NReflect::GetInstance().AddReflect<implementClass>(flag); }      \
        ~ReflectClass(){ NUI::Base::NReflect::GetInstance().RemoveReflect<implementClass>(); }  \
        } MERGE_MACRO(g_ReflectClassObj, implementClass);}

#define IMPLEMENT_REFLECTION(implementClass)    IMPLEMENT_REFLECTION_EX(implementClass, (NUI::Base::NReflect::None))


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

            NBaseObj* Create(LPCTSTR szNamespace, LPCTSTR szClassName, LPCSTR filePath, int line);

        private:
            void ReleaseData();
            bool GetClassData(LPCTSTR szNamespace, LPCTSTR szClassName, ClassData*& pClassData);

        private:
            typedef std::map<tstring, ClassData> ClassDataMap;
            typedef std::map<tstring, ClassDataMap> NamespaceInfoMap;
            NamespaceInfoMap   m_mapNamespaceInfo;
        };

        END_USE_UNEXPORT_TEMPLATE()
    }
}