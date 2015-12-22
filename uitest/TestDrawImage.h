#pragma once



class CTestDrawImage
{
public:
    CTestDrawImage();
    ~CTestDrawImage();

    bool DrawCallback(NBaseObj* baseObj, NEventData* eventData);

public:
    void Test();

private:
    NAutoPtr<NShapeDraw> m_pShape;
    NAutoPtr<NImageDraw> m_pImage;
};