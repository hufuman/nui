#pragma once


#include "Employees.h"
#include "Bonuses.h"

class MainUi
{
public:
    MainUi(void);
    ~MainUi(void);

    void Show();

private:
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);

    bool OnBtnPrev(Base::NBaseObj* source, NEventData* eventData);
    bool OnBtnNext(Base::NBaseObj* source, NEventData* eventData);
    bool OnBtnView(Base::NBaseObj* source, NEventData* eventData);
    bool OnBtnReset(Base::NBaseObj* source, NEventData* eventData);
    bool OnChkBonus(Base::NBaseObj* source, NEventData* eventData);

    void ShowStep(int step);
    void StartRoll();
    void StopRoll();

    void RollProc();
    void ShowResultOnceProc();

    void ShowBonusList();
    void MarkEmployeeBonus(int employeeIndex);
    static unsigned int __stdcall LoadDataProc(void* param);

private:
    NInstPtr<NWindow> window_;

    HANDLE loadDataThread_;

    // not started
    int step_;
    int bonusCount_;
    int employeeBonusedCount_;

    int bonusIndex_;
    int employeeIndex_;

    NLink* btnPrev_;
    NLink* btnNext_;
    NFrame* bonusFrame_;
    NFrame* bonusTitle_;
    NFrame* bonusDesc_;
    NLabel* labelName_;
    NLabel* labelSeq_;
    NCheckBox* chkBonus_;
    NLayout* btnGroup_;
    NFrame* employeePhoto_;
    NFrame* bonusList_;

    NHolder rollTimer_;
    NHolder showResultOnceTimer_;
};

