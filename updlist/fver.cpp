
#include "stdafx.h"
#include <string>
#include <windows.h> // Linker requires version.lib

#include "fver.h"

using namespace std;

fver::fver() {
	verData[0]=0;
	verData[1]=0;
	verData[2]=0;
	verData[3]=0;
}

fver::fver(unsigned int major, unsigned int minor, unsigned int build, unsigned int revision) {
	verData[0]=major;
	verData[1]=minor;
	verData[2]=build;
	verData[3]=revision;
}

//fver::fver(char* verstring) {
//	sscanf_s(verstring,"%d.%d.%d.%d",verData,verData+1,verData+2,verData+3);
//}

fver::fver(const wchar_t* verstring) {
	int s=swscanf_s(verstring,L"%d.%d.%d.%d",verData,verData+1,verData+2,verData+3);
	switch(s) {
		case 1: verData[1]=0;
		case 2: verData[2]=0;
		case 3: verData[3]=0;
		case 4: break;
		default:
			verData[0]=0; verData[1]=0; verData[2]=0; verData[3]=0;
		break;
	}
}


bool fver::operator<(const fver& b) const {
	if (verData[0]<b.verData[0]) { return true; }
	else if (verData[0]>b.verData[0]) { return false; }
	else {
		if (verData[1]<b.verData[1]) { return true; }
		else if (verData[1]>b.verData[1]) { return false; }
		else {
			if (verData[2]<b.verData[2]) { return true; }
			else if (verData[2]>b.verData[2]) { return false; }
			else {
				if (verData[3]<b.verData[3]) { return true; }
				else { return false; }
			}
		}
	}
}
bool fver::operator>(const fver& b) const  { return   b<*this ; }
bool fver::operator<=(const fver& b) const { return !(b<*this); }
bool fver::operator>=(const fver& b) const { return !(*this<b); }

bool fver::operator==(const fver& b) const {
	return verData[0]==b.verData[0] &&
		   verData[1]==b.verData[1] &&
		   verData[2]==b.verData[2] &&
		   verData[3]==b.verData[3];
}
bool fver::operator!=(const fver& b) const { return !(*this==b); }

fver::~fver() {
	// Nothing to explicitly put in the destructor yet.
}

std::string fver::format() const {
	char buffer[64];
	if(verData[0]==0 && verData[1]==0 && verData[2]==0 && verData[3]==0) { buffer[0]='0'; buffer[1]=0; }
	else { sprintf_s(buffer,"%d.%d.%d.%d",verData[0],verData[1],verData[2],verData[3]); }
	return std::string(buffer);
}

std::string fver::format2() const {
	char buffer[64];
	sprintf_s(buffer,"%d.%d",verData[0],verData[1]);
	return std::string(buffer);
}

int fver::major() const {
	return verData[0];
}

fver getFileVer(const std::wstring& lptstrFilename,int* status) {

	*status=0;
	fver v;

	//std::wstring lptstrFilename = utf8_decode(path);
	DWORD lpdwHandle=NULL;
	DWORD verSize = GetFileVersionInfoSizeW(lptstrFilename.c_str(),&lpdwHandle);
		if(verSize==NULL) {*status=-1; return v;}

	LPSTR verData = new char[verSize];
	int lRet=GetFileVersionInfoW(lptstrFilename.c_str(),lpdwHandle,verSize,verData);
		if(lRet==0) { *status=-2; delete[] verData; return v; }
	
	LPWSTR lpSubBlock=L"\\";
	LPBYTE lpBuffer=NULL;
	UINT uLen=0;
	lRet=VerQueryValueW(verData,lpSubBlock,(VOID FAR* FAR*)&lpBuffer,&uLen);
		if (uLen==0) { *status=-3; delete[] verData; return v; }

	VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
	
	v=fver((verInfo->dwFileVersionMS >> 16) & 0xffff,
		   (verInfo->dwFileVersionMS >> 0 ) & 0xffff,
		   (verInfo->dwFileVersionLS >> 16) & 0xffff,
		   (verInfo->dwFileVersionLS >> 0 ) & 0xffff);
	
	delete[] verData;
	return v;
}

__int64 getFileSize(/*const wchar_t* name*/ std::wstring name) {
    HANDLE hFile = CreateFile(name.c_str(), GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile==INVALID_HANDLE_VALUE)
        return -1; // error condition, could call GetLastError to find out more

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        CloseHandle(hFile);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(hFile);
    return size.QuadPart;
}

bool fileExists(std::wstring name) {
    HANDLE hFile = CreateFile(name.c_str(), GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
        return false; // error condition, could call GetLastError to find out more
	}
    CloseHandle(hFile);
    return true;
}