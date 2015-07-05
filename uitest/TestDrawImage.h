#pragma once



class CTestDrawImage
{
public:
    CTestDrawImage();
    ~CTestDrawImage();

    bool DrawCallback(NWindow*, NRender* render, HRGN clipRgn);

public:
    void Test();

private:
    NAutoPtr<NShapeDraw> m_pShape;
    NAutoPtr<NImageDraw> m_pImage;
};