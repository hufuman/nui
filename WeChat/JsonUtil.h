#pragma once


namespace JsonUtil
{
    NString GetValue(const Json::Value& data, LPCSTR name);
    int GetIntValue(const Json::Value& data, LPCSTR name);
}