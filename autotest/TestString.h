#pragma once




class TestString : public testing::Test
{
public:
    TestString()
    {
        ;
    }
    ~TestString()
    {
        ;
    }
};

TEST_F(TestString, Constructor)
{
    NString s1;
    EXPECT_TRUE(s1.IsEmpty());

    LPCTSTR szTestCase = _T("AKwef23fwafsdds");
    NString s2(szTestCase);
    EXPECT_EQ(s2, szTestCase);

    NString s3(s2);
    EXPECT_EQ(s2, s3);

    NString s4;
    s4 = s3;
    EXPECT_EQ(s3, s4);
}

TEST_F(TestString, Operator)
{
    LPCTSTR szTestCase = _T("AKwI#@$&(&#@g23fwafsdds");
    NString s1;

    s1 = szTestCase;
    EXPECT_EQ(s1, szTestCase);

    EXPECT_STREQ(s1 + _T("(#FSKJDF(*#"), _T("AKwI#@$&(&#@g23fwafsdds(#FSKJDF(*#"));

    s1 += _T("(#FSKJDF(*#");
    EXPECT_STREQ(s1, _T("AKwI#@$&(&#@g23fwafsdds(#FSKJDF(*#"));

    EXPECT_STREQ(s1 + _T('B'), _T("AKwI#@$&(&#@g23fwafsdds(#FSKJDF(*#B"));

    s1 += _T('B');
    EXPECT_STREQ(s1, _T("AKwI#@$&(&#@g23fwafsdds(#FSKJDF(*#B"));

    EXPECT_TRUE(s1 == _T("AKwI#@$&(&#@g23fwafsdds(#FSKJDF(*#B"));
    EXPECT_TRUE(s1 != _T("g23fwafsdds(#FSKJDF(*#B"));

    NString s2, s3;
    s2 = _T("WEM@DMSDF");
    s3 = _T("W#LKJF*#LKS");

    EXPECT_EQ(s2 + s3, _T("WEM@DMSDFW#LKJF*#LKS"));

    s2 += s3;
    EXPECT_EQ(s2, _T("WEM@DMSDFW#LKJF*#LKS"));

    EXPECT_TRUE(s2 == s2);
    EXPECT_TRUE(s2 != s3);
}

