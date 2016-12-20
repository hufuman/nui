#pragma once



class BonusInfo
{
public:
    NString name;
    NImageDraw* titleDraw;
    NImageDraw* descDraw;
    int count;
    bool showResultOnce;
};

class Bonuses
{
    Bonuses();
    Bonuses(const Bonuses&);
    Bonuses& operator = (const Bonuses&);
public:
    ~Bonuses(void);

    static Bonuses& Get();

    void Load();
    void Destroy();

    void SetBonusShowResultOnce(int index, bool showResultOnce);
    int Count() const;
    const BonusInfo& GetBonusInfo(int index) const;

private:
    NArrayT<BonusInfo> bonuses_;
};
