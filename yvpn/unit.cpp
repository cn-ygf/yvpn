#include "stdafx.h"
#include "unit.h"

char* UnicodeToUtf8(const wchar_t* szText, LPDWORD lpdwSize)
{
	if (!szText)
		return NULL;
	int len = WideCharToMultiByte(CP_UTF8, 0, szText, -1, NULL, 0, NULL, NULL);
	char *utf8 = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, szText, -1, utf8, len, NULL, NULL);
	*lpdwSize = len;
	return utf8;
}

wchar_t* Utf8ToUnicode(const char *szUtf8, LPDWORD lpdwSize)
{
	if (!szUtf8)
	{
		return NULL;
	}
	int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	wchar_t *unicode = new wchar_t[dwUnicodeLen + 2];
	memset(unicode, 0, dwUnicodeLen*sizeof(wchar_t)+2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, unicode, dwUnicodeLen);
	*lpdwSize = dwUnicodeLen;
	return unicode;
}


wchar_t* LocalToUnicode(UINT nCodePage, const char *szText)
{
	int nLength = MultiByteToWideChar(nCodePage, 0, szText, -1, NULL, 0);
	wchar_t *lpBuff = new wchar_t[nLength + 1];
	MultiByteToWideChar(nCodePage, 0, szText, -1, lpBuff, nLength);
	lpBuff[nLength] = 0;
	return lpBuff;
}

char* UnicodeToLocal(UINT nCodePage, const wchar_t* szText)
{
	int nLength = WideCharToMultiByte(nCodePage, 0, szText, -1, NULL, 0, NULL, NULL);
	char* pBuffer = new char[nLength + 1];
	WideCharToMultiByte(nCodePage, 0, szText, -1, pBuffer, nLength, NULL, NULL);
	pBuffer[nLength] = 0;
	return pBuffer;
}

CDuiString ToNumberChar(CDuiString lpszText)
{
	CDuiString result;
	for (int i = 0; i < lpszText.GetLength();i++)
	{
		TCHAR a = lpszText.GetAt(i);
		TCHAR b[2] = { a,0x00 };
		if (a >= 0x30 && a <= 0x39)
		{
			result.Append(b);
		}
	}
	return result;
}