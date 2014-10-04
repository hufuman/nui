// AutoTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestUtil.h"


#include "TestDataTypes.h"
#include "TestString.h"
#include "TestXmlDataReader.h"
#include "TestReflect.h"
#include "TestCore.h"
#include "TestFileUtil.h"

int _tmain(int argc, _TCHAR* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

