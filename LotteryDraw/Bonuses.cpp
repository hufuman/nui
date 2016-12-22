#include "StdAfx.h"
#include "Bonuses.h"

Bonuses::Bonuses(void)
{
}

Bonuses::~Bonuses(void)
{
}

Bonuses& Bonuses::Get()
{
    static Bonuses instance;
    return instance;
}

void Bonuses::Load()
{
    NInstPtr<NFileSystem> fs(MemToolParam);
    NInstPtr<NBuffer> buffer(MemToolParam);
    fs->LoadFile(_T("@data:Bonuses.txt"), buffer);
    NString result = utf82t(static_cast<LPCSTR>(buffer->GetBuffer())).Replace(_T("\r"), _T(""));

    int lineTokenPos = 0;
    NString line;
    NString title, titleImage, name, descImage, count, showResultOnce;
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();

    // first line is column
    result.Tokenize(lineTokenPos, _T("\n"), false, line);

    NString msg;
    while(result.Tokenize(lineTokenPos, _T("\n"), false, line))
    {
        int pos = 0;
        if(!line.Tokenize(pos, _T("\t"), false, title)
            || !line.Tokenize(pos, _T("\t"), false, titleImage)
            || !line.Tokenize(pos, _T("\t"), false, name)
            || !line.Tokenize(pos, _T("\t"), false, descImage)
            || !line.Tokenize(pos, _T("\t"), false, count)
            || !line.Tokenize(pos, _T("\t"), false, showResultOnce))
        {
            break;
        }

        BonusInfo info;
        info.title = title;

        NString imagePath(_T("@images:Bonuses\\"));
        info.titleDraw = loader->LoadImage(imagePath + titleImage);
        info.descDraw = loader->LoadImage(imagePath + descImage);

        if(info.titleDraw == NULL)
        {
            msg = _T("Failed to load image: ");
            msg += titleImage;
            ::MessageBox(NULL, msg, _T("Error"), MB_OK);
            return;
        }

        if(info.descDraw == NULL)
        {
            msg = _T("Failed to load image: ");
            msg += descImage;
            ::MessageBox(NULL, msg, _T("Error"), MB_OK);
            return;
        }

        info.titleDraw->AddRef();
        info.descDraw->AddRef();

        info.name = name;
        info.count = _ttoi(count);
        info.showResultOnce = showResultOnce == _T("ÊÇ");
        bonuses_.AddItem(info);
    }
}

void Bonuses::Destroy()
{
    int count = bonuses_.Count();
    for(int i=0; i<count; ++ i)
    {
        const BonusInfo& info = bonuses_[i];
        if(info.titleDraw != NULL)
            info.titleDraw->Release();
        if(info.descDraw != NULL)
            info.descDraw->Release();
    }
    bonuses_.Clear();
}

void Bonuses::Reset()
{
    Destroy();
    Load();
}

void Bonuses::SetBonusShowResultOnce(int index, bool showResultOnce)
{
    bonuses_[index].showResultOnce = showResultOnce;
}

int Bonuses::Count() const
{
    return bonuses_.Count();
}

const BonusInfo& Bonuses::GetBonusInfo(int index) const
{
    return bonuses_[index];
}
