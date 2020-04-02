#include "stdafx.h"
#include "cmpupdate.h"
#include "fver.h"
#include "winsku.h"
#include "registry.h"
#include <vector>
#include <string>

#define NN name->push_back
#define XX exe->push_back

using namespace std;

void componentUpdates(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,
					  int sp,std::vector<std::string> *notifications) {

	int status=0;

	// Hotfix path and standard arguments
	const std::string sw="start /wait ";
	const std::string p="Packages\\";
	const std::string a1=" /passive /norestart /overwriteoem /nobackup";

	// Create SKU masks
	int XP_HOME_PRO = XP_PRO|XP_PRON|XP_HOME|XP_HOMEN;
	int XPE_2009 = XPE_POSREADY2009|XPE_WES2009;
	int XP_EMBEDDED = XPE_2009;
	int XP_ALL =XP_HOME_PRO|XP_STARTER|XP_TABLET|XP_MCE2002|XP_MCE2004|XP_MCE2005|XPE_FLP|XPE_2009;

	// Identify system paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring System32 = SystemRoot + L"\\system32";
	std::wstring ehome = SystemRoot + L"\\ehome";

	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);
	std::wstring InternetExplorer = ProgramFiles + L"\\Internet Explorer";

	// Read file version information
	fver zero = fver(0,0,0,0);

	fver _D3DCompiler_43 = getFileVer(System32+L"\\D3DCompiler_43.dll",&status);
	fver _d3dcsx_43_dll  = getFileVer(System32+L"\\d3dcsx_43.dll",&status);
	fver _d3dx10_43_dll  = getFileVer(System32+L"\\d3dx10_43.dll",&status);
	fver _d3dx11_43_dll  = getFileVer(System32+L"\\d3dx11_43.dll",&status);
	fver _D3DX9_43_dll   = getFileVer(System32+L"\\D3DX9_43.dll",&status);

	fver _msctf_dll    = getFileVer(System32+L"\\msctf.dll",&status);
	fver _iexplore_exe = getFileVer(InternetExplorer+L"\\iexplore.exe",&status);
	fver _wmp_dll      = getFileVer(System32+L"\\wmp.dll",&status);

	fver _ehshell_exe  = getFileVer(ehome+L"\\ehshell.exe",&status);

	// Flag updates;
	if((sku & XP_ALL) && (  _D3DCompiler_43 <fver(9,29,952,3111)
		              ||    _d3dcsx_43_dll  <fver(9,29,952,3111)
					  ||    _d3dx10_43_dll  <fver(9,29,952,3111)
					  ||    _d3dx11_43_dll  <fver(9,29,952,3111)
					  ||    _D3DX9_43_dll   <fver(9,29,952,3111) )) {
		NN("DirectX End-User Runtimes (June 2010)");
		XX(std::string("Extras\\directx_Jun2010_redist.exe /Q /C /T:%TEMP%\\directx_Jun2010_redist\n")
			+"pushd %TEMP%\\directx_Jun2010_redist\n"
			+"start /wait DXSETUP.exe /silent\n"
			+"popd\n"
			+"rd /S /Q %TEMP%\\directx_Jun2010_redist");
	}

	if( sp>=2 && ( sku & XP_MCE2005 ) && (sku & XP_ALL) && (  _wmp_dll <fver(11,0,5721,5145) )) {
		if(_ehshell_exe >= fver(5,1,2710,2732)) {
			NN("Windows Media Player 11 (for Windows Media Center Edition 2005)");
			XX("\"Windows Media Player\\X86-en-wmp11-windowsxp-x86-enu_f6f975548c03c3761ab4ce55f80d1e17ae353428.exe\" /Q");
		} else {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Update Rollup 2 for Windows XP Media Center")
			                               +"|Edition 2005 must be installed prior to"
							               +"|installing Windows Media Player 11."
							               +"| "
							               +"|Please run 'update_windows.bat' to install"
							               +"|Update Rollup 2, then try again.");
		}
	}
	else if( sp>=2 && !( sku & XP_MCE2005 ) && (sku & XP_ALL) && (  _wmp_dll <fver(11,0,5721,5145) )) {
		NN("Windows Media Player 11");
		XX("\"Windows Media Player\\wmp11-windowsxp-x86-enu.exe\" /Q");
	}
	if( sp==2 && (sku & XP_ALL) /*&& _msctf_dll>zero*/ && _msctf_dll<fver(5,1,2600,3319)) {
		NN("Update for Windows XP (KB932823)");
		XX(p+"windowsxp-kb932823-v3-x86-enu_d0806094569c5bbce9f6e0313cd67558316d048a.exe"+a1);
	}
	if((sku & XP_ALL) && (  _iexplore_exe <fver(8,0,0,0) ) &&
			( sp==2 || (sp==3 && _msctf_dll>fver()))) {
		NN("Internet Explorer 8 for Windows XP");
		XX("\"Internet Explorer\\IE8-WindowsXP-x86-ENU.EXE\" /passive /update-no");
	}

}

