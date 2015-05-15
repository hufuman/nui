#pragma once



#if defined(_DEBUG) || defined(_ENABLE_NASSERT_)

NUI_API void NAssertImpl(bool exp, LPCTSTR filePath, int line, LPCTSTR format, ...);
NUI_API bool NVerifyImpl(bool exp, LPCTSTR filePath, int line, LPCTSTR format, ...);
NUI_API bool NAssertEnableImpl(bool enable);

#define NAssertEnable(enable)           NAssertEnableImpl(enable)
#define NAssertError(exp, format, ...)  NAssertImpl(!!(exp), __TFILE__, __LINE__, format, __VA_ARGS__)

#define NVerify(exp, format, ...)  NVerifyImpl((exp), __TFILE__, __LINE__, format, __VA_ARGS__)

class NTempDisableAssert : protected nui::Base::Noncopyable
{
public:
    NTempDisableAssert()
    {
        enabledPrev_ = NAssertEnableImpl(false);
    }
    ~NTempDisableAssert()
    {
        NAssertEnableImpl(enabledPrev_);
    }
private:
    bool enabledPrev_;
};
#define NAssertTempDisable()    NTempDisableAssert disableAssert;

#else

#define NAssertEnable(enable)           __noop
#define NAssertError(exp, format, ...)  __noop
#define NAssertTempDisable()            __noop

#define NVerify(exp, format, ...)  (!!(exp))
#endif
