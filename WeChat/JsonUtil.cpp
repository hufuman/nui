#include "StdAfx.h"
#include "JsonUtil.h"


namespace JsonUtil
{
    NString GetValue(const Json::Value& data, LPCSTR name)
    {
        return utf82t(data[name].asString().c_str());
    }

    int GetIntValue(const Json::Value& data, LPCSTR name)
    {
        return data[name].asInt();
    }
}
