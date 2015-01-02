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
        zip_ = NULL;
    }
    virtual void TearDown()
    {
        if(zip_ != NULL)
        {
            zip_->Release();
            zip_ = NULL;
        }
    }

    void TestUnZip(LPCTSTR filePath1, LPCTSTR filePath2)
    {
        NZip *zip1, *zip2;
        ASSERT_TRUE(NReflectCreate(zip1));
        ASSERT_TRUE(NReflectCreate(zip2));

        ASSERT_TRUE(zip1->LoadFile(filePath1));
        ASSERT_TRUE(zip2->LoadFile(filePath2));

        LPCTSTR relativePath[] =
        {
            _T("Data/1/11/111.txt"),
            _T("Data/1/11.txt"),
            _T("Data/2/22/222.txt"),
            _T("Data/2/22.txt"),
        };

        for(int i=0; i<_countof(relativePath); ++ i)
        {
            int index1, index2;
            LPBYTE data1, data2;
            DWORD size1, size2;
            ASSERT_TRUE(zip1->GetFileContent(relativePath[i], index1, data1, size1));
            ASSERT_TRUE(zip2->GetFileContent(relativePath[i], index2, data2, size2));
            EXPECT_TRUE(size1 == size2
                && data1 != NULL && data2 != NULL
                && memcmp(data1, data2, size1) == 0);
            zip1->ReleaseFileContent(index1);
            zip2->ReleaseFileContent(index2);
        }
        zip1->Close();
        zip2->Close();
    }

    bool CompareZipFile(LPCTSTR filePath1, LPCTSTR filePath2)
    {
        HZIP zipFile1 = OpenZip(static_cast<void*>(const_cast<LPTSTR>(filePath1)), 0, ZIP_FILENAME);
        HZIP zipFile2 = OpenZip(static_cast<void*>(const_cast<LPTSTR>(filePath2)), 0, ZIP_FILENAME);

        bool result = false;
        NBuffer *buffer1 = NULL;
        NBuffer *buffer2 = NULL;
        for(;;)
        {
            if(zipFile1 == NULL
                || zipFile2 == NULL)
            {
                break;
            }

            if(!NReflectCreate(buffer1)
                || !NReflectCreate(buffer2))
            {
                break;
            }

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
        if(buffer1 != NULL)
            buffer1->Release();
        if(buffer2 != NULL)
            buffer2->Release();
        if(zipFile1 != NULL)
            CloseZip(zipFile1);
        if(zipFile2 != NULL)
            CloseZip(zipFile2);
        return result;
    }

protected:
    NZip* zip_;
};

TEST_F(TestZip, Zip)
{
    NString folderPath = TestUtil::GetTestFile(_T("Zip\\Data"));

    NString correctFile = TestUtil::GetTestFile(_T("Zip\\Data.zip"));
    NString zip1File = TestUtil::GetTestFile(_T("Temp\\Zip1.zip"));
    NString zip2File = TestUtil::GetTestFile(_T("Temp\\Zip2.zip"));

    ASSERT_TRUE(!NUI::Util::File::IsFileExists(zip1File.GetData()) || ::DeleteFile(zip1File.GetData()));
    ASSERT_TRUE(!NUI::Util::File::IsFileExists(zip2File.GetData()) || ::DeleteFile(zip2File.GetData()));

    ASSERT_TRUE(NReflectCreate(zip_));
    ASSERT_TRUE(zip_->ZipFolder(folderPath.GetData(), zip1File.GetData()));
    ASSERT_TRUE(zip_->ZipFolder((folderPath + _T("\\")).GetData(), zip2File.GetData()));
    zip_->Release();
    zip_ = NULL;

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
