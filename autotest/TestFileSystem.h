#pragma once


class TestFileSystem : public testing::Test
{
public:
    TestFileSystem()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }

    void CompareFile(nui::Base::NAutoPtr<nui::Data::NFileSystem> fs, LPCTSTR relativePath, LPCTSTR realPath)
    {
        nui::Base::NInstPtr<NBuffer> buffer(MemToolParam);
        ASSERT_TRUE(fs->LoadFile(relativePath, buffer));
        DWORD dwSize;
        LPVOID pData = TestUtil::GetFileContent(realPath, dwSize);
        ASSERT_TRUE(pData != NULL);
        EXPECT_TRUE(dwSize == buffer->GetSize());
        if(dwSize == buffer->GetSize())
            EXPECT_TRUE(memcmp(pData, buffer->GetBuffer(), dwSize) == 0);
        free(pData);
    }

    void TestImpl(nui::Base::NAutoPtr<nui::Data::NFileSystem> fs)
    {
        EXPECT_TRUE(!fs->IsFileExists(_T("@root:Data\\txt")));
        EXPECT_TRUE(!fs->IsFileExists(_T("@root:Data\\xml")));

        LPCTSTR rootFiles[] = 
        {
            _T("@root:Data\\data.txt"),
            _T("@root:Data\\txt\\123.txt"),
            _T("@root:Data\\txt\\456.txt"),
            _T("@root:Data\\xml\\data.xml"),
            _T("@root:Data\\xml\\window.xml"),
        };

        for(int i=0; i<_countof(rootFiles); ++ i)
        {
            EXPECT_TRUE(fs->IsFileExists(rootFiles[i])) << rootFiles[i];
            NString realPath = TestUtil::GetTestFile(_T("Resource\\package\\")) + rootFiles[i];
            realPath.Replace(_T("@root:"), _T(""));
            CompareFile(fs, rootFiles[i], realPath.GetData());
        }

        LPCTSTR dataFiles[] = 
        {
            _T("@data:data.txt"),
            _T("@data:txt\\123.txt"),
            _T("@data:txt\\456.txt"),
            _T("@data:xml\\data.xml"),
            _T("@data:xml\\window.xml"),
        };

        for(int i=0; i<_countof(dataFiles); ++ i)
        {
            EXPECT_TRUE(fs->IsFileExists(dataFiles[i])) << dataFiles[i];
            NString realPath = TestUtil::GetTestFile(_T("Resource\\package\\data\\")) + dataFiles[i];
            realPath.Replace(_T("@data:"), _T(""));
            CompareFile(fs, dataFiles[i], realPath.GetData());
        }
    }
};

TEST_F(TestFileSystem, RealFS)
{
    nui::Base::NInstPtr<nui::Data::NFileSystem> fs(MemToolParam);
    NString package = TestUtil::GetTestFile(_T("Resource\\Package"));
    ASSERT_TRUE(fs->Init(package.GetData()));
    TestImpl(fs);
}

TEST_F(TestFileSystem, PackFS)
{
    NString xfPath = TestUtil::GetTestFile(_T("Temp\\pack.xf"));
    if(File::IsFileExists(xfPath))
        ASSERT_TRUE(!!::DeleteFile(xfPath));

    NInstPtr<NZip> zip(MemToolParam);
    NString folderPath = TestUtil::GetTestFile(_T("Resource\\Package\\"));
    ASSERT_TRUE(zip->ZipFolder(folderPath, xfPath));

    nui::Base::NInstPtr<nui::Data::NFileSystem> fs(MemToolParam);
    NString package = TestUtil::GetTestFile(_T("Temp\\pack"));
    ASSERT_TRUE(fs->Init(package.GetData()));
    TestImpl(fs);
}
