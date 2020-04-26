
#ifndef REGISTRY_H
#define REGISTRY_H

#include "stdafx.h"
#include <windows.h>
#include <string>

std::wstring regQueryValue(const std::wstring& lpSubKey,const std::wstring& lpValueName,int* status);
LPBYTE regQueryBinaryData(const std::wstring& lpSubKey,const std::wstring& lpValueName,int* status);
DWORD regQueryDWORD(const std::wstring& lpSubKey,const std::wstring& lpValueName,int* status);
bool regTestKey(const std::wstring& lpSubKey);

#endif