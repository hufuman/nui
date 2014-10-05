#pragma once

#include "noncopyable.h"
#include "BaseObj.h"
#include "NMemTool.h"

namespace NUI

{
    namespace Base
    {

#pragma warning(push)
#pragma warning(disable: 4251)

        typedef std::tr1::function<NBaseObj* ()> ObjectCreator;

        template < typename T >
        NBaseObj* BasicObjectCreator()
        {
            return NNew<T>();
        }

#define DECLARE_REFLECTION(namespaceName, className)        \
    public:                                                 \
    static LPCTSTR GetNamespace()                           \
        { return namespaceName; }                           \
        static LPCTSTR GetClassName()                       \
        { return className; }

#define IMPLEMENT_REFLECTION(implementClass)                \
    namespace   {                                           \
        class ReflectClass                                  \
        {                                                   \
        public:                                             \
        ReflectClass(){ NUI::Base::NReflect::GetInstance().AddReflect<implementClass>(); }      \
        ~ReflectClass(){ NUI::Base::NReflect::GetInstance().RemoveReflect<implementClass>(); }  \
        } MERGE_MACRO(g_ReflectClassObj, implementClass);}

        class NUI_CLASS NReflect : protected Noncopyable
        {
            NReflect();
        public:
            ~NReflect();

            static NReflect& GetInstance();

            bool AddReflect(LPCTSTR szNamespace, LPCTSTR szClassName, ObjectCreator objCreator);
            bool RemoveReflect(LPCTSTR szNamespace, LPCTSTR szClassName);
            bool RemoveAllReflect(LPCTSTR szNamespace);
            bool IsClassExists(LPCTSTR szNamespace, LPCTSTR szClassName) const;

            template < typename T>
            bool AddReflect(LPCTSTR szNamespace, LPCTSTR szClassName)
            {
                ObjectCreator objCreator(BasicObjectCreator<T>);
                return AddReflect(szNamespace, szClassName, objCreator);
            }

            template < typename T>
            bool AddReflect()
            {
                ObjectCreator objCreator(BasicObjectCreator<T>);
                return AddReflect(T::GetNamespace(), T::GetClassName(), objCreator);
            }

            template < typename T>
            bool RemoveReflect()
            {
                return RemoveReflect(T::GetNamespace(), T::GetClassName());
            }

            template < typename T>
            bool Create(T*& data)  const
            {
                NBaseObj* obj = Create(T::GetNamespace(), T::GetClassName());
                if(obj == NULL)
                    return NULL;
                data = dynamic_cast<T*>(obj);
                if(data)
                    return true;
                obj->Release();
                return false;
            }

            NBaseObj* Create(LPCTSTR szNamespace, LPCTSTR szClassName) const;

        private:
            bool GetClassCreator(LPCTSTR szNamespace, LPCTSTR szClassName, ObjectCreator& creator) const;

        private:
            typedef std::map<tstring, ObjectCreator> ObjCreatorMap;
            typedef std::map<tstring, ObjCreatorMap> NamespaceInfoMap;
            NamespaceInfoMap   m_mapNamespaceInfo;
        };

#pragma warning(pop)

    }
}