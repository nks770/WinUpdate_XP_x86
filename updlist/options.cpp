#include "stdafx.h"
#include "options.h"
#include "winsku.h"
#include "fver.h"
#include "registry.h"
#include "nfxdetect.h"
#include <vector>
#include <string>

using namespace std;

void detectOptions(bool* components) {

	int status=0;

	// Index the array
	bool* rdp60   =components+0; *rdp60=false;
	bool* rdp61   =components+1; *rdp61=false;
	bool* rdp70   =components+2; *rdp70=false;
	bool* wmp6cdcs=components+3; *wmp6cdcs=false;
	bool* imapi2  =components+4; *imapi2=false;
	bool* smartcard=components+5; *smartcard=false;
	bool* wsearch4=components+6; *wsearch4=false;
	bool* jview   =components+7; *jview=false;
	bool* pshell1 =components+8; *pshell1=false;
	bool* pshell2 =components+9; *pshell2=false;
	bool* winrms  =components+10; *winrms=false;
	bool* msi45   =components+11; *msi45=false;
	bool *msxml4  =components+12; *msxml4=false;
	bool *msxml6  =components+13; *msxml6=false;
	bool *bitlocker=components+14; *bitlocker=false;
	bool *silverlight=components+15; *silverlight=false;
	bool* wga     =components+16; *wga=false;
	bool* xpeos   =components+17; *xpeos=false;
	bool* rktools =components+18; *rktools=false;
	bool* msjvm   =components+19; *msjvm=false;

	// Identify system paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring System32 = SystemRoot + L"\\system32";
	std::wstring drivers = System32+L"\\drivers";
	std::wstring pshelldir = System32+L"\\WindowsPowerShell\\v1.0";

	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);
	std::wstring jnl = ProgramFiles+L"\\Windows Journal Viewer";
	std::wstring MicrosoftSilverlight = ProgramFiles + L"\\Microsoft Silverlight";
	std::wstring rktooldir = ProgramFiles + L"\\Windows Resource Kits\\Tools";

	// Get file versions
	fver _mstsc_exe    = getFileVer(System32+L"\\mstsc.exe",&status);

	fver _msaud32_acm  = getFileVer(System32+L"\\msaud32.acm",&status);
	fver _mp4sdmod_dll = getFileVer(System32+L"\\MP4SDMOD.DLL",&status);
	fver _wmadmod_dll  = getFileVer(System32+L"\\WMADMOD.DLL",&status);
	fver _wmsdmod_dll  = getFileVer(System32+L"\\WMSDMOD.DLL",&status);
	fver _wmv8dmod_dll = getFileVer(System32+L"\\WMV8DMOD.DLL",&status);
	fver _wmv9vcm_dll  = getFileVer(System32+L"\\WMV9VCM.dll",&status);
	fver _wmvdmod_dll  = getFileVer(System32+L"\\WMVDMOD.DLL",&status);
	fver _mp4sds32_ax  = getFileVer(System32+L"\\mp4sds32.AX",&status);
	fver _mpg4ds32_ax  = getFileVer(System32+L"\\MPG4DS32.AX",&status);
	fver _msadds32_ax  = getFileVer(System32+L"\\msadds32.ax",&status);
	fver _msscds32_ax  = getFileVer(System32+L"\\MSSCDS32.AX",&status);
	fver _wmavds32_ax  = getFileVer(System32+L"\\WMAVDS32.AX",&status);
	fver _wmv8ds32_ax  = getFileVer(System32+L"\\WMV8DS32.AX",&status);
	fver _wmvds32_ax   = getFileVer(System32+L"\\WMVDS32.AX",&status);

	fver _cdrom_sys    = getFileVer(drivers+L"\\cdrom.sys",&status);
	fver _imapi2_dll   = getFileVer(System32+L"\\imapi2.dll",&status);
	fver _imapi2fs_dll = getFileVer(System32+L"\\imapi2fs.dll",&status);

	fver _WdfCoInstaller01007_dll = getFileVer(System32+L"\\WdfCoInstaller01007.dll",&status);
	fver _winusbcoinstaller_dll = getFileVer(System32+L"\\winusbcoinstaller.dll",&status);
	fver _wudfupdate_01007_dll  = getFileVer(System32+L"\\wudfupdate_01007.dll",&status);
	fver _wudfusbcciddriver_dll = getFileVer(System32+L"\\drivers\\UMDF\\wudfusbcciddriver.dll",&status);

	fver _searchfilterhost_exe   = getFileVer(System32+L"\\searchfilterhost.exe",&status);
	fver _searchindexer_exe      = getFileVer(System32+L"\\searchindexer.exe",&status);
	fver _searchprotocolhost_exe = getFileVer(System32+L"\\searchprotocolhost.exe",&status);

	fver _jntview_exe            = getFileVer(jnl+L"\\jntview.exe",&status);

	fver _powershell_exe         = getFileVer(pshelldir+L"\\powershell.exe",&status);
	fver _pwrshmsg_dll           = getFileVer(pshelldir+L"\\pwrshmsg.dll",&status);
	fver _pwrshsip_dll           = getFileVer(pshelldir+L"\\pwrshsip.dll",&status);
	fver _winrs_exe              = getFileVer(System32+L"\\winrs.exe",&status);

	fver _msdrm_dll    = getFileVer(System32+L"\\msdrm.dll",&status);
	fver _RmActivate_exe = getFileVer(System32+L"\\RmActivate.exe",&status);
	fver _RmActivate_isv_exe = getFileVer(System32+L"\\RmActivate_isv.exe",&status);
	fver _RmActivate_ssp_exe = getFileVer(System32+L"\\RmActivate_ssp.exe",&status);
	fver _RmActivate_ssp_isv_exe = getFileVer(System32+L"\\RmActivate_ssp_isv.exe",&status);
	fver _SecProc_dll  = getFileVer(System32+L"\\SecProc.dll",&status);
	fver _SecProc_isv_dll = getFileVer(System32+L"\\SecProc_isv.dll",&status);
	fver _SecProc_ssp_dll = getFileVer(System32+L"\\SecProc_ssp.dll",&status);
	fver _SecProc_ssp_isv_dll = getFileVer(System32+L"\\SecProc_ssp_isv.dll",&status);

	fver _LegitCheckControl_dll = getFileVer(System32+L"\\LegitCheckControl.dll",&status);
	fver _WgaLogon_dll          = getFileVer(System32+L"\\WgaLogon.dll",&status);
	fver _WgaTray_exe           = getFileVer(System32+L"\\WgaTray.exe",&status);
	fver _xp_eos_exe   = getFileVer(System32+L"\\xp_eos.exe",&status);

	fver _msiexec_exe  = getFileVer(System32+L"\\msiexec.exe",&status);

	fver _msxml4_dll   = getFileVer(System32+L"\\msxml4.dll",&status);
	fver _msxml4r_dll  = getFileVer(System32+L"\\msxml4r.dll",&status);
	fver _msxml6_dll   = getFileVer(System32+L"\\msxml6.dll",&status);
	fver _msxml6r_dll  = getFileVer(System32+L"\\msxml6r.dll",&status);

	fver _BitLockerToGo_exe = getFileVer(SystemRoot+L"\\BitLockerDiscoveryVolumeContents\\BitLockerToGo.exe",&status);

	fver _sllauncher_exe = getFileVer(MicrosoftSilverlight+L"\\sllauncher.exe",&status);

	fver _robocopy_exe   = getFileVer(rktooldir+L"\\robocopy.exe",&status);
	fver _sleep_exe      = getFileVer(rktooldir+L"\\sleep.exe",&status);
	fver _compress_exe   = getFileVer(rktooldir+L"\\compress.exe",&status);
	fver _dvdburn_exe    = getFileVer(rktooldir+L"\\dvdburn.exe",&status);
	fver _cdburn_exe     = getFileVer(rktooldir+L"\\cdburn.exe",&status);

	fver _jview_exe    = getFileVer(System32+L"\\jview.exe",&status);
	fver _msjava_dll   = getFileVer(System32+L"\\msjava.dll",&status);

	if(_mstsc_exe>=fver(6,0,6000,16386) && _mstsc_exe<fver(6,0,6001,0)) {
		*rdp60=true;
	}
	else if(_mstsc_exe>=fver(6,0,6001,18000) && _mstsc_exe<fver(6,0,6002,0)) {
		*rdp61=true;
	}
	else if(_mstsc_exe>=fver(6,1,7600,16385)) {
		*rdp70=true;
	}
	if(    (_msaud32_acm  >=fver(7,0,0,1954))
		//&& (_mp4sdmod_dll >=fver(8,0,0,344))
		&& (_wmadmod_dll  >=fver(7,1,0,3055))
		&& (_wmsdmod_dll  >=fver(7,1,0,3055))
		//&& (_wmv8dmod_dll >=fver(8,0,0,371))
		&& (_wmv9vcm_dll  >=fver(9,0,1,369))
		&& (_wmvdmod_dll  >=fver(7,1,0,3055))
		&& (_mp4sds32_ax  >=fver(8,0,0,344))
		&& (_mpg4ds32_ax  >=fver(7,1,0,3055))
		&& (_msadds32_ax  >=fver(7,0,0,1954))
		&& (_msscds32_ax  >=fver(7,1,0,3055))
		&& (_wmavds32_ax  >=fver(9,0,0,3224))
		&& (_wmv8ds32_ax  >=fver(8,0,0,371))
		&& (_wmvds32_ax   >=fver(7,1,0,3055)) ) {
		*wmp6cdcs=true;
	}
	if(    (_cdrom_sys    >fver())
		&& (_imapi2_dll   >fver())
		&& (_imapi2fs_dll >fver()) ) {
		*imapi2=true;
	}
	if(    (_WdfCoInstaller01007_dll>fver())
		&& (_winusbcoinstaller_dll  >fver())
		&& (_wudfupdate_01007_dll   >fver())
		&& (_wudfusbcciddriver_dll  >fver()) ) {
		*smartcard=true;
	}
	if(    (_searchfilterhost_exe   >=fver(7,0,6001,16503))
		&& (_searchindexer_exe      >=fver(7,0,6001,16503))
		&& (_searchprotocolhost_exe >=fver(7,0,6001,16503))
		&& regQueryValue(L"SOFTWARE\\Microsoft\\Windows Search",L"CurrentVersion",&status)==L"04.00.6001.503" ) {
		*wsearch4=true;
	}
	if(  _jntview_exe  >fver() ) {
		*jview=true;
	}
	if(    ( _powershell_exe == fver(6,0,5430,0))
		&& ( _pwrshmsg_dll   == fver(6,0,5430,0))
		&& ( _pwrshsip_dll   == fver(6,0,5430,0)) ) {
		*pshell1=true;
	}
	if(    ( _powershell_exe >=fver(6,0,6002,18111))
		&& ( _winrs_exe      >=fver(6,0,6002,18111)) ) {
		*pshell2=true;
	}
	if(    (_msdrm_dll              >=fver(5,2,3790,433))
		&& (_RmActivate_exe         >=fver(6,0,6406,0))
		&& (_RmActivate_isv_exe     >=fver(6,0,6406,0))
		&& (_RmActivate_ssp_exe     >=fver(6,0,6406,0))
		&& (_RmActivate_ssp_isv_exe >=fver(6,0,6406,0))
		&& (_SecProc_dll            >=fver(6,0,6406,0))
		&& (_SecProc_isv_dll        >=fver(6,0,6406,0))
		&& (_SecProc_ssp_dll        >=fver(6,0,6406,0))
		&& (_SecProc_ssp_isv_dll    >=fver(6,0,6406,0)) ) {
		*winrms=true;
	}
	if( _msiexec_exe >= fver(4,5,0,0)) {
		*msi45=true;
	}
	if(    ( _msxml4_dll   >fver())
		|| ( _msxml4r_dll  >fver()) ) {
		*msxml4=true;
	}
	if(    ( _msxml6_dll   >fver())
		|| ( _msxml6r_dll  >fver()) ) {
		*msxml6=true;
	}
	if( _BitLockerToGo_exe > fver()) {
		*bitlocker=true;
	}
	if( _sllauncher_exe >= fver(5,1,0,0)) {
		*silverlight=true;
	}
	if(    (_LegitCheckControl_dll >fver())
		&& (_WgaLogon_dll          >fver())
		&& (_WgaTray_exe           >fver()) ) {
		*wga=true;
	}
	if( _xp_eos_exe>=fver(5,1,2600,6526)) {
		*xpeos=true;
	}
	if(    (_robocopy_exe >= fver(5,1,1,1010))
		&& (_sleep_exe    >= fver(5,2,3790,0))
		&& (_compress_exe >= fver(5,2,3790,0))
		&& (_dvdburn_exe  >= fver(5,2,3790,0))
		&& (_cdburn_exe   >= fver(5,2,3790,0)) ) {
		*rktools=true;
	}
	if( (_jview_exe > fver() || _msjava_dll > fver()) ) {
		*msjvm=true;
	}

