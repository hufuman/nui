// AutoTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TestUtil.h"


// Base
#include "TestDataTypes.h"
#include "TestReflect.h"
#include "TestCore.h"

// Data
#include "TestString.h"
#include "TestXmlDataReader.h"
#include "TestModule.h"
#include "TestBuffer.h"
#include "TestFileMapping.h"
#include "TestZip.h"

// Util
#include "TestFileUtil.h"

int _tmain(int argc, _TCHAR* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