void installMBSA(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku) {

	int status=0;

	// Hotfix path and standard arguments
	const std::string sw="start /wait ";
	const std::string p="Packages\\";
	const std::string a1=" /passive /norestart /overwriteoem /nobackup";
	const std::string a5=" /wuforce /quiet /norestart";

	// Create SKU masks
	int XP_HOME_PRO = XP_PRO|XP_PRON|XP_HOME|XP_HOMEN;
	int XPE_2009 = XPE_POSREADY2009|XPE_WES2009;
	int XP_EMBEDDED = XPE_2009;
	int XP_ALL =XP_HOME_PRO|XP_STARTER|XP_TABLET|XP_MCE2002|XP_MCE2004|XP_MCE2005|XPE_FLP|XPE_2009;

	// Identify system paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring System32 = SystemRoot + L"\\system32";

	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);
	std::wstring MBSA = ProgramFiles + L"\\Microsoft Baseline Security Analyzer 2";

	// Read file version information
	fver zero = fver(0,0,0,0);

	fver _msi_dll      = getFileVer(System32+L"\\msi.dll",&status);
	fver _msiexec_exe  = getFileVer(System32+L"\\msiexec.exe",&status);
	fver _msihnd_dll   = getFileVer(System32+L"\\msihnd.dll",&status);
	fver _msimsg_dll   = getFileVer(System32+L"\\msimsg.dll",&status);
	fver _msisip_dll   = getFileVer(System32+L"\\msisip.dll",&status);

	fver _cdm_dll      = getFileVer(System32+L"\\cdm.dll",&status);
	fver _wuapi_dll    = getFileVer(System32+L"\\wuapi.dll",&status);
	fver _wuauclt_exe  = getFileVer(System32+L"\\wuauclt.exe",&status);
	fver _wuaucpl_cpl  = getFileVer(System32+L"\\wuaucpl.cpl",&status);
	fver _wuaueng_dll  = getFileVer(System32+L"\\wuaueng.dll",&status);
	fver _wuauserv_dll = getFileVer(System32+L"\\wuauserv.dll",&status);
	fver _wucltui_dll  = getFileVer(System32+L"\\wucltui.dll",&status);
	fver _wups2_dll    = getFileVer(System32+L"\\wups2.dll",&status);
	fver _wups_dll     = getFileVer(System32+L"\\wups.dll",&status);
	fver _wuweb_dll    = getFileVer(System32+L"\\wuweb.dll",&status);

	fver _mbsa_exe     = getFileVer(MBSA+L"\\mbsa.exe",&status);
	fver _mbsacli_exe  = getFileVer(MBSA+L"\\mbsacli.exe",&status);
	fver _qfecheck_exe = getFileVer(System32+L"\\qfecheck.exe",&status);

	// Flag updates;
	if((sku & XP_ALL) && (  _msi_dll      <fver(3,1,4000,2435)
					  ||    _msiexec_exe  <fver(3,1,4000,1823)
					  ||    _msihnd_dll   <fver(3,1,4000,1823)
					  ||    _msimsg_dll   <fver(3,1,4000,1823)
					  ||    _msisip_dll   <fver(3,1,4000,1823) )) {
		NN("Microsoft Windows Installer 3.1");
		XX(p+"WindowsInstaller-KB893803-v2-x86.exe"+a1);
	}

	if((sku & XP_ALL) && (  _cdm_dll      <fver(7,0,6000,374)
		              ||    _wuapi_dll    <fver(7,0,6000,374)
					  ||    _wuauclt_exe  <fver(7,0,6000,374)
					  ||    _wuaucpl_cpl  <fver(7,0,6000,374)
					  ||    _wuaueng_dll  <fver(7,0,6000,374)
					  //||    _wuauserv_dll <fver(7,0,6000,374)
					  ||    _wucltui_dll  <fver(7,0,6000,374)
					  ||    _wups2_dll    <fver(7,0,6000,374)
					  ||    _wups_dll     <fver(7,0,6000,374)
					  ||    _wuweb_dll    <fver(7,0,6000,374) )) {
		NN("Windows Update Agent 3.0");
		XX(p+"WindowsUpdateAgent30-x86.exe"+a5);
	}

	if((sku & XP_ALL) && (  _mbsa_exe     <fver(2,2,2170,0)
					  ||    _mbsacli_exe  <fver(2,2,2170,0) )) {
		NN("Microsoft Baseline Security Analyzer 2.2");
		XX("msiexec /i MBSA\\MBSASetup-x86-EN.msi /passive /norestart");
	}

	if((sku & XP_ALL) && (  _qfecheck_exe <fver(5,0,2195,4605) )) {
		NN("Qfecheck.exe Update Check Utility (Q282784)");
		XX("Extras\\Q282784_WXP_SP1_x86_ENU.exe -u -n -o -z");
	}

}


