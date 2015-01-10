#pragma once


#include "../NCore.h"


class NCoreImpl : public nui::Base::NCore
{
public:
    NCoreImpl(void);
    virtual ~NCoreImpl(void);

    virtual bool InitCore(LPCTSTR packFilePath, LPCTSTR lang);
    virtual void DestroyCore();

protected:
    ULONG_PTR   m_uGdiplusToken;
    HRESULT     m_hOleInitResult;
};
