#pragma once

#define GetH(c) ((c) >> 16)
#define GetS(c) (((c) >> 8) & 0xff)
#define GetL(c)	((c) & 0xff)

class CSSE
{
public:
	CSSE();
	typedef BOOL(*pfnDoXor)(DWORD dwKey, char * pBuff, int nLen);
	typedef void(*pfnDoAnd)(DWORD dwKey, char * pBuff, int nLen);
	typedef void(*pfnMemSetDWord)(LPVOID pBuff, DWORD dwVal, int nSize);
	typedef void (*pfnMemCopy)(PVOID pDest, const PVOID pSrc, int nSize);
	typedef void(*pfnDoGray)(LPVOID pBuff, int nSize);
	typedef void(*pfnOpenAlpha)(LPVOID pBuff, int nSize);
	typedef void(*pfnSetColorKey)(LPVOID pBuff, int nSize, DWORD dwColor);
	typedef void(*pfnDoOr)(DWORD dwKey, LPVOID pBuff, int nLen);
	typedef void(*pfnSetHSL)(LPVOID pBuff, int nSize, int h, int s, int l);

	static BOOL IsSupportSSE();
	static DWORD RGBToHSL(DWORD dwColor);
	static DWORD HSLToRGB(DWORD dwHsl);

	static void MirrorX(LPVOID pBuff, int nWidth, int nHeight);
	static void MirrorY(LPVOID pBuff, int nWidth, int nHeight);
	static void AdjustAlpha(DWORD &dwColor);
	static void Stretch(LPVOID pBuff, int nWidth, int nHeight, LPVOID pDest, int cx, int cy);

	static pfnDoXor DoXor;
    static pfnDoAnd DoAnd;
    static pfnMemSetDWord MemSetDWord;
	static pfnMemCopy MemCopy;
    static pfnDoGray DoGray;
	static pfnOpenAlpha OpenAlpha;
	static pfnSetColorKey SetColorKey;
	static pfnDoOr DoOr;
	static pfnSetHSL SetHSL;

private:
	static void MemCopySSE(PVOID pDest, const PVOID pSrc, int nSize);
	static void MemCopyNormal(PVOID pDest, const PVOID pSrc, int nSize);
	static void SetColorKeySSE(LPVOID pBuff, int nSize, DWORD dwColor);
	static void SetColorKeyNormal(LPVOID pBuff, int nSize, DWORD dwColor);
	static BOOL DoXorNormal(DWORD dwKey, char * pBuff, int nLen);
	static BOOL DoXorSSE(DWORD dwKey, char * pBuff, int nLen);
	static void DoOrNormal(DWORD dwKey, LPVOID pBuff, int nLen);
	static void DoOrSSE(DWORD dwKey, LPVOID pBuff, int nLen);
	static void DoAndNormal(DWORD dwKey, char * pBuff, int nLen);
    static void DoAndSSE(DWORD dwKey, char * pBuff, int nLen);
    static void MemSetDWordNormal(LPVOID pBuff, DWORD dwVal, int nSize);
    static void MemSetDWordSSE(LPVOID pBuff, DWORD dwVal, int nSize);
	static void DoGrayNormal(LPVOID pBuff, int nSize);
	static void DoGraySSE(LPVOID pBuff, int nSize);
	static void OpenAlphaNormal(LPVOID pBuff, int nSize);
	static void OpenAlphaSSE(LPVOID pBuff, int nSize);
	static void SetHSLNormal(LPVOID pBuff, int nSize, int h, int s, int l);
	static void SetHSLSSE(LPVOID pBuff, int nSize, int h, int s, int l);
	static int m_nSupportSSE;
};