/*
	Test Case: <Windows Embedded Standard 2009>
msaud32.acm	0		7.0.0.1954		*
mp4sdmod.dll	11.0.5721.5145	11.0.5721.5145
wmadmod.dll	11.0.5721.5145	11.0.5721.5145
wmsdmod.dll	11.0.5721.5145	11.0.5721.5145
wmv8dmod.dll	0		0
wmv9vcm.dll	0		9.0.1.369		*
wmvdmod.dll	11.0.5721.5145	11.0.5721.5145
mp4sds32.ax	0		8.0.0.344		*
mpg4ds32.ax	8.0.0.4487	8.0.0.4487
msadds32.ax	8.0.0.4487	8.0.0.4487
msscds32.ax	8.0.0.4487	8.0.0.4487
wmavds32.ax	0		9.0.0.3224		*
wmv8ds32.ax	8.0.0.4000	8.0.0.4000
wmvds32.ax	8.0.0.4487	8.0.0.4487

	Test Case: <Windows Embedded POSReady 2009>
msaud32.acm	0					7.0.0.1954	*
mp4sdmod.dll	0				0		
wmadmod.dll	11.0.5721.5145		11.0.5721.5145	
wmsdmod.dll	11.0.5721.5145		11.0.5721.5145	
wmv8dmod.dll	0				0		
wmv9vcm.dll		0				9.0.1.369	*
wmvdmod.dll		11.0.5721.5145	11.0.5721.5145	
mp4sds32.ax		0				8.0.0.344	*
mpg4ds32.ax		0				7.1.0.3055	*
msadds32.ax		8.0.0.4487		8.0.0.4487	
msscds32.ax		8.0.0.4487		8.0.0.4487	
wmavds32.ax		0				9.0.0.3224	*
wmv8ds32.ax		8.0.0.4000		8.0.0.4000
wmvds32.ax		8.0.0.4487		8.0.0.4487
*/
/*
	printf("wmp6cdcs DEBUG:\n");
	printf("%s\t%s\n","msaud32.acm",_msaud32_acm.format().c_str());
	printf("%s\t%s\n","mp4sdmod.dll",_mp4sdmod_dll.format().c_str());
	printf("%s\t%s\n","wmadmod.dll",_wmadmod_dll.format().c_str());
	printf("%s\t%s\n","wmsdmod.dll",_wmsdmod_dll.format().c_str());
	printf("%s\t%s\n","wmv8dmod.dll",_wmv8dmod_dll.format().c_str());
	printf("%s\t%s\n","wmv9vcm.dll",_wmv9vcm_dll.format().c_str());
	printf("%s\t%s\n","wmvdmod.dll",_wmvdmod_dll.format().c_str());
	printf("%s\t%s\n","mp4sds32.ax",_mp4sds32_ax.format().c_str());
	printf("%s\t%s\n","mpg4ds32.ax",_mpg4ds32_ax.format().c_str());
	printf("%s\t%s\n","msadds32.ax",_msadds32_ax.format().c_str());
	printf("%s\t%s\n","msscds32.ax",_msscds32_ax.format().c_str());
	printf("%s\t%s\n","wmavds32.ax",_wmavds32_ax.format().c_str());
	printf("%s\t%s\n","wmv8ds32.ax",_wmv8ds32_ax.format().c_str());
	printf("%s\t%s\n","wmvds32.ax",_wmvds32_ax.format().c_str());
*/
}

