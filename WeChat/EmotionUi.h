#pragma once



class EmotionUi
{
public:
    EmotionUi(void);
    ~EmotionUi(void);

public:
    class EmotionEventData : public NEventData
    {
    public:
        NString emotion;
    };
    NEvent EmotionSelectedEvent;

public:
    void Create(NWindow* parentWindow);
    void Show(NFrame* frame);
    void Destroy();

public:
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);
    bool OnEmotionClicked(Base::NBaseObj* source, NEventData* eventData);
    bool MsgCallback(NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&);
    bool GetEmotionIndx(int x, int y, int& horzIndex, int& vertIndex) const;

private:
    NAutoPtr<NWindow> window_;
    NAutoPtr<NFrame> emotionImg_;
    NRect emotionRect_;
    EmotionEventData emotionEventData_;
};
