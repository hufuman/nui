#pragma once



class CTestDrawImage
{
public:
    CTestDrawImage();
    ~CTestDrawImage();

    bool DrawCallback(NWindow*, NRender* render, const NRect& clipRect);

public:
    void Test();

private:
    NAutoPtr<NShapeDraw> m_pShape;
    NAutoPtr<NImageDraw> m_pImage;
};