#pragma once


#include "../NCore.h"


class NCoreImpl : public NUI::Base::NCore
{
public:
    NCoreImpl(void);
    virtual ~NCoreImpl(void);

    virtual bool InitCore(LPCTSTR szUIData, LPCTSTR szLang);
    virtual void DestroyCore();

protected:
    ULONG_PTR   m_uGdiplusToken;
    HRESULT     m_hOleInitResult;
};
