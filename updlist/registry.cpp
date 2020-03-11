
#include "stdafx.h"
#include "registry.h"
#include <windows.h>
#include <string>
//#include <shlwapi.h>

using namespace std;

std::wstring regQueryValue(const std::wstring& lpSubKey,const std::wstring& lpValueName,int* status) {
	
	*status=0;
	std::wstring rval = L"";

	HKEY hKey = HKEY_LOCAL_MACHINE;
	DWORD ulOptions = NULL;
	REGSAM samDesired = KEY_QUERY_VALUE; //KEY_ALL_ACCESS;
	HKEY phkResult = NULL;
	//wprintf(L"Accessing: %ls\n",lpSubKey.c_str());
	LONG lRet = RegOpenKeyExW(hKey,lpSubKey.c_str(),ulOptions,samDesired,&phkResult);

	if (lRet != ERROR_SUCCESS) {
		*status=-1;
		return rval;
	}

	LPDWORD lpReserved=NULL;
	DWORD lpType;
	WCHAR lpData[4096] = {0};
	LPDWORD lpcbData = new DWORD();
	*lpcbData = sizeof(lpData);

	lRet = RegQueryValueExW(phkResult,lpValueName.c_str(),lpReserved,&lpType,(LPBYTE)lpData,lpcbData);
	delete lpcbData;
	if (lRet != ERROR_SUCCESS) {
		*status=-2;
		return rval;
	}

	lRet = RegCloseKey(phkResult);
	if (lRet != ERROR_SUCCESS) {
		*status=-4;
		return rval;
	}

	switch(lpType) {
		case REG_SZ:
			rval=lpData;
			break;
		case REG_DWORD:
			wchar_t buffer[4096];
			swprintf_s(buffer,4096,L"%d",(int)lpData[0]);
			rval=std::wstring(buffer);
			break;
		default:
			*status=-3;
			break;
	}
	return rval;
}

DWORD regQueryDWORD(const std::wstring& lpSubKey,const std::wstring& lpValueName,int* status) {
	
	*status=0;
	DWORD rval = 0;

	HKEY hKey = HKEY_LOCAL_MACHINE;
	DWORD ulOptions = NULL;
	REGSAM samDesired = KEY_QUERY_VALUE; //KEY_ALL_ACCESS;
	HKEY phkResult = NULL;
	//wprintf(L"Accessing: %ls\n",lpSubKey.c_str());
	LONG lRet = RegOpenKeyExW(hKey,lpSubKey.c_str(),ulOptions,samDesired,&phkResult);

	if (lRet != ERROR_SUCCESS) {
		*status=-1;
		return rval;
	}

	LPDWORD lpReserved=NULL;
	DWORD lpType;
	LPDWORD lpData=(DWORD*)malloc(sizeof(DWORD));
	DWORD lpcbData = sizeof(DWORD);

	lRet = RegQueryValueExW(phkResult,lpValueName.c_str(),lpReserved,&lpType,(LPBYTE)lpData,&lpcbData);
	if (lRet != ERROR_SUCCESS) {
		*status=-2;
		free(lpData);
		return rval;
	}

	lRet = RegCloseKey(phkResult);
	if (lRet != ERROR_SUCCESS) {
		*status=-4;
		free(lpData);
		return rval;
	}

	switch(lpType) {
		case REG_DWORD:
			rval=lpData[0];
			break;
		default:
			*status=-3;
			break;
	}
	free(lpData);
	return rval;
}

bool regTestKey(const std::wstring& lpSubKey) {

	HKEY hKey = HKEY_LOCAL_MACHINE;
	DWORD ulOptions = NULL;
	REGSAM samDesired = KEY_QUERY_VALUE; //KEY_ALL_ACCESS;
	HKEY phkResult = NULL;
	//wprintf(L"Accessing: %ls\n",lpSubKey.c_str());
	LONG lRet = RegOpenKeyExW(hKey,lpSubKey.c_str(),ulOptions,samDesired,&phkResult);

	if (lRet != ERROR_SUCCESS) {
		return false;
	}

	lRet = RegCloseKey(phkResult);
	if (lRet != ERROR_SUCCESS) {
		return false;
	}
	return true;
}
