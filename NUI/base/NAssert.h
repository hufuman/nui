#pragma once



#if defined(_DEBUG) || defined(_ENABLE_NASSERT_)

NUI_API void NAssertImpl(LPCTSTR filePath, int line, LPCTSTR format, ...);
NUI_API bool NAssertEnableImpl(bool enable);

#define NAssertEnable(enable)           NAssertEnableImpl(enable)
#define NAssertError(exp, format, ...)  for(;!(exp);){NAssertImpl(__TFILE__, __LINE__, format, __VA_ARGS__);break;}

class NTempDisableAssert : protected NUI::Base::Noncopyable
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

#endif
