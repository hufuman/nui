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
    void ReleaseAll();

private:
    void OnTimer();
    void ReleaseObjs();

private:
    typedef std::list<nui::Base::NBaseObj*> PointerList;
    PointerList releaseQueue_;
    PointerList waitReleaseQueue_;

    bool inReleasing_;
    nui::Base::NHolder releaseTimer_;
};
