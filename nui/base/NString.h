#pragma once


namespace nui
{
    namespace Base
    {

        class NUI_CLASS NString
        {
        public:
            NString();
            NString(const NString& arg);
            NString(LPCTSTR arg);
            NString& operator = (const NString& arg);
            ~NString();

        public:
            NString& operator = (TCHAR arg);
            NString& operator = (LPCTSTR arg);

            NString operator + (TCHAR arg) const;
            NString& operator += (TCHAR arg);

            NString operator + (LPCTSTR arg) const;
            NString& operator += (LPCTSTR arg);
            bool operator == (LPCTSTR arg) const;
            bool operator != (LPCTSTR arg) const;

            NString operator + (NString arg) const;
            NString& operator += (NString arg);
            bool operator == (NString arg) const;
            bool operator != (NString arg) const;

            bool operator < (const NString& arg) const;
            bool operator < (LPCTSTR arg) const;

            operator LPCTSTR() const;

        public:
            NString& Assign(LPCTSTR arg, int length);

            NString& MakeLower();
            NString& MakeUpper();

            NString ToLower() const;
            NString ToUpper() const;

            NString& Trim();
            NString& Replace(LPCTSTR szSrc, LPCTSTR szDes);
            NString SubString(int startPos) const;
            NString SubString(int startPos, int count) const;

            NString& Resize(int size);

            bool CompareNoCases(LPCTSTR arg) const;
            bool IsEmpty() const;

            int IndexOf(LPCTSTR arg) const;
            int IndexOf(LPCTSTR arg, int startPos) const;
            int LastIndexOf(LPCTSTR arg) const;
            int LastIndexOf(LPCTSTR arg, int startPos) const;
            int IndexOfAny(LPCTSTR arg) const;
            int IndexOfAny(LPCTSTR arg, int startPos) const;

            int IndexOf(TCHAR arg) const;
            int IndexOf(TCHAR arg, int startPos) const;
            int LastIndexOf(TCHAR arg) const;
            int LastIndexOf(TCHAR arg, int startPos) const;

            LPCTSTR GetData() const;
            int GetLength() const;

            NString& Format(LPCTSTR szFormat, ...);

            bool Tokenize(int& position, LPCTSTR szSplitter, bool includeEmpty, NString& token) const;

        public:
            NString arg(LPCTSTR wstr) const;
            NString arg(short n) const;
            NString arg(unsigned short n) const;
            NString arg(int n) const;
            NString arg(unsigned int n) const;
            NString arg(long n) const;
            NString arg(unsigned long n) const;
            NString arg(LONGLONG n) const;
            NString arg(ULONGLONG n) const;
            NString arg(wchar_t n) const;
            NString arg(double d) const;
            NString arg(const NString& str) const;

        public:
            TCHAR& operator [] (int index);
            const TCHAR& operator [] (int index) const;

        protected:
            NString ArgImpl(LPCTSTR str) const;

        private:
            int argPos_;
            tstring data_;
        };

    }
}