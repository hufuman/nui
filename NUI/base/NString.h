#pragma once


namespace NUI
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

        public:
            NString& Assign(LPCTSTR arg, size_t length);

            NString& MakeLower();
            NString& MakeUpper();
            NString& Trim();
            NString& Replace(LPCTSTR szSrc, LPCTSTR szDes);
            NString SubString(size_t startPos);
            NString SubString(size_t startPos, size_t count);

            bool CompareNoCases(LPCTSTR arg) const;
            bool IsEmpty() const;

            size_t IndexOf(LPCTSTR arg) const;
            size_t IndexOf(LPCTSTR arg, size_t startPos) const;
            size_t LastIndexOf(LPCTSTR arg) const;
            size_t LastIndexOf(LPCTSTR arg, size_t startPos) const;

            size_t IndexOf(TCHAR arg) const;
            size_t IndexOf(TCHAR arg, size_t startPos) const;
            size_t LastIndexOf(TCHAR arg) const;
            size_t LastIndexOf(TCHAR arg, size_t startPos) const;

            LPCTSTR GetData() const;
            size_t GetLength() const;

            NString& Format(LPCTSTR szFormat, ...);

            bool Tokenize(size_t& position, LPCTSTR szSplitter, bool includeEmpty, NString& token) const;

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
            TCHAR& operator [] (size_t index);

        protected:
            NString ArgImpl(LPCTSTR str) const;

        private:
            size_t argPos_;
            tstring data_;
        };

    }
}