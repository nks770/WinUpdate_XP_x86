#include "stdafx.h"
//#include <stdio.h>
#include <string>
#include <shlwapi.h> // Linker requires shlwapi.lib
#include <windows.h> // Linker requires version.lib

#include "registry.h"
#include "winsku.h"
#include "fver.h"

using namespace std;

/*
std::string getSKUStr(winsku sku) {
	char buffer[256];
	getSKUName(sku,buffer,sizeof(buffer));
	return std::string(buffer);
}

bool getSKUName(winsku sku,char* buffer,int bufsize) {
	bool r=true;
	switch(sku) {
		case XP_PRO: strncpy_s(buffer,bufsize,"Windows XP Professional",bufsize); break;
		case XP_PRON: strncpy_s(buffer,bufsize,"Windows XP Professional N",bufsize); break;
		case XP_HOME: strncpy_s(buffer,bufsize,"Windows XP Home Edition",bufsize); break;
		case XP_HOMEN: strncpy_s(buffer,bufsize,"Windows XP Home Edition N",bufsize); break;
		case XP_STARTER: strncpy_s(buffer,bufsize,"Windows XP Starter Edition",bufsize); break;
		case XP_TABLET: strncpy_s(buffer,bufsize,"Windows XP Tablet PC Edition",bufsize); break;
		case XP_MCE2002: strncpy_s(buffer,bufsize,"Windows XP Media Center Edition 2002",bufsize); break;
		case XP_MCE2004: strncpy_s(buffer,bufsize,"Windows XP Media Center Edition 2004",bufsize); break;
		case XP_MCE2005: strncpy_s(buffer,bufsize,"Windows XP Media Center Edition 2005",bufsize); break;
		case XPE_FLP: strncpy_s(buffer,bufsize,"Windows Fundamentals for Legacy PCs",bufsize); break;
		case XPE_POSREADY2009: strncpy_s(buffer,bufsize,"Windows POSReady 2009",bufsize); break;
		case XPE_WES2009: strncpy_s(buffer,bufsize,"Windows Embedded Standard 2009",bufsize); break;
		default: strncpy_s(buffer,bufsize,"Windows",bufsize); r=false; break;
	}
	buffer[bufsize-1]=0;
	return r;
}
*/

std::string getSKUName(winsku sku) {
	switch(sku) {
		case XP_PRO: return "Windows XP Professional"; break;
		case XP_PRON: return "Windows XP Professional N"; break;
		case XP_HOME: return "Windows XP Home Editon"; break;
		case XP_HOMEN: return "Windows XP Home Edition N"; break;
		case XP_STARTER: return "Windows XP Starter Edition"; break;
		case XP_TABLET: return "Windows XP Tablet PC Edition"; break;
		case XP_MCE2002: return "Windows XP Media Center Edition 2002"; break;
		case XP_MCE2004: return "Windows XP Media Center Edition 2004"; break;
		case XP_MCE2005: return "Windows XP Media Center Edition 2005"; break;
		case XPE_FLP: return "Windows Fundamentals for Legacy PCs"; break;
		case XPE_POSREADY2009: return "Windows Embedded POSReady 2009"; break;
		case XPE_WES2009: return "Windows Embedded Standard 2009"; break;
		default: return "Windows"; break;
	}
}

std::string getSPName(winsp sp) {
	switch(sp) {
		case SP0: return "RTM"; break;
		case SP1: return "SP1"; break;
		case SP2: return "SP2"; break;
		case SP3: return "SP3"; break;
		default: return "Unknown"; break;
	}
}


winsku getSKU() {

	int status=0;

	int CannotFindSystemRoot   =0;
	int CannotFindProgramFiles =0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);

	// Use GetSystemMetrics() API function.
	if( GetSystemMetrics(SM_STARTER) ) { return XP_STARTER; }
	if( GetSystemMetrics(SM_TABLETPC) ) { return XP_TABLET; }

	// Check for Media Center Edition
	std::wstring ehshell_path=SystemRoot+L"\\eHome\\ehshell.exe";
	bool ehshellExists=(!CannotFindSystemRoot && PathFileExistsW(ehshell_path.c_str())!=0);
	fver ehshell_ver=getFileVer(ehshell_path.c_str(),&status);
	if( ehshellExists && !status && GetSystemMetrics(SM_MEDIACENTER) ) {
		if     (ehshell_ver>=fver(5,1,2700,2180)) { return XP_MCE2005; }
		else if(ehshell_ver>=fver(5,1,2600,1217)) { return XP_MCE2004; }
		else if(ehshell_ver>=fver(5,1,2600,1106)) { return XP_MCE2002; }
		else   { return UNKNOWN_OS; }
	}

	// Check for Windows Embedded Versions
	std::wstring Fundamentals      =regQueryValue(L"SYSTEM\\WPA\\Fundamentals",L"Installed",&status);
	std::wstring POSReady          =regQueryValue(L"SYSTEM\\WPA\\POSReady",L"Installed",&status);
	std::wstring FeaturePackVersion=regQueryValue(L"SYSTEM\\CurrentControlSet\\Control\\WindowsEmbedded\\ProductVersion",L"FeaturePackVersion",&status);

	if(Fundamentals==L"1")                                    { return XPE_FLP;          }
	if(POSReady==L"1")                                        { return XPE_POSREADY2009; }
	if(FeaturePackVersion==L"Windows Embedded Standard 2009") { return XPE_WES2009;      }

	// Check contents of prodspec.ini (to differentiate Pro vs Home)
	std::wstring ProdSpec = SystemRoot + L"\\system32\\prodspec.ini";
	bool ProdSpecExists=(!CannotFindSystemRoot && PathFileExistsW(ProdSpec.c_str())!=0);
	FILE* prodspec_ini = NULL;
	char buffer[256];
	if (ProdSpecExists && !_wfopen_s(&prodspec_ini,ProdSpec.c_str(),L"r")) {
		while(!feof(prodspec_ini)) {
			fgets(buffer,sizeof(buffer),prodspec_ini);
			if(strncmp(buffer,"Product=",8)==0) {
				break;
			}
		}
	fclose(prodspec_ini);
	}
	
	// Check for mplayer2.exe (to differentiate the "N" editions)
	std::wstring mplayer2_path = ProgramFiles + L"\\Windows Media Player\\mplayer2.exe";
	bool mplayer2=(!CannotFindProgramFiles && PathFileExistsW(mplayer2_path.c_str())!=0);

	if(strncmp(buffer+19,"Professional",12)==0) {
		if(mplayer2) { return XP_PRO; }
		else { return XP_PRON; }
	}
	else if(strncmp(buffer+19,"Home Edition",12)==0) {
		if(mplayer2) { return XP_HOME; }
		else { return XP_HOMEN; }
	}

	return UNKNOWN_OS;
}

winsp getSP() {
	int status=0;

	std::wstring CSDVersion=regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"CSDVersion",&status);

	if(status==-2)                                    { return SP0; }
	else if(!status && CSDVersion==L"Service Pack 1") { return SP1; }
	else if(!status && CSDVersion==L"Service Pack 2") { return SP2; }
	else if(!status && CSDVersion==L"Service Pack 3") { return SP3; }
	else { return SP_UNKNOWN; }
}

int sp_to_int(winsp sp) {
	switch(sp) {
		case SP3: return 3;
			break;
		case SP2: return 2;
			break;
		case SP1: return 1;
			break;
		case SP0: return 0;
			break;
		default: return -1;
	}
}