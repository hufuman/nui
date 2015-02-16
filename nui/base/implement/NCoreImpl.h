#pragma once


#include "../NCore.h"
#include "../../ui/NRenderDef.h"


class NCoreImpl : public nui::Base::NCore
{
public:
    NCoreImpl(void);
    virtual ~NCoreImpl(void);

    virtual bool InitCore(LPCTSTR packFilePath, LPCTSTR lang, nui::Ui::NRenderType::Type type);
    virtual void DestroyCore();

protected:
    ULONG_PTR   m_uGdiplusToken;
    HRESULT     m_hOleInitResult;
};