int installServicePack(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
					   std::vector<std::string> *notifications) {

	int status=0;
	int r=0;

	// Hotfix path and standard arguments
	const std::string sw="start /wait ";
	const std::string p="Packages\\";
	const std::string a1=" /passive /norestart /overwriteoem /nobackup";
//	const std::string a7=" /passive /warnrestart /overwriteoem /nobackup";
	const std::string a7=" /passive /warnrestart /overwriteoem /nobackup /forceappsclose";

	// Create SKU masks
	int XP_HOME_PRO = XP_PRO|XP_PRON|XP_HOME|XP_HOMEN;
	int XPE_2009 = XPE_POSREADY2009|XPE_WES2009;
	int XP_EMBEDDED = XPE_2009;
	int XP_ALL =XP_HOME_PRO|XP_STARTER|XP_TABLET|XP_MCE2002|XP_MCE2004|XP_MCE2005|XPE_FLP|XPE_2009;

	// Identify system paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring System32 = SystemRoot + L"\\system32";

	fver _msctf_dll    = getFileVer(System32+L"\\msctf.dll",&status);
	fver _photometadatahandler_dll = getFileVer(System32+L"\\photometadatahandler.dll",&status);
	fver _windowscodecs_dll = getFileVer(System32+L"\\windowscodecs.dll",&status);
	fver _windowscodecsext_dll = getFileVer(System32+L"\\windowscodecsext.dll",&status);
	fver _wmphoto_dll  = getFileVer(System32+L"\\wmphoto.dll",&status);

	// Read file version information
	fver zero = fver(0,0,0,0);

	// Flag updates;
	if( sp==2 && (sku & XP_ALL) /*&& _msctf_dll>zero*/ && _msctf_dll<fver(5,1,2600,3319)) {
		NN("Update for Windows XP (KB932823)");
		XX(p+"windowsxp-kb932823-v3-x86-enu_d0806094569c5bbce9f6e0313cd67558316d048a.exe"+a1);
	}
	if( sp<3 && (sku & XP_ALL) && ( 
		                  ( _photometadatahandler_dll <fver(6,0,5840,16388))
					  ||  ( _windowscodecs_dll        <fver(6,0,5840,16388))
					  ||  ( _windowscodecsext_dll     <fver(6,0,5840,16388))
					  ||  ( _wmphoto_dll              <fver(6,0,5840,16388)) )) {
		if(    _photometadatahandler_dll <=fver(6,0,5840,16388)
			&& _windowscodecs_dll        <=fver(6,0,5840,16388)
			&& _windowscodecsext_dll     <=fver(6,0,5840,16388)
			&& _wmphoto_dll              <=fver(6,0,5840,16388)) {
			NN("Windows Imaging Component for Windows XP and Server 2003");
			XX(p+"wic_x86_enu.exe"+a1);
		} else {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Windows Imaging Component is missing, but")
			                               +"|cannot be installed because newer parts of"
							               +"|it are already present on the system.");
		}
	}
	if((sku & XPE_FLP) && sp<3 ) {
		r=3;
		NN("Service Pack 3 for Windows Fundamentals for Legacy PCs and Windows Embedded for Point of Service (KB958255)");
		XX("\"Service Packs\\WindowsXP-KB958255-ENU.exe\""+a7);
	}
	else if( !(sku & XPE_FLP) && sp<3 ) {
		r=3;
		NN("Windows XP Service Pack 3 (KB936929)");
		XX("\"Service Packs\\WindowsXP-KB936929-SP3-x86-ENU.exe\""+a7);
	}

	return r;

}