TEST_F(TestString, Util)
{
    NString s1;
    s1.Assign(_T("abcd1234%^&*"), 12);
    EXPECT_STREQ(s1, _T("abcd1234%^&*"));
    s1.Assign(_T("abcd1234%^&*"), 12);
    EXPECT_STREQ(s1, _T("abcd1234%^&*"));
    s1.Assign(_T("abcd1234%^&*"), 13);
    EXPECT_EQ(s1.GetLength(), 13);
    EXPECT_FALSE(s1 == _T("abcd1234%^&*"));

    // Lower / Upper / Trim / Replace / SubString
    NString s2;
    s2 = _T("aBcDeFg");
    EXPECT_TRUE(s2.MakeLower() == _T("abcdefg"));
    s2 = _T("aBcDeFg");
    EXPECT_TRUE(s2.MakeUpper() == _T("ABCDEFG"));
    s2 = _T(" \r\n \tabc\t \r\n");
    EXPECT_TRUE(s2.Trim() == _T("abc"));

    s2 = _T("abcabcab");
    EXPECT_TRUE(s2.Replace(_T("ab"), _T("")) == _T("cc"));
    s2 = _T("abcabcab");
    EXPECT_TRUE(s2.Replace(_T("ab"), _T("bb")) == _T("bbcbbcbb"));
    s2 = _T("abcabcab");
    EXPECT_TRUE(s2.Replace(_T("ab"), _T("ab")) == _T("abcabcab"));

    s2 = _T("aBcDeFg");
    EXPECT_TRUE(s2.CompareNoCases(_T("abcdefg")));

    NString s3;
    EXPECT_TRUE(s3.IsEmpty());

    s2 = _T("abcdefg");
    EXPECT_EQ(s2.SubString(2), _T("cdefg"));
    EXPECT_EQ(s2.SubString(3), _T("defg"));
    EXPECT_EQ(s2.SubString(4), _T("efg"));
    EXPECT_EQ(s2.SubString(100), _T(""));

    s2 = _T("abcdefg");
    EXPECT_EQ(s2.SubString(2, 1), _T("c"));
    EXPECT_EQ(s2.SubString(2, 2), _T("cd"));
    EXPECT_EQ(s2.SubString(2, 3), _T("cde"));
    EXPECT_EQ(s2.SubString(100, 1), _T(""));

    // IndexOf
    s2 = _T("abcabcabc");
    EXPECT_TRUE(s2.IndexOf(_T('a')) == 0);
    EXPECT_TRUE(s2.IndexOf(_T('b')) == 1);
    EXPECT_TRUE(s2.IndexOf(_T('c')) == 2);
    EXPECT_TRUE(s2.IndexOf(_T('d')) == -1);
    EXPECT_TRUE(s2.IndexOf(_T("a")) == 0);
    EXPECT_TRUE(s2.IndexOf(_T("b")) == 1);
    EXPECT_TRUE(s2.IndexOf(_T("c")) == 2);
    EXPECT_TRUE(s2.IndexOf(_T("ab")) == 0);
    EXPECT_TRUE(s2.IndexOf(_T("bc")) == 1);
    EXPECT_TRUE(s2.IndexOf(_T("ca")) == 2);
    EXPECT_TRUE(s2.IndexOf(_T("abc")) == 0);
    EXPECT_TRUE(s2.IndexOf(_T("bca")) == 1);
    EXPECT_TRUE(s2.IndexOf(_T("cab")) == 2);

    s2 = _T("abcabcabc");
    EXPECT_TRUE(s2.IndexOf(_T('a'), 3) == 3);
    EXPECT_TRUE(s2.IndexOf(_T('b'), 3) == 4);
    EXPECT_TRUE(s2.IndexOf(_T('c'), 3) == 5);
    EXPECT_TRUE(s2.IndexOf(_T('d'), 3) == -1);
    EXPECT_TRUE(s2.IndexOf(_T("a"), 3) == 3);
    EXPECT_TRUE(s2.IndexOf(_T("b"), 3) == 4);
    EXPECT_TRUE(s2.IndexOf(_T("c"), 3) == 5);
    EXPECT_TRUE(s2.IndexOf(_T("ab"), 3) == 3);
    EXPECT_TRUE(s2.IndexOf(_T("bc"), 3) == 4);
    EXPECT_TRUE(s2.IndexOf(_T("ca"), 3) == 5);
    EXPECT_TRUE(s2.IndexOf(_T("abc"), 3) == 3);
    EXPECT_TRUE(s2.IndexOf(_T("bca"), 3) == 4);
    EXPECT_TRUE(s2.IndexOf(_T("cab"), 3) == 5);

    // LastIndexOf
    s2 = _T("abcabcabc");
    EXPECT_TRUE(s2.LastIndexOf(_T('a')) == 6);
    EXPECT_TRUE(s2.LastIndexOf(_T('b')) == 7);
    EXPECT_TRUE(s2.LastIndexOf(_T('c')) == 8);
    EXPECT_TRUE(s2.LastIndexOf(_T('d')) == -1);
    EXPECT_TRUE(s2.LastIndexOf(_T("a")) == 6);
    EXPECT_TRUE(s2.LastIndexOf(_T("b")) == 7);
    EXPECT_TRUE(s2.LastIndexOf(_T("c")) == 8);
    EXPECT_TRUE(s2.LastIndexOf(_T("ab")) == 6);
    EXPECT_TRUE(s2.LastIndexOf(_T("bc")) == 7);
    EXPECT_TRUE(s2.LastIndexOf(_T("ca")) == 5);
    EXPECT_TRUE(s2.LastIndexOf(_T("abc")) == 6);
    EXPECT_TRUE(s2.LastIndexOf(_T("bca")) == 4);
    EXPECT_TRUE(s2.LastIndexOf(_T("cab")) == 5);

    s2 = _T("abcabcabc");
    EXPECT_TRUE(s2.LastIndexOf(_T('a'), 5) == 3);
    EXPECT_TRUE(s2.LastIndexOf(_T('b'), 5) == 4);
    EXPECT_TRUE(s2.LastIndexOf(_T('c'), 5) == 5);
    EXPECT_TRUE(s2.LastIndexOf(_T('d'), 5) == -1);
    EXPECT_TRUE(s2.LastIndexOf(_T("a"), 5) == 3);
    EXPECT_TRUE(s2.LastIndexOf(_T("b"), 5) == 4);
    EXPECT_TRUE(s2.LastIndexOf(_T("c"), 5) == 5);
    EXPECT_TRUE(s2.LastIndexOf(_T("ab"), 5) == 3);
    EXPECT_TRUE(s2.LastIndexOf(_T("bc"), 5) == 4);
    EXPECT_TRUE(s2.LastIndexOf(_T("ca"), 5) == 5);
    EXPECT_TRUE(s2.LastIndexOf(_T("abc"), 5) == 3);
    EXPECT_TRUE(s2.LastIndexOf(_T("bca"), 5) == 4);
    EXPECT_TRUE(s2.LastIndexOf(_T("cab"), 5) == 5);
}

TEST_F(TestString, Format)
{
    NString s1;
    EXPECT_EQ(s1.Format(_T("")), _T(""));
    EXPECT_EQ(s1.Format(_T("a")), _T("a"));
    EXPECT_EQ(s1.Format(_T("abc")), _T("abc"));
    EXPECT_EQ(s1.Format(_T("%%")), _T("%"));
    EXPECT_EQ(s1.Format(_T("a%%a")), _T("a%a"));
    EXPECT_EQ(s1.Format(_T("%d%s"), 2345234, _T("abwejf")), _T("2345234abwejf"));
}

