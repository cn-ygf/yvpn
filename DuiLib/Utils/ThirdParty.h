#ifndef __THIRD_PARTY_H__
#define __THIRD_PARTY_H__

#pragma once

namespace DuiLib
{
	namespace ThirdParty
	{
		BYTE* ParseImage(BYTE const *buffer, int len, OUT int &width, OUT int &height);
		void  FreeImage(void *pImage);

		// ���flagsΪ2����ʾ���ļ����أ�ָ��bufferΪ�ļ���������Ҫָ��len
		// ���flagsΪ3����ʾ���ڴ���أ�ָ��bufferΪ�ڴ��,lenΪbuffer����
		HANDLE OpenZip(void *buffer, unsigned int len, DWORD flags);
		void CloseZip(HANDLE hz);

		// strFilePath:���Ƥ���ļ��е��ļ�·��
		BYTE* LoadFromFile(LPCTSTR strFilePath, OUT DWORD &dwFileSize);

		// strFilePath:���Ƥ���ļ��е��ļ�·��
		BYTE* LoadFromZip(LPCTSTR strFilePath, OUT DWORD &dwFileSize);

		// strFilePath:�ļ�����·��
		BYTE* LoadFromAbsoluteFile(LPCTSTR strFilePath, OUT DWORD &dwFileSize);

		// strResName:��Դ���֣�resType:��Դ����
		BYTE* LoadFromResource(LPCTSTR strResName, LPCTSTR resType, OUT DWORD &dwFileSize);
	}
}// namespace DuiLib

#endif // __THIRD_PARTY_H__