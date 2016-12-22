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
    window_->KeyEvent.AddHandler(this, &MainUi::OnKeyEvent);

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
    lotteryBkg_ = rootFrame->GetChildById<NImage*>(_T("lotteryBkg"));
    defaultBkg_ = rootFrame->GetChildById<NImage*>(_T("defaultBkg"));

    btnPrev_->ClickEvent.AddHandler(this, &MainUi::OnBtnPrev);
    btnNext_->ClickEvent.AddHandler(this, &MainUi::OnBtnNext);
    rootFrame->GetChildById<NFrame*>(_T("btnView"))->ClickEvent.AddHandler(this, &MainUi::OnBtnView);
    rootFrame->GetChildById<NFrame*>(_T("btnReset"))->ClickEvent.AddHandler(this, &MainUi::OnBtnReset);

    chkBonus_->ClickEvent.AddHandler(this, &MainUi::OnChkBonus);

    // Load Data
    btnPrev_->SetEnabled(false);
    btnNext_->SetEnabled(false);
    rootFrame->GetChildById<NFrame*>(_T("btnView"))->SetEnabled(false);
    rootFrame->GetChildById<NFrame*>(_T("btnReset"))->SetEnabled(false);

    loadDataThread_ = (HANDLE)::_beginthreadex(0, 0, &MainUi::LoadDataProc, (void*)this, 0, 0);

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
                if(count % 5 == 0)
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
    ToggleBonus();
    return true;
}

bool MainUi::OnKeyEvent(nui::Base::NBaseObj *source, nui::Ui::NEventData *eventData)
{
    if(!btnGroup_->IsVisible())
        return false;

    NWindow::KeyEventData* data = static_cast<NWindow::KeyEventData*>(eventData);
    if(data->isDownEvent)
        return false;
    if(data->key == ' ' && step_ > 0)
    {
        chkBonus_->SetCheck(!chkBonus_->IsChecked());
        ToggleBonus();
    }
    else if(data->key == VK_LEFT)
    {
        ShowStep(step_ - 1);
    }
    else if(data->key == VK_RIGHT)
    {
        ShowStep(step_ + 1);
    }
    return true;
}

void MainUi::ToggleBonus()
{
    if(chkBonus_->IsChecked())
        StartRoll();
    else
        StopRoll();
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

    defaultBkg_->SetVisible(step == 0);
    lotteryBkg_->SetVisible(step != 0);
    employeePhoto_->SetVisible(false);
    labelName_->SetText(_T(""));
    labelSeq_->SetText(_T(""));
}

void MainUi::StartRoll()
{
    rollTickCount_ = ::GetTickCount();
    NInstPtr<NTimerSrv> timer(MemToolParam);
    rollTimer_ = timer->startTimer(20, MakeDelegate(this, &MainUi::RollProc));

    bonusDesc_->SetVisible(false);
}

void MainUi::StopRoll()
{
    const BonusInfo& bonus = Bonuses::Get().GetBonusInfo(bonusIndex_);

    if(bonus.showResultOnce)
    {
        rollTickCount_ = ::GetTickCount();
        chkBonus_->SetEnabled(false);
        employeeBonusedCount_ = 0;
    }
    else
    {
        MarkEmployeeBonus(employeeIndex_);
        rollTimer_.Release();
    }
}

void MainUi::RollProc()
{
    const BonusInfo& bonus = Bonuses::Get().GetBonusInfo(bonusIndex_);
    if(bonus.showResultOnce && (::GetTickCount() - rollTickCount_) >= 1000 && !chkBonus_->IsEnabled())
    {
        rollTimer_.Release();

        MarkEmployeeBonus(employeeIndex_);
        ++ employeeBonusedCount_;
        const BonusInfo& bonus = Bonuses::Get().GetBonusInfo(bonusIndex_);
        if(employeeBonusedCount_ >= bonus.count)
        {
            chkBonus_->SetEnabled(true);
            Bonuses::Get().SetBonusShowResultOnce(bonusIndex_, false);
            return;
        }

        NInstPtr<NTimerSrv> timer(MemToolParam);
        showResultOnceTimer_ = timer->startTimer(700, MakeDelegate(this, &MainUi::ShowResultOnceProc));
    }
    else
    {
        employeeIndex_ = Employees::Get().GetRandomNextIndex();

        const EmployeeInfo& employee = Employees::Get().GetEmployeeInfo(employeeIndex_);
        employeePhoto_->SetForeDraw(employee.draw);
        employeePhoto_->SetVisible(true);

        labelName_->SetText(employee.name);
        labelSeq_->SetText(employee.seq);
    }
}

void MainUi::ShowResultOnceProc()
{
    rollTickCount_ = ::GetTickCount();
    showResultOnceTimer_.Release();

    NInstPtr<NTimerSrv> timer(MemToolParam);
    rollTimer_ = timer->startTimer(20, MakeDelegate(this, &MainUi::RollProc));
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

unsigned int MainUi::LoadDataProc(void* param)
{
    MainUi* pThis = (MainUi*)param;
    pThis->btnGroup_->SetVisible(false);
    Employees::Get().Load();
    Bonuses::Get().Load();

    NFrame* rootFrame = pThis->window_->GetRootFrame();

    pThis->btnPrev_->SetEnabled(true);
    pThis->btnNext_->SetEnabled(true);
    rootFrame->GetChildById<NFrame*>(_T("btnView"))->SetEnabled(true);
    rootFrame->GetChildById<NFrame*>(_T("btnReset"))->SetEnabled(true);

    pThis->bonusCount_ = Bonuses::Get().Count();

    for(int i=0;i<pThis->bonusCount_; ++ i)
    {
        const std::list<int>* list = Employees::Get().GetBonusEmployees(i);
        if(list != NULL && !list->empty())
        {
            Bonuses::Get().SetBonusShowResultOnce(i, false);
        }
    }
    pThis->btnGroup_->SetVisible(true);
    return 0;
}
