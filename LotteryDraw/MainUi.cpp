#include "StdAfx.h"
#include "MainUi.h"

#include "UiUtil.h"
#include "ResultUi.h"


MainUi::MainUi(void) : window_(MemToolParam)
{
    step_ = 0;
    bonusCount_ = 0;
    employeeIndex_ = 0;
}

MainUi::~MainUi(void)
{
}

void MainUi::Show()
{
    Employees::Get().Load();
    Bonuses::Get().Load();

    bonusCount_ = Bonuses::Get().Count();

    for(int i=0;i<bonusCount_; ++ i)
    {
        const std::list<int>* list = Employees::Get().GetBonusEmployees(i);
        if(list != NULL && !list->empty())
        {
            Bonuses::Get().SetBonusShowResultOnce(i, false);
        }
    }

    window_->SetStyle(WindowStyle::Top);
    window_->WindowCreatedEvent.AddHandler(this, &MainUi::OnWindowCreated);
    window_->SetVisible(true);
    window_->DoModalWithStyle(NULL, _T("@MainUi:MainUi"));

    Employees::Get().Destroy();
    Bonuses::Get().Destroy();
    rollTimer_.Release();
    showResultOnceTimer_.Release();

    window_ = NULL;
}

bool MainUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame* rootFrame = window_->GetRootFrame();

    btnPrev_ = rootFrame->GetChildById<NLink*>(_T("btnPrev"));
    btnNext_ = rootFrame->GetChildById<NLink*>(_T("btnNext"));
    bonusFrame_ = rootFrame->GetChildById<NFrame*>(_T("bonusFrame"));
    bonusTitle_ = rootFrame->GetChildById<NFrame*>(_T("bonusTitle"));
    bonusDesc_ = rootFrame->GetChildById<NFrame*>(_T("bonusDesc"));
    labelName_ = rootFrame->GetChildById<NLabel*>(_T("eName"));
    labelSeq_ = rootFrame->GetChildById<NLabel*>(_T("eSeq"));
    chkBonus_ = rootFrame->GetChildById<NCheckBox*>(_T("chkBonus"));
    btnGroup_ = rootFrame->GetChildById<NLayout*>(_T("btnGroup"));
    employeePhoto_ = rootFrame->GetChildById<NFrame*>(_T("employeePhoto"));
    bonusList_ = rootFrame->GetChildById<NFrame*>(_T("resultBkg"));

    btnPrev_->ClickEvent.AddHandler(this, &MainUi::OnBtnPrev);
    btnNext_->ClickEvent.AddHandler(this, &MainUi::OnBtnNext);
    rootFrame->GetChildById<NFrame*>(_T("btnView"))->ClickEvent.AddHandler(this, &MainUi::OnBtnView);
    rootFrame->GetChildById<NFrame*>(_T("btnReset"))->ClickEvent.AddHandler(this, &MainUi::OnBtnReset);

    chkBonus_->ClickEvent.AddHandler(this, &MainUi::OnChkBonus);

    return false;
}

bool MainUi::OnBtnPrev(nui::Base::NBaseObj *source, nui::Ui::NEventData *eventData)
{
    ShowStep(step_ - 1);
    return true;
}

bool MainUi::OnBtnNext(nui::Base::NBaseObj *source, nui::Ui::NEventData *eventData)
{
    ShowStep(step_ + 1);
    return true;
}

bool MainUi::OnBtnView(nui::Base::NBaseObj *source, nui::Ui::NEventData *eventData)
{
    NInstPtr<NStringBundle> bundle(MemToolParam);
    NString result;
    for(int i=0; i<bonusCount_; ++ i)
    {
        int count = 0;
        const BonusInfo& bonusInfo = Bonuses::Get().GetBonusInfo(i);
        const std::list<int>* employeeIndexes = Employees::Get().GetBonusEmployees(i);
        result += (_T("\r\n"));
        result += bundle->GetString(_T("@Main:BonusListTitle")).arg(bonusInfo.name);
        result += _T("\r\n    ");
        if(employeeIndexes != NULL)
        {
            std::list<int>::const_iterator ite = employeeIndexes->begin();
            for(; ite != employeeIndexes->end(); ++ ite)
            {
                int index = *ite;
                const EmployeeInfo& employee = Employees::Get().GetEmployeeInfo(index);
                result += employee.name;
                ++ count;
                if(count % 3 == 0)
                    result += _T("\r\n    ");
                else
                    result += _T("    ");
            }
            result += _T("\r\n");
        }
        else
        {
            result += _T("None\r\n");
        }
    }
    ResultUi ui;
    ui.Show(window_, result);
    return true;
}

