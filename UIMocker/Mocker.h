#pragma once

class CMocker
{
public:
    CMocker(void);
    ~CMocker(void);

    bool mock(const NString& styleName, NString& error);

private:
    NAutoPtr<NFrame> frame_;
    bool onWindowCreated(NBaseObj*, NEventData*);
};
