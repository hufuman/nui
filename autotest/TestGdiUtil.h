#pragma once


#include "../nui/ui/implement/Gdi/GdiUtil.h"

class TestGdiUtil : public testing::Test
{
public:

protected:
};

TEST_F(TestGdiUtil, MemExtInfo)
{
    Base::NString filePath = TestUtil::GetTestFile(_T("images\\button.png"));
    NString tmpPath = TestUtil::GetTestFile(_T("Temp\\button.tmp.png"));
    ASSERT_TRUE(!nui::Util::File::IsFileExists(tmpPath.GetData()) || ::DeleteFile(tmpPath.GetData()));
    ASSERT_TRUE(!!::CopyFile(filePath.GetData(), tmpPath.GetData(), TRUE));

    DWORD dwSize = 0;
    stImageExtInfo extInfo;
    stImageExtInfo testExtInfo;

    // Test not contain extInfo
    LPCVOID pData = TestUtil::GetFileContent(tmpPath.GetData(), dwSize);
    ASSERT_FALSE(pData == NULL || dwSize == 0);

    EXPECT_FALSE(Gdi::GetImageData((const BYTE*)pData, dwSize, extInfo));
    free((LPVOID)pData);

    // Save extInfo
    extInfo.drawType = (ImageDrawType::Type)rand();
    extInfo.horzCount = rand();
    extInfo.vertCount = rand();
    extInfo.leftParam = rand();
    extInfo.topParam = rand();
    extInfo.rightParam = rand();
    extInfo.bottomParam = rand();
    std::string strData;
    EXPECT_TRUE(Gdi::SetImageData(extInfo, strData));

    ASSERT_TRUE(TestUtil::AppendFile(tmpPath.GetData(), (LPCVOID)strData.c_str(), strData.size()));

    // Test not contain extInfo
    pData = TestUtil::GetFileContent(tmpPath.GetData(), dwSize);
    ASSERT_FALSE(pData == NULL || dwSize == 0);

    EXPECT_TRUE(Gdi::GetImageData((const BYTE*)pData, dwSize, testExtInfo));
    free((LPVOID)pData);

    EXPECT_TRUE(extInfo.drawType == testExtInfo.drawType);
    EXPECT_TRUE(extInfo.horzCount == testExtInfo.horzCount);
    EXPECT_TRUE(extInfo.vertCount == testExtInfo.vertCount);
    EXPECT_TRUE(extInfo.leftParam == testExtInfo.leftParam);
    EXPECT_TRUE(extInfo.topParam == testExtInfo.topParam);
    EXPECT_TRUE(extInfo.rightParam == testExtInfo.rightParam);
    EXPECT_TRUE(extInfo.bottomParam == testExtInfo.bottomParam);
}
