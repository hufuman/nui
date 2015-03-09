#pragma once


#include <XZip.h>
#include <XUnzip.h>


class TestZip : public testing::Test
{
public:
    TestZip()
    {
    }
    virtual void SetUp()
    {
    }
    virtual void TearDown()
    {
    }

    void TestUnZip(LPCTSTR filePath1, LPCTSTR filePath2)
    {
        NInstPtr<NZip> zip1(MemToolParam), zip2(MemToolParam);

        ASSERT_TRUE(zip1->LoadFile(filePath1));
        ASSERT_TRUE(zip2->LoadFile(filePath2));

        LPCTSTR relativePath[] =
        {
            _T("1/11/111.txt"),
            _T("1/11.txt"),
            _T("2/22/222.txt"),
            _T("2/22.txt"),
        };

        ASSERT_FALSE(zip1->IsFileExists(_T("asdfasfd")));
        ASSERT_FALSE(zip2->IsFileExists(_T("asdfasfd")));

        NInstPtr<NBuffer> buffer1(MemToolParam);
        for(int i=0; i<_countof(relativePath); ++ i)
        {
            LPVOID data1, data2;
            DWORD size1, size2;

            NInstPtr<NBuffer> buffer2(MemToolParam);
            ASSERT_TRUE(zip1->GetFileContent(relativePath[i], buffer1));
            ASSERT_TRUE(zip2->GetFileContent(relativePath[i], buffer2));

            data1 = buffer1->GetBuffer();
            size1 = buffer1->GetSize();
            data2 = buffer2->GetBuffer();
            size2 = buffer2->GetSize();

            ASSERT_TRUE(zip1->IsFileExists(relativePath[i]));
            ASSERT_TRUE(zip2->IsFileExists(relativePath[i]));

            EXPECT_TRUE(size1 == size2
                && data1 != NULL && data2 != NULL
                && memcmp(data1, data2, size1) == 0);
        }
        zip1->Close();
        zip2->Close();
    }

    bool CompareZipFile(LPCTSTR filePath1, LPCTSTR filePath2)
    {
        HZIP zipFile1 = OpenZip(static_cast<void*>(const_cast<LPTSTR>(filePath1)), 0, ZIP_FILENAME);
        HZIP zipFile2 = OpenZip(static_cast<void*>(const_cast<LPTSTR>(filePath2)), 0, ZIP_FILENAME);

        bool result = false;
        for(;;)
        {
            if(zipFile1 == NULL
                || zipFile2 == NULL)
            {
                break;
            }

            NInstPtr<NBuffer> buffer1(MemToolParam);
            NInstPtr<NBuffer> buffer2(MemToolParam);

            ZIPENTRYW entry1, entry2;

            ZRESULT zResult1, zResult2;
            for(int i=0; ; ++ i)
            {
                zResult1 = GetZipItem(zipFile1, i, &entry1);
                if(zResult1 != ZR_OK)
                {
                    break;
                }

                int index1 = i;
                int index2 = 0;
                for(int j=0; ; ++j)
                {
                    zResult2 = GetZipItem(zipFile2, j, &entry2);
                    if(zResult2 != ZR_OK)
                        break;
                    if(_tcscmp(entry1.name, entry2.name) == 0)
                    {
                        index2 = j;
                        break;
                    }
                }
                if(zResult2 != ZR_OK)
                    break;

                if(_tcscmp(entry1.name, entry2.name) != 0
                    || entry1.unc_size != entry2.unc_size)
                {
                    break;
                }

                if((entry1.attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY
                    && (entry2.attr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
                {
                    continue;
                }

                zResult1 = UnzipItem(zipFile1, index1, buffer1->GetBuffer(entry1.unc_size), entry1.unc_size, ZIP_MEMORY);
                zResult2 = UnzipItem(zipFile2, index2, buffer2->GetBuffer(entry2.unc_size), entry2.unc_size, ZIP_MEMORY);
                if((zResult1 != ZR_OK && zResult1 != ZR_MORE)
                    || (zResult2 != ZR_OK && zResult2 != ZR_MORE))
                {
                    break;
                }

                LPVOID data1 = buffer1->GetBuffer();
                LPVOID data2 = buffer2->GetBuffer();
                if(memcmp(data1, data2, entry1.unc_size) != 0)
                {
                    break;
                }
            }

            result = (zResult1 == ZR_ARGS);
            break;
        }
        if(zipFile1 != NULL)
            CloseZip(zipFile1);
        if(zipFile2 != NULL)
            CloseZip(zipFile2);
        return result;
    }
};

TEST_F(TestZip, Zip)
{
    NString folderPath = TestUtil::GetTestFile(_T("Zip\\Data"));

    NString correctFile = TestUtil::GetTestFile(_T("Zip\\Data.zip"));
    NString zip1File = TestUtil::GetTestFile(_T("Temp\\Zip1.zip"));
    NString zip2File = TestUtil::GetTestFile(_T("Temp\\Zip2.zip"));

    ::CreateDirectory(TestUtil::GetTestFile(_T("Temp")), NULL);

    ASSERT_TRUE(!nui::Util::File::IsFileExists(zip1File.GetData()) || ::DeleteFile(zip1File.GetData()));
    ASSERT_TRUE(!nui::Util::File::IsFileExists(zip2File.GetData()) || ::DeleteFile(zip2File.GetData()));

    NInstPtr<NZip> zip(MemToolParam);
    ASSERT_TRUE(zip->ZipFolder(folderPath.GetData(), zip1File.GetData()));
    ASSERT_TRUE(zip->ZipFolder((folderPath + _T("\\")).GetData(), zip2File.GetData()));

    EXPECT_TRUE(CompareZipFile(correctFile.GetData(), zip1File.GetData()));
    EXPECT_TRUE(CompareZipFile(zip1File.GetData(), correctFile.GetData()));
    EXPECT_TRUE(CompareZipFile(correctFile.GetData(), zip2File.GetData()));
    EXPECT_TRUE(CompareZipFile(zip2File.GetData(), correctFile.GetData()));
}

TEST_F(TestZip, UnZip)
{
    NString correctFile = TestUtil::GetTestFile(_T("Zip\\Data.zip"));
    NString zip1File = TestUtil::GetTestFile(_T("Temp\\Zip1.zip"));
    NString zip2File = TestUtil::GetTestFile(_T("Temp\\Zip2.zip"));

    TestUnZip(correctFile.GetData(), zip1File.GetData());
    TestUnZip(correctFile.GetData(), zip2File.GetData());
}