TEST_F(TestString, Tokenize)
{
    NString s1(_T(""));
    NString token;
    int pos = 0;
    EXPECT_FALSE(s1.Tokenize(pos, _T(""), false, token));

    pos = 0;
    s1 = _T("abc");
    EXPECT_TRUE(s1.Tokenize(pos, _T(""), false, token) && token == _T("a"));
    EXPECT_TRUE(s1.Tokenize(pos, _T(""), false, token) && token == _T("b"));
    EXPECT_TRUE(s1.Tokenize(pos, _T(""), false, token) && token == _T("c"));
    EXPECT_FALSE(s1.Tokenize(pos, _T(""), false, token));;

    pos = 0;
    s1 = _T("abc");
    EXPECT_TRUE(s1.Tokenize(pos, _T("\\"), false, token) && token == _T("abc"));

    pos = 0;
    s1 = _T("a2b2c");
    EXPECT_TRUE(s1.Tokenize(pos, _T("2"), false, token) && token == _T("a"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("2"), false, token) && token == _T("b"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("2"), false, token) && token == _T("c"));
    EXPECT_TRUE(!s1.Tokenize(pos, _T("2"), false, token));

    pos = 0;
    s1= _T("a\t\tb\t\tc\t\t");
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), false, token) && token == _T("a"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), false, token) && token == _T("b"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), false, token) && token == _T("c"));
    EXPECT_FALSE(s1.Tokenize(pos, _T("\t"), false, token));

    pos = 0;
    s1= _T("a\t\tb\t\tc\t\t");
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), true, token) && token == _T("a"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), true, token) && token == _T(""));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), true, token) && token == _T("b"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), true, token) && token == _T(""));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), true, token) && token == _T("c"));
    EXPECT_TRUE(s1.Tokenize(pos, _T("\t"), true, token) && token == _T(""));
    EXPECT_FALSE(s1.Tokenize(pos, _T("\t"), true, token));
}

TEST_F(TestString, arg)
{
    NString s1(_T("%1"));
    EXPECT_EQ(s1.arg(_T("abc")),                _T("abc"));
    EXPECT_EQ(s1.arg((SHORT)-1234),             _T("-1234"));
    EXPECT_EQ(s1.arg((USHORT)2345),             _T("2345"));
    EXPECT_EQ(s1.arg((INT32)-3456),             _T("-3456"));
    EXPECT_EQ(s1.arg((UINT32)4567),             _T("4567"));
    EXPECT_EQ(s1.arg((INT64)-4294967296),       _T("-4294967296"));
    EXPECT_EQ(s1.arg((UINT64)4294967300),       _T("4294967300"));
    EXPECT_EQ(s1.arg((LONG)-7890),              _T("-7890"));
    EXPECT_EQ(s1.arg((ULONG)8766),              _T("8766"));
    EXPECT_EQ(s1.arg(_T('a')),                  _T("a"));
    EXPECT_EQ(s1.arg(-6554.5678).SubString(0, 10),               _T("-6554.5678"));
    EXPECT_EQ(s1.arg(NString(_T("aslkdj"))),    _T("aslkdj"));

    s1 = _T("%1%2%3");
    EXPECT_EQ(s1.arg(_T("aa")), _T("aa%2%3"));
    EXPECT_EQ(s1.arg(_T("aa")).arg(_T("bb")), _T("aabb%3"));
    EXPECT_EQ(s1.arg(_T("aa")).arg(_T("bb")).arg(_T("cc")), _T("aabbcc"));

    s1 = _T("%%1%1");
    EXPECT_EQ(s1.arg(_T("aa")).arg(_T("bb")), _T("%aa%1"));
}

TEST_F(TestString, StartWith)
{
    NString s1, s2, s3;

    s1 = _T("123");
    s2 = _T("1234");
    EXPECT_FALSE(s1.StartWith(s2));
    EXPECT_TRUE(s2.StartWith(s1));
    EXPECT_FALSE(s1.StartWith(_T("abc")));
    EXPECT_TRUE(s2.StartWith(_T("12")));

    EXPECT_FALSE(s1.StartWith(s3));
    EXPECT_FALSE(s1.StartWith(NULL));
    EXPECT_FALSE(s1.StartWith(_T("abc")));
}

TEST_F(TestString, EndWith)
{
    NString s1, s2, s3;

    s1 = _T("234");
    s2 = _T("1234");
    EXPECT_FALSE(s1.EndWith(s2));
    EXPECT_TRUE(s2.EndWith(s1));
    EXPECT_FALSE(s1.EndWith(_T("abc")));
    EXPECT_TRUE(s2.EndWith(_T("34")));
    EXPECT_TRUE(s2.EndWith(s2));
    EXPECT_TRUE(s1.EndWith(s1));

    EXPECT_FALSE(s1.EndWith(s3));
    EXPECT_FALSE(s1.EndWith(NULL));
    EXPECT_FALSE(s1.EndWith(_T("abca")));
}
