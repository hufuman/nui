#include "StdAfx.h"
#include "EmotionUi.h"

LPCTSTR g_EmotionText[] = 
{
    _T("Î¢Ð¦"),
    _T("Æ²×ì"),
    _T("É«"),
    _T("·¢´ô"),
    _T("µÃÒâ"),
    _T("Á÷Àá"),
    _T("º¦Ðß"),
    _T("±Õ×ì"),
    _T("Ë¯"),
    _T("´ó¿Þ"),
    _T("ÞÏÞÎ"),
    _T("·¢Å­"),
    _T("µ÷Æ¤"),
    _T("ßÚÑÀ"),
    _T("¾ªÑÈ"),
    _T("ÄÑ¹ý"),
    _T("¿á"),
    _T("Àäº¹"),
    _T("×¥¿ñ"),
    _T("ÍÂ"),
    _T("ÍµÐ¦"),
    _T("Óä¿ì"),
    _T("°×ÑÛ"),
    _T("°ÁÂý"),
    _T("¼¢¶ö"),
    _T("À§"),
    _T("¾ª¿Ö"),
    _T("Á÷º¹"),
    _T("º©Ð¦"),
    _T("ÓÆÏÐ"),
    _T("·Ü¶·"),
    _T("ÖäÂî"),
    _T("ÒÉÎÊ"),
    _T("Ðê"),
    _T("ÔÎ"),
    _T("·èÁË"),
    _T("Ë¥"),
    _T("÷¼÷Ã"),
    _T("ÇÃ´ò"),
    _T("ÔÙ¼û"),
    _T("²Áº¹"),
    _T("¿Ù±Ç"),
    _T("¹ÄÕÆ"),
    _T("ôÜ´óÁË"),
    _T("»µÐ¦"),
    _T("×óºßºß"),
    _T("ÓÒºßºß"),
    _T("¹þÇ·"),
    _T("±ÉÊÓ"),
    _T("Î¯Çü"),
    _T("¿ì¿ÞÁË"),
    _T("ÒõÏÕ"),
    _T("Ç×Ç×"),
    _T("ÏÅ"),
    _T("¿ÉÁ¯"),
    _T("²Ëµ¶"),
    _T("Î÷¹Ï"),
    _T("Æ¡¾Æ"),
    _T("ÀºÇò"),
    _T("Æ¹ÅÒ"),
    _T("¿§·È"),
    _T("·¹"),
    _T("ÖíÍ·"),
    _T("Ãµ¹å"),
    _T("µòÐ»"),
    _T("×ì´½"),
    _T("°®ÐÄ"),
    _T("ÐÄËé"),
    _T("µ°¸â"),
    _T("ÉÁµç"),
    _T("Õ¨µ¯"),
    _T("µ¶"),
    _T("×ãÇò"),
    _T("Æ°³æ"),
    _T("±ã±ã"),
    _T("ÔÂÁÁ"),
    _T("Ì«Ñô"),
    _T("ÀñÎï"),
    _T("Óµ±§"),
    _T("Ç¿"),
    _T("Èõ"),
    _T("ÎÕÊÖ"),
    _T("Ê¤Àû"),
    _T("±§È­"),
    _T("¹´Òý"),
    _T("È­Í·"),
    _T("²î¾¢"),
    _T("°®Äã"),
    _T("NO"),
    _T("OK"),
    _T("°®Çé"),
    _T("·ÉÎÇ"),
    _T("ÌøÌø"),
    _T("·¢¶¶"),
    _T("âæ»ð"),
    _T("×ªÈ¦"),
    _T("¿ÄÍ·"),
    _T("»ØÍ·"),
    _T("ÌøÉþ"),
    _T("Í¶½µ"),
    _T("¼¤¶¯"),
    _T("ÂÒÎè"),
    _T("Ï×ÎÇ"),
    _T("×óÌ«¼«"),
    _T("ÓÒÌ«¼«")
};

EmotionUi::EmotionUi(void)
{
}

EmotionUi::~EmotionUi(void)
{
}

void EmotionUi::Create(NWindow* parentWindow)
{
    window_.Create(MemToolParam);
    window_->SetStyle(WindowStyle::MenuLike);
    window_->WindowCreatedEvent.AddHandler(this, &EmotionUi::OnWindowCreated);
    window_->Create(parentWindow->GetNative());
    window_->GetRootFrame()->ApplyStyle(_T("@EmotionUi:MainUi"));
    window_->SetMsgFilterCallback(MakeDelegate(this, &EmotionUi::MsgCallback));
}

void EmotionUi::Show(NFrame* frame)
{
    emotionEventData_.emotion = _T("");
    NRect frameRect = frame->GetRootRect();
    NPoint point(frameRect.Left, frameRect.Top);
    ::ClientToScreen(frame->GetWindow()->GetNative(), point);

    NRect wndRect;
    window_->GetRect(wndRect);
    window_->SetPos(point.X, point.Y - wndRect.Height());

    emotionImg_ = window_->GetRootFrame()->GetChildById<NButton*>(_T("emotion"));
    emotionImg_->ClickEvent.AddHandler(MakeDelegate(this, &EmotionUi::OnEmotionClicked));
    emotionRect_ = emotionImg_->GetRootRect();

    window_->SetVisible(true);
}

bool EmotionUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    return false;
}

bool EmotionUi::OnEmotionClicked(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame::ClickEventData* data = static_cast<NFrame::ClickEventData*>(eventData);

    int horzIndex = 0;
    int vertIndex = 0;

    if(!GetEmotionIndx(data->point.X, data->point.Y, horzIndex, vertIndex))
        return false;

    window_->SetVisible(false);

    emotionEventData_.emotion = g_EmotionText[horzIndex + vertIndex * 15];
    EmotionSelectedEvent.Invoke(NULL, &emotionEventData_);

    return false;
}

bool EmotionUi::MsgCallback(NWindowBase*, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT&)
{
    if(msg != WM_MOUSEMOVE)
        return false;

    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);

    int horzIndex = 0;
    int vertIndex = 0;

    if(!GetEmotionIndx(xPos, yPos, horzIndex, vertIndex))
        return false;

    NString text = g_EmotionText[horzIndex + vertIndex * 15];
    emotionImg_->SetTooltip(text);
    emotionImg_->ShowTooltip();

    return false;
}

void EmotionUi::Destroy()
{
    window_ = NULL;
}

bool EmotionUi::GetEmotionIndx(int x, int y, int& horzIndex, int& vertIndex) const
{
    if(!emotionRect_.Contains(x, y))
        return false;

    x -= emotionRect_.Left;
    y -= emotionRect_.Top;

    horzIndex = x / 29;
    vertIndex = y / 29;

    if(horzIndex < 0 || horzIndex >= 15 || vertIndex < 0 || vertIndex >= 7)
        return false;
    return true;
}