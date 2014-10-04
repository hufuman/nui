#pragma once



namespace testing
{
    namespace internal
    {
        AssertionResult CmpHelperSTREQ(const char expected_expression[],
            const char actual_expression[],
            NUI::Base::NString expected,
            const wchar_t actual[])
        {
            return CmpHelperSTREQ(expected_expression, actual_expression, expected.GetData(), actual);
        }

        AssertionResult CmpHelperSTREQ(const char expected_expression[],
            const char actual_expression[],
            NUI::Base::NString expected,
            NUI::Base::NString actual)
        {
            return CmpHelperSTREQ(expected_expression, actual_expression, expected.GetData(), actual.GetData());
        }
    }
}