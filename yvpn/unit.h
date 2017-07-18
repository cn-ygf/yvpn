#pragma once

wchar_t* Utf8ToUnicode(const char *szUtf8, LPDWORD lpdwSize);
char* UnicodeToUtf8(const wchar_t* szText, LPDWORD lpdwSize);
wchar_t* LocalToUnicode(UINT nCodePage, const char *szText);
char* UnicodeToLocal(UINT nCodePage, const wchar_t* szText);
CDuiString ToNumberChar(CDuiString lpszText);