void argumentOptions(int argc, _TCHAR* argv[], bool* installed, bool* components, const int ncomp, bool disable_install,
					 winsku sku, int sp, std::vector<std::string> *notifications) {

	// Index the array
	bool disable_all=false;
	bool enable_all=false;
	bool* rdp60   =components+0; *rdp60=true; bool* rdp60_installed = installed+0;
	bool* rdp61   =components+1; *rdp61=true; bool* rdp61_installed = installed+1;
	bool* rdp70   =components+2; *rdp70=true; bool* rdp70_installed = installed+2;
	bool* wmp6cdcs=components+3; *wmp6cdcs=true;
	bool* imapi2  =components+4; *imapi2=true;
	bool* smartcard=components+5; *smartcard=true;
	bool* wsearch4=components+6; *wsearch4=false;
	bool* jview   =components+7; *jview=true;
	bool* pshell1 =components+8; *pshell1=true;
	bool* pshell2 =components+9; *pshell2=true;
	bool* winrms  =components+10; *winrms=true;
	bool* msi45   =components+11; *msi45=true;
	bool* msxml4  =components+12; *msxml4=true;
	bool* msxml6  =components+13; *msxml6=true;
	bool* bitlocker=components+14; *bitlocker=true;
	bool* silverlight=components+15; *silverlight=true;
	bool* wga     =components+16; *wga=true;
	bool* xpeos   =components+17; *xpeos=true;
	bool* rktools =components+18; *rktools=true;
	bool* msjvm   =components+19; *msjvm=true;

	const int minimum_sp[COMPONENT_COUNT] = { 2,2,3,0,2,2,2,0,2,3,0,2,0,0,2,2,0,3,0,0 };

	// Detect .NET Framework parameters
	int nfxServicePack[NFX_VERSION_COUNT];
	nfxDetect(nfxServicePack);
	int* nfx20=nfxServicePack+3;

	// Windows Installer version
	int status;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring System32 = SystemRoot + L"\\system32";
	fver _msiexec_exe  = getFileVer(System32+L"\\msiexec.exe",&status);

	// Parse arguments
	int i=0;
	for(i=0; i<argc; i++) {
		if(!wcscmp(argv[i],L"--disable-all")) { disable_all=true; }
		if(!wcscmp(argv[i],L"--enable-all")) { enable_all=true; }
		if(!wcscmp(argv[i],L"--enable-rdp60")) { *rdp60=true; *rdp61=false; *rdp70=false; }
		if(!wcscmp(argv[i],L"--disable-rdp60")) { *rdp60=false; }
		if(!wcscmp(argv[i],L"--enable-rdp61")) { *rdp61=true; *rdp60=false; *rdp70=false; }
		if(!wcscmp(argv[i],L"--disable-rdp61")) { *rdp61=false; }
		if(!wcscmp(argv[i],L"--enable-rdp70")) { *rdp70=true; *rdp60=false; *rdp61=false; }
		if(!wcscmp(argv[i],L"--disable-rdp70")) { *rdp70=false; }
		if(!wcscmp(argv[i],L"--enable-wmp6cdcs")) { *wmp6cdcs=true; }
		if(!wcscmp(argv[i],L"--disable-wmp6cdcs")) { *wmp6cdcs=false; }
		if(!wcscmp(argv[i],L"--enable-imapi2")) { *imapi2=true; }
		if(!wcscmp(argv[i],L"--disable-imapi2")) { *imapi2=false; }
		if(!wcscmp(argv[i],L"--enable-smartcard")) { *smartcard=true; }
		if(!wcscmp(argv[i],L"--disable-smartcard")) { *smartcard=false; }
		if(!wcscmp(argv[i],L"--enable-wsearch4")) { *wsearch4=true; }
		if(!wcscmp(argv[i],L"--disable-wsearch4")) { *wsearch4=false; }
		if(!wcscmp(argv[i],L"--enable-jview")) { *jview=true; }
		if(!wcscmp(argv[i],L"--disable-jview")) { *jview=false; }
		if(!wcscmp(argv[i],L"--enable-pshell")) { *pshell1=true; *pshell2=true; }
		if(!wcscmp(argv[i],L"--disable-pshell")) { *pshell1=false; *pshell2=false; }
		if(!wcscmp(argv[i],L"--enable-winrms")) { *winrms=true; }
		if(!wcscmp(argv[i],L"--disable-winrms")) { *winrms=false; }
		if(!wcscmp(argv[i],L"--enable-msxml4")) { *msxml4=true; }
		if(!wcscmp(argv[i],L"--disable-msxml4")) { *msxml4=false; }
		if(!wcscmp(argv[i],L"--enable-msxml6")) { *msxml6=true; }
		if(!wcscmp(argv[i],L"--disable-msxml6")) { *msxml6=false; }
		if(!wcscmp(argv[i],L"--enable-bitlocker")) { *bitlocker=true; }
		if(!wcscmp(argv[i],L"--disable-bitlocker")) { *bitlocker=false; }
		if(!wcscmp(argv[i],L"--enable-silverlight")) { *silverlight=true; }
		if(!wcscmp(argv[i],L"--disable-silverlight")) { *silverlight=false; }
		if(!wcscmp(argv[i],L"--enable-wga")) { *wga=true; }
		if(!wcscmp(argv[i],L"--disable-wga")) { *wga=false; }
		if(!wcscmp(argv[i],L"--enable-xpeos")) { *xpeos=true; }
		if(!wcscmp(argv[i],L"--disable-xpeos")) { *xpeos=false; }
		if(!wcscmp(argv[i],L"--enable-rktools")) { *rktools=true; }
		if(!wcscmp(argv[i],L"--disable-rktools")) { *rktools=false; }
		if(!wcscmp(argv[i],L"--enable-msjvm")) { *msjvm=true; }
		if(!wcscmp(argv[i],L"--disable-msjvm")) { *msjvm=false; }
	}

	// Miscellaneous restrictions
	if(sp>2) { *rdp60=false; }
	if(sku & XPE_WES2009) { *wga=false; }
	if(sku & (XPE_WES2009|XPE_POSREADY2009)) { *xpeos=false; }
	if(sku & XP_TABLET) { *jview=false; }

	// Enable all components
	if(enable_all) {
		for(i=0; i<ncomp; i++) {
			components[i]=true;
		}
	}

	if((*pshell1 || *pshell2) && sp<2 && !disable_install && !disable_all) {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Cannot install Windows PowerShell because it")
			                   +"|requires at least Windows XP SP2.");
	}

	// Disable components that don't meet minimum SP requirement
	for(i=0; i<ncomp; i++) {
		if(sp<minimum_sp[i]) {components[i]=false;}
	}

	// Disable all components
	if(disable_all || disable_install) {
		for(i=0; i<ncomp; i++) {
			components[i]=false;
		}
	}

	// Disable PowerShell if .NET Framework 2.0 is not present
	if((*pshell1 || *pshell2) && *nfx20<0 && !disable_install) {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Cannot install Windows PowerShell because it")
			                   +"|requires Microsoft .NET Framework 2.0, which"
							   +"|is not installed.");
		*pshell1=false; *pshell2=false;
	}
	
	// Disable MSXML6 if Windows Installer 3.1 is not present
	if(*msxml6 && _msiexec_exe<fver(3,1,4000,1823)) {
		//                        ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("MSXML6 requires that you first install")
			                   +"|Windows Installer 3.1 and reboot."
							   +"| |Run 'update_windows.bat' again after your"
							   +"|system restarts.");
		*msxml6=false;
	}

	// Disable components that are already installed
	for(i=0; i<ncomp; i++) {
		if(installed[i]) { components[i]=false; }
	}

	// Miscellaneous Rules
	if(     *rdp70 || *rdp70_installed ) { *rdp61=false; *rdp60=false; }
	else if(*rdp61 || *rdp61_installed ) { *rdp60=false; }
	if( *pshell2 ) { *pshell1=false; }
}

