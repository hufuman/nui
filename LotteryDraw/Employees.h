#pragma once


class EmployeeInfo
{
public:
    NString name;
    NString seq;
    NImageDraw* draw;
    int bonusIndex;
};

class Employees
{
    Employees();
    Employees(const Employees&);
    Employees& operator = (const Employees&);
public:
    ~Employees(void);

    static Employees& Get();

    void Load();
    void LoadBonus();
    void Destroy();

    void Reset();
    int Count() const;
    int GetRandomNextIndex() const;
    const std::list<int>* GetBonusEmployees(int bonusIndex) const;
    const EmployeeInfo& GetEmployeeInfo(int index) const;
    void MarkEmployeeBonus(int employeeIndex, int bonusIndex);

private:
    bool loading_;
    NArrayT<EmployeeInfo> employees_;

    // bonusIndex => employeeIndexs;
    typedef std::map<int, std::list<int>> BonusResultMap;
    BonusResultMap bonusResultMap_;
};
