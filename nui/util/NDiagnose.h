#pragma once


#if defined(_DEBUG) || defined(USE_NUI_DIAGNOSE)

namespace nui
{
    namespace Util
    {
        namespace Diag
        {
            class NScopedElapseGuard
            {
            public:
                NScopedElapseGuard(bool condition, LPCTSTR msg)
                {
                    msg_ = msg;
                    condition_ = condition;
                    startTickCount_ = ::GetTickCount();
                }
                ~NScopedElapseGuard()
                {
                    DWORD dwElapse = ::GetTickCount() - startTickCount_;
                    NVerify(condition_ || dwElapse < 80, _T("[ScopedElapseGuard] Too long: %d, %s"), dwElapse, msg_.GetData());
                }
            private:
                bool condition_;
                Base::NString msg_;
                DWORD startTickCount_;
            };

#define GUARD_SCOPE(condition, msg, ...) nui::Util::Diag::NScopedElapseGuard scopedElapseGuard((condition), nui::Util::Misc::GetVariantString(msg, __VA_ARGS__));
        }
    }
}

#else

#define GUARD_SCOPE(condition, msg, ...) __noop


#endif