void displayOptions(bool* installed, bool* install, bool batchmode, const int ncomp, const int sp) {
	
	// Component names
	std::vector<std::string> component;
	component.push_back("Remote Desktop Connection 6.0");
	component.push_back("Remote Desktop Connection 6.1");
	component.push_back("Remote Desktop Connection 7.0");
	component.push_back("Codec Package for Windows Media Player 6.4");
	component.push_back("Image Mastering API v2.0 (IMAPIv2.0)");
	component.push_back("ICCD Smart Card Driver");
	component.push_back("Windows Search 4.0");
	component.push_back("Windows Journal Viewer 1.5");
	component.push_back("Windows PowerShell(TM) 1.0");
	component.push_back("Windows PowerShell 2.0 and WinRM 2.0");
	component.push_back("Windows Rights Management Client with SP2");
	component.push_back("Windows Installer 4.5");
	component.push_back("Microsoft XML Core Services 4.0");
	component.push_back("Microsoft XML Core Services 6.0");
	component.push_back("BitLocker To Go Reader");
	component.push_back("Microsoft Silverlight 5.1");
	component.push_back("Windows Genuine Advantage (WGA) Notification");
	component.push_back("Windows XP End of Support Notification");
	component.push_back("Windows Server 2003 Resource Kit Tools");
	component.push_back("Microsoft Java Virtual Machine (MSJVM)");

	// Arrays to decide which options get shown for each SP level
	const int show_rtm[COMPONENT_COUNT] = { 0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,1 };
	const int show_sp1[COMPONENT_COUNT] = { 0,0,0,1,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,1 };
	const int show_sp2[COMPONENT_COUNT] = { 1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1 };
	const int show_sp3[COMPONENT_COUNT] = { 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	int show[COMPONENT_COUNT];
	for(int i=0; i<COMPONENT_COUNT; i++) {
		switch(sp){
			case 0: show[i]=show_rtm[i]; break;
			case 1: show[i]=show_sp1[i]; break;
			case 2: show[i]=show_sp2[i]; break;
			case 3: show[i]=show_sp3[i]; break;
			default: show[i]=0; break;
		}
	}

	printf("%sOptional Components:\n",batchmode?"echo ":"");
	for(int i=0; i<ncomp; i++) {
		if(show[i]) {
			if(batchmode) {
				printf("echo [%s] %s\n",installed[i]?"X":(install[i]?"^>":" "),component[i].c_str());
			} else {
			printf("[%s] %s\n",installed[i]?"X":(install[i]?">":" "),component[i].c_str());
			}
		}
	}
	printf("%s\n",batchmode?"echo.":"");
}

void systemComponentVersions(bool batchmode) {

	int status=0;

	// Identify system paths
	int CannotFindSystemRoot=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring System32 = SystemRoot + L"\\system32";

	std::wstring iepath=regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE",L"",&status);

	// Get file versions
	fver _wmp_dll      = getFileVer(System32+L"\\wmp.dll",&status);
	fver _wmpcore_dll  = getFileVer(System32+L"\\wmpcore.dll",&status);
	fver _msdxm_ocx    = getFileVer(System32+L"\\msdxm.ocx",&status);
	fver _shdocvw_dll  = getFileVer(System32+L"\\shdocvw.dll",&status);
	fver _iexplore_exe = getFileVer(iepath,&status);

	// Default to get version of wmplayer, but fall back to mplayer2 if needed.
	fver wmp = _wmp_dll;
	if(wmp==fver()) { wmp=_wmpcore_dll; }
	if(wmp==fver()) { wmp=_msdxm_ocx; }

	fver ie = _iexplore_exe;
	if(ie>fver() && ie<fver(7,0,0,0)) {
		ie = _shdocvw_dll;
	}

	if(wmp.major() >= 8) {
		printf("%s     Windows Media Player %d (%s)\n",batchmode?"echo ":"",wmp.major(),wmp.format().c_str());
	} else if(wmp.major() >= 6) {
		printf("%s     Windows Media Player %s (%s)\n",batchmode?"echo ":"",wmp.format2().c_str(),wmp.format().c_str());
	} else {
		printf("%s     Windows Media Player is not present.\n",batchmode?"echo ":"");
	}
	if(ie>=fver(6,0,2800,1106) && ie<fver(7,0,0,0)){
		printf("%s     Internet Explorer 6 SP1 (%s)\n",batchmode?"echo ":"",ie.format().c_str());
	} else {
		printf("%s     Internet Explorer %d (%s)\n",batchmode?"echo ":"",ie.major(),ie.format().c_str());
	}
	printf("%s\n",batchmode?"echo.":"");
}