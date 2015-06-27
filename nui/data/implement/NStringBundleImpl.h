#pragma once


#include "../NStringBundle.h"


class NStringBundleImpl : public nui::Data::NStringBundle
{
public:
    NStringBundleImpl(void);
    virtual ~NStringBundleImpl(void);

    virtual bool InitStringBundle(LPCTSTR stringLang);
    virtual const nui::Base::NString& GetString(const nui::Base::NString& name);

protected:
    typedef std::map<nui::Base::NString, nui::Base::NString> StringBundle;
    typedef std::map<nui::Base::NString, StringBundle> BundleMap;

    BundleMap   bundleMap_;
    nui::Base::NString basePath_;
};
