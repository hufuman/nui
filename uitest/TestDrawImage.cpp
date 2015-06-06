#include "stdafx.h"
#include "TestDrawImage.h"


CTestDrawImage::CTestDrawImage()
{
    ;
}

CTestDrawImage::~CTestDrawImage()
{
    ;
}

bool CTestDrawImage::DrawCallback(NWindow* window, NRender* render, const NRect& clipRect)
{
    NRect dstRect;
    window->GetRect(dstRect);
    dstRect.Offset(-dstRect.Left, -dstRect.Top);
    render->DrawShape(m_pShape, dstRect);
    render->DrawImage(m_pImage, 0, 0, dstRect, 0);
    return true;
}

void CTestDrawImage::Test()
{
    NInstPtr<NWindow> window(MemToolParam);
    window->Create(NULL);
    window->SetSize(520, 420);
    window->CenterWindow(NULL);
    window->SetText(_T("Test Window"));
    window->SetVisible(true);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    m_pImage = loader->LoadImage(_T("@skin:images\\nineimage.png"));
    m_pShape = loader->CreateShape(MemToolParam);
    m_pShape->SetBorderWidth(0)->SetFillColor(MakeArgb(255, 0, 255, 0))->SetStyle(NShapeDraw::Rect);

    // m_pImage->SetDrawType(ImageDrawType::Stretch);

    // m_pImage->SetDrawType(ImageDrawType::Tile);

    m_pImage->SetDrawType(ImageDrawType::NineStretch)->SetStretchParam(60, 50, 60, 50);

    window->SetDrawCallback(MakeDelegate(this, &CTestDrawImage::DrawCallback));

    nui::Ui::NMsgLoop loop;
    loop.Loop(window->GetNative());

    m_pShape = NULL;
    m_pImage = NULL;
    window = NULL;
}