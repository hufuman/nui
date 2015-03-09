#pragma once




class CAlphaDC
{
public:
    CAlphaDC();
    ~CAlphaDC(void);

    operator HDC ();

    /**
     * copyOrg, must be true if there is any text-drawing. otherwise it could be false
     */
    bool Init(HDC hDc, const nui::Base::NRect& rcPaint, const nui::Base::NSize& size, bool bCopyOrg);
    bool EndDraw(BYTE byAlpha);

private:
    void Destroy();
    void CopyOrg();

private:
    HDC memDc_;
    HBITMAP memBmp_;
    HGDIOBJ oldBmp_;
    BITMAP bmpInfo_;

    HDC orgDc_;
    nui::Base::NRect rcPaint_;
};
