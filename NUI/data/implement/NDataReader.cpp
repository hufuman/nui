#include "stdafx.h"
#include "../NDataReader.h"
#include "XmlDataReader.h"

namespace NUI
{
    namespace Data
    {

        bool NDataReader::ParseUtf8(const char* data)
        {
            return ParseUtf8(data, strlen(data));
        }

        bool NDataReader::ReadValue(LPCTSTR name, INT32& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;
            value = _ttoi(data.GetData());
            return true;
        }

        bool NDataReader::ReadValue(LPCTSTR name, DWORD& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;
            _stscanf(data.GetData(), TEXT("%u"), &value);
            return true;
        }

        bool NDataReader::ReadValue(LPCTSTR name, INT64& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;
            value = _tstoi64(data.GetData());
            return true;
        }

        bool NDataReader::ReadValue(LPCTSTR name, bool& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;
            data.MakeLower();

            value = false;
            if(data == TEXT("true"))
                value = true;
            NAssertError(data == TEXT("true")
                || data == TEXT("false"),
                TEXT("invalid boolean value"));
            return true;
        }

        bool NDataReader::ReadValue(LPCTSTR name, NUI::Base::TPoint& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;
            size_t pos = data.IndexOf(TEXT(","));
            if(pos == -1)
                return false;
            NUI::Base::NString x = data.SubString(0, pos);
            NUI::Base::NString y = data.SubString(pos + 1);
            value.X = _ttoi(x.GetData());
            value.Y = _ttoi(y.GetData());
            return true;
        }

        bool NDataReader::ReadValue(LPCTSTR name, NUI::Base::TSize& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;

            size_t pos = data.IndexOf(TEXT(","));
            if(pos == -1)
                return false;

            NUI::Base::NString width = data.SubString(0, pos);
            NUI::Base::NString height = data.SubString(pos + 1);
            value.Width = _ttoi(width.GetData());
            value.Height = _ttoi(height.GetData());
            return true;
        }

        bool NDataReader::ReadValue(LPCTSTR name, NUI::Base::TRect& value)
        {
            NUI::Base::NString data;
            if(!ReadValue(name, data))
                return false;

            size_t pos = 0;
            size_t last = 0;
            NUI::Base::NString rect[4];
            for(int i=0; i<3 && last < data.GetLength(); ++ i)
            {
                pos = data.IndexOf(TEXT(","), last);
                if(pos == -1)
                    return false;
                rect[i] = data.SubString(last, pos - last);
                last = pos + 1;
            }
            rect[3] = data.SubString(last);
            value.SetRect(_ttoi(rect[0].GetData()),
                _ttoi(rect[1].GetData()),
                _ttoi(rect[2].GetData()),
                _ttoi(rect[3].GetData()));
            return true;
        }
        NUI_API NDataReader* CreateDataReader(DataReaderType type)
        {
            NDataReader* reader = NULL;
            if(type == ReaderJson)
            {
            }
            else if(type == ReaderXml)
            {
                reader = dynamic_cast<NDataReader*>(new XmlDataReader());
            }
            NAssertError(reader != NULL, TEXT("unknown type of DataReader: %d"), type);
            return reader;
        }

    }
}