bool MainUi::OnBtnReset(nui::Base::NBaseObj *source, nui::Ui::NEventData *eventData)
{
    if(chkBonus_->IsChecked())
        return true;

    NInstPtr<NStringBundle> bundle(MemToolParam);
    NString msg = bundle->GetString(_T("@Main:QueryResetMsg"));
    NString title = bundle->GetString(_T("@Common:appTitle"));
    if(::MessageBox(window_->GetNative(), msg, title, MB_YESNO | MB_ICONQUESTION) != IDYES)
    {
        return true;
    }

    employeePhoto_->SetVisible(false);
    labelName_->SetText(_T(""));
    labelSeq_->SetText(_T(""));
    bonusList_->SetText(_T(""));
    bonusList_->SetVisible(false);
    bonusDesc_->SetVisible(true);

    Bonuses::Get().Reset();
    Employees::Get().Reset();

    return true;
}

bool MainUi::OnChkBonus(nui::Base::NBaseObj *source, nui::Ui::NEventData *eventData)
{
    if(chkBonus_->IsChecked())
        StartRoll();
    else
        StopRoll();
    return true;
}

void MainUi::ShowStep(int step)
{
    NFrame* rootFrame = window_->GetRootFrame();
    NCheckBox* chkBonus = rootFrame->GetChildById<NCheckBox*>(_T("chkBonus"));

    if(step < 0 || step > bonusCount_ || chkBonus->IsChecked())
        return;

    step_ = step;
    bonusIndex_ = step - 1;

    btnPrev_->SetVisible(step > 0);
    btnNext_->SetVisible(step < bonusCount_);
    bonusFrame_->SetVisible(step > 0);
    btnGroup_->RelayoutChilds();

    if(step > 0)
    {
        const BonusInfo& bonus = Bonuses::Get().GetBonusInfo(bonusIndex_);
        bonusTitle_->SetForeDraw(bonus.titleDraw);
        bonusDesc_->SetForeDraw(bonus.descDraw);
        bonusDesc_->SetVisible(true);
        ShowBonusList();
    }
    else
    {
        bonusList_->SetVisible(false);
    }

    employeePhoto_->SetVisible(false);
    labelName_->SetText(_T(""));
    labelSeq_->SetText(_T(""));
}

void MainUi::StartRoll()
{
    NInstPtr<NTimerSrv> timer(MemToolParam);
    rollTimer_ = timer->startTimer(20, MakeDelegate(this, &MainUi::RollProc));

    bonusDesc_->SetVisible(false);
}

void MainUi::StopRoll()
{
    const BonusInfo& bonus = Bonuses::Get().GetBonusInfo(bonusIndex_);

    if(bonus.showResultOnce)
    {
        if(showResultOnceTimer_)
        {
            return;
        }

        Bonuses::Get().SetBonusShowResultOnce(bonusIndex_, false);
        chkBonus_->SetEnabled(false);
        NInstPtr<NTimerSrv> timer(MemToolParam);
        employeeBonusedCount_ = 0;
        showResultOnceTimer_ = timer->startTimer(400, MakeDelegate(this, &MainUi::ShowResultOnceProc));
    }
    else
    {
        MarkEmployeeBonus(employeeIndex_);
        rollTimer_.Release();
    }
}

void MainUi::RollProc()
{
    employeeIndex_ = Employees::Get().GetRandomNextIndex();

    const EmployeeInfo& employee = Employees::Get().GetEmployeeInfo(employeeIndex_);
    employeePhoto_->SetForeDraw(employee.draw);
    employeePhoto_->SetVisible(true);

    labelName_->SetText(employee.name);
    labelSeq_->SetText(employee.seq);
}

void MainUi::ShowResultOnceProc()
{
    MarkEmployeeBonus(employeeIndex_);
    ++ employeeBonusedCount_;
    const BonusInfo& bonus = Bonuses::Get().GetBonusInfo(bonusIndex_);
    if(employeeBonusedCount_ >= bonus.count)
    {
        chkBonus_->SetEnabled(true);
        rollTimer_.Release();
        showResultOnceTimer_.Release();
    }
}

void MainUi::ShowBonusList()
{
    NString names(_T("\r\n"));
    int index = 0;
    int count = 0;
    const std::list<int>* employeeIndexes = Employees::Get().GetBonusEmployees(bonusIndex_);
    if(employeeIndexes != NULL)
    {
        std::list<int>::const_iterator ite = employeeIndexes->begin();
        for(; ite != employeeIndexes->end(); ++ ite)
        {
            int index = *ite;
            const EmployeeInfo& employee = Employees::Get().GetEmployeeInfo(index);
            names += employee.name;
            ++ count;
            if(count % 3 == 0)
                names += _T("\r\n");
            else
                names += _T("    ");
        }
        names += _T("\r\n");
        bonusList_->SetText(names);
    }
    bonusList_->SetVisible(count > 0);
}

void MainUi::MarkEmployeeBonus(int employeeIndex)
{
    Employees::Get().MarkEmployeeBonus(employeeIndex, bonusIndex_);
    ShowBonusList();
}
