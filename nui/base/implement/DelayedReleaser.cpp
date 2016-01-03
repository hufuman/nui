#include "stdafx.h"
#include "DelayedReleaser.h"


DelayedReleaser::DelayedReleaser()
{
    inReleasing_ = false;
}

DelayedReleaser::~DelayedReleaser()
{
    ReleaseAll();
}

DelayedReleaser& DelayedReleaser::GetInstance()
{
    static DelayedReleaser inst;
    return inst;
}

void DelayedReleaser::AddPointer(nui::Base::NBaseObj* pointer)
{
    if(!releaseTimer_)
    {
        nui::Base::NInstPtr<nui::Ui::NTimerSrv> timerSrv(MemToolParam);
        releaseTimer_ = timerSrv->startTimer(1000, MakeDelegate(this, &DelayedReleaser::OnTimer));
    }
    if(inReleasing_)
        waitReleaseQueue_.push_back(pointer);
    else
        releaseQueue_.push_back(pointer);
}

void DelayedReleaser::ReleaseAll()
{
    releaseTimer_.Release();
    ReleaseObjs();
    // twice, to make sure waitReleaseQueue_ will be released
    ReleaseObjs();
}

void DelayedReleaser::OnTimer()
{
    ReleaseObjs();
}

void DelayedReleaser::ReleaseObjs()
{
    inReleasing_ = true;
    PointerList::iterator ite = releaseQueue_.begin();
    for(; ite != releaseQueue_.end(); ++ ite)
    {
        (*ite)->Release();
    }
    releaseQueue_.clear();
    inReleasing_ = false;
    releaseQueue_.insert(releaseQueue_.end(), waitReleaseQueue_.begin(), waitReleaseQueue_.end());
    waitReleaseQueue_.clear();

    if(releaseQueue_.empty() && waitReleaseQueue_.empty())
        releaseTimer_.Release();
}

namespace nui
{
    namespace Base
    {
        void NDelayedReleaseImpl(void* ptr)
        {
            DelayedReleaser::GetInstance().AddPointer(reinterpret_cast<nui::Base::NBaseObj*>(ptr));
        }

    }
}