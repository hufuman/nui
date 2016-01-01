#pragma once

#include <list>
#include "../../ui/NTimerSrv.h"

class DelayedReleaser
{
    DelayedReleaser();
    DelayedReleaser(const DelayedReleaser&);
    DelayedReleaser& operator = (const DelayedReleaser&);
public:
    ~DelayedReleaser();

    static DelayedReleaser& GetInstance();

    void AddPointer(nui::Base::NBaseObj* pointer);

private:
    void OnTimer();

private:
    typedef std::list<nui::Base::NBaseObj*> PointerList;
    PointerList releaseQueue_;
    PointerList waitReleaseQueue_;

    bool inTimer_;
    nui::Base::NHolder releaseTimer_;
};
