#include "stdafx.h"
#include "winupdate.h"
#include "nfxdetect.h"
#include "fver.h"
#include "winsku.h"
#include "registry.h"
#include "certificates.h"
#include "kb980195.h"
#include "kb2900986.h"
#include "kb914798.h"
#include "vcredist.h"
#include "md5.h"
#include <vector>
#include <string>

#define NN name->push_back
#define XX exe->push_back

using namespace std;

// NOTES
//
// /q[n|b|r|f] Sets user interface level
//    n - No UI
//    b - Basic UI
//    r - Reduced UI
//    f - Full UI
// /quiet   Same as /q
// /passive Same as /qb
//
// /q[:u | :a] 	/q - Specifies quiet mode, or suppresses prompts.
// /q:u - Specifies user-quiet mode, which presents some dialog boxes to the user.
// /q:a - Specifies administrator-quiet mode, which does not present any dialog boxes to the user.
//
// /r:n Never restarts the computer after installation.
// /r:i Prompts the user to restart the computer if a restart is required, except when used with /q:a.
// /r:a Always restarts the computer after installation.
// /r:s Restarts the computer after installation without prompting the user.

void windowsUpdates(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
					bool* options,std::vector<std::string> *notifications) {

	int status=0;

	// Index the array
	bool* rdp60   =options+0;
	bool* rdp61   =options+1;
	bool* rdp70   =options+2;
	bool* wmp6cdcs=options+3;
	bool* imapi2  =options+4;
	bool* smartcard=options+5;
	bool* wsearch4=options+6;
	bool* jview   =options+7;
	bool* pshell1 =options+8;
	bool* pshell2 =options+9;
	bool* winrms  =options+10;
	bool* msi45   =options+11;
	bool* msxml4  =options+12;
	bool* msxml6  =options+13;
	bool* bitlocker=options+14;
	bool* silverlight=options+15;
	bool* wga = options+16;
	bool* xpeos = options+17;
	bool* rktools = options+18;
	bool* msjvm = options+19;

	// Detect .NET Framework parameters
	int nfxServicePack[NFX_VERSION_COUNT];
	nfxDetect(nfxServicePack);
	int* nfx11=nfxServicePack+2;

	// Hotfix path and standard arguments
	const std::string sw="start /wait ";
	const std::string p="Packages\\";
	const std::string rtm=p+"RTM\\";
	const std::string p1=p+"SP1\\";
	const std::string p2=p+"SP2\\";
	const std::string p3=p+"SP3\\";
	const std::string vcredist=p+"vcredist\\";
	const std::string mstsc=p+"mstsc\\";
	const std::string np="NetFx\\";
	const std::string a1=" /passive /norestart /overwriteoem /nobackup";
	const std::string a2=" /Q";
	const std::string a2l=" /q";
	const std::string a3=" /passive /norestart";
	const std::string a4=" /u /z";
	const std::string a5=" /wuforce /quiet /norestart";
	const std::string a6=" /passive /norestart /o /n";
	const std::string a7=" -u -n -o -z";
	const std::string a8=" /Q:U /R:N";

	// Create SKU masks
	int XP_HOME_PRO = XP_PRO|XP_PRON|XP_HOME|XP_HOMEN;
	int XP_MCE = XP_MCE2002|XP_MCE2004|XP_MCE2005;
	int XP_OTHER_EDITIONS = XP_STARTER|XP_TABLET|XP_MCE;
	int XP_CLIENT = XP_HOME_PRO|XP_OTHER_EDITIONS;
	int XP_PLUS_FLP = XP_CLIENT|XPE_FLP;
	int XPE_2009 = XPE_POSREADY2009|XPE_WES2009;
	int XP_EMBEDDED = XPE_2009;
	int XP_EMBEDDED_PLUS_FLP = XP_EMBEDDED|XPE_FLP;
	int XP_ALL =XP_CLIENT|XP_EMBEDDED|XPE_FLP;

	// Identify system paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";
	std::wstring dllcache = System32 + L"\\dllcache";
	std::wstring msagent = SystemRoot + L"\\msagent";
	std::wstring ehome = SystemRoot + L"\\ehome";
	std::wstring CreateDisc = ehome + L"\\CreateDisc";
	std::wstring ehomeFilters = CreateDisc + L"\\Filters";
	std::wstring Sfxplugins = CreateDisc + L"\\Sfxplugins";
	std::wstring ehHelp = ehome + L"\\ehHelp";
	std::wstring NetworkDiagnostic = SystemRoot + L"\\Network Diagnostic";
	std::wstring usmt = System32 + L"\\usmt";
	std::wstring Flash = System32 + L"\\Macromed\\Flash";
	std::wstring wbem = System32 + L"\\wbem";
	std::wstring Drivers = System32 + L"\\Drivers";
	std::wstring DriverCache = SystemRoot + L"\\Driver Cache\\i386";
	std::wstring inetsrv = System32 + L"\\inetsrv";
	std::wstring Restore = System32 + L"\\Restore";

	std::wstring helpctr_binaries = SystemRoot + L"\\pchealth\\helpctr\\binaries";
	std::wstring UploadLB = SystemRoot + L"\\pchealth\\UploadLB\\Binaries";

	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);
	std::wstring InternetExplorer = ProgramFiles + L"\\Internet Explorer";
	std::wstring Accessories = ProgramFiles + L"\\Windows NT\\Accessories";
	std::wstring OutlookExpress = ProgramFiles + L"\\Outlook Express";
	std::wstring Messenger = ProgramFiles + L"\\Messenger";
	std::wstring MovieMaker = ProgramFiles + L"\\Movie Maker";
	std::wstring MicrosoftSilverlight = ProgramFiles + L"\\Microsoft Silverlight";
	std::wstring NetMeeting = ProgramFiles + L"\\NetMeeting";
	std::wstring CommonFilesSystem = ProgramFiles + L"\\Common Files\\System";
	std::wstring msadc = CommonFilesSystem + L"\\msadc";
	std::wstring ado = CommonFilesSystem + L"\\ado";
	std::wstring OleDB = CommonFilesSystem + L"\\Ole DB";
	std::wstring TextConv = ProgramFiles + L"\\Common Files\\Microsoft Shared\\TextConv";
	std::wstring Triedit = ProgramFiles + L"\\Common Files\\Microsoft Shared\\Triedit";
	std::wstring VGX = ProgramFiles + L"\\Common Files\\Microsoft Shared\\VGX";
	std::wstring WDS = ProgramFiles + L"\\Windows Desktop Search";
	std::wstring WDS_mui = WDS + L"\\en-US";
	std::wstring WindowsJournalViewer = ProgramFiles+L"\\Windows Journal Viewer";
	std::wstring WindowsJournal = ProgramFiles+L"\\Windows Journal";
	std::wstring WindowsMediaPlayer = ProgramFiles+L"\\Windows Media Player";
	std::wstring Ink = ProgramFiles+L"\\Common Files\\Microsoft Shared\\Ink";
	std::wstring WMV9_VCM = ProgramFiles+L"\\WMV9_VCM";

	std::wstring wxp_x86_0409_v1 = System32+L"\\PreInstall\\WinSE\\wxp_x86_0409_v1";

	bool _secdrv_sys_exist   = fileExists(Drivers+L"\\secdrv.sys");
	bool _undo_guimode_txt   = fileExists(System32+L"\\undo_guimode.txt");
	bool kb891122 = false;
	bool kb900325 = false;
	bool kb913800 = false;
	bool wmv9vcm_install = false;
	bool wm9codecs_v1_install = false;
	bool wm9codecs_v2_install = false;

	// File MD5 hashes
	MD5 md5;
	char* md5_ptr = md5.digestFileW((System32+L"\\shgina.dll").c_str(),false);
	char _shgina_dll_md5[33];
	strncpy_s(_shgina_dll_md5,33,md5_ptr,33);

	md5.digestFileW((inetsrv+L"\\httpodbc.dll").c_str(),false);
	char _httpodbc_dll_md5[33];
	strncpy_s(_httpodbc_dll_md5,33,md5_ptr,33);
	
	md5.digestFileW((inetsrv+L"\\ssinc.dll").c_str(),false);
	char _ssinc_dll_md5[33];
	strncpy_s(_ssinc_dll_md5,33,md5_ptr,33);

	md5.digestFileW((WMV9_VCM+L"\\Film-320x240-vbr.wv9").c_str(),false);
	char _film_320x240_vbr_wv9_md5[33];
	strncpy_s(_film_320x240_vbr_wv9_md5,33,md5_ptr,33);

//	printf("%s\n",_shgina_dll_md5);
//	printf("%s\n",_httpodbc_dll_md5);
//	printf("%s\n",_ssinc_dll_md5);
	
	// Read file version information
	fver zero = fver(0,0,0,0);

	fver _acgenral_dll = getFileVer(SystemRoot+L"\\AppPatch\\acgenral.dll",&status);
	fver _aclayers_dll = getFileVer(SystemRoot+L"\\AppPatch\\aclayers.dll",&status);
	fver _acspecfc_dll = getFileVer(SystemRoot+L"\\AppPatch\\acspecfc.dll",&status);
	fver _acxtrnal_dll = getFileVer(SystemRoot+L"\\AppPatch\\acxtrnal.dll",&status);
	fver _explorer_exe = getFileVer(SystemRoot+L"\\explorer.exe",&status);
	fver _hh_exe       = getFileVer(SystemRoot+L"\\hh.exe",&status);
	fver _kb913800_exe = getFileVer(SystemRoot+L"\\kb913800.exe",&status);
	fver _srchui_dll   = getFileVer(SystemRoot+L"\\srchasst\\srchui.dll",&status);

	fver _6to4svc_dll  = getFileVer(System32+L"\\6to4svc.dll",&status);
	fver _aaclient_dll = getFileVer(System32+L"\\aaclient.dll",&status);
	fver _aaclient_dll_mui = getFileVer(System32+L"\\en-US\\aaclient.dll.mui",&status);
	fver _accwiz_exe   = getFileVer(System32+L"\\accwiz.exe",&status);
	fver _advapi32_dll = getFileVer(System32+L"\\advapi32.dll",&status);
	fver _advpack_dll  = getFileVer(System32+L"\\advpack.dll",&status);
	fver _appwiz_cpl   = getFileVer(System32+L"\\appwiz.cpl",&status);
	fver _asycfilt_dll = getFileVer(System32+L"\\asycfilt.dll",&status);
	fver _atl_dll      = getFileVer(System32+L"\\atl.dll",&status);
	fver _atmfd_dll    = getFileVer(System32+L"\\atmfd.dll",&status);
	fver _authz_dll    = getFileVer(System32+L"\\authz.dll",&status);
	fver _avifil32_dll = getFileVer(System32+L"\\avifil32.dll",&status);
	fver _axaltocm_dll = getFileVer(System32+L"\\axaltocm.dll",&status);
	fver _basecsp_dll  = getFileVer(System32+L"\\basecsp.dll",&status);
	fver _basesrv_dll  = getFileVer(System32+L"\\basesrv.dll",&status);
	fver _bcsprsrc_dll = getFileVer(System32+L"\\bcsprsrc.dll",&status);
	fver _bitsprx2_dll = getFileVer(System32+L"\\bitsprx2.dll",&status);
	fver _bitsprx3_dll = getFileVer(System32+L"\\bitsprx3.dll",&status);
	fver _blackbox_dll = getFileVer(System32+L"\\blackbox.dll",&status);
	fver _browser_dll  = getFileVer(System32+L"\\browser.dll",&status);
	fver _browseui_dll = getFileVer(System32+L"\\browseui.dll",&status);
	fver _cabview_dll  = getFileVer(System32+L"\\cabview.dll",&status);
	fver _catsrv_dll   = getFileVer(System32+L"\\catsrv.dll",&status);
	fver _catsrvut_dll = getFileVer(System32+L"\\catsrvut.dll",&status);
	fver _cdfview_dll  = getFileVer(System32+L"\\cdfview.dll",&status);
	fver _cdosys_dll   = getFileVer(System32+L"\\cdosys.dll",&status);
	fver _cewmdm_dll   = getFileVer(System32+L"\\cewmdm.dll",&status);
	fver _ciodm_dll    = getFileVer(System32+L"\\ciodm.dll",&status);
	fver _clbcatex_dll = getFileVer(System32+L"\\clbcatex.dll",&status);
	fver _clbcatq_dll  = getFileVer(System32+L"\\clbcatq.dll",&status);
	fver _clspack_exe  = getFileVer(System32+L"\\clspack.exe",&status);
	fver _colbact_dll  = getFileVer(System32+L"\\colbact.dll",&status);
	fver _comctl32_dll = getFileVer(System32+L"\\comctl32.dll",&status);
	fver _comrepl_dll  = getFileVer(System32+L"\\comrepl.dll",&status);
	fver _comrepl_exe  = getFileVer(System32+L"\\Com\\comrepl.exe",&status);
	fver _comsvcs_dll  = getFileVer(System32+L"\\comsvcs.dll",&status);
	fver _comuid_dll   = getFileVer(System32+L"\\comuid.dll",&status);
	fver _corpol_dll   = getFileVer(System32+L"\\corpol.dll",&status);
	fver _credssp_dll  = getFileVer(System32+L"\\credssp.dll",&status);
	fver _crypt32_dll  = getFileVer(System32+L"\\crypt32.dll",&status);
	fver _cryptdlg_dll = getFileVer(System32+L"\\cryptdlg.dll",&status);
	fver _cryptsvc_dll = getFileVer(System32+L"\\cryptsvc.dll",&status);
	fver _cryptui_dll  = getFileVer(System32+L"\\cryptui.dll",&status);
	fver _cscdll_dll   = getFileVer(System32+L"\\cscdll.dll",&status);
	fver _cscript_exe  = getFileVer(System32+L"\\cscript.exe",&status);
	fver _csrsrv_dll   = getFileVer(System32+L"\\csrsrv.dll",&status);
	fver _danim_dll    = getFileVer(System32+L"\\danim.dll",&status);
	fver _dao360_dll   = getFileVer(System32+L"\\dao360.dll",&status);
	fver _dbnetlib_dll = getFileVer(System32+L"\\dbnetlib.dll",&status);
	fver _devenum_dll  = getFileVer(System32+L"\\devenum.dll",&status);
	fver _dhcpcsvc_dll = getFileVer(System32+L"\\dhcpcsvc.dll",&status);
	fver _dnsapi_dll   = getFileVer(System32+L"\\dnsapi.dll",&status);
	fver _dnsrslvr_dll = getFileVer(System32+L"\\dnsrslvr.dll",&status);
	fver _dpcdll_dll   = getFileVer(System32+L"\\dpcdll.dll",&status);
	fver _dplayx_dll   = getFileVer(System32+L"\\dplayx.dll",&status);
	fver _dpnet_dll    = getFileVer(System32+L"\\dpnet.dll",&status);
	fver _dpwsockx_dll = getFileVer(System32+L"\\dpwsockx.dll",&status);
	fver _drmclien_dll = getFileVer(System32+L"\\drmclien.dll",&status);
	fver _drmstor_dll  = getFileVer(System32+L"\\drmstor.dll",&status);
	fver _drmupgds_exe = getFileVer(System32+L"\\drmupgds.exe",&status);
	fver _drmv2clt_dll = getFileVer(System32+L"\\drmv2clt.dll",&status);
	fver _dssenh_dll   = getFileVer(System32+L"\\dssenh.dll",&status);
	fver _dwwin_exe    = getFileVer(System32+L"\\dwwin.exe",&status);
	fver _dxmasf_dll   = getFileVer(System32+L"\\dxmasf.dll",&status);
	fver _dxmrtp_dll   = getFileVer(System32+L"\\dxmrtp.dll",&status);
	fver _dxtmsft_dll  = getFileVer(System32+L"\\dxtmsft.dll",&status);
	fver _dxtrans_dll  = getFileVer(System32+L"\\dxtrans.dll",&status);
	fver _encdec_dll   = getFileVer(System32+L"\\encdec.dll",&status);
	fver _es_dll       = getFileVer(System32+L"\\es.dll",&status);
	fver _esent_dll    = getFileVer(System32+L"\\esent.dll",&status);
	fver _expsrv_dll   = getFileVer(System32+L"\\expsrv.dll",&status);
	fver _extmgr_dll   = getFileVer(System32+L"\\extmgr.dll",&status);
	fver _faultrep_dll = getFileVer(System32+L"\\faultrep.dll",&status);
	fver _fldrclnr_dll = getFileVer(System32+L"\\fldrclnr.dll",&status);
	fver _fltlib_dll   = getFileVer(System32+L"\\fltlib.dll",&status);
	fver _fltMc_exe    = getFileVer(System32+L"\\fltMc.exe",&status);
	fver _fmifs_dll    = getFileVer(System32+L"\\fmifs.dll",&status);
	fver _fontsub_dll  = getFileVer(System32+L"\\fontsub.dll",&status);
	fver _format_com   = getFileVer(System32+L"\\format.com",&status);
	fver _fxsclnt_exe  = getFileVer(System32+L"\\fxsclnt.exe",&status);
	fver _fxscover_exe = getFileVer(System32+L"\\fxscover.exe",&status);
	fver _gdi32_dll    = getFileVer(System32+L"\\gdi32.dll",&status);
	fver _gpkcsp_dll   = getFileVer(System32+L"\\gpkcsp.dll",&status);
	fver _gpprefcl_dll = getFileVer(System32+L"\\gpprefcl.dll",&status);
	fver _gptext_dll   = getFileVer(System32+L"\\gptext.dll",&status);
	fver _grpconv_exe  = getFileVer(System32+L"\\grpconv.exe",&status);
	fver _h323_tsp     = getFileVer(System32+L"\\h323.tsp",&status);
	fver _h323msp_dll  = getFileVer(System32+L"\\h323msp.dll",&status);
	fver _hccoin_dll   = getFileVer(System32+L"\\hccoin.dll",&status);
	fver _hhctrl_ocx   = getFileVer(System32+L"\\hhctrl.ocx",&status);
	fver _hhsetup_dll  = getFileVer(System32+L"\\hhsetup.dll",&status);
	fver _hlink_dll    = getFileVer(System32+L"\\hlink.dll",&status);
	fver _html_iec     = getFileVer(System32+L"\\html.iec",&status);
	fver _httpapi_dll  = getFileVer(System32+L"\\httpapi.dll",&status);
	fver _hypertrm_dll = getFileVer(System32+L"\\hypertrm.dll",&status);
	fver _iacenc_dll   = getFileVer(System32+L"\\iacenc.dll",&status);
	fver _icardie_dll  = getFileVer(System32+L"\\icardie.dll",&status);
	fver _iccvid_dll   = getFileVer(System32+L"\\iccvid.dll",&status);
	fver _icm32_dll    = getFileVer(System32+L"\\icm32.dll",&status);
	fver _ie4uinit_exe = getFileVer(System32+L"\\ie4uinit.exe",&status);
	fver _ieakeng_dll  = getFileVer(System32+L"\\ieakeng.dll",&status);
	fver _ieaksie_dll  = getFileVer(System32+L"\\ieaksie.dll",&status);
	fver _ieakui_dll   = getFileVer(System32+L"\\ieakui.dll",&status);
	fver _ieapfltr_dat = getFileVer(System32+L"\\ieapfltr.dat",&status);
	fver _ieapfltr_dll = getFileVer(System32+L"\\ieapfltr.dll",&status);
	fver _iedkcs32_dll = getFileVer(System32+L"\\iedkcs32.dll",&status);
	fver _ieencode_dll = getFileVer(System32+L"\\ieencode.dll",&status);
	fver _ieframe_dll  = getFileVer(System32+L"\\ieframe.dll",&status);
	fver _ieframe_dll_mui = getFileVer(System32+L"\\ieframe.dll.mui",&status);
	fver _iepeers_dll  = getFileVer(System32+L"\\iepeers.dll",&status);
	fver _iernonce_dll = getFileVer(System32+L"\\iernonce.dll",&status);
	fver _iertutil_dll = getFileVer(System32+L"\\iertutil.dll",&status);
	fver _ieudinit_exe = getFileVer(System32+L"\\ieudinit.exe",&status);
	fver _ifsutil_dll  = getFileVer(System32+L"\\ifsutil.dll",&status);
	fver _ifxcardm_dll = getFileVer(System32+L"\\ifxcardm.dll",&status);
	fver _imagehlp_dll = getFileVer(System32+L"\\imagehlp.dll",&status);
	fver _imapi_exe    = getFileVer(System32+L"\\imapi.exe",&status);
	fver _imapi2_dll   = getFileVer(System32+L"\\imapi2.dll",&status);
	fver _imapi2fs_dll = getFileVer(System32+L"\\imapi2fs.dll",&status);
	fver _imekr61_ime  = getFileVer(System32+L"\\imekr61.ime",&status);
	fver _inetcomm_dll = getFileVer(System32+L"\\inetcomm.dll",&status);
	fver _inetcpl_cpl  = getFileVer(System32+L"\\inetcpl.cpl",&status);
	fver _inetmib1_dll = getFileVer(System32+L"\\inetmib1.dll",&status);
	fver _inetpp_dll   = getFileVer(System32+L"\\inetpp.dll",&status);
	fver _inetres_dll  = getFileVer(System32+L"\\inetres.dll",&status);
	fver _inseng_dll   = getFileVer(System32+L"\\inseng.dll",&status);
	fver _iphlpapi_dll = getFileVer(System32+L"\\iphlpapi.dll",&status);
	fver _ipnathlp_dll = getFileVer(System32+L"\\ipnathlp.dll",&status);
	fver _iprtrmgr_dll = getFileVer(System32+L"\\iprtrmgr.dll",&status);
	fver _ipsecsnp_dll = getFileVer(System32+L"\\ipsecsnp.dll",&status);
	fver _ipsecsvc_dll = getFileVer(System32+L"\\ipsecsvc.dll",&status);
	fver _ipsmsnap_dll = getFileVer(System32+L"\\ipsmsnap.dll",&status);
	fver _ipv6_exe     = getFileVer(System32+L"\\ipv6.exe",&status);
	fver _ipv6mon_dll  = getFileVer(System32+L"\\ipv6mon.dll",&status);
	fver _isign32_dll  = getFileVer(System32+L"\\isign32.dll",&status);
	fver _itircl_dll   = getFileVer(System32+L"\\itircl.dll",&status);
	fver _itss_dll     = getFileVer(System32+L"\\itss.dll",&status);
	fver _iyuv_32_dll  = getFileVer(System32+L"\\iyuv_32.dll",&status);
	fver _javacypt_dll = getFileVer(System32+L"\\javacypt.dll",&status);
	fver _javaee_dll   = getFileVer(System32+L"\\javaee.dll",&status);
	fver _javaprxy_dll = getFileVer(System32+L"\\javaprxy.dll",&status);
	fver _javart_dll   = getFileVer(System32+L"\\javart.dll",&status);
	fver _jdbgmgr_exe  = getFileVer(System32+L"\\jdbgmgr.exe",&status);
	fver _jgdw400_dll  = getFileVer(System32+L"\\jgdw400.dll",&status);
	fver _jgpl400_dll  = getFileVer(System32+L"\\jgpl400.dll",&status);
	fver _jit_dll      = getFileVer(System32+L"\\jit.dll",&status);
	fver _jscript_dll  = getFileVer(System32+L"\\jscript.dll",&status);
	fver _jsproxy_dll  = getFileVer(System32+L"\\jsproxy.dll",&status);
	fver _jview_exe    = getFileVer(System32+L"\\jview.exe",&status);
	fver _kerberos_dll = getFileVer(System32+L"\\kerberos.dll",&status);
	fver _kernel32_dll = getFileVer(System32+L"\\kernel32.dll",&status);
	fver _krnl386_exe  = getFileVer(System32+L"\\krnl386.exe",&status);
	fver _l3codeca_acm = getFileVer(System32+L"\\l3codeca.acm",&status);
	fver _l3codecx_ax  = getFileVer(System32+L"\\l3codecx.ax",&status);
	fver _LAPRXY_dll   = getFileVer(System32+L"\\LAPRXY.dll",&status);
	fver _licmgr10_dll = getFileVer(System32+L"\\licmgr10.dll",&status);
	fver _linkinfo_dll = getFileVer(System32+L"\\linkinfo.dll",&status);
	fver _localspl_dll = getFileVer(System32+L"\\localspl.dll",&status);
	fver _locator_exe  = getFileVer(System32+L"\\locator.exe",&status);
	fver _logagent_exe = getFileVer(System32+L"\\logagent.exe",&status);
	fver _lsasrv_dll   = getFileVer(System32+L"\\lsasrv.dll",&status);
	fver _magnify_exe  = getFileVer(System32+L"\\magnify.exe",&status);
	fver _mciseq_dll   = getFileVer(System32+L"\\mciseq.dll",&status);
	fver _mf3216_dll   = getFileVer(System32+L"\\mf3216.dll",&status);
	fver _mfc40_dll    = getFileVer(System32+L"\\mfc40.dll",&status);
	fver _mfc40u_dll   = getFileVer(System32+L"\\mfc40u.dll",&status);
	fver _mfc42_dll    = getFileVer(System32+L"\\mfc42.dll",&status);
	fver _mfc42u_dll   = getFileVer(System32+L"\\mfc42u.dll",&status);
	fver _mfplat_dll   = getFileVer(System32+L"\\mfplat.dll",&status);
	fver _MP43DMOD_dll = getFileVer(System32+L"\\MP43DMOD.dll",&status);
	fver _mp4sdecd_dll = getFileVer(System32+L"\\mp4sdecd.dll",&status);
	fver _mp4sdmod_dll = getFileVer(System32+L"\\mp4sdmod.dll",&status);
	fver _mp4sds32_ax  = getFileVer(System32+L"\\mp4sds32.ax",&status);
	fver _mpeg2data_ax = getFileVer(System32+L"\\mpeg2data.ax",&status);
	fver _mpg2splt_ax  = getFileVer(System32+L"\\mpg2splt.ax",&status);
	fver _MPG4DMOD_dll = getFileVer(System32+L"\\MPG4DMOD.dll",&status);
	fver _mpg4ds32_ax  = getFileVer(System32+L"\\mpg4ds32.ax",&status);
	fver _mqqm_dll     = getFileVer(System32+L"\\mqqm.dll",&status);
	fver _mqad_dll     = getFileVer(System32+L"\\mqad.dll",&status);
	fver _mqbkup_exe   = getFileVer(System32+L"\\mqbkup.exe",&status);
	fver _mqdscli_dll  = getFileVer(System32+L"\\mqdscli.dll",&status);
	fver _mqise_dll    = getFileVer(System32+L"\\mqise.dll",&status);
	fver _mqoa_dll     = getFileVer(System32+L"\\mqoa.dll",&status);
	fver _mqrt_dll     = getFileVer(System32+L"\\mqrt.dll",&status);
	fver _mqrtdep_dll  = getFileVer(System32+L"\\mqrtdep.dll",&status);
	fver _mqsec_dll    = getFileVer(System32+L"\\mqsec.dll",&status);
	fver _mqsnap_dll   = getFileVer(System32+L"\\mqsnap.dll",&status);
	fver _mqsvc_exe    = getFileVer(System32+L"\\mqsvc.exe",&status);
	fver _mqtgsvc_exe  = getFileVer(System32+L"\\mqtgsvc.exe",&status);
	fver _mqtrig_dll   = getFileVer(System32+L"\\mqtrig.dll",&status);
	fver _mqupgrd_dll  = getFileVer(System32+L"\\mqupgrd.dll",&status);
	fver _mqutil_dll   = getFileVer(System32+L"\\mqutil.dll",&status);
	fver _mrt_exe      = getFileVer(System32+L"\\mrt.exe",&status);
	fver _msasn1_dll   = getFileVer(System32+L"\\msasn1.dll",&status);
	fver _msaud32_acm  = getFileVer(System32+L"\\msaud32.acm",&status);
	fver _msawt_dll    = getFileVer(System32+L"\\msawt.dll",&status);
	fver _mscms_dll    = getFileVer(System32+L"\\mscms.dll",&status);
	fver _msctf_dll    = getFileVer(System32+L"\\msctf.dll",&status);
	fver _msdart_dll   = getFileVer(System32+L"\\msdart.dll",&status);
	fver _msdrm_dll    = getFileVer(System32+L"\\msdrm.dll",&status);
	fver _msdtclog_dll = getFileVer(System32+L"\\msdtclog.dll",&status);
	fver _msdtcprx_dll = getFileVer(System32+L"\\msdtcprx.dll",&status);
	fver _msdtctm_dll  = getFileVer(System32+L"\\msdtctm.dll",&status);
	fver _msdtcuiu_dll = getFileVer(System32+L"\\msdtcuiu.dll",&status);
	fver _msdvbnp_ax   = getFileVer(System32+L"\\msdvbnp.ax",&status);
	fver _msdxm_ocx    = getFileVer(System32+L"\\msdxm.ocx",&status);
	fver _msexch40_dll = getFileVer(System32+L"\\msexch40.dll",&status);
	fver _msexcl40_dll = getFileVer(System32+L"\\msexcl40.dll",&status);
	fver _msfeeds_dll  = getFileVer(System32+L"\\msfeeds.dll",&status);
	fver _msfeedsbs_dll= getFileVer(System32+L"\\msfeedsbs.dll",&status);
	fver _msftedit_dll = getFileVer(System32+L"\\msftedit.dll",&status);
	fver _msgina_dll   = getFileVer(System32+L"\\msgina.dll",&status);
	fver _msgsvc_dll   = getFileVer(System32+L"\\msgsvc.dll",&status);
	fver _mshtml_dll   = getFileVer(System32+L"\\mshtml.dll",&status);
	fver _mshtmled_dll = getFileVer(System32+L"\\mshtmled.dll",&status);
	fver _msi_dll      = getFileVer(System32+L"\\msi.dll",&status);
	fver _msident_dll  = getFileVer(System32+L"\\msident.dll",&status);
	fver _msieftp_dll  = getFileVer(System32+L"\\msieftp.dll",&status);
	fver _msiexec_exe  = getFileVer(System32+L"\\msiexec.exe",&status);
	fver _msihnd_dll   = getFileVer(System32+L"\\msihnd.dll",&status);
	fver _msimsg_dll   = getFileVer(System32+L"\\msimsg.dll",&status);
	fver _msisip_dll   = getFileVer(System32+L"\\msisip.dll",&status);
	fver _msjava_dll   = getFileVer(System32+L"\\msjava.dll",&status);
	fver _msjdbc10_dll = getFileVer(System32+L"\\msjdbc10.dll",&status);
	fver _msjet40_dll  = getFileVer(System32+L"\\msjet40.dll",&status);
	fver _msjetol1_dll = getFileVer(System32+L"\\msjetol1.dll",&status);
	fver _msjint40_dll = getFileVer(System32+L"\\msjint40.dll",&status);
	fver _msjter40_dll = getFileVer(System32+L"\\msjter40.dll",&status);
	fver _msjtes40_dll = getFileVer(System32+L"\\msjtes40.dll",&status);
	fver _msltus40_dll = getFileVer(System32+L"\\msltus40.dll",&status);
	fver _msmqocm_dll  = getFileVer(System32+L"\\Setup\\msmqocm.dll",&status);
	fver _msnetobj_dll = getFileVer(System32+L"\\msnetobj.dll",&status);
	fver _msnp_ax      = getFileVer(System32+L"\\msnp.ax",&status);
	fver _msoeacct_dll = getFileVer(System32+L"\\msoeacct.dll",&status);
	fver _msoert2_dll  = getFileVer(System32+L"\\msoert2.dll",&status);
	fver _msorcl32_dll = getFileVer(System32+L"\\msorcl32.dll",&status);
	fver _mspaint_exe  = getFileVer(System32+L"\\mspaint.exe",&status);
	fver _mspbde40_dll = getFileVer(System32+L"\\mspbde40.dll",&status);
	fver _mspmsnsv_dll = getFileVer(System32+L"\\mspmsnsv.dll",&status);
	fver _mspmsp_dll   = getFileVer(System32+L"\\mspmsp.dll",&status);
	fver _msrating_dll = getFileVer(System32+L"\\msrating.dll",&status);
	fver _msrd2x40_dll = getFileVer(System32+L"\\msrd2x40.dll",&status);
	fver _msrd3x40_dll = getFileVer(System32+L"\\msrd3x40.dll",&status);
	fver _msrepl40_dll = getFileVer(System32+L"\\msrepl40.dll",&status);
	fver _msrle32_dll  = getFileVer(System32+L"\\msrle32.dll",&status);
	fver _msscp_dll    = getFileVer(System32+L"\\msscp.dll",&status);
	fver _mstask_dll   = getFileVer(System32+L"\\mstask.dll",&status);
	fver _mstext40_dll = getFileVer(System32+L"\\mstext40.dll",&status);
	fver _mstime_dll   = getFileVer(System32+L"\\mstime.dll",&status);
	fver _mstinit_exe  = getFileVer(System32+L"\\mstinit.exe",&status);
	fver _mstsc_exe    = getFileVer(System32+L"\\mstsc.exe",&status);
	fver _mstsc_exe_mui = getFileVer(System32+L"\\en-US\\mstsc.exe.mui",&status);
	fver _mstscax_dll  = getFileVer(System32+L"\\mstscax.dll",&status);
	fver _mstscax_dll_mui = getFileVer(System32+L"\\en-US\\mstscax.dll.mui",&status);
	fver _msv1_0_dll   = getFileVer(System32+L"\\msv1_0.dll",&status);
	fver _msvidc32_dll = getFileVer(System32+L"\\msvidc32.dll",&status);
	fver _msvidctl_dll = getFileVer(System32+L"\\msvidctl.dll",&status);
	fver _msw3prt_dll  = getFileVer(System32+L"\\msw3prt.dll",&status);
	fver _mswdat10_dll = getFileVer(System32+L"\\mswdat10.dll",&status);
	fver _mswebdvd_dll = getFileVer(System32+L"\\mswebdvd.dll",&status);
	fver _mswmdm_dll   = getFileVer(System32+L"\\mswmdm.dll",&status);
	fver _mswsock_dll  = getFileVer(System32+L"\\mswsock.dll",&status);
	fver _mswstr10_dll = getFileVer(System32+L"\\mswstr10.dll",&status);
	fver _msxbde40_dll = getFileVer(System32+L"\\msxbde40.dll",&status);
	fver _msxml2_dll   = getFileVer(System32+L"\\msxml2.dll",&status);
	fver _msxml3_dll   = getFileVer(System32+L"\\msxml3.dll",&status);
	fver _msxml4_dll   = getFileVer(System32+L"\\msxml4.dll",&status);
	fver _msxml4r_dll  = getFileVer(System32+L"\\msxml4r.dll",&status);
	fver _msxml6_dll   = getFileVer(System32+L"\\msxml6.dll",&status);
	fver _msxml6r_dll   = getFileVer(System32+L"\\msxml6r.dll",&status);
	fver _msyuv_dll    = getFileVer(System32+L"\\msyuv.dll",&status);
	fver _mtxclu_dll   = getFileVer(System32+L"\\mtxclu.dll",&status);
	fver _mtxoci_dll   = getFileVer(System32+L"\\mtxoci.dll",&status);
	fver _narrator_exe = getFileVer(System32+L"\\narrator.exe",&status);
	fver _nddenb32_dll = getFileVer(System32+L"\\nddenb32.dll",&status);
	fver _netapi32_dll = getFileVer(System32+L"\\netapi32.dll",&status);
	fver _netdde_exe   = getFileVer(System32+L"\\netdde.exe",&status);
	fver _netman_dll   = getFileVer(System32+L"\\netman.dll",&status);
	fver _netsetup_exe = getFileVer(System32+L"\\netsetup.exe",&status);
	fver _netsh_exe    = getFileVer(System32+L"\\netsh.exe",&status);
	fver _netshell_dll = getFileVer(System32+L"\\netshell.dll",&status);
	fver _newdev_dll   = getFileVer(System32+L"\\newdev.dll",&status);
	fver _ntdll_dll    = getFileVer(System32+L"\\ntdll.dll",&status);
	fver _ntkrnlmp_exe = getFileVer(System32+L"\\ntkrnlmp.exe",&status);
	fver _ntkrnlpa_exe = getFileVer(System32+L"\\ntkrnlpa.exe",&status);
	fver _ntkrpamp_exe = getFileVer(System32+L"\\ntkrpamp.exe",&status);
	fver _ntlanman_dll = getFileVer(System32+L"\\ntlanman.dll",&status);
	fver _ntoskrnl_exe = getFileVer(System32+L"\\ntoskrnl.exe",&status);
	fver _ntvdm_exe    = getFileVer(System32+L"\\ntvdm.exe",&status);
	fver _ntvdmd_dll   = getFileVer(System32+L"\\ntvdmd.dll",&status);
	fver _nwapi32_dll  = getFileVer(System32+L"\\nwapi32.dll",&status);
	fver _nwprovau_dll = getFileVer(System32+L"\\nwprovau.dll",&status);
	fver _nwwks_dll    = getFileVer(System32+L"\\nwwks.dll",&status);
	fver _oakley_dll   = getFileVer(System32+L"\\oakley.dll",&status);
	fver _occache_dll  = getFileVer(System32+L"\\occache.dll",&status);
	fver _odbc32_dll   = getFileVer(System32+L"\\odbc32.dll",&status);
	fver _odbcbcp_dll  = getFileVer(System32+L"\\odbcbcp.dll",&status);
	fver _odbccp32_dll = getFileVer(System32+L"\\odbccp32.dll",&status);
	fver _ole32_dll    = getFileVer(System32+L"\\ole32.dll",&status);
	fver _oleacc_dll   = getFileVer(System32+L"\\oleacc.dll",&status);
	fver _oleaccrc_dll = getFileVer(System32+L"\\oleaccrc.dll",&status);
	fver _oleaut32_dll = getFileVer(System32+L"\\oleaut32.dll",&status);
	fver _olecli32_dll = getFileVer(System32+L"\\olecli32.dll",&status);
	fver _olecnv32_dll = getFileVer(System32+L"\\olecnv32.dll",&status);
	fver _oledlg_dll   = getFileVer(System32+L"\\oledlg.dll",&status);
	fver _oleprn_dll   = getFileVer(System32+L"\\oleprn.dll",&status);
	fver _olepro32_dll = getFileVer(System32+L"\\olepro32.dll",&status);
	fver _osk_exe      = getFileVer(System32+L"\\osk.exe",&status);
	fver _p2p_dll      = getFileVer(System32+L"\\p2p.dll",&status);
	fver _p2pgasvc_dll = getFileVer(System32+L"\\p2pgasvc.dll",&status);
	fver _p2pgraph_dll = getFileVer(System32+L"\\p2pgraph.dll",&status);
	fver _p2pnetsh_dll = getFileVer(System32+L"\\p2pnetsh.dll",&status);
	fver _p2psvc_dll   = getFileVer(System32+L"\\p2psvc.dll",&status);
	fver _packager_exe = getFileVer(System32+L"\\packager.exe",&status);
	fver _photometadatahandler_dll = getFileVer(System32+L"\\photometadatahandler.dll",&status);
	fver _pintool_exe  = getFileVer(System32+L"\\pintool.exe",&status);
	fver _pnrpnsp_dll  = getFileVer(System32+L"\\pnrpnsp.dll",&status);
	fver _pdh_dll      = getFileVer(System32+L"\\pdh.dll",&status);
	fver _pngfilt_dll  = getFileVer(System32+L"\\pngfilt.dll",&status);
	fver _polstore_dll = getFileVer(System32+L"\\polstore.dll",&status);
	fver _printui_dll  = getFileVer(System32+L"\\printui.dll",&status);
	fver _psisdecd_dll = getFileVer(System32+L"\\psisdecd.dll",&status);
	fver _psisrndr_ax  = getFileVer(System32+L"\\psisrndr.ax",&status);
	fver _qasf_dll     = getFileVer(System32+L"\\qasf.dll",&status);
	fver _qfecheck_exe = getFileVer(System32+L"\\qfecheck.exe",&status);
	fver _qdvd_dll     = getFileVer(System32+L"\\qdvd.dll",&status);
	fver _qedit_dll    = getFileVer(System32+L"\\qedit.dll",&status);
	fver _qmgr_dll     = getFileVer(System32+L"\\qmgr.dll",&status);
	fver _qmgrprxy_dll = getFileVer(System32+L"\\qmgrprxy.dll",&status);
	fver _quartz_dll   = getFileVer(System32+L"\\quartz.dll",&status);
	fver _query_dll    = getFileVer(System32+L"\\query.dll",&status);
	fver _rasadhlp_dll = getFileVer(System32+L"\\rasadhlp.dll",&status);
	fver _rasapi32_dll = getFileVer(System32+L"\\rasapi32.dll",&status);
	fver _raschap_dll  = getFileVer(System32+L"\\raschap.dll",&status);
	fver _rasctrs_dll  = getFileVer(System32+L"\\rasctrs.dll",&status);
	fver _rasdlg_dll   = getFileVer(System32+L"\\rasdlg.dll",&status);
	fver _rasmans_dll  = getFileVer(System32+L"\\rasmans.dll",&status);
	fver _rasmxs_dll   = getFileVer(System32+L"\\rasmxs.dll",&status);
	fver _rassapi_dll  = getFileVer(System32+L"\\rassapi.dll",&status);
	fver _rastls_dll   = getFileVer(System32+L"\\rastls.dll",&status);
	fver _rdchost_dll  = getFileVer(System32+L"\\rdchost.dll",&status);
	fver _rdpdd_dll    = getFileVer(System32+L"\\rdpdd.dll",&status);
	fver _remotesp_tsp = getFileVer(System32+L"\\remotesp.tsp",&status);
	fver _rhttpaa_dll  = getFileVer(System32+L"\\rhttpaa.dll",&status);
	fver _riched20_dll = getFileVer(System32+L"\\riched20.dll",&status);
	fver _RmActivate_exe = getFileVer(System32+L"\\RmActivate.exe",&status);
	fver _RmActivate_isv_exe = getFileVer(System32+L"\\RmActivate_isv.exe",&status);
	fver _RmActivate_ssp_exe = getFileVer(System32+L"\\RmActivate_ssp.exe",&status);
	fver _RmActivate_ssp_isv_exe = getFileVer(System32+L"\\RmActivate_ssp_isv.exe",&status);
	fver _rpcrt4_dll   = getFileVer(System32+L"\\rpcrt4.dll",&status);
	fver _rpcss_dll    = getFileVer(System32+L"\\rpcss.dll",&status);
	fver _rsaenh_dll   = getFileVer(System32+L"\\rsaenh.dll",&status);
	fver _rtcdll_dll   = getFileVer(System32+L"\\rtcdll.dll",&status);
	fver _safrslv_dll  = getFileVer(System32+L"\\safrslv.dll",&status);
	fver _samsrv_dll   = getFileVer(System32+L"\\samsrv.dll",&status);
	fver _sbe_dll      = getFileVer(System32+L"\\sbe.dll",&status);
	fver _sc_exe       = getFileVer(System32+L"\\sc.exe",&status);
	fver _scesrv_dll   = getFileVer(System32+L"\\scesrv.dll",&status);
	fver _schannel_dll = getFileVer(System32+L"\\schannel.dll",&status);
	fver _schedsvc_dll = getFileVer(System32+L"\\schedsvc.dll",&status);
	fver _scrobj_dll   = getFileVer(System32+L"\\scrobj.dll",&status);
	fver _scrrun_dll   = getFileVer(System32+L"\\scrrun.dll",&status);
	fver _seclogon_dll = getFileVer(System32+L"\\seclogon.dll",&status);
	fver _SecProc_dll  = getFileVer(System32+L"\\SecProc.dll",&status);
	fver _SecProc_isv_dll = getFileVer(System32+L"\\SecProc_isv.dll",&status);
	fver _SecProc_ssp_dll = getFileVer(System32+L"\\SecProc_ssp.dll",&status);
	fver _SecProc_ssp_isv_dll = getFileVer(System32+L"\\SecProc_ssp_isv.dll",&status);
	fver _secur32_dll  = getFileVer(System32+L"\\secur32.dll",&status);
	fver _services_exe = getFileVer(System32+L"\\services.exe",&status);
	fver _sessmgr_exe  = getFileVer(System32+L"\\sessmgr.exe",&status);
	fver _sfcfiles_dll = getFileVer(System32+L"\\sfcfiles.dll",&status);
	fver _shdoclc_dll  = getFileVer(System32+L"\\shdoclc.dll",&status);
	fver _shdocvw_dll  = getFileVer(System32+L"\\shdocvw.dll",&status);
	fver _shell32_dll  = getFileVer(System32+L"\\shell32.dll",&status);
	fver _shgina_dll   = getFileVer(System32+L"\\shgina.dll",&status);
	fver _shimgvw_dll  = getFileVer(System32+L"\\shimgvw.dll",&status);
	fver _shlwapi_dll  = getFileVer(System32+L"\\shlwapi.dll",&status);
	fver _shmedia_dll  = getFileVer(System32+L"\\shmedia.dll",&status);
	fver _shmgrate_exe = getFileVer(System32+L"\\shmgrate.exe",&status);
	fver _shsvcs_dll   = getFileVer(System32+L"\\shsvcs.dll",&status);
	fver _snmp_exe     = getFileVer(System32+L"\\snmp.exe",&status);
	fver _snmpapi_dll  = getFileVer(System32+L"\\snmpapi.dll",&status);
	fver _spiisupd_exe = getFileVer(System32+L"\\spiisupd.exe",&status);
	fver _spoolsv_exe  = getFileVer(System32+L"\\spoolsv.exe",&status);
	fver _sqlsrv32_dll = getFileVer(System32+L"\\sqlsrv32.dll",&status);
	fver _srvsvc_dll   = getFileVer(System32+L"\\srvsvc.dll",&status);
	fver _srrstr_dll   = getFileVer(System32+L"\\srrstr.dll",&status);
	fver _ssdpapi_dll  = getFileVer(System32+L"\\ssdpapi.dll",&status);
	fver _ssdpsrv_dll  = getFileVer(System32+L"\\ssdpsrv.dll",&status);
	fver _stclient_dll = getFileVer(System32+L"\\stclient.dll",&status);
	fver _strmdll_dll  = getFileVer(System32+L"\\strmdll.dll",&status);
	fver _strmfilt_dll = getFileVer(System32+L"\\strmfilt.dll",&status);
	fver _sxs_dll      = getFileVer(System32+L"\\sxs.dll",&status);
	fver _synceng_dll  = getFileVer(System32+L"\\synceng.dll",&status);
	fver _t2embed_dll  = getFileVer(System32+L"\\t2embed.dll",&status);
	fver _tapisrv_dll  = getFileVer(System32+L"\\tapisrv.dll",&status);
	fver _tdc_ocx      = getFileVer(System32+L"\\tdc.ocx",&status);
	fver _telnet_exe   = getFileVer(System32+L"\\telnet.exe",&status);
	fver _termsrv_dll  = getFileVer(System32+L"\\termsrv.dll",&status);
	fver _themeui_dll  = getFileVer(System32+L"\\themeui.dll",&status);
	fver _tlntsess_exe = getFileVer(System32+L"\\tlntsess.exe",&status);
	fver _tsbyuv_dll   = getFileVer(System32+L"\\tsbyuv.dll",&status);
	fver _tsgqec_dll   = getFileVer(System32+L"\\tsgqec.dll",&status);
	fver _tspkg_dll    = getFileVer(System32+L"\\tspkg.dll",&status);
	fver _txflog_dll   = getFileVer(System32+L"\\txflog.dll",&status);
	fver _tzchange_exe = getFileVer(System32+L"\\tzchange.exe",&status);
	fver _uexfat_dll   = getFileVer(System32+L"\\uexfat.dll",&status);
	fver _uiautomationcore_dll = getFileVer(System32+L"\\uiautomationcore.dll",&status);
	fver _ulib_dll     = getFileVer(System32+L"\\ulib.dll",&status);
	fver _umandlg_dll  = getFileVer(System32+L"\\umandlg.dll",&status);
	fver _umpnpmgr_dll = getFileVer(System32+L"\\umpnpmgr.dll",&status);
	fver _unregmp2_exe = getFileVer(SystemRoot+L"\\INF\\unregmp2.exe",&status);
	fver _upnp_dll     = getFileVer(System32+L"\\upnp.dll",&status);
	fver _upnphost_dll = getFileVer(System32+L"\\upnphost.dll",&status);
	fver _ups_exe      = getFileVer(System32+L"\\ups.exe",&status);
	fver _url_dll      = getFileVer(System32+L"\\url.dll",&status);
	fver _urlmon_dll   = getFileVer(System32+L"\\urlmon.dll",&status);
	fver _user32_dll   = getFileVer(System32+L"\\user32.dll",&status);
	fver _userenv_dll  = getFileVer(System32+L"\\userenv.dll",&status);
	fver _usp10_dll    = getFileVer(System32+L"\\usp10.dll",&status);
	fver _utilman_exe  = getFileVer(System32+L"\\utilman.exe",&status);
	fver _uwdf_exe     = getFileVer(System32+L"\\uwdf.exe",&status);
	fver _vbajet32_dll = getFileVer(System32+L"\\vbajet32.dll",&status);
	fver _vbicodec_ax  = getFileVer(System32+L"\\vbicodec.ax",&status);
	fver _vbscript_dll = getFileVer(System32+L"\\vbscript.dll",&status);
	fver _vdmdbg_dll   = getFileVer(System32+L"\\vdmdbg.dll",&status);
	fver _verclsid_exe = getFileVer(System32+L"\\verclsid.exe",&status);
	fver _vmhelper_dll = getFileVer(System32+L"\\vmhelper.dll",&status);
	fver _wdfapi_dll   = getFileVer(System32+L"\\wdfapi.dll",&status);
	fver _WdfCoInstaller01007_dll = getFileVer(System32+L"\\WdfCoInstaller01007.dll",&status);
	fver _wdfmgr_exe   = getFileVer(System32+L"\\wdfmgr.exe",&status);
	fver _wdigest_dll  = getFileVer(System32+L"\\wdigest.dll",&status);
	fver _webcheck_dll = getFileVer(System32+L"\\webcheck.dll",&status);
	fver _webclnt_dll  = getFileVer(System32+L"\\webclnt.dll",&status);
	fver _wiaservc_dll = getFileVer(System32+L"\\wiaservc.dll",&status);
	fver _win32k_sys   = getFileVer(System32+L"\\win32k.sys",&status);
	fver _win32spl_dll = getFileVer(System32+L"\\win32spl.dll",&status);
	fver _windowscodecs_dll = getFileVer(System32+L"\\windowscodecs.dll",&status);
	fver _windowscodecsext_dll = getFileVer(System32+L"\\windowscodecsext.dll",&status);
	fver _winhttp_dll  = getFileVer(System32+L"\\winhttp.dll",&status);
	fver _WINHTTP5_DLL = getFileVer(System32+L"\\WINHTTP5.DLL",&status);
	fver _wininet_dll  = getFileVer(System32+L"\\wininet.dll",&status);
	fver _winipsec_dll = getFileVer(System32+L"\\winipsec.dll",&status);
	fver _winlogon_exe = getFileVer(System32+L"\\winlogon.exe",&status);
	fver _winmm_dll    = getFileVer(System32+L"\\winmm.dll",&status);
	fver _winspool_drv = getFileVer(System32+L"\\winspool.drv",&status);
	fver _winsrv_dll   = getFileVer(System32+L"\\winsrv.dll",&status);
	fver _wintrust_dll = getFileVer(System32+L"\\wintrust.dll",&status);
	fver _winusbcoinstaller_dll = getFileVer(System32+L"\\winusbcoinstaller.dll",&status);
	fver _wjview_exe   = getFileVer(System32+L"\\wjview.exe",&status);
	fver _wksprt_exe   = getFileVer(System32+L"\\wksprt.exe",&status);
	fver _wkssvc_dll   = getFileVer(System32+L"\\wkssvc.dll",&status);
	fver _wldap32_dll  = getFileVer(System32+L"\\wldap32.dll",&status);
	fver _WMADMOD_dll  = getFileVer(System32+L"\\WMADMOD.dll",&status);
	fver _WMADMOE_dll  = getFileVer(System32+L"\\WMADMOE.dll",&status);
	fver _wmasf_dll    = getFileVer(System32+L"\\wmasf.dll",&status);
	fver _wmavds32_ax  = getFileVer(System32+L"\\wmavds32.ax",&status);
	fver _wmdmlog_dll  = getFileVer(System32+L"\\wmdmlog.dll",&status);
	fver _wmdmps_dll   = getFileVer(System32+L"\\wmdmps.dll",&status);
	fver _wmdrmdev_dll = getFileVer(System32+L"\\wmdrmdev.dll",&status);
	fver _wmdrmnet_dll = getFileVer(System32+L"\\wmdrmnet.dll",&status);
	fver _wmdrmsdk_dll = getFileVer(System32+L"\\wmdrmsdk.dll",&status);
	fver _wmidx_dll    = getFileVer(System32+L"\\wmidx.dll",&status);
	fver _wmnetmgr_dll = getFileVer(System32+L"\\wmnetmgr.dll",&status);
	fver _wmp_dll      = getFileVer(System32+L"\\wmp.dll",&status);
	fver _wmpcore_dll  = getFileVer(System32+L"\\wmpcore.dll",&status);
	fver _wmpdxm_dll   = getFileVer(System32+L"\\wmpdxm.dll",&status);
	fver _wmpeffects_dll = getFileVer(System32+L"\\wmpeffects.dll",&status);
	fver _wmphoto_dll  = getFileVer(System32+L"\\wmphoto.dll",&status);
	fver _wmploc_dll   = getFileVer(System32+L"\\wmploc.dll",&status);
	fver _wmpui_dll    = getFileVer(System32+L"\\wmpui.dll",&status);
	fver _wmsdmod_dll  = getFileVer(System32+L"\\wmsdmod.dll",&status);
	fver _wmsdmoe2_dll = getFileVer(System32+L"\\wmsdmoe2.dll",&status);
	fver _wmspdmod_dll = getFileVer(System32+L"\\wmspdmod.dll",&status);
	fver _WMSPDMOE_dll = getFileVer(System32+L"\\WMSPDMOE.dll",&status);
	fver _wmv9vcm_dll  = getFileVer(System32+L"\\wmv9vcm.dll",&status);
	fver _WMVADVD_dll  = getFileVer(System32+L"\\WMVADVD.dll",&status);
	fver _WMVADVE_DLL  = getFileVer(System32+L"\\WMVADVE.DLL",&status);
	fver _wmvcore_dll  = getFileVer(System32+L"\\wmvcore.dll",&status);
	fver _wmvdecod_dll = getFileVer(System32+L"\\wmvdecod.dll",&status);
	fver _wmvdmod_dll  = getFileVer(System32+L"\\wmvdmod.dll",&status);
	fver _wmvdmoe2_dll = getFileVer(System32+L"\\wmvdmoe2.dll",&status);
	fver _wow32_dll    = getFileVer(System32+L"\\wow32.dll",&status);
	fver _wpdconns_dll = getFileVer(System32+L"\\wpdconns.dll",&status);
	fver _wpdmtp_dll   = getFileVer(System32+L"\\wpdmtp.dll",&status);
	fver _wpdmtpdr_dll = getFileVer(System32+L"\\drivers\\UMDF\\wpdmtpdr.dll",&status);
	fver _wpdmtpus_dll = getFileVer(System32+L"\\wpdmtpus.dll",&status);
	fver _wpdsp_dll    = getFileVer(System32+L"\\wpdsp.dll",&status);
	fver _wpdtrace_dll = getFileVer(System32+L"\\wpdtrace.dll",&status);
	fver _wpd_ci_dll   = getFileVer(System32+L"\\wpd_ci.dll",&status);
	fver _ws2_32_dll   = getFileVer(System32+L"\\ws2_32.dll",&status);
	fver _wscript_exe  = getFileVer(System32+L"\\wscript.exe",&status);
	fver _wshext_dll   = getFileVer(System32+L"\\wshext.dll",&status);
	fver _wship6_dll   = getFileVer(System32+L"\\wship6.dll",&status);
	fver _wshom_ocx    = getFileVer(System32+L"\\wshom.ocx",&status);
	fver _wsnmp32_dll  = getFileVer(System32+L"\\wsnmp32.dll",&status);
	fver _wstpager_ax  = getFileVer(System32+L"\\wstpager.ax",&status);
	fver _wstrenderer_ax = getFileVer(System32+L"\\wstrenderer.ax",&status);
	fver _wudfupdate_01007_dll  = getFileVer(System32+L"\\wudfupdate_01007.dll",&status);
	fver _wudfusbcciddriver_dll = getFileVer(System32+L"\\drivers\\UMDF\\wudfusbcciddriver.dll",&status);
	fver _wzcdlg_dll   = getFileVer(System32+L"\\wzcdlg.dll",&status);
	fver _wzcsapi_dll  = getFileVer(System32+L"\\wzcsapi.dll",&status);
	fver _wzcsvc_dll   = getFileVer(System32+L"\\wzcsvc.dll",&status);
	fver _xactsrv_dll  = getFileVer(System32+L"\\xactsrv.dll",&status);
	fver _xenroll_dll  = getFileVer(System32+L"\\xenroll.dll",&status);
	fver _xmllite_dll  = getFileVer(System32+L"\\xmllite.dll",&status);
	fver _xolehlp_dll  = getFileVer(System32+L"\\xolehlp.dll",&status);
	fver _xp_eos_exe   = getFileVer(System32+L"\\xp_eos.exe",&status);
	fver _xpob2res_dll = getFileVer(System32+L"\\xpob2res.dll",&status);
	fver _xpsp1hfm_exe = getFileVer(System32+L"\\xpsp1hfm.exe",&status);
	fver _xpsp4res_dll = getFileVer(System32+L"\\xpsp4res.dll",&status);
	fver _zipfldr_dll  = getFileVer(System32+L"\\zipfldr.dll",&status);

	// Windows Search System32 Files
	fver _msscntrs_dll  = getFileVer(System32+L"\\msscntrs.dll",&status);
	fver _mssitlb_dll  = getFileVer(System32+L"\\mssitlb.dll",&status);
	fver _searchprotocolhost_exe  = getFileVer(System32+L"\\searchprotocolhost.exe",&status);
	fver _searchfilterhost_exe  = getFileVer(System32+L"\\searchfilterhost.exe",&status);
	fver _searchindexer_exe  = getFileVer(System32+L"\\searchindexer.exe",&status);
	fver _mssph_dll  = getFileVer(System32+L"\\mssph.dll",&status);
	fver _mssphtb_dll  = getFileVer(System32+L"\\mssphtb.dll",&status);
	fver _mssprxy_dll  = getFileVer(System32+L"\\mssprxy.dll",&status);
	fver _mssrch_dll  = getFileVer(System32+L"\\mssrch.dll",&status);
	fver _msstrc_dll  = getFileVer(System32+L"\\msstrc.dll",&status);
	fver _propdefs_dll  = getFileVer(System32+L"\\propdefs.dll",&status);
	fver _tquery_dll  = getFileVer(System32+L"\\tquery.dll",&status);
	fver _msscb_dll  = getFileVer(System32+L"\\msscb.dll",&status);
	fver _msshooks_dll  = getFileVer(System32+L"\\msshooks.dll",&status);
	fver _msshsq_dll  = getFileVer(System32+L"\\msshsq.dll",&status);
	fver _xmlfilter_dll  = getFileVer(System32+L"\\xmlfilter.dll",&status);
	fver _rtffilt_dll  = getFileVer(System32+L"\\rtffilt.dll",&status);
	fver _srchadmin_dll  = getFileVer(System32+L"\\srchadmin.dll",&status);
	fver _propsys_dll  = getFileVer(System32+L"\\propsys.dll",&status);
	fver _searchindexer_exe_mui  = getFileVer(System32+L"\\searchindexer.exe.mui",&status);
	fver _mssrch_dll_mui  = getFileVer(System32+L"\\mssrch.dll.mui",&status);
	fver _tquery_dll_mui  = getFileVer(System32+L"\\tquery.dll.mui",&status);
	fver _mssphtb_dll_mui  = getFileVer(System32+L"\\mssphtb.dll.mui",&status);
	fver _mssph_dll_mui  = getFileVer(System32+L"\\mssph.dll.mui",&status);
	fver _srchadmin_dll_mui  = getFileVer(System32+L"\\srchadmin.dll.mui",&status);
	fver _propsys_dll_mui  = getFileVer(System32+L"\\propsys.dll.mui",&status);
	fver _oeph_dll  = getFileVer(System32+L"\\oeph.dll",&status);
	fver _oephres_dll  = getFileVer(System32+L"\\oephres.dll",&status);
	fver _uncph_dll  = getFileVer(System32+L"\\uncph.dll",&status);
	fver _uncres_dll  = getFileVer(System32+L"\\uncres.dll",&status);
	fver _unccplext_dll  = getFileVer(System32+L"\\unccplext.dll",&status);
	fver _uncdms_dll  = getFileVer(System32+L"\\uncdms.dll",&status);
	fver _uncne_dll  = getFileVer(System32+L"\\uncne.dll",&status);

	// Detect Windows Media Player version
	fver wmp = _wmp_dll;
	if(wmp==fver()) { wmp=_wmpcore_dll; }

	// Windows XP Media Center Edition files
	fver _audiodepthconverter_ax = getFileVer(ehomeFilters+L"\\audiodepthconverter.ax",&status);
	fver _directshowtap_ax       = getFileVer(ehomeFilters+L"\\directshowtap.ax",&status);
	fver _sonicmpegaudio_dll     = getFileVer(ehomeFilters+L"\\sonicmpegaudio.dll",&status);
	fver _sonicmpegsplitter_dll  = getFileVer(ehomeFilters+L"\\sonicmpegsplitter.dll",&status);
	fver _sonicmpegvideo_dll     = getFileVer(ehomeFilters+L"\\sonicmpegvideo.dll",&status);
	fver _rtstreamsink_ax        = getFileVer(ehomeFilters+L"\\rtstreamsink.ax",&status);

	fver _bdatunepia_dll=getFileVer(ehome+L"\\bdatunepia.dll",&status);
	fver _ehcett_dll   = getFileVer(ehome+L"\\ehcett.dll",&status);
	fver _ehchhime_dll = getFileVer(ehome+L"\\ehchhime.dll",&status);
	fver _ehchtime_dll = getFileVer(ehome+L"\\ehchtime.dll",&status);
	fver _ehcir_dll    = getFileVer(ehome+L"\\ehcir.dll",&status);
	fver _ehcm_dll     = getFileVer(ehome+L"\\ehcm.dll",&status);
	fver _ehcommon_dll = getFileVer(ehome+L"\\ehcommon.dll",&status);
	fver _ehcyrtt_dll  = getFileVer(ehome+L"\\ehcyrtt.dll",&status);
	fver _ehdebug_dll  = getFileVer(ehome+L"\\ehdebug.dll",&status);
	fver _ehdrop_dll   = getFileVer(ehome+L"\\ehdrop.dll",&status);
	fver _ehentt_dll   = getFileVer(ehome+L"\\ehentt.dll",&status);
	fver _ehepg_dll    = getFileVer(ehome+L"\\ehepg.dll",&status);
	fver _ehepgdat_dll = getFileVer(ehome+L"\\ehepgdat.dll",&status);
	fver _ehextcom_dll = getFileVer(ehome+L"\\ehextcom.dll",&status);
	fver _ehexthost_exe= getFileVer(ehome+L"\\ehexthost.exe",&status);
	fver _ehglid_dll   = getFileVer(ehome+L"\\ehglid.dll",&status);
	fver _ehiextcom_dll= getFileVer(ehome+L"\\ehiextcom.dll",&status);
	fver _ehiextens_dll= getFileVer(ehome+L"\\ehiextens.dll",&status);
	fver _ehiplay_dll  = getFileVer(ehome+L"\\ehiplay.dll",&status);
	fver _ehiproxy_dll = getFileVer(ehome+L"\\ehiproxy.dll",&status);
	fver _ehiuserxp_dll= getFileVer(ehome+L"\\ehiuserxp.dll",&status);
	fver _ehividctl_dll= getFileVer(ehome+L"\\ehividctl.dll",&status);
	fver _ehiwuapi_dll = getFileVer(ehome+L"\\ehiwuapi.dll",&status);
	fver _ehjpnime_dll = getFileVer(ehome+L"\\ehjpnime.dll",&status);
	fver _ehkeyctl_dll = getFileVer(ehome+L"\\ehkeyctl.dll",&status);
	fver _ehkorime_dll = getFileVer(ehome+L"\\ehkorime.dll",&status);
	fver _ehmcxins_exe = getFileVer(ehome+L"\\ehmcxins.exe",&status);
	fver _ehmsas_exe   = getFileVer(ehome+L"\\ehmsas.exe",&status);
	fver _ehplayer_dll = getFileVer(ehome+L"\\ehplayer.dll",&status);
	fver _ehproxy_dll  = getFileVer(ehome+L"\\ehproxy.dll",&status);
	fver _ehrec_exe    = getFileVer(ehome+L"\\ehrec.exe",&status);
	fver _ehrecobj_dll = getFileVer(ehome+L"\\ehrecobj.dll",&status);
	fver _ehrecvr_exe  = getFileVer(ehome+L"\\ehrecvr.exe",&status);
	fver _ehres_dll    = getFileVer(ehome+L"\\ehres.dll",&status);
	fver _ehreschs_dll  = getFileVer(ehome+L"\\zh-chs\\ehepg.resources.dll",&status);
	fver _ehrescht_dll  = getFileVer(ehome+L"\\zh-cht\\ehepg.resources.dll",&status);
	fver _ehrescs_dll  = getFileVer(ehome+L"\\cs\\ehepg.resources.dll",&status);
	fver _ehresda_dll  = getFileVer(ehome+L"\\da\\ehepg.resources.dll",&status);
	fver _ehresde_dll  = getFileVer(ehome+L"\\de\\ehepg.resources.dll",&status);
	fver _ehreses_dll  = getFileVer(ehome+L"\\es\\ehepg.resources.dll",&status);
	fver _ehresfi_dll  = getFileVer(ehome+L"\\fi\\ehepg.resources.dll",&status);
	fver _ehresfr_dll  = getFileVer(ehome+L"\\fr\\ehepg.resources.dll",&status);
	fver _ehresit_dll  = getFileVer(ehome+L"\\it\\ehepg.resources.dll",&status);
	fver _ehresja_dll  = getFileVer(ehome+L"\\ja\\ehepg.resources.dll",&status);
	fver _ehresko_dll  = getFileVer(ehome+L"\\ko\\ehepg.resources.dll",&status);
	fver _ehresnl_dll  = getFileVer(ehome+L"\\nl\\ehepg.resources.dll",&status);
	fver _ehresno_dll  = getFileVer(ehome+L"\\no\\ehepg.resources.dll",&status);
	fver _ehrespl_dll  = getFileVer(ehome+L"\\pl\\ehepg.resources.dll",&status);
	fver _ehrespt_dll  = getFileVer(ehome+L"\\pt\\ehepg.resources.dll",&status);
	fver _ehresru_dll  = getFileVer(ehome+L"\\ru\\ehepg.resources.dll",&status);
	fver _ehressv_dll  = getFileVer(ehome+L"\\sv\\ehepg.resources.dll",&status);
	fver _ehrestr_dll  = getFileVer(ehome+L"\\tr\\ehepg.resources.dll",&status);
	fver _ehsched_exe  = getFileVer(ehome+L"\\ehsched.exe",&status);
	fver _ehshell_exe  = getFileVer(ehome+L"\\ehshell.exe",&status);
	fver _ehtktt_dll   = getFileVer(ehome+L"\\ehtktt.dll",&status);
	fver _ehtrace_dll  = getFileVer(ehome+L"\\ehtrace.dll",&status);
	fver _ehtray_exe   = getFileVer(ehome+L"\\ehtray.exe",&status);
	fver _ehui_dll     = getFileVer(ehome+L"\\ehui.dll",&status);
	fver _ehuihlp_dll  = getFileVer(ehome+L"\\ehuihlp.dll",&status);
	fver _mcrdsvc_exe  = getFileVer(ehome+L"\\mcrdsvc.exe",&status);
	fver _mcrmgr_dll   = getFileVer(ehome+L"\\mcrmgr.dll",&status);
	fver _mcrmgr_exe   = getFileVer(ehome+L"\\mcrmgr.exe",&status);
	fver _medctrro_exe = getFileVer(ehome+L"\\medctrro.exe",&status);
	fver _microsoft_mediacenter_dll = getFileVer(ehome+L"\\Microsoft.MediaCenter.dll",&status);
	fver _mstvcapn_dll = getFileVer(ehome+L"\\mstvcapn.dll",&status);
	fver _sqldb20_dll  = getFileVer(ehome+L"\\sqldb20.dll",&status);
	fver _sqlqp20_dll  = getFileVer(ehome+L"\\sqlqp20.dll",&status);
	fver _sqlse20_dll  = getFileVer(ehome+L"\\sqlse20.dll",&status);

	fver _ContextTagger_dll = getFileVer(Ink+L"\\ContextTagger.dll",&status);
	fver _TabTip_exe        = getFileVer(Ink+L"\\TabTip.exe",&status);
	fver _TCServer_exe      = getFileVer(Ink+L"\\TCServer.exe",&status);
	fver _TipLibrary_dll    = getFileVer(Ink+L"\\TipLibrary.dll",&status);
	fver _tiptsf_dll        = getFileVer(Ink+L"\\tiptsf.dll",&status);

	fver _standardfx_plugin_dll = getFileVer(Sfxplugins+L"\\standardfx_plugin.dll",&status);

	fver _tenfoothelp_exe = getFileVer(ehHelp+L"\\tenfoothelp.exe",&status);

	fver _authorscript_dll       = getFileVer(CreateDisc+L"\\authorscript.dll",&status);
	fver _createdisc_dll         = getFileVer(CreateDisc+L"\\createdisc.dll",&status);
	fver _mce_wmadrm_dll         = getFileVer(CreateDisc+L"\\mce_wmadrm.dll",&status);
	fver _primosdk_dll           = getFileVer(CreateDisc+L"\\primosdk.dll",&status);
	fver _px_dll                 = getFileVer(CreateDisc+L"\\px.dll",&status);
	fver _pxdrv_dll              = getFileVer(CreateDisc+L"\\pxdrv.dll",&status);
	fver _pxhpinst_exe           = getFileVer(CreateDisc+L"\\pxhpinst.exe",&status);
	fver _pxmas_dll              = getFileVer(CreateDisc+L"\\pxmas.dll",&status);
	fver _pxwavedec_dll          = getFileVer(CreateDisc+L"\\pxwavedec.dll",&status);
	fver _pxwma_dll              = getFileVer(CreateDisc+L"\\pxwma.dll",&status);
	fver _sbeserver_exe          = getFileVer(CreateDisc+L"\\sbeserver.exe",&status);
	fver _sbeserverps_dll        = getFileVer(CreateDisc+L"\\sbeserverps.dll",&status);
	fver _simpleburnuires_dll    = getFileVer(CreateDisc+L"\\simpleburnuires.dll",&status);
	fver _sonicmceburnengine_dll = getFileVer(CreateDisc+L"\\sonicmceburnengine.dll",&status);
	fver _sonicmmburnengine_exe  = getFileVer(CreateDisc+L"\\sonicmmburnengine.exe",&status);
	fver _vxblock_dll            = getFileVer(CreateDisc+L"\\vxblock.dll",&status);

	// Windows Genuine Advantge (WGA) Files
	fver _LegitCheckControl_dll = getFileVer(System32+L"\\LegitCheckControl.dll",&status);
	fver _WgaLogon_dll          = getFileVer(System32+L"\\WgaLogon.dll",&status);
	fver _WgaTray_exe           = getFileVer(System32+L"\\WgaTray.exe",&status);

	// Windows Search WDS files
	fver _deskbar_dll  = getFileVer(WDS+L"\\deskbar.dll",&status);
	fver _dbsetup_dll  = getFileVer(WDS+L"\\dbsetup.dll",&status);
	fver _dbres_dll  = getFileVer(WDS+L"\\dbres.dll",&status);
	fver _wordwheel_dll  = getFileVer(WDS+L"\\wordwheel.dll",&status);
	fver _wwres_dll  = getFileVer(WDS+L"\\wwres.dll",&status);
	fver _msnlext_dll  = getFileVer(WDS+L"\\msnlext.dll",&status);
	fver _msnlextres_dll  = getFileVer(WDS+L"\\msnlextres.dll",&status);
	fver _msnldl_dll  = getFileVer(WDS+L"\\msnldl.dll",&status);
	fver _msnldlps_dll  = getFileVer(WDS+L"\\msnldlps.dll",&status);
	fver _msnlnamespacemgr_dll  = getFileVer(WDS+L"\\msnlnamespacemgr.dll",&status);
	fver _wdsshell_dll  = getFileVer(WDS+L"\\wdsshell.dll",&status);
	fver _wdsview_dll  = getFileVer(WDS+L"\\wdsview.dll",&status);
	fver _mapine_dll  = getFileVer(WDS+L"\\mapine.dll",&status);
	fver _mapineres_dll  = getFileVer(WDS+L"\\mapineres.dll",&status);
	fver _xppreviewproxy_dll  = getFileVer(WDS+L"\\xppreviewproxy.dll",&status);
	fver _msnlred_dll  = getFileVer(WDS+L"\\msnlred.dll",&status);
	fver _wdsmkttools_dll  = getFileVer(WDS+L"\\wdsmkttools.dll",&status);
	fver _windowssearchres_dll  = getFileVer(WDS+L"\\windowssearchres.dll",&status);
	fver _windowssearch_exe  = getFileVer(WDS+L"\\windowssearch.exe",&status);

	fver _msnlextres_dll_mui  = getFileVer(WDS_mui+L"\\msnlextres.dll.mui",&status);
	fver _dbres_dll_mui  = getFileVer(WDS_mui+L"\\dbres.dll.mui",&status);
	fver _wwres_dll_mui  = getFileVer(WDS_mui+L"\\wwres.dll.mui",&status);
	fver _mapineres_dll_mui  = getFileVer(WDS_mui+L"\\mapineres.dll.mui",&status);
	fver _windowssearchres_dll_mui  = getFileVer(WDS_mui+L"\\windowssearchres.dll.mui",&status);
	
	fver _oephres_dll_mui  = getFileVer(System32+L"\\en-US\\oephres.dll.mui",&status);
	fver _uncres_dll_mui  = getFileVer(System32+L"\\en-US\\uncres.dll.mui",&status);

	fver _comadmin_dll = getFileVer(System32+L"\\Com\\comadmin.dll",&status);
	fver _migregdb_exe = getFileVer(System32+L"\\Com\\migregdb.exe",&status);

	fver _aec_sys      = getFileVer(Drivers+L"\\aec.sys",&status);
	fver _afd_sys      = getFileVer(Drivers+L"\\afd.sys",&status);
	fver _bthport_sys  = getFileVer(Drivers+L"\\bthport.sys",&status);
	fver _cdrom_sys    = getFileVer(Drivers+L"\\cdrom.sys",&status);
	fver _dmusic_sys   = getFileVer(Drivers+L"\\dmusic.sys",&status);
	fver _dxg_sys      = getFileVer(Drivers+L"\\dxg.sys",&status);
	fver _hidir_sys    = getFileVer(Drivers+L"\\hidir.sys",&status);
	fver _hidparse_sys = getFileVer(Drivers+L"\\hidparse.sys",&status);
	fver _exfat_sys    = getFileVer(Drivers+L"\\exfat.sys",&status);
	fver _fastfat_sys  = getFileVer(Drivers+L"\\fastfat.sys",&status);
	fver _fltMgr_sys   = getFileVer(Drivers+L"\\fltMgr.sys",&status);
	fver _fs_rec_sys   = getFileVer(Drivers+L"\\fs_rec.sys",&status);
	fver _http_sys     = getFileVer(Drivers+L"\\http.sys",&status);
	fver _imapi_sys    = getFileVer(Drivers+L"\\imapi.sys",&status);
	fver _ipnat_sys    = getFileVer(Drivers+L"\\ipnat.sys",&status);
	fver _ipsec_sys    = getFileVer(Drivers+L"\\ipsec.sys",&status);
	fver _irbus_sys    = getFileVer(Drivers+L"\\irbus.sys",&status);
	fver _kmixer_sys   = getFileVer(Drivers+L"\\kmixer.sys",&status);
	fver _ks_sys       = getFileVer(Drivers+L"\\ks.sys",&status);
	fver _ksecdd_sys   = getFileVer(Drivers+L"\\ksecdd.sys",&status);
	fver _mrxdav_sys   = getFileVer(Drivers+L"\\mrxdav.sys",&status);
	fver _mrxsmb_sys   = getFileVer(Drivers+L"\\mrxsmb.sys",&status);
	fver _mqac_sys     = getFileVer(Drivers+L"\\mqac.sys",&status);
	fver _msfs_sys     = getFileVer(Drivers+L"\\msfs.sys",&status);
	fver _mup_sys      = getFileVer(Drivers+L"\\mup.sys",&status);
	fver _ndis_sys     = getFileVer(Drivers+L"\\ndis.sys",&status);
	fver _ndistapi_sys = getFileVer(Drivers+L"\\ndistapi.sys",&status);
	fver _ndisuio_sys  = getFileVer(Drivers+L"\\ndisuio.sys",&status);
	fver _ndiswan_sys  = getFileVer(Drivers+L"\\ndiswan.sys",&status);
	fver _ndproxy_sys  = getFileVer(Drivers+L"\\ndproxy.sys",&status);
	fver _netbt_sys    = getFileVer(Drivers+L"\\netbt.sys",&status);
	fver _npfs_sys     = getFileVer(Drivers+L"\\npfs.sys",&status);
	fver _ntfs_sys     = getFileVer(Drivers+L"\\ntfs.sys",&status);
	fver _nwrdr_sys    = getFileVer(Drivers+L"\\nwrdr.sys",&status);
	fver _pci_sys      = getFileVer(Drivers+L"\\pci.sys",&status);
	fver _powerfil_sys = getFileVer(Drivers+L"\\powerfil.sys",&status);
	fver _pxhelp20_sys = getFileVer(Drivers+L"\\pxhelp20.sys",&status);
	fver _raspptp_sys  = getFileVer(Drivers+L"\\raspptp.sys",&status);
	fver _rmcast_sys   = getFileVer(Drivers+L"\\rmcast.sys",&status);
	fver _rdbss_sys    = getFileVer(Drivers+L"\\rdbss.sys",&status);
	fver _rdpwd_sys    = getFileVer(Drivers+L"\\rdpwd.sys",&status);
	fver _sbp2port_sys = getFileVer(Drivers+L"\\sbp2port.sys",&status);
	fver _secdrv_sys   = getFileVer(Drivers+L"\\secdrv.sys",&status);
	fver _splitter_sys = getFileVer(Drivers+L"\\splitter.sys",&status);
	fver _srv_sys      = getFileVer(Drivers+L"\\srv.sys",&status);
	fver _tcpip6_sys   = getFileVer(Drivers+L"\\tcpip6.sys",&status);
	fver _tcpip_sys    = getFileVer(Drivers+L"\\tcpip.sys",&status);
	fver _termdd_sys   = getFileVer(Drivers+L"\\termdd.sys",&status);
	fver _tunmp_sys    = getFileVer(Drivers+L"\\tunmp.sys",&status);
	fver _update_sys   = getFileVer(Drivers+L"\\update.sys",&status);
	fver _udfs_sys     = getFileVer(Drivers+L"\\udfs.sys",&status);
	fver _usb8023_sys  = getFileVer(Drivers+L"\\usb8023.sys",&status);
	fver _usb8023x_sys = getFileVer(Drivers+L"\\usb8023x.sys",&status);
	fver _usbaudio_sys = getFileVer(Drivers+L"\\usbaudio.sys",&status);
	fver _usbccgp_sys  = getFileVer(Drivers+L"\\usbccgp.sys",&status);
	fver _usbd_sys     = getFileVer(Drivers+L"\\usbd.sys",&status);
	fver _usbehci_sys  = getFileVer(Drivers+L"\\usbehci.sys",&status);
	fver _usbhub_sys   = getFileVer(Drivers+L"\\usbhub.sys",&status);
	fver _usbohci_sys  = getFileVer(Drivers+L"\\usbohci.sys",&status);
	fver _usbport_sys  = getFileVer(Drivers+L"\\usbport.sys",&status);
	fver _usbstor_sys  = getFileVer(Drivers+L"\\usbstor.sys",&status);
	fver _usbscan_sys  = getFileVer(Drivers+L"\\usbscan.sys",&status);
	fver _usbuhci_sys  = getFileVer(Drivers+L"\\usbuhci.sys",&status);
	fver _usbvideo_sys = getFileVer(Drivers+L"\\usbvideo.sys",&status);
	fver _wdmaud_sys   = getFileVer(Drivers+L"\\wdmaud.sys",&status);
	fver _wpdusb_sys   = getFileVer(Drivers+L"\\wpdusb.sys",&status);

//	fver _dmusic_sys_cache   = getFileVer(DriverCache+L"\\dmusic.sys",&status);
	fver _hidir_sys_cache    = getFileVer(DriverCache+L"\\hidir.sys",&status);
	fver _irbus_sys_cache    = getFileVer(DriverCache+L"\\irbus.sys",&status);
	fver _powerfil_sys_cache = getFileVer(DriverCache+L"\\powerfil.sys",&status);
	fver _sbp2port_sys_cache = getFileVer(DriverCache+L"\\sbp2port.sys",&status);

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

	fver _asp_dll      = getFileVer(inetsrv+L"\\asp.dll",&status);
	fver _asp51_dll_dllcache    = getFileVer(dllcache+L"\\asp51.dll",&status);
	fver _ftpsvc2_dll  = getFileVer(inetsrv+L"\\ftpsvc2.dll",&status);
	fver _httpext_dll  = getFileVer(inetsrv+L"\\httpext.dll",&status);
	fver _httpodbc_dll = getFileVer(inetsrv+L"\\httpodbc.dll",&status);
	fver _iischema_dll = getFileVer(inetsrv+L"\\iischema.dll",&status);
	fver _infocomm_dll = getFileVer(inetsrv+L"\\infocomm.dll",&status);
	fver _isatq_dll    = getFileVer(inetsrv+L"\\isatq.dll",&status);
	fver _lonsint_dll  = getFileVer(inetsrv+L"\\lonsint.dll",&status);
	fver _smtpsvc_dll  = getFileVer(inetsrv+L"\\smtpsvc.dll",&status);
	fver _ssinc_dll    = getFileVer(inetsrv+L"\\ssinc.dll",&status);
	fver _w3svc_dll    = getFileVer(inetsrv+L"\\w3svc.dll",&status);

	fver _iecompat_dll = getFileVer(InternetExplorer+L"\\iecompat.dll",&status);
	fver _iedvtool_dll = getFileVer(InternetExplorer+L"\\iedvtool.dll",&status);
	fver _iedw_exe     = getFileVer(InternetExplorer+L"\\iedw.exe",&status);
	fver _ieproxy_dll  = getFileVer(InternetExplorer+L"\\ieproxy.dll",&status);
	fver _iexplore_exe = getFileVer(InternetExplorer+L"\\iexplore.exe",&status);
	fver _jsdbgui_dll  = getFileVer(InternetExplorer+L"\\jsdbgui.dll",&status);
	fver _jsprofilerui_dll = getFileVer(InternetExplorer+L"\\jsprofilerui.dll",&status);
	fver _xpshims_dll  = getFileVer(InternetExplorer+L"\\xpshims.dll",&status);

	fver _npdsplay_dll = getFileVer(WindowsMediaPlayer+L"\\npdsplay.dll",&status);
	fver _setup_wm_exe = getFileVer(WindowsMediaPlayer+L"\\setup_wm.exe",&status);
	fver _wmplayer_exe = getFileVer(WindowsMediaPlayer+L"\\wmplayer.exe",&status);
	fver _wmsetsdk_exe = getFileVer(WindowsMediaPlayer+L"\\wmsetsdk.exe",&status);

	fver _vgx_dll      = getFileVer(VGX+L"\\vgx.dll",&status);

	fver _custsat_dll   = getFileVer(NetworkDiagnostic+L"\\custsat.dll",&status);
	fver _xpnetdiag_exe = getFileVer(NetworkDiagnostic+L"\\xpnetdiag.exe",&status);

	fver _migload_exe  = getFileVer(usmt+L"\\migload.exe",&status);
	fver _migwiz_exe   = getFileVer(usmt+L"\\migwiz.exe",&status);
	fver _migwiza_exe  = getFileVer(usmt+L"\\migwiza.exe",&status);
	fver _migwiz_a_exe = getFileVer(usmt+L"\\migwiz_a.exe",&status);
	fver _cobramsg_dll = getFileVer(usmt+L"\\cobramsg.dll",&status);
	fver _guitrn_dll   = getFileVer(usmt+L"\\guitrn.dll",&status);
	fver _guitrna_dll  = getFileVer(usmt+L"\\guitrna.dll",&status);
	fver _guitrn_a_dll = getFileVer(usmt+L"\\guitrn_a.dll",&status);
	fver _iconlib_dll  = getFileVer(usmt+L"\\iconlib.dll",&status);
	fver _log_dll      = getFileVer(usmt+L"\\log.dll",&status);
	fver _migism_dll   = getFileVer(usmt+L"\\migism.dll",&status);
	fver _migisma_dll  = getFileVer(usmt+L"\\migisma.dll",&status);
	fver _script_dll   = getFileVer(usmt+L"\\script.dll",&status);
	fver _scripta_dll  = getFileVer(usmt+L"\\scripta.dll",&status);
	fver _script_a_dll = getFileVer(usmt+L"\\script_a.dll",&status);
	fver _sysmod_dll   = getFileVer(usmt+L"\\sysmod.dll",&status);
	fver _sysmoda_dll  = getFileVer(usmt+L"\\sysmoda.dll",&status);
	fver _sysmod_a_dll = getFileVer(usmt+L"\\sysmod_a.dll",&status);
	fver _unctrn_dll   = getFileVer(usmt+L"\\unctrn.dll",&status);
	fver _unctrn_a_dll = getFileVer(usmt+L"\\unctrn_a.dll",&status);

	fver _msgsc_dll    = getFileVer(Messenger+L"\\msgsc.dll",&status);
	fver _msgslang_dll = getFileVer(Messenger+L"\\msgslang.dll",&status);
	fver _msmsgs_exe   = getFileVer(Messenger+L"\\msmsgs.exe",&status);
	fver _msmsgsin_exe = getFileVer(Messenger+L"\\msmsgsin.exe",&status);
	fver _rtcimsp_dll  = getFileVer(Messenger+L"\\rtcimsp.dll",&status);

	fver _callcont_dll = getFileVer(NetMeeting+L"\\callcont.dll",&status);
	fver _mst120_dll   = getFileVer(NetMeeting+L"\\mst120.dll",&status);
	fver _nmcom_dll    = getFileVer(NetMeeting+L"\\nmcom.dll",&status);

	fver _msimn_exe   = getFileVer(OutlookExpress+L"\\msimn.exe",&status);
	fver _msoe_dll    = getFileVer(OutlookExpress+L"\\msoe.dll",&status);
	fver _msoeres_dll = getFileVer(OutlookExpress+L"\\msoeres.dll",&status);
	fver _oeimport_dll= getFileVer(OutlookExpress+L"\\oeimport.dll",&status);
	fver _oemig50_exe = getFileVer(OutlookExpress+L"\\oemig50.exe",&status);
	fver _oemiglib_dll= getFileVer(OutlookExpress+L"\\oemiglib.dll",&status);
	fver _wab_exe     = getFileVer(OutlookExpress+L"\\wab.exe",&status);
	fver _wabfind_dll = getFileVer(OutlookExpress+L"\\wabfind.dll",&status);
	fver _wabimp_dll  = getFileVer(OutlookExpress+L"\\wabimp.dll",&status);
	fver _wabmig_exe  = getFileVer(OutlookExpress+L"\\wabmig.exe",&status);

	fver _dhtmled_ocx  = getFileVer(Triedit+L"\\dhtmled.ocx",&status);
	fver _triedit_dll  = getFileVer(Triedit+L"\\triedit.dll",&status);
	fver _wordpad_exe  = getFileVer(Accessories+L"\\wordpad.exe",&status);
	fver _mswrd6_wpc = getFileVer(Accessories+L"\\mswrd6.wpc",&status);
	fver _mswrd8_wpc = getFileVer(Accessories+L"\\mswrd8.wpc",&status);
	fver _write_wpc = getFileVer(Accessories+L"\\write.wpc",&status);
	fver _msconv97_dll = getFileVer(TextConv+L"\\msconv97.dll",&status);
	fver _html32_cnv   = getFileVer(TextConv+L"\\html32.cnv",&status);
	fver _mswrd632_wpc   = getFileVer(TextConv+L"\\mswrd632.wpc",&status);
	fver _mswrd832_cnv   = getFileVer(TextConv+L"\\mswrd832.cnv",&status);
	fver _write32_wpc   = getFileVer(TextConv+L"\\write32.wpc",&status);
	fver _flash_ocx = getFileVer(Flash+L"\\flash.ocx",&status);
	fver _flash6_ocx = getFileVer(Flash+L"\\Flash6.ocx",&status);
	fver _swflash_ocx = getFileVer(Flash+L"\\swflash.ocx",&status);
	fver _moviemk_exe = getFileVer(MovieMaker+L"\\moviemk.exe",&status);

	fver _helpctr_exe  = getFileVer(helpctr_binaries+L"\\helpctr.exe",&status);
	fver _helphost_exe = getFileVer(helpctr_binaries+L"\\helphost.exe",&status);
	fver _helpsvc_exe  = getFileVer(helpctr_binaries+L"\\helpsvc.exe",&status);
	fver _HscUpd_exe   = getFileVer(helpctr_binaries+L"\\HscUpd.exe",&status);
	fver _msconfig_exe = getFileVer(helpctr_binaries+L"\\msconfig.exe",&status);
	fver _pchshell_dll = getFileVer(helpctr_binaries+L"\\pchshell.dll",&status);
	fver _pchsvc_dll   = getFileVer(helpctr_binaries+L"\\pchsvc.dll",&status);
	fver _srdiag_exe   = getFileVer(Restore+L"\\srdiag.exe",&status);
	fver _uploadm_exe  = getFileVer(UploadLB+L"\\uploadm.exe",&status);

	fver _directdb_dll = getFileVer(CommonFilesSystem+L"\\directdb.dll",&status);
	fver _wab32_dll    = getFileVer(CommonFilesSystem+L"\\wab32.dll",&status);

	fver _msadce_dll = getFileVer(msadc+L"\\msadce.dll",&status);
	fver _msadco_dll = getFileVer(msadc+L"\\msadco.dll",&status);
	fver _msado15_dll = getFileVer(ado+L"\\msado15.dll",&status);
	fver _msadomd_dll = getFileVer(ado+L"\\msadomd.dll",&status);
	fver _msador15_dll = getFileVer(ado+L"\\msador15.dll",&status);
	fver _msadox_dll = getFileVer(ado+L"\\msadox.dll",&status);
	fver _msjro_dll = getFileVer(ado+L"\\msjro.dll",&status);
	fver _msado20_tlb = getFileVer(ado+L"\\msado20.tlb",&status);
	fver _msado21_tlb = getFileVer(ado+L"\\msado21.tlb",&status);
	fver _msado25_tlb = getFileVer(ado+L"\\msado25.tlb",&status);
	fver _msado26_tlb = getFileVer(ado+L"\\msado26.tlb",&status);
	fver _msado27_tlb = getFileVer(ado+L"\\msado27.tlb",&status);
	fver _msado28_tlb = getFileVer(ado+L"\\msado28.tlb",&status);
	fver _msdaora_dll = getFileVer(OleDB+L"\\msdaora.dll",&status);
	fver _oledb32_dll = getFileVer(OleDB+L"\\oledb32.dll",&status);
	fver _sqloledb_dll = getFileVer(OleDB+L"\\sqloledb.dll",&status);

	fver _CmdEvTgProv_dll = getFileVer(wbem+L"\\CmdEvTgProv.dll",&status);
	fver _fastprox_dll = getFileVer(wbem+L"\\fastprox.dll",&status);
	fver _wbemess_dll  = getFileVer(wbem+L"\\wbemess.dll",&status);
	fver _wmiprvsd_dll = getFileVer(wbem+L"\\wmiprvsd.dll",&status);
	fver _wmiprvse_exe = getFileVer(wbem+L"\\wmiprvse.exe",&status);

	fver _admin_exe_dllcache    = getFileVer(dllcache+L"\\admin.exe",&status);
	fver _author_dll_dllcache   = getFileVer(dllcache+L"\\author.dll",&status);
	fver _author_exe_dllcache   = getFileVer(dllcache+L"\\author.exe",&status);
	fver _cfgwiz_exe_dllcache   = getFileVer(dllcache+L"\\cfgwiz.exe",&status);
	fver _fp4amsft_dll_dllcache = getFileVer(dllcache+L"\\fp4amsft.dll",&status);
	fver _fp4anscp_dll_dllcache = getFileVer(dllcache+L"\\fp4anscp.dll",&status);
	fver _fp4apws_dll_dllcache  = getFileVer(dllcache+L"\\fp4apws.dll",&status);
	fver _fp4areg_dll_dllcache  = getFileVer(dllcache+L"\\fp4areg.dll",&status);
	fver _fp4atxt_dll_dllcache  = getFileVer(dllcache+L"\\fp4atxt.dll",&status);
	fver _fp4autl_dll_dllcache  = getFileVer(dllcache+L"\\fp4autl.dll",&status);
	fver _fp4avnb_dll_dllcache  = getFileVer(dllcache+L"\\fp4avnb.dll",&status);
	fver _fp4avss_dll_dllcache  = getFileVer(dllcache+L"\\fp4avss.dll",&status);
	fver _fp4awebs_dll_dllcache = getFileVer(dllcache+L"\\fp4awebs.dll",&status);
	fver _fp4awel_dll_dllcache  = getFileVer(dllcache+L"\\fp4awel.dll",&status);
	fver _fpadmcgi_exe_dllcache = getFileVer(dllcache+L"\\fpadmcgi.exe",&status);
	fver _fpadmdll_dll_dllcache = getFileVer(dllcache+L"\\fpadmdll.dll",&status);
	fver _fpcount_exe_dllcache  = getFileVer(dllcache+L"\\fpcount.exe",&status);
	fver _fpexedll_dll_dllcache = getFileVer(dllcache+L"\\fpexedll.dll",&status);
	fver _fpmmc_dll_dllcache    = getFileVer(dllcache+L"\\fpmmc.dll",&status);
	fver _fpremadm_exe_dllcache = getFileVer(dllcache+L"\\fpremadm.exe",&status);
	fver _shtml_dll_dllcache    = getFileVer(dllcache+L"\\shtml.dll",&status);
	fver _shtml_exe_dllcache    = getFileVer(dllcache+L"\\shtml.exe",&status);

	// FrontPage Server Extensions
	std::wstring isapi_vtiadm = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\isapi\\_vti_adm";
	std::wstring vtibin_vtiadm = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\_vti_bin\\_vti_adm";
	std::wstring isapi_vtiaut = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\isapi\\_vti_aut";
	std::wstring vtibin_vtiaut = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\_vti_bin\\_vti_aut";
	std::wstring wse_bin = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\bin";
	std::wstring srvsupp = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\servsupp";
	std::wstring vinavbar = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\bots\\vinavbar";
	std::wstring v30bin = ProgramFiles+L"\\microsoft frontpage\\version3.0\\bin";
	std::wstring admcgi_scripts = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\admcgi\\scripts";
	std::wstring admisapi_scripts = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\admisapi\\scripts";
	std::wstring vtibin = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\_vti_bin";
	std::wstring isapi = ProgramFiles+L"\\Common Files\\Microsoft Shared\\web server extensions\\40\\isapi";

	fver _admin_dll = getFileVer(isapi_vtiadm+L"\\admin.dll",&status);
	fver _admin_exe = getFileVer(vtibin_vtiadm+L"\\admin.exe",&status);
	fver _author_dll = getFileVer(isapi_vtiaut+L"\\author.dll",&status);
	fver _author_exe = getFileVer(vtibin_vtiaut+L"\\author.exe",&status);
	fver _cfgwiz_exe = getFileVer(wse_bin+L"\\cfgwiz.exe",&status);
	fver _fp4areg_dll = getFileVer(wse_bin+L"\\fp4areg.dll",&status);
	fver _fp4atxt_dll = getFileVer(wse_bin+L"\\fp4atxt.dll",&status);
	fver _fp4autl_dll = getFileVer(wse_bin+L"\\fp4autl.dll",&status);
	fver _fp4avss_dll = getFileVer(wse_bin+L"\\fp4avss.dll",&status);
	fver _fp4awel_dll = getFileVer(wse_bin+L"\\fp4awel.dll",&status);
	fver _fpencode_dll = getFileVer(wse_bin+L"\\fpencode.dll",&status);
	fver _fpexedll_dll = getFileVer(wse_bin+L"\\fpexedll.dll",&status);
	fver _fpmmc_dll = getFileVer(wse_bin+L"\\fpmmc.dll",&status);
	fver _fpremadm_exe = getFileVer(wse_bin+L"\\fpremadm.exe",&status);
	fver _fpsrvadm_exe = getFileVer(wse_bin+L"\\fpsrvadm.exe",&status);
	fver _tcptest_exe = getFileVer(wse_bin+L"\\tcptest.exe",&status);
	fver _fp4amsft_dll = getFileVer(srvsupp+L"\\fp4amsft.dll",&status);
	fver _fp4anscp_dll = getFileVer(srvsupp+L"\\fp4anscp.dll",&status);
	fver _fp4apws_dll = getFileVer(srvsupp+L"\\fp4apws.dll",&status);
	fver _fp4awebs_dll = getFileVer(srvsupp+L"\\fp4awebs.dll",&status);
	fver _fp4avnb_dll = getFileVer(vinavbar+L"\\fp4avnb.dll",&status);
	fver _fp98sadm_exe = getFileVer(v30bin+L"\\fp98sadm.exe",&status);
	fver _fp98swin_exe = getFileVer(v30bin+L"\\fp98swin.exe",&status);
	fver _stub_fpsrvadm_exe = getFileVer(v30bin+L"\\fpsrvadm.exe",&status);
	fver _stub_fpsrvwin_exe = getFileVer(v30bin+L"\\fpsrvwin.exe",&status);
	fver _fpadmcgi_exe = getFileVer(admcgi_scripts+L"\\fpadmcgi.exe",&status);
	fver _fpadmdll_dll = getFileVer(admisapi_scripts+L"\\fpadmdll.dll",&status);
	fver _fpcount_exe1 = getFileVer(vtibin+L"\\fpcount.exe",&status);
	fver _fpcount_exe2 = getFileVer(isapi+L"\\fpcount.exe",&status);
	fver _shtml_dll = getFileVer(isapi+L"\\shtml.dll",&status);
	fver _shtml_exe = getFileVer(vtibin+L"\\shtml.exe",&status);

	fver DirectXVersion = fver(regQueryValue(L"SOFTWARE\\Microsoft\\DirectX",L"Version",&status).c_str());

	fver _agcore_dll = getFileVer(MicrosoftSilverlight+L"\\agcore.dll",&status);
	fver _slup_exe = getFileVer(MicrosoftSilverlight+L"\\slup.exe",&status);
	fver _sllauncher_exe = getFileVer(MicrosoftSilverlight+L"\\sllauncher.exe",&status);

	fver _agcore_debug_dll_2_0_31005 = getFileVer(MicrosoftSilverlight+L"\\2.0.31005.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_2_0_40115 = getFileVer(MicrosoftSilverlight+L"\\2.0.40115.0\\agcore.debug.dll",&status);

	fver _agcore_dll_2_0_31005 = getFileVer(MicrosoftSilverlight+L"\\2.0.31005.0\\agcore.dll",&status);
	fver _agcore_dll_2_0_40115 = getFileVer(MicrosoftSilverlight+L"\\2.0.40115.0\\agcore.dll",&status);
	fver _agcore_dll_3_0_40307 = getFileVer(MicrosoftSilverlight+L"\\3.0.40307.0\\agcore.dll",&status);

	fver _agcore_debug_dll_3_0_40307 = getFileVer(MicrosoftSilverlight+L"\\3.0.40307.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_3_0_40624 = getFileVer(MicrosoftSilverlight+L"\\3.0.40624.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_3_0_40723 = getFileVer(MicrosoftSilverlight+L"\\3.0.40723.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_3_0_40818 = getFileVer(MicrosoftSilverlight+L"\\3.0.40818.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_3_0_50106 = getFileVer(MicrosoftSilverlight+L"\\3.0.50106.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_3_0_50611 = getFileVer(MicrosoftSilverlight+L"\\3.0.50611.0\\agcore.debug.dll",&status);

	fver _agcore_debug_dll_4_0_50401 = getFileVer(MicrosoftSilverlight+L"\\4.0.50401.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_50524 = getFileVer(MicrosoftSilverlight+L"\\4.0.50524.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_50826 = getFileVer(MicrosoftSilverlight+L"\\4.0.50826.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_50917 = getFileVer(MicrosoftSilverlight+L"\\4.0.50917.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_51204 = getFileVer(MicrosoftSilverlight+L"\\4.0.51204.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_60129 = getFileVer(MicrosoftSilverlight+L"\\4.0.60129.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_60310 = getFileVer(MicrosoftSilverlight+L"\\4.0.60310.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_60531 = getFileVer(MicrosoftSilverlight+L"\\4.0.60531.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_0_60831 = getFileVer(MicrosoftSilverlight+L"\\4.0.60831.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_1_10111 = getFileVer(MicrosoftSilverlight+L"\\4.1.10111.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_4_1_10329 = getFileVer(MicrosoftSilverlight+L"\\4.1.10329.0\\agcore.debug.dll",&status);

	fver _agcore_debug_dll_5_0_60401 = getFileVer(MicrosoftSilverlight+L"\\5.0.60401.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_0_60818 = getFileVer(MicrosoftSilverlight+L"\\5.0.60818.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_0_61118 = getFileVer(MicrosoftSilverlight+L"\\5.0.61118.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_10411 = getFileVer(MicrosoftSilverlight+L"\\5.1.10411.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_20125 = getFileVer(MicrosoftSilverlight+L"\\5.1.20125.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_20513 = getFileVer(MicrosoftSilverlight+L"\\5.1.20513.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_20913 = getFileVer(MicrosoftSilverlight+L"\\5.1.20913.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_30214 = getFileVer(MicrosoftSilverlight+L"\\5.1.30214.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_30514 = getFileVer(MicrosoftSilverlight+L"\\5.1.30514.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_40416 = getFileVer(MicrosoftSilverlight+L"\\5.1.40416.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_40728 = getFileVer(MicrosoftSilverlight+L"\\5.1.40728.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_41105 = getFileVer(MicrosoftSilverlight+L"\\5.1.41105.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_41212 = getFileVer(MicrosoftSilverlight+L"\\5.1.41212.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50428 = getFileVer(MicrosoftSilverlight+L"\\5.1.50428.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50709 = getFileVer(MicrosoftSilverlight+L"\\5.1.50709.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50901 = getFileVer(MicrosoftSilverlight+L"\\5.1.50901.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50905 = getFileVer(MicrosoftSilverlight+L"\\5.1.50905.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50906 = getFileVer(MicrosoftSilverlight+L"\\5.1.50906.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50907 = getFileVer(MicrosoftSilverlight+L"\\5.1.50907.0\\agcore.debug.dll",&status);
	fver _agcore_debug_dll_5_1_50918 = getFileVer(MicrosoftSilverlight+L"\\5.1.50918.0\\agcore.debug.dll",&status);

	if( _agcore_dll_2_0_31005 > _agcore_dll ) { _agcore_dll = _agcore_dll_2_0_31005; }
	if( _agcore_dll_2_0_40115 > _agcore_dll ) { _agcore_dll = _agcore_dll_2_0_40115; }
	if( _agcore_dll_3_0_40307 > _agcore_dll ) { _agcore_dll = _agcore_dll_3_0_40307; }

	fver _agcore_debug_dll = zero;

	if( _agcore_debug_dll_2_0_31005 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_2_0_31005; }
	if( _agcore_debug_dll_2_0_40115 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_2_0_40115; }

	if( _agcore_debug_dll_3_0_40624 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_3_0_40624; }
	if( _agcore_debug_dll_3_0_40723 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_3_0_40723; }
	if( _agcore_debug_dll_3_0_40818 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_3_0_40818; }
	if( _agcore_debug_dll_3_0_50106 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_3_0_50106; }
	if( _agcore_debug_dll_3_0_50611 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_3_0_50611; }

	if( _agcore_debug_dll_4_0_50401 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_50401; }
	if( _agcore_debug_dll_4_0_50524 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_50524; }
	if( _agcore_debug_dll_4_0_50826 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_50826; }
	if( _agcore_debug_dll_4_0_50917 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_50917; }
	if( _agcore_debug_dll_4_0_51204 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_51204; }
	if( _agcore_debug_dll_4_0_60129 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_60129; }
	if( _agcore_debug_dll_4_0_60310 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_60310; }
	if( _agcore_debug_dll_4_0_60531 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_60531; }
	if( _agcore_debug_dll_4_0_60831 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_0_60831; }
	if( _agcore_debug_dll_4_1_10111 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_1_10111; }
	if( _agcore_debug_dll_4_1_10329 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_4_1_10329; }

	if( _agcore_debug_dll_5_0_60401 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_0_60401; }
	if( _agcore_debug_dll_5_0_60818 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_0_60818; }
	if( _agcore_debug_dll_5_0_61118 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_0_61118; }
	if( _agcore_debug_dll_5_1_10411 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_10411; }
	if( _agcore_debug_dll_5_1_20125 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_20125; }
	if( _agcore_debug_dll_5_1_20513 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_20513; }
	if( _agcore_debug_dll_5_1_20913 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_20913; }
	if( _agcore_debug_dll_5_1_30214 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_30214; }
	if( _agcore_debug_dll_5_1_30514 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_30514; }
	if( _agcore_debug_dll_5_1_40416 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_40416; }
	if( _agcore_debug_dll_5_1_40728 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_40728; }
	if( _agcore_debug_dll_5_1_41105 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_41105; }
	if( _agcore_debug_dll_5_1_41212 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_41212; }
	if( _agcore_debug_dll_5_1_50428 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50428; }
	if( _agcore_debug_dll_5_1_50709 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50709; }
	if( _agcore_debug_dll_5_1_50901 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50901; }
	if( _agcore_debug_dll_5_1_50905 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50905; }
	if( _agcore_debug_dll_5_1_50906 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50906; }
	if( _agcore_debug_dll_5_1_50907 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50907; }
	if( _agcore_debug_dll_5_1_50918 > _agcore_debug_dll ) { _agcore_debug_dll = _agcore_debug_dll_5_1_50918; }
 
	fver _jntview_exe            = getFileVer(WindowsJournalViewer+L"\\jntview.exe",&status);
	fver _jntfiltr_dll           = getFileVer(WindowsJournal+L"\\jntfiltr.dll",&status);
	fver _jnwdrv_dll             = getFileVer(WindowsJournal+L"\\jnwdrv.dll",&status);
	fver _nbdoc_dll              = getFileVer(WindowsJournal+L"\\nbdoc.dll",&status);

	fver _spcustom_dll_ref = getFileVer(wxp_x86_0409_v1+L"\\spcustom.dll.ref",&status);
	fver _spmsg_dll_ref    = getFileVer(wxp_x86_0409_v1+L"\\spmsg.dll.ref",&status);
	fver _spuninst_exe_ref = getFileVer(wxp_x86_0409_v1+L"\\spuninst.exe.ref",&status);
	fver _spupdsvc_exe_ref = getFileVer(wxp_x86_0409_v1+L"\\spupdsvc.exe.ref",&status);
	fver _update_exe_ref   = getFileVer(wxp_x86_0409_v1+L"\\update.exe.ref",&status);
	fver _updspapi_dll_ref = getFileVer(wxp_x86_0409_v1+L"\\updspapi.dll.ref",&status);

	fver _BitLockerToGo_exe = getFileVer(SystemRoot+L"\\BitLockerDiscoveryVolumeContents\\BitLockerToGo.exe",&status);

	fver _agentdp2_dll = getFileVer(msagent+L"\\agentdp2.dll",&status);
	fver _agentdpv_dll = getFileVer(msagent+L"\\agentdpv.dll",&status);
	fver _agentsvr_exe = getFileVer(msagent+L"\\agentsvr.exe",&status);

	fver _gdiplus_dll_0     = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.0.1_x-ww_8d353f14\\GdiPlus.dll",&status);
	fver _gdiplus_dll_1360  = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.2600.1360_x-ww_24a2ed47\\GdiPlus.dll",&status);
	fver _gdiplus_dll_22319 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6001.22319_x-ww_f0b4c2df\\GdiPlus.dll",&status);
	fver _gdiplus_dll_22791 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.22791_x-ww_c8dff154\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23084 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23084_x-ww_f3f35550\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23386 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23386_x-ww_f56d83d5\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23535 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23535_x-ww_f664a19b\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23588 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23588_x-ww_f669a2d9\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23609 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23609_x-ww_f6dfb063\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23693 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23693_x-ww_f6e8b294\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23746 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23746_x-ww_f761c0dd\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23930 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23930_x-ww_f85cdf9a\\GdiPlus.dll",&status);
	fver _gdiplus_dll_23984 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.23984_x-ww_f861e0d9\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24064 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24064_x-ww_22782591\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24072 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24072_x-ww_227925ce\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24180 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24180_x-ww_22f8358c\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24439 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24439_x-ww_246d62dd\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24465 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24465_x-ww_24706396\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24489 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24489_x-ww_24726418\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24533 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24533_x-ww_24eb7258\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24561 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24561_x-ww_24ee7313\\GdiPlus.dll",&status);
	fver _gdiplus_dll_24563 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6002.24563_x-ww_24ee7315\\GdiPlus.dll",&status);
	fver _gdiplus_dll_20488 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.GdiPlus_6595b64144ccf1df_1.0.6003.20488_x-ww_40820f9c\\GdiPlus.dll",&status);

	fver _comctl32_dll_1342 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1331_x-ww_7abf6d02\\comctl32.dll",&status);
	fver _comctl32_dll_1515 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1515_x-ww_7bb98b8a\\comctl32.dll",&status);
	fver _comctl32_dll_1579 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1579_x-ww_7bbf8d08\\comctl32.dll",&status);
	fver _comctl32_dll_1740 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1740_x-ww_7cb8ab44\\comctl32.dll",&status);
	fver _comctl32_dll_1816 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1816_x-ww_7d33ba0e\\comctl32.dll",&status);
	fver _comctl32_dll_1873 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1873_x-ww_7d39bb85\\comctl32.dll",&status);
	fver _comctl32_dll_1891 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.1891_x-ww_7d3bbc01\\comctl32.dll",&status);
	fver _comctl32_dll_2982 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.2982_x-ww_ac3f9c03\\comctl32.dll",&status);
	fver _comctl32_dll_7660 = getFileVer(WinSxS+L"\\x86_Microsoft.Windows.Common-Controls_6595b64144ccf1df_6.0.2600.7660_x-ww_936580bb\\comctl32.dll",&status);

	fver _atl_dll_9792     = getFileVer(WinSxS+L"\\x86_Microsoft.Tools.VisualCPlusPlus.Runtime-Libraries_6595b64144ccf1df_6.0.9792.0_x-ww_08a6620a\\atl.dll",&status);
	fver _mfc42_dll_9792   = getFileVer(WinSxS+L"\\x86_Microsoft.Tools.VisualCPlusPlus.Runtime-Libraries_6595b64144ccf1df_6.0.9792.0_x-ww_08a6620a\\mfc42.dll",&status);
	fver _mfc42u_dll_9792  = getFileVer(WinSxS+L"\\x86_Microsoft.Tools.VisualCPlusPlus.Runtime-Libraries_6595b64144ccf1df_6.0.9792.0_x-ww_08a6620a\\mfc42u.dll",&status);
	fver _msvcp60_dll_9792 = getFileVer(WinSxS+L"\\x86_Microsoft.Tools.VisualCPlusPlus.Runtime-Libraries_6595b64144ccf1df_6.0.9792.0_x-ww_08a6620a\\msvcp60.dll",&status);


	// Windows XP RTM Updates
	if((sku & XP_ALL) && (  _qfecheck_exe <fver(5,0,2195,4605) )) {
		NN("Qfecheck.exe Update Check Utility (Q282784)");
		XX("Extras\\Q282784_WXP_SP1_x86_ENU.exe"+a7);
	}
	if( sp>=1 && (sku & XP_ALL) && (  _spcustom_dll_ref <fver(6,1,22,4)
		              ||    _spmsg_dll_ref    <fver(6,1,22,4)
					  ||    _spuninst_exe_ref <fver(6,1,22,4)
					  ||    _spupdsvc_exe_ref <fver(6,1,22,4)
					  ||    _update_exe_ref   <fver(6,1,22,4)
					  ||    _updspapi_dll_ref <fver(6,1,22,4) )) {
		NN("Update for Windows XP (KB898461)");
		XX(p3+"WindowsXP-KB898461-x86-ENU.exe"+a1);
	}
	if( sp<2 && (sku & XP_ALL) && (
	      /*(_bitsinst_exe >zero && _bitsinst_exe <fver(6,6,2600,1580))
	   ||*/ (_bitsprx2_dll >zero && _bitsprx2_dll <fver(6,6,2600,1569))
	   || (_bitsprx3_dll >zero && _bitsprx3_dll <fver(6,6,2600,1569))
	   || (_qmgr_dll     >zero && _qmgr_dll     <fver(6,6,2600,1569))
	   || (_qmgrprxy_dll >zero && _qmgrprxy_dll <fver(6,6,2600,1569))
	   || (_winhttp_dll  >zero && _winhttp_dll  <fver(5,1,2600,1557))
	   || (_xpob2res_dll >zero && _xpob2res_dll <fver(5,1,2600,1570)) )) {
		NN("Update for Background Intelligent Transfer Service (BITS) 2.0 and WinHTTP 5.1 (KB842773)");
		XX(p1+"windowsxp-kb842773-v2-x86-enu_1d3d2bc417bf9d881206ed238ad6a4a9c189cfb3.exe"+a6);
	}
	/*
	// Hotfix KB883357 fixes a flaw with how KB842773 v1 writes a REG_MULTI_SZ value
	// to the registry.  The value should be terminated with two NULL wchar_t
	// values (0x0000).  The faulty KB883357 patch wrote one null (0x0000) and one
	// unicode character (0xeeef).  KB842773v2 fixed the issue, which means that
	// KB883357 is not explicitly needed.
	wchar_t* bits = (wchar_t*)regQueryBinaryData(L"SYSTEM\\CurrentControlSet\\Control\\BackupRestore\\FilesNotToBackup",L"BITS_metadata",&status);
	for(int i=0; i<(status/sizeof(wchar_t)); i++) {
			printf("%04x ",bits[i]);
	} printf("\n");
	if( sp<1 && (sku & XP_ALL) && 1==0) {
		NN("Update for Background Intelligent Transfer Service (BITS) (KB883357)");
		XX(p1+"windowsxp-kb883357-x86-enu_f4bd524df7dbf1db8bed916d4a4f63b0f29f777c.exe"+a6);
	}*/
	if( sp==0 && (sku & XP_ALL) && _itircl_dll>zero && _itircl_dll<fver(5,2,3790,80)) {
		NN("Security Update for Microsoft Windows XP (KB825119)");
		XX(rtm+"windowsxp-kb825119-x86-enu_1b9f23b64b002d1e9d1eaba62f5f8fd.exe"+a7);
	}
	/*if((sp==0 && (sku & XP_ALL) && (
			    (_ole32_dll  >zero && _ole32_dll  <fver(5,1,2600,118))
			||  (_rpcrt4_dll >zero && _rpcrt4_dll <fver(5,1,2600,109))
			||  (_rpcss_dll  >zero && _rpcss_dll  <fver(5,1,2600,118))))
	 ||(sp==1 && (sku & XP_ALL) && (
			    (_ole32_dll  >zero && _ole32_dll  <fver(5,1,2600,1263))
			||  (_rpcrt4_dll >zero && _rpcrt4_dll <fver(5,1,2600,1254))
			||  (_rpcss_dll  >zero && _rpcss_dll  <fver(5,1,2600,1263))))) {
		// KB824146 is replaced by KB826939
		NN("Security Update for Windows XP (KB824146)");
		XX(p1+"WindowsXP-KB824146-x86-ENU.exe"+a7);
	}*/
	if( sp==0 && (sku & XP_ALL) && (
			    (_ole32_dll  >zero && _ole32_dll  <fver(5,1,2600,115))
			||  (_rpcrt4_dll >zero && _rpcrt4_dll <fver(5,1,2600,109))
			||  (_rpcss_dll  >zero && _rpcss_dll  <fver(5,1,2600,115)) )) {
		NN("MS03-026: Security Update for Windows XP (823980)");
		XX(rtm+"WindowsXP-KB823980-x86-ENU.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _msxml2_dll>zero && _msxml2_dll<fver(8,2,8307,0)) {
		NN("Security Update, February 13, 2002 (MSXML 2.6)");
		//XX(rtm+"q318202_a37106c874860f0782d1eadd9fa9c41cd65f1307.exe"+a2);
		XX(sw+rtm+"q318202_a37106c874860f0782d1eadd9fa9c41cd65f1307.exe /Q:A /R:N /C:\"dahotfix.exe /q /n\"");
	}
	if( sp==0 && (sku & XP_ALL) && _msxml3_dll>zero && _msxml3_dll<fver(8,20,9415,0)) {
		NN("Security Update, February 13, 2002 (MSXML 3.0)");
		//XX(rtm+"q318203_c0d4c77cc65c64a062f915de6feef911843afc51.exe"+a2);
		XX(sw+rtm+"q318203_c0d4c77cc65c64a062f915de6feef911843afc51.exe /Q:A /R:N /C:\"dahotfix.exe /q /n\"");
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_ssdpapi_dll  >zero && _ssdpapi_dll  <fver(5,1,2600,23))
			||  (_netsetup_exe >zero && _netsetup_exe <fver(6,0,2448,0))
			||  (_ssdpsrv_dll  >zero && _ssdpsrv_dll  <fver(5,1,2600,23))
			||  (_upnp_dll     >zero && _upnp_dll     <fver(5,1,2600,23)) )) {
		NN("Security Update, December 17, 2001");
		XX(rtm+"Q315000_WXP_SP1_x86_ENU.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _quartz_dll>zero && _quartz_dll<fver(6,4,2600,113)) {
		NN("Security Update for Windows XP (819696)");
		XX(rtm+"q819696_wxp_sp2_x86_enu_d7500cd8f1a2691f3b2223835b04b63.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,106)) {
		NN("810577: Security Update");
		XX(rtm+"q810577_wxp_en_36413db9fb3c4a63217d4612a5b0d74.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_rasapi32_dll >zero && _rasapi32_dll <fver(5,1,2600,28))
			||  (_rasdlg_dll   >zero && _rasdlg_dll   <fver(5,1,2600,28))
			||  (_rassapi_dll  >zero && _rassapi_dll  <fver(5,1,2600,28)) )) {
		NN("Q318138: Security Update (Windows XP)");
		XX(rtm+"q318138_x86_0778b3715bed9bd7f1f3f6fab13f02b7bc84073b.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_xenroll_dll >zero && _xenroll_dll <fver(5,131,3659,0)) )) {
		NN("Q323172: Security Update (Windows XP)");
		XX(rtm+"q323172_xp_en_924f6391915566a5d62c377f81e918d.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_helpctr_exe  >zero && _helpctr_exe  <fver(5,1,2600,101))
			||  (_helpsvc_exe  >zero && _helpsvc_exe  <fver(5,1,2600,101))
//			||  (_hscmui_cab   >zero && _hscmui_cab   <fver())
			||  (_HscUpd_exe   >zero && _HscUpd_exe   <fver(5,1,2600,101))
//			||  (_hscxpsp1_cab >zero && _hscxpsp1_cab <fver())
			||  (_msconfig_exe >zero && _msconfig_exe <fver(5,1,2600,101))
			||  (_pchshell_dll >zero && _pchshell_dll <fver(5,1,2600,101))
			||  (_pchsvc_dll   >zero && _pchsvc_dll   <fver(5,1,2600,101)) )) {
		NN("Security Update for Microsoft Windows XP (KB328940)");
		XX(rtm+"q328940_wxp_sp1_x86_enu_00ed243d76d57a997ce99e22e96be9b.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_rdpdd_dll >zero && _rdpdd_dll <fver(5,1,2600,48))
			||  (_rdpwd_sys >zero && _rdpwd_sys <fver(5,1,2600,48)) )) {
		NN("Q324380: Security Update (Windows XP)");
		XX(rtm+"q324380_xpsp1_en_7c993133cd978d619b3db3fe70821ab.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_mup_sys >zero && _mup_sys <fver(5,1,2600,19)) )) {
		NN("Q311967: Security Update");
		XX(rtm+"q311967_ac5efc3697ad46f2b02646ba229f31bac1c0cc26.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_admin_exe    >zero && _admin_exe    <fver(4,0,2,6513))
			||  (_author_dll   >zero && _author_dll   <fver(4,0,2,6513))
			||  (_author_exe   >zero && _author_exe   <fver(4,0,2,6513))
			||  (_cfgwiz_exe   >zero && _cfgwiz_exe   <fver(4,0,2,6513))
			||  (_fp4amsft_dll >zero && _fp4amsft_dll <fver(4,0,2,6513))
			||  (_fp4anscp_dll >zero && _fp4anscp_dll <fver(4,0,2,6513))
			||  (_fp4apws_dll  >zero && _fp4apws_dll  <fver(4,0,2,6513))
			||  (_fp4areg_dll  >zero && _fp4areg_dll  <fver(4,0,2,6513))
			||  (_fp4atxt_dll  >zero && _fp4atxt_dll  <fver(4,0,2,6513))
			||  (_fp4autl_dll  >zero && _fp4autl_dll  <fver(4,0,2,6513))
			||  (_fp4avnb_dll  >zero && _fp4avnb_dll  <fver(4,0,2,6513))
			||  (_fp4avss_dll  >zero && _fp4avss_dll  <fver(4,0,2,6513))
			||  (_fp4awebs_dll >zero && _fp4awebs_dll <fver(4,0,2,6513))
			||  (_fp4awel_dll  >zero && _fp4awel_dll  <fver(4,0,2,6513))
			||  (_fpadmcgi_exe >zero && _fpadmcgi_exe <fver(4,0,2,6513))
			||  (_fpadmdll_dll >zero && _fpadmdll_dll <fver(4,0,2,6513))
			||  (_fpcount_exe1  >zero && _fpcount_exe1  <fver(4,0,2,6513))
			||  (_fpcount_exe2  >zero && _fpcount_exe2  <fver(4,0,2,6513))
			||  (_fpexedll_dll >zero && _fpexedll_dll <fver(4,0,2,6513))
			||  (_fpmmc_dll    >zero && _fpmmc_dll    <fver(4,0,2,6513))
			||  (_fpremadm_exe >zero && _fpremadm_exe <fver(4,0,2,6513))
			||  (_fpsrvadm_exe >zero && _fpsrvadm_exe <fver(4,0,2,6513))
			||  (_shtml_dll    >zero && _shtml_dll    <fver(4,0,2,6513))
			||  (_shtml_exe    >zero && _shtml_exe    <fver(4,0,2,6513))
			||  (_admin_exe_dllcache    <fver(4,0,2,6513))
			||  (_author_dll_dllcache   <fver(4,0,2,6513))
			||  (_author_exe_dllcache   <fver(4,0,2,6513))
			||  (_cfgwiz_exe_dllcache   <fver(4,0,2,6513))
			||  (_fp4amsft_dll_dllcache <fver(4,0,2,6513))
			||  (_fp4anscp_dll_dllcache <fver(4,0,2,6513))
			||  (_fp4apws_dll_dllcache  <fver(4,0,2,6513))
			||  (_fp4areg_dll_dllcache  <fver(4,0,2,6513))
			||  (_fp4atxt_dll_dllcache  <fver(4,0,2,6513))
			||  (_fp4autl_dll_dllcache  <fver(4,0,2,6513))
			||  (_fp4avnb_dll_dllcache  <fver(4,0,2,6513))
			||  (_fp4avss_dll_dllcache  <fver(4,0,2,6513))
			||  (_fp4awebs_dll_dllcache <fver(4,0,2,6513))
			||  (_fp4awel_dll_dllcache  <fver(4,0,2,6513))
			||  (_fpadmcgi_exe_dllcache <fver(4,0,2,6513))
			||  (_fpadmdll_dll_dllcache <fver(4,0,2,6513))
			||  (_fpcount_exe_dllcache  <fver(4,0,2,6513))
			||  (_fpexedll_dll_dllcache <fver(4,0,2,6513))
			||  (_fpmmc_dll_dllcache    <fver(4,0,2,6513))
			||  (_fpremadm_exe_dllcache <fver(4,0,2,6513))
//			||  (_fpsrvadm_exe_dllcache <fver(4,0,2,6513))
			||  (_shtml_dll_dllcache    <fver(4,0,2,6513))
			||  (_shtml_exe_dllcache    <fver(4,0,2,6513)) )) {
		NN("Q324096: Security Update (Windows XP)");
		XX(rtm+"q324096_wxp_sp1_en_1556063f6e8f4175aca99c531d2a8b4.exe"+a7);
	}
	if( sp<2 && (sku & XP_ALL) && (
			    (_admin_dll    >zero && _admin_dll    <fver(4,0,2,7523))
			||  (_admin_exe    >zero && _admin_exe    <fver(4,0,2,7523))
			||  (_author_dll   >zero && _author_dll   <fver(4,0,2,7523))
			||  (_author_exe   >zero && _author_exe   <fver(4,0,2,7523))
			||  (_cfgwiz_exe   >zero && _cfgwiz_exe   <fver(4,0,2,7523))
			||  (_fp4amsft_dll >zero && _fp4amsft_dll <fver(4,0,2,7523))
			||  (_fp4anscp_dll >zero && _fp4anscp_dll <fver(4,0,2,7523))
			||  (_fp4apws_dll  >zero && _fp4apws_dll  <fver(4,0,2,7523))
			||  (_fp4areg_dll  >zero && _fp4areg_dll  <fver(4,0,2,7523))
			||  (_fp4atxt_dll  >zero && _fp4atxt_dll  <fver(4,0,2,7523))
			||  (_fp4autl_dll  >zero && _fp4autl_dll  <fver(4,0,2,7523))
			||  (_fp4avnb_dll  >zero && _fp4avnb_dll  <fver(4,0,2,7523))
			||  (_fp4avss_dll  >zero && _fp4avss_dll  <fver(4,0,2,7523))
			||  (_fp4awebs_dll >zero && _fp4awebs_dll <fver(4,0,2,7523))
			||  (_fp4awel_dll  >zero && _fp4awel_dll  <fver(4,0,2,7802))
			||  (_fp98sadm_exe >zero && _fp98sadm_exe <fver(3,0,2,1706))
			||  (_fp98swin_exe >zero && _fp98swin_exe <fver(3,0,2,1706))
			||  (_fpadmcgi_exe >zero && _fpadmcgi_exe <fver(4,0,2,7523))
			||  (_fpadmdll_dll >zero && _fpadmdll_dll <fver(4,0,2,7523))
			||  (_fpcount_exe1  >zero && _fpcount_exe1  <fver(4,0,2,7523))
			||  (_fpcount_exe2  >zero && _fpcount_exe2  <fver(4,0,2,7523))
			||  (_fpencode_dll >zero && _fpencode_dll <fver(1997,5,27,0))
			||  (_fpexedll_dll >zero && _fpexedll_dll <fver(4,0,2,7523))
			||  (_fpmmc_dll    >zero && _fpmmc_dll    <fver(4,0,2,7523))
			||  (_fpremadm_exe >zero && _fpremadm_exe <fver(4,0,2,7523))
			||  (_fpsrvadm_exe >zero && _fpsrvadm_exe <fver(4,0,2,7523))
			||  (_shtml_dll    >zero && _shtml_dll    <fver(4,0,2,7523))
			||  (_shtml_exe    >zero && _shtml_exe    <fver(4,0,2,7523))
			||  (_stub_fpsrvadm_exe >zero && _stub_fpsrvadm_exe    <fver(4,0,2,7523))
			||  (_stub_fpsrvwin_exe >zero && _stub_fpsrvwin_exe    <fver(4,0,2,7523))
			||  (_tcptest_exe  >zero && _tcptest_exe    <fver(4,0,2,7523)) )) {
		NN("Security Update for Windows XP (KB810217)");
		XX(p1+"windowsxp-kb810217-x86-enu_696190f151ea0bcb063f0a89471e45b.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
                  ( _catsrv_dll   >zero && _catsrv_dll   <fver(2001,12,4414,53))
			  ||  ( _catsrvut_dll >zero && _catsrvut_dll <fver(2001,12,4414,53))
			  ||  ( _clbcatex_dll >zero && _clbcatex_dll <fver(2001,12,4414,53))
			  ||  ( _clbcatq_dll  >zero && _clbcatq_dll  <fver(2001,12,4414,53))
			  ||  ( _colbact_dll  >zero && _colbact_dll  <fver(2001,12,4414,53))
			  ||  ( _comadmin_dll >zero && _comadmin_dll <fver(2001,12,4414,53))
			  ||  ( _comrepl_exe  >zero && _comrepl_exe  <fver(2001,12,4414,53))
			  ||  ( _comsvcs_dll  >zero && _comsvcs_dll  <fver(2001,12,4414,53))
			  ||  ( _comuid_dll   >zero && _comuid_dll   <fver(2001,12,4414,53))
			  ||  ( _es_dll       >zero && _es_dll       <fver(2001,12,4414,53))
			  ||  ( _migregdb_exe >zero && _migregdb_exe <fver(2001,12,4414,53))
			  ||  ( _msdtcprx_dll >zero && _msdtcprx_dll <fver(2001,12,4414,53))
			  ||  ( _msdtctm_dll  >zero && _msdtctm_dll  <fver(2001,12,4414,53))
			  ||  ( _msdtcuiu_dll >zero && _msdtcuiu_dll <fver(2001,12,4414,53))
			  ||  ( _mtxclu_dll   >zero && _mtxclu_dll   <fver(2001,12,4414,53))
			  ||  ( _mtxoci_dll   >zero && _mtxoci_dll   <fver(2001,12,4414,53))
			  ||  ( _ole32_dll    >zero && _ole32_dll    <fver(5,1,2600,136))
			  ||  ( _rpcrt4_dll   >zero && _rpcrt4_dll   <fver(5,1,2600,135))
			  ||  ( _rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,135))
			  ||  ( _txflog_dll   >zero && _txflog_dll   <fver(2001,12,4414,53)) )) {
		NN("Security Update for Windows XP (KB828741)");
		XX(rtm+"windowsxp-kb828741-x86-enu_e7bb870e5a83f1a8c39061e0a4db1b2.exe"+a6);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_itss_dll     >zero && _itss_dll     <fver(5,2,3790,185))
			||  (_xpsp1hfm_exe >zero && _xpsp1hfm_exe <fver(5,5,13,0)) )) {
		NN("Security Update for Windows XP (KB840315)");
		XX(rtm+"windowsxp-kb840315-x86-enu_b07233fce45da8b90e43ed27916f4c80b7cbcb19.exe"+a6);
	}
	if( sp==0 && (sku & XP_ALL) && (
		// Q323255 might be replaced by Q811630
			    (_hh_exe      >zero && _hh_exe      <fver(5,2,3644,0))
			||  (_hhctrl_ocx  >zero && _hhctrl_ocx  <fver(5,2,3669,0))
			||  (_hhsetup_dll >zero && _hhsetup_dll <fver(5,2,3644,0))
			||  (_itircl_dll  >zero && _itircl_dll  <fver(5,2,3644,0))
			||  (_itss_dll    >zero && _itss_dll    <fver(5,2,3644,0)) )) {
		NN("Q323255: Security Update (Windows XP)");
		XX(rtm+"q323255_x86_en_0eba14f85cc1851ae65f87755f7154e.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
		// Q811630 is replaced by kb896358 on SP1
			    (_hh_exe      >zero && _hh_exe      <fver(5,2,3644,0))
			||  (_hhctrl_ocx  >zero && _hhctrl_ocx  <fver(5,2,3735,0))
			||  (_hhsetup_dll >zero && _hhsetup_dll <fver(5,2,3644,0))
			||  (_itircl_dll  >zero && _itircl_dll  <fver(5,2,3644,0))
			||  (_itss_dll    >zero && _itss_dll    <fver(5,2,3644,0)) )) {
		NN("811630: Critical Update (Windows XP)");
		XX(rtm+"q811630_wxp_sp2_en_f77c384ac26c6ceed7f657e256f574d.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_srrstr_dll >zero && _srrstr_dll <fver(5,1,2600,101)) )) {
		NN("Q329441: Critical Update");
		XX(rtm+"windowsxp-kb329441-x86-enu_4526376824a7a2be48c6f4bcbbcf829.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && (
			    (_httpext_dll  >zero && _httpext_dll  <fver(6,0,2600,165))
			||  (_msxml3_dll   >zero && _msxml3_dll   <fver(8,50,2162,0))
			||  (_xpsp1hfm_exe >zero && _xpsp1hfm_exe <fver(5,5,31,0))))
	 ||(sp==1 && (sku & XP_ALL) && (
			    (_httpext_dll  >zero && _httpext_dll  <fver(6,0,2600,1579))
			||  (_msxml3_dll   >zero && _msxml3_dll   <fver(8,50,2162,0)) ))) {
		NN("Security Update for Windows XP (KB824151)");
		XX(p1+"windowsxp-kb824151-x86-enu_1e13d4fd702b9d5028429062023bbd7e02d9c714.exe"+a6);
	}
	if( sp==0 && (sku & XP_ALL) && (
			    (_snmp_exe    >zero && _snmp_exe    <fver(5,1,2600,28))
			||  (_snmpapi_dll >zero && _snmpapi_dll <fver(5,1,2600,28))
			||  (_wsnmp32_dll >zero && _wsnmp32_dll <fver(5,1,2600,28)) )) {
		NN("Security Update, February 12, 2002");
		XX(rtm+"q314147_4448f8c22d771895880c0cbd09491b03670c483a.exe"+a7);
	}
	if( sp<2 && (sku & XP_ALL) && (
                  ( _asp_dll      >zero && _asp_dll      <fver(5,1,2600,1181))
			  ||  ( _ftpsvc2_dll  >zero && _ftpsvc2_dll  <fver(5,1,2600,1173))
			  ||  ( _httpext_dll  >zero && _httpext_dll  <fver(6,0,2600,1189))
			  ||  ( _httpodbc_dll >zero && _httpodbc_dll <fver(5,1,2600,1172))
			  ||  ( _iischema_dll >zero && _iischema_dll <fver(5,1,2600,1152))
			  ||  ( _infocomm_dll >zero && _infocomm_dll <fver(6,0,2600,1167))
			  ||  ( _isatq_dll    >zero && _isatq_dll    <fver(6,0,2600,1182))
			  ||  ( _lonsint_dll  >zero && _lonsint_dll  <fver(6,0,2600,1167))
			  ||  ( /*_spiisupd_exe >zero &&*/ _spiisupd_exe <fver(5,1,2600,1152))
			  ||  ( _ssinc_dll    >zero && _ssinc_dll    <fver(5,1,2600,1152))
			  ||  ( _w3svc_dll    >zero && _w3svc_dll    <fver(5,1,2600,1166)) )) {
		NN("Q811114: Security Update (Windows XP or Windows XP Service Pack 1)");
		XX(p1+"q811114_wxp_sp2_x86_enu_63cfc7cfc1fb0ad0b7df3c483b75760.exe"+a7);
	}

	// Windows XP SP1 updates
	if((sp==0 && (sku & XP_ALL) && (
	      (_browser_dll  >zero && _browser_dll  <fver(5,1,2600,105))
	   || (_mstask_dll   >zero && _mstask_dll   <fver(5,1,2600,155))
	   || (_mstinit_exe  >zero && _mstinit_exe  <fver(5,1,2600,155))
	   || (_netapi32_dll >zero && _mstinit_exe  <fver(5,1,2600,122))
	   || (_schedsvc_dll >zero && _schedsvc_dll <fver(5,1,2600,155))))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_mstask_dll   >zero && _mstask_dll   <fver(5,1,2600,1564))
	   || (_mstinit_exe  >zero && _mstinit_exe  <fver(5,1,2600,1564))
	   || (_netapi32_dll >zero && _mstinit_exe  <fver(5,1,2600,1562))
	   || (_schedsvc_dll >zero && _schedsvc_dll <fver(5,1,2600,1564))))) {
		NN("Security Update for Windows XP (KB841873)");
		XX(p1+"windowsxp-kb841873-x86-enu_f0f4cb2c535f8adf154e72f2f2c60d6f9b710d6d.exe"+a6);
	}
	if((sp==0 && (sku & XP_ALL) && _shmedia_dll>zero && _shmedia_dll<fver(6,0,2600,101))
	 ||(sp==1 && (sku & XP_ALL) && _shmedia_dll>zero && _shmedia_dll<fver(6,0,2800,1125))) {
		NN("Q329390: Security Update");
		XX(p1+"q329390_wxp_3f60064794271f0053892985402fe5b6679d3f2d.exe"+a7);
	}
	if( sp<2 && (sku & XP_ALL) && _crypt32_dll>zero && _crypt32_dll<fver(5,131,2600,1123)) {
		NN("Q329115: Security Update (Windows XP)");
		XX(p1+"q329115_wxp_sp2_x86_1d09793faf21249febcc160d341612338dfd3154.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && (_flash_ocx>zero || _swflash_ocx>zero) && _flash_ocx<fver(6,0,84,0)) {
		NN("Security Update for Flash Player (KB913433)");
		XX(p1+"Windows-KB913433-x86-ENU.exe"+a2);
	}
	/*if((sp==0 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,105))
	 ||(sp==1 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,1154))) {
		NN("329170: Security Update");
		XX(p1+"q329170_wxp_sp2_en_90a3d54a8af7b392e7b3ff4deddc745.exe"+a7);
	}*/
	if((sp==0 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,112))
	 ||(sp==1 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,1193))) {
		// MS03-024: Windows XP Security Patch: Buffer Overrun in Windows Could Lead to Data Corruption
		NN("817606: Security Update (Windows XP)");
		XX(p1+"Q817606_WXP_SP2_x86_ENU.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && _ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,114))
	 ||(sp==1 && (sku & XP_ALL) && _ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,1217))) {
		NN("Security Update for Windows XP (815021)");
		XX(p1+"q815021_wxp_sp2_x86_enu_b56c13ca24cdeefbe85e72cdbf34fb0.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_dbnetlib_dll >fver(2000,80,747,0)  && _dbnetlib_dll <fver(2000,81,9002,0))
//	   || (_msdart_dll   >zero                 && _msdart_dll   <fver(2,71,9042,0))
	   || (_odbc32_dll   >fver(3,520,7502,0)   && _odbc32_dll   <fver(3,520,9002,0))
	   || (_odbcbcp_dll  >fver(2000,80,747,0)  && _odbcbcp_dll  <fver(2000,81,9002,0))
	   || (_odbccp32_dll >fver(3,520,7502,0)   && _odbccp32_dll <fver(3,520,9002,0))
	   || (_oledb32_dll  >fver(2,70,0,0)       && _oledb32_dll  <fver(2,70,9002,0))
	   || (_sqloledb_dll >fver(2000,80,747,0)  && _sqloledb_dll <fver(2000,81,9002,0))
	   || (_sqlsrv32_dll >fver(2000,80,747,0)  && _sqlsrv32_dll <fver(2000,81,9002,0)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_dbnetlib_dll >fver(2000,81,9002,0) && _dbnetlib_dll <fver(2000,81,9042,0))
//	   || (_msdart_dll   >zero                 && _msdart_dll   <fver(2,71,9042,0))
	   || (_odbc32_dll   >fver(3,520,9002,0)   && _odbc32_dll   <fver(3,520,9042,0))
	   || (_odbcbcp_dll  >fver(2000,81,9002,0) && _odbcbcp_dll  <fver(2000,81,9042,0))
	   || (_odbccp32_dll >fver(3,520,9002,0)   && _odbccp32_dll <fver(3,520,9042,0))
	   || (_oledb32_dll  >fver(2,70,9002,0)    && _oledb32_dll  <fver(2,71,9042,0))
	   || (_sqloledb_dll >fver(2000,81,9002,0)  && _sqloledb_dll <fver(2000,81,9042,0))
	   || (_sqlsrv32_dll >fver(2000,81,9002,0)  && _sqlsrv32_dll <fver(2000,81,9042,0)) ))) {
		NN("Security Update for Microsoft Data Access Components (KB823718)");
		//XX(p1+"ENU_Q832483_MDAC_x86.EXE"+a2);
		XX(sw+p1+"ENU_Q832483_MDAC_x86.exe /Q:A /R:N /C:\"dahotfix.exe /q /n\"");
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_gdi32_dll  >zero && _gdi32_dll  <fver(5,1,2600,1789))
	   || (_mf3216_dll >zero && _mf3216_dll <fver(5,1,2600,1331)) )) {
		NN("Security Update for Windows XP (KB912919)");
		XX(p1+"windowsxp-kb912919-x86-enu_a4b4638c85d74c1d9ceaf63189ad46aa34f60293.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && (
			    (_mqac_sys    >zero && _mqac_sys    <fver(5,1,0,1044))
			||  (_mqad_dll    >zero && _mqad_dll    <fver(5,1,0,1044))
			||  (_mqdscli_dll >zero && _mqdscli_dll <fver(5,1,0,1044))
			||  (_mqise_dll   >zero && _mqise_dll   <fver(5,1,0,1044))
			||  (_mqqm_dll    >zero && _mqqm_dll    <fver(5,1,0,1044))
			||  (_mqrt_dll    >zero && _mqrt_dll    <fver(5,1,0,1044))
			||  (_mqsec_dll   >zero && _mqsec_dll   <fver(5,1,0,1044))
			||  (_mqupgrd_dll >zero && _mqupgrd_dll <fver(5,1,0,1044))
			||  (_mqutil_dll  >zero && _mqutil_dll  <fver(5,1,0,1044)) )) {
		NN("Security Update for Windows XP (KB892944)");
		XX(p1+"windowsxp-kb892944-x86-enu_4bdf561e99e401a81737b04f5ce9f2dde640e06f.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,1869)) {
		NN("Security Update for Windows XP (KB917422)");
		XX(p1+"windowsxp-kb917422-x86-enu_8a32d9119235c80bc6a82793403e0e5443c36789.exe"+a1);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_nddenb32_dll >zero && _nddenb32_dll <fver(5,1,2600,149))
	   || (_netdde_exe   >zero && _netdde_exe   <fver(5,1,2600,158))
	   || (_winlogon_exe >zero && _winlogon_exe <fver(5,1,2600,149)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_nddenb32_dll >zero && _nddenb32_dll <fver(5,1,2600,1555))
	   || (_netdde_exe   >zero && _netdde_exe   <fver(5,1,2600,1567))
	   || (_winlogon_exe >zero && _winlogon_exe <fver(5,1,2600,1557)) ))) {
		NN("Security Update for Windows XP (KB841533)");
		XX(p1+"windowsxp-kb841533-x86-enu_755d3599e96f8b8131966683e1cf15c860cde2dd.exe"+a6);
	}
	if((sp==0 && (sku & XP_ALL) && _netbt_sys>zero && _netbt_sys<fver(5,1,2600,117))
	 ||(sp==1 && (sku & XP_ALL) && _netbt_sys>zero && _netbt_sys<fver(5,1,2600,1243))) {
		NN("Security Update for Microsoft Windows (KB824105)");
		XX(p1+"windowsxp-kb824105-x86-enu_92192cc7a15f27eda4f0e6a932db355.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && _tcpip_sys>zero && _tcpip_sys<fver(5,1,2600,1831)) {
		NN("Security Update for Windows XP (KB917953)");
		XX(p1+"windowsxp-kb917953-x86-enu_a1c66e00d1a487f25ca16af5a7f858858136c228.exe"+a1);
	}
	/*if((sp==0 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2600,115))
	 ||(sp==1 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2800,1233))) {
		// KB821557 is replaced by KB841356
		NN("Security Update for Windows XP (KB821557)");
		XX(p1+"WindowsXP-KB821557-x86-ENU.exe"+a7);
	}*/
	if((sp==0 && (sku & XP_ALL) && (
	      (_fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2750,165))
	   || (_grpconv_exe  >zero && _grpconv_exe  <fver(5,1,2600,166))
	   || (_linkinfo_dll >zero && _linkinfo_dll <fver(5,1,2600,165))
	   || (_ntlanman_dll >zero && _ntlanman_dll <fver(5,1,2600,165))
	   || (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2750,166))
	   || (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2750,167))
	   || (_xpsp1hfm_exe >zero && _xpsp1hfm_exe <fver(5,5,31,0)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2800,1579))
	   || (_grpconv_exe  >zero && _grpconv_exe  <fver(5,1,2600,1580))
	   || (_linkinfo_dll >zero && _linkinfo_dll <fver(5,1,2600,1579))
	   || (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1580))
	   || (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1584))
	   || (_sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1579))
	   || ( _comctl32_dll_1579 < fver(6,0,2800,1579)) ))) {
		NN("Security Update for Windows XP (KB841356)");
		XX(p1+"windowsxp-kb841356-x86-enu_c263b64a1074aa6b5bb34ee43935e7dc82f7acc9.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && _rmcast_sys>zero && _rmcast_sys<fver(5,1,2600,1873)) {
//	if( sp==2 && (sku & XP_ALL) && _rmcast_sys>zero && _rmcast_sys<fver(5,1,2600,2951)) {
		// KB919007 is replaced by KB950762 on SP2
		NN("Security Update for Windows XP (KB919007)");
		XX(p1+"windowsxp-kb919007-x86-enu_dc2307d635a64c87bbf1f216442104ef4b4ada0b.exe"+a1);
	}
	if((sp==0 && (sku & XP_ALL) && _zipfldr_dll>zero && _zipfldr_dll<fver(6,0,2600,101))
	 ||(sp==1 && (sku & XP_ALL) && _zipfldr_dll>zero && _zipfldr_dll<fver(6,0,2800,1126))) {
		NN("Q329048: Security Update");
		XX(p1+"q329048_xp_bdcca3f0c06417895bf3ee93b6db09f6d04d0282.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && _zipfldr_dll>zero && _zipfldr_dll<fver(6,0,2750,167))
	 ||(sp==1 && (sku & XP_ALL) && _zipfldr_dll>zero && _zipfldr_dll<fver(6,0,2800,1584))) {
		NN("Security Update for Windows XP (KB873376)");
		XX(p1+"windowsxp-kb873376-x86-enu_5c67061f5227844ef8b5f1eec8596bd2520c542d.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && _msieftp_dll>zero && _msieftp_dll<fver(6,0,2800,1724)) {
		NN("Security Update for Windows XP (KB905495)");
		XX(p1+"windowsxp-kb905495-x86-enu_10db795016004f5543ea1e556c4b41f471f5f99f.exe"+a1);
	}
	if((sp==0 && (sku & XP_ALL) && _locator_exe>zero && _locator_exe<fver(5,1,2600,108))
	 ||(sp==1 && (sku & XP_ALL) && _locator_exe>zero && _locator_exe<fver(5,1,2600,1147))) {
		NN("810833: Security Update (Windows XP)");
		XX(p1+"q810833_wxp_sp2_45a2502a400329a5db92327f4cb345a441fc1d2e.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,136))
	   || ( _gdiplus_dll_1360 < fver(5,1,3102,1360)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1363))
	   || ( _gdiplus_dll_1360 < fver(5,1,3102,1360)) ))) {
		NN("Security Update for Windows XP (KB833987)");
		XX(p1+"windowsxp-kb833987-x86-enu_8af7883db1147e663caee85b7ddea002ad4da772.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2800,1579))
	   || (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1873))
	   || (_sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1579))
	   || ( _comctl32_dll_1873 < fver(6,0,2800,1873)) )) {
		NN("Security Update for Windows XP (KB921398)");
		XX(p1+"windowsxp-kb921398-x86-enu_47a6965d63200295eb57c1d7a5440d26c43d90d1.exe"+a1);
	}
	if( sp<2 && (sku & XP_ALL) && (
	      (_dao360_dll   >zero && _dao360_dll   <fver(3,60,8618,0))
	   || (_expsrv_dll   >zero && _expsrv_dll   <fver(6,0,72,9589))
	   || (_msexch40_dll >zero && _msexch40_dll <fver(4,0,6807,0))
	   || (_msexcl40_dll >zero && _msexcl40_dll <fver(4,0,8618,0))
	   || (_msjet40_dll  >zero && _msjet40_dll  <fver(4,0,8618,0))
	   || (_msjetol1_dll >zero && _msjetol1_dll <fver(4,0,8227,0))
	   || (_msjint40_dll >zero && _msjint40_dll <fver(4,0,6508,0))
	   || (_msjter40_dll >zero && _msjter40_dll <fver(4,0,6508,0))
	   || (_msjtes40_dll >zero && _msjtes40_dll <fver(4,0,8618,0))
	   || (_msltus40_dll >zero && _msltus40_dll <fver(4,0,6508,0))
	   || (_mspbde40_dll >zero && _mspbde40_dll <fver(4,0,8015,0))
	   || (_msrd2x40_dll >zero && _msrd2x40_dll <fver(4,0,7328,0))
	   || (_msrd3x40_dll >zero && _msrd3x40_dll <fver(4,0,6508,0))
	   || (_msrepl40_dll >zero && _msrepl40_dll <fver(4,0,8015,0))
	   || (_mstext40_dll >zero && _mstext40_dll <fver(4,0,8015,0))
	   || (_mswdat10_dll >zero && _mswdat10_dll <fver(4,0,6508,0))
	   || (_mswstr10_dll >zero && _mswstr10_dll <fver(4,0,6508,0))
	   || (_msxbde40_dll >zero && _msxbde40_dll <fver(4,0,8025,0))
	   || (_vbajet32_dll >zero && _vbajet32_dll <fver(6,0,1,9431)) )) {
		NN("Security Update for Windows XP (KB837001)");
		XX(p1+"windowsxp-kb837001-x86-enu_09e9d2c15ee5ee9323b210decd84941.exe"+a6);
	}
	/*if((sp==0 && (sku & XP_ALL) && (
	      (_user32_dll      >zero && _user32_dll <fver(5,1,2600,118))
	   || ( _win32k_sys     >zero && _win32k_sys <fver(5,1,2600,115)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_user32_dll      >zero && _user32_dll <fver(5,1,2600,1255))
	   || ( _win32k_sys     >zero && _win32k_sys <fver(5,1,2600,1275)) ))) {
		// KB824141 is replaced by KB840987
		NN("Security Update for Microsoft Windows (KB824141)");
		XX(p1+"WindowsXP-KB824141-x86-ENU.exe"+a6);
	}*/
	if((sp==0 && (sku & XP_ALL) && (
	      (_basesrv_dll  >zero && _basesrv_dll  <fver(5,1,2600,159))
	   || (_gdi32_dll    >zero && _gdi32_dll    <fver(5,1,2600,151))
	   || (_kernel32_dll >zero && _kernel32_dll <fver(5,1,2600,153))
	   || (_mf3216_dll   >zero && _mf3216_dll   <fver(5,1,2600,132))
	   || (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,160))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,160))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,160))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,160))
	   || (_ntvdm_exe    >zero && _ntvdm_exe    <fver(5,1,2600,157))
	   || (_ntvdmd_dll   >zero && _ntvdmd_dll   <fver(5,1,2600,153))
//	   || (_sysmain_sdb  >zero && _sysmain_sdb  <fver())
	   || (_user32_dll   >zero && _user32_dll   <fver(5,1,2600,152))
	   || (_vdmdbg_dll   >zero && _vdmdbg_dll   <fver(5,1,2600,153))
	   || (_win32k_sys   >zero && _win32k_sys   <fver(5,1,2600,166))
	   || (_winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,153))
	   || (_wow32_dll    >zero && _wow32_dll    <fver(5,1,2600,156))
	   || (_xpsp1hfm_exe >zero && _xpsp1hfm_exe <fver(5,5,31,0)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_basesrv_dll  >zero && _basesrv_dll  <fver(5,1,2600,1566))
	   || (_gdi32_dll    >zero && _gdi32_dll    <fver(5,1,2600,1561))
	   || (_kernel32_dll >zero && _kernel32_dll <fver(5,1,2600,1560))
	   || (_krnl386_exe  >zero && _krnl386_exe  <fver(3,10,0,103))
	   || (_mf3216_dll   >zero && _mf3216_dll   <fver(5,1,2600,1331))
//	   || (_ntio_sys     >zero && _ntio_sys     <fver())
//	   || (_ntio404_sys  >zero && _ntio404_sys  <fver())
//	   || (_ntio411_sys  >zero && _ntio411_sys  <fver())
//	   || (_ntio412_sys  >zero && _ntio412_sys  <fver())
//	   || (_ntio804_sys  >zero && _ntio804_sys  <fver())
	   || (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,1568))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,1568))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,1568))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,1568))
	   || (_ntvdm_exe    >zero && _ntvdm_exe    <fver(5,1,2600,1564))
	   || (_ntvdmd_dll   >zero && _ntvdmd_dll   <fver(5,1,2600,1560))
	   || (_user32_dll   >zero && _user32_dll   <fver(5,1,2600,1561))
	   || (_vdmdbg_dll   >zero && _vdmdbg_dll   <fver(5,1,2600,1560))
	   || (_win32k_sys   >zero && _win32k_sys   <fver(5,1,2600,1581))
	   || (_winlogon_exe >zero && _winlogon_exe <fver(5,1,2600,1557))
	   || (_winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,1561))
	   || (_wow32_dll    >zero && _wow32_dll    <fver(5,1,2600,1562)) ))) {
		NN("Security Update for Windows XP (KB840987)");
		XX(p1+"windowsxp-kb840987-x86-enu_60b89d9ecf98d6f749d0f9c87ff99c4758ef0de7.exe"+a6);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_msgsvc_dll >zero && _msgsvc_dll <fver(5,1,2600,121))
	   || (_wkssvc_dll >zero && _wkssvc_dll <fver(5,1,2600,121))))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_msgsvc_dll >zero && _msgsvc_dll <fver(5,1,2600,1309))
	   || (_wkssvc_dll >zero && _wkssvc_dll <fver(5,1,2600,1309))))) {
		NN("Security Update for Microsoft Windows XP (KB828035)");
		XX(p1+"windowsxp-kb828035-x86-enu_d911770163b58b6809b00f033230b46.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && !kb914798_installed() ) {
		NN("Security Update for Windows XP (KB914798)");
		XX(p1+"windowsxp-kb914798-v2-x86-enu_e1655311aaea236143c52067105754193e90c3ce.exe"+a1);
	}
	if( sp<2 && (sku & XP_ALL) && (
	      (_html32_cnv   >zero && _html32_cnv   <fver(2003,1100,5426,0))
	   || (_msconv97_dll >zero && _msconv97_dll <fver(2003,1100,5426,0)) )) {
		NN("823559: Security Update for Microsoft Windows");
		XX(p1+"windowsxp-kb823559-x86-enu_3a1b8659662198db00ff48a89d4689f.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && _raspptp_sys>zero && _raspptp_sys<fver(5,1,2600,101))
	 ||(sp==1 && (sku & XP_ALL) && _raspptp_sys>zero && _raspptp_sys<fver(5,1,2600,1129))) {
		NN("Security Update for Windows XP (329834)");
		XX(p1+"q329834_wxp_sp2_en_be9ddcff9200ff0d4879ed5939e73ed.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_gdi32_dll  >zero && _gdi32_dll  <fver(5,1,2600,1755))
	   || (_mf3216_dll >zero && _mf3216_dll <fver(5,1,2600,1331))
	   || (_user32_dll >zero && _user32_dll <fver(5,1,2600,1634))
	   || (_win32k_sys >zero && _win32k_sys <fver(5,1,2600,1755)) )) {
		NN("Security Update for Windows XP (KB896424)");
		XX(p1+"windowsxp-kb896424-x86-enu_bc0a35c5dd2dded71405dab707d0c61831b2a58f.exe"+a1);
	}
	if(sp==1 && (sku & XP_ALL) && (
	      (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1348))
	   || (_sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1336))
	   || ( _comctl32_dll_1342 < fver(6,0,2800,1342)) )) {
		NN("Update for Windows XP Shop for Music Online Link (KB833998)");
		XX(p1+"windowsxp-kb833998-x86-enu_aeee0743a7f7604e32e3c80f5f3a694.exe"+a6);
	}
	if((sp==0 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2600,151))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2800,1515))
	   || (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1556))
	   || (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1514))
	   || (_sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1515))
	   || ( _comctl32_dll_1515 < fver(6,0,2800,1515))) )) {
		NN("Security Update for Windows XP (KB839645)");
		XX(p1+"windowsxp-kb839645-x86-enu_18b27da784cef2d2f9c1888ef3aa2619e02b402a.exe"+a6);
	}
	if((sp==0 && (sku & XP_ALL) && _cryptui_dll>zero && _cryptui_dll<fver(5,131,2600,117))
	 ||(sp==1 && (sku & XP_ALL) && _cryptui_dll>zero && _cryptui_dll<fver(5,131,2600,1243))) {
		NN("Security Update for Microsoft Windows (KB823182)");
		XX(p1+"windowsxp-kb823182-x86-enu_211ee84995ef332148cf9b6657c9038.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && _hhctrl_ocx>zero && _hhctrl_ocx<fver(5,2,3790,558)) {
		NN("Security Update for Windows XP (KB922616)");
		XX(p1+"windowsxp-kb922616-x86-enu_cf1ee106a318c1fe135978f94ec0867312cea73b.exe"+a7);
	}
	/*if(    (sp==0 && (sku & XP_ALL) && _msasn1_dll>zero && _msasn1_dll<fver(5,1,2600,119))
		|| (sp==1 && (sku & XP_ALL) && _msasn1_dll>zero && _msasn1_dll<fver(5,1,2600,1274))) {
		// KB828028 is replaced by KB835732
		NN("Security Update for Windows XP (KB828028)");
		XX(p1+"WindowsXP-KB828028-x86-ENU.exe"+a6);
	}*/
	if((sp==0 && (sku & XP_ALL) && (
	      (_browser_dll     >zero && _browser_dll     <fver(5,1,2600,105))
	   || (_callcont_dll    >zero && _callcont_dll    <fver(5,1,2600,133))
	   || (_CmdEvTgProv_dll >zero && _CmdEvTgProv_dll <fver(5,1,2600,136))
	   || (_gdi32_dll       >zero && _gdi32_dll       <fver(5,1,2600,132))
	   || (_h323_tsp        >zero && _h323_tsp        <fver(5,1,2600,134))
	   || (_h323msp_dll     >zero && _h323msp_dll     <fver(5,1,2600,134))
	   || (_helpctr_exe     >zero && _helpctr_exe     <fver(5,1,2600,128))
	   || (_ipnathlp_dll    >zero && _ipnathlp_dll    <fver(5,1,2600,137))
	   || (_lsasrv_dll      >zero && _lsasrv_dll      <fver(5,1,2600,134))
	   || (_mf3216_dll      >zero && _mf3216_dll      <fver(5,1,2600,132))
	   || (_msasn1_dll      >zero && _msasn1_dll      <fver(5,1,2600,137))
	   || (_msgina_dll      >zero && _msgina_dll      <fver(5,1,2600,128))
	   || (_mst120_dll      >zero && _mst120_dll      <fver(5,1,2600,133))
	   || (_netapi32_dll    >zero && _netapi32_dll    <fver(5,1,2600,122))
	   || (_nmcom_dll       >zero && _nmcom_dll       <fver(5,1,2600,133))
	   || (_rtcdll_dll      >zero && _rtcdll_dll      <fver(5,1,2600,134))
	   || (_schannel_dll    >zero && _schannel_dll    <fver(5,1,2600,136)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_callcont_dll    >zero && _callcont_dll    <fver(5,1,2600,1348))
	   || (_CmdEvTgProv_dll >zero && _CmdEvTgProv_dll <fver(5,1,2600,1363))
	   || (_gdi32_dll       >zero && _gdi32_dll       <fver(5,1,2600,1346))
	   || (_h323_tsp        >zero && _h323_tsp        <fver(5,1,2600,1348))
	   || (_h323msp_dll     >zero && _h323msp_dll     <fver(5,1,2600,1348))
	   || (_helpctr_exe     >zero && _helpctr_exe     <fver(5,1,2600,1340))
	   || (_ipnathlp_dll    >zero && _ipnathlp_dll    <fver(5,1,2600,1364))
	   || (_lsasrv_dll      >zero && _lsasrv_dll      <fver(5,1,2600,1361))
	   || (_mf3216_dll      >zero && _mf3216_dll      <fver(5,1,2600,1331))
	   || (_msasn1_dll      >zero && _msasn1_dll      <fver(5,1,2600,1362))
	   || (_msgina_dll      >zero && _msgina_dll      <fver(5,1,2600,1343))
	   || (_mst120_dll      >zero && _mst120_dll      <fver(5,1,2600,1348))
	   || (_netapi32_dll    >zero && _netapi32_dll    <fver(5,1,2600,1343))
	   || (_nmcom_dll       >zero && _nmcom_dll       <fver(5,1,2600,1348))
	   || (_rtcdll_dll      >zero && _rtcdll_dll      <fver(5,1,2600,1351))
	   || (_schannel_dll    >zero && _schannel_dll    <fver(5,1,2600,1347)) ))) {
		NN("Security Update for Windows XP (KB835732)");
		XX(p1+"windowsxp-kb835732-x86-enu_99fc98fc8e8cbf6720f28028a1f05eb.exe"+a6);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_helpctr_exe >zero && _helpctr_exe <fver(5,1,2600,137))
	   || (_HscUpd_exe  >zero && _HscUpd_exe  <fver(5,1,2600,134)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_helpctr_exe >zero && _helpctr_exe <fver(5,1,2600,1515))
	   || (_HscUpd_exe  >zero && _HscUpd_exe  <fver(5,1,2600,1515)) ))) {
		NN("Security Update for Windows XP (KB840374)");
		XX(p1+"windowsxp-kb840374-x86-enu_26d1a97f5266edb9af508be2cb8242d.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && _netapi32_dll>zero && _netapi32_dll<fver(5,1,2600,1874)) {
		NN("Security Update for Windows XP (KB921883)");
		XX(p1+"windowsxp-kb921883-x86-enu_80bd35bdac09cbe1974111ee623b36f016b639be.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _msxml3_dll>zero && _msxml3_dll<fver(8,70,1113,0)) {
		NN("Security Update for Windows XP (KB924191)");
		XX(p1+"windowsxp-kb924191-x86-enu_5e3ee7c5954da4cd38a5121623c05d617e547951.exe"+a1);
	}

	// Pre-SP2 Nonsecurity updates
	if((sp==0 && (sku & XP_ALL) && _dmusic_sys>zero && _dmusic_sys <fver(5,1,2600,106))
	 ||(sp==1 && (sku & XP_ALL) && _dmusic_sys>zero && _dmusic_sys <fver(5,1,2600,1142)) ) {
		NN("810272: Recommended Update");
		XX(p1+"Q810272_WXP_SP2_x86_ENU.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && _acgenral_dll>zero && _acgenral_dll<fver(5,1,2600,1170)) {
		NN("814995: Recommended Update");
		XX(p1+"q814995_wxp_sp2_x86_enu_9e15819376b7ecb637bc9f9bfac2d16.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
	      (_acgenral_dll >zero && _acgenral_dll <fver(5,1,2600,40))
	   || (_aclayers_dll >zero && _aclayers_dll <fver(5,1,2600,40))
	   || (_acspecfc_dll >zero && _acspecfc_dll <fver(5,1,2600,40))
	   || (_acxtrnal_dll >zero && _acxtrnal_dll <fver(5,1,2600,40)) )) {
		NN("Windows XP Application Compatibility Update, April 2002");
		XX(rtm+"q319580_7377086567f22a47d655346683359c1306df1074.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_appwiz_cpl   >zero && _appwiz_cpl   <fver(5,1,2600,1228))
	   || (_explorer_exe >zero && _explorer_exe <fver(6,0,2800,1221))
	   || (_shmgrate_exe >zero && _shmgrate_exe <fver(5,1,2600,1221)) )) {
		NN("820291: Recommended Update (Windows XP)");
		XX(p1+"windowsxp-kb820291-x86-enu_ff8ae591885c2b35e7cdc22035850e7.exe"+a7);
	}
	if( sp<2 && (sku & XP_ALL) && _ups_exe>zero && _ups_exe<fver(5,1,2600,15)) {
		NN("Computer Fails When Connected To A UPS");
		XX(rtm+"q310437_x86_42416c3872a03a1d2e9d99b879891076368ce296.exe"+a7);
	}
	if( sp<2 && (sku & XP_ALL) && 
		regQueryDWORD(L"SOFTWARE\\Microsoft\\Internet Explorer\\ActiveX Compatibility\\{00000566-0000-0010-8000-00AA006D2EA4}",
		              L"Compatibility Flags",&status) != 1024) {
		NN("Critical Update for ADODB.stream (KB870669)");
		XX(sw+p1+"windows-kb870669-x86-enu_e3bab5ab3953b33dd4fd28b64c6f670aac446996.exe"+a2);
	}
	/*
	if( sp==0 && (sku & XP_ALL) && (
	      (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,31))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,31))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,31))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,31)) )) {
		NN("System Recovered Error Message Update");
		XX(rtm+"q317277_8c5a39cf107c69cc9462b9adf1cf511df228c575.exe"+a7);
	}
	*/
	/*if( sp==0 && (sku & XP_ALL) && _xpsp1hfm_exe>zero && _xpsp1hfm_exe<fver(5,5,31,0)) {
		NN("Critical Update for Windows XP (KB887822)");
		// KB887822 might be replaced by KB887811, as KB887811 installs a newer version of XPSP1HFM.EXE
		XX(rtm+"windowsxp-kb887822-x86-enu_a3e55bed585f126e5ab10f200f6a5a1ffbb66c96.exe"+a7);
	}*/
	/*if((sp==0 && (sku & XP_ALL) && (
	      (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,108))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,108))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,108))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,108)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,1151))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,1151))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,1151))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,1151)) ) )) {
	   // Q811493 is included in KB826939
		NN("811493: Security Update (Windows XP)");
		XX(rtm+"Q811493_WXP_SP2_x86_ENU.exe"+a7);
	}*/
	if( sp==0 && (sku & XP_ALL) && (
	      (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,170))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,170))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,170))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,170))
	   || (_xpsp1hfm_exe >zero && _xpsp1hfm_exe <fver(5,5,33,0)) )) {
		NN("Critical Update for Windows XP (KB887811)");
		XX(rtm+"windowsxp-kb887811-v2-x86-enu_092387bd05fc6f5d0d027df8fed841153f82c031.exe"+a6);
	}
	if( sp==0 && (sku & XP_ALL) && _ntfs_sys>zero && _ntfs_sys<fver(5,1,2600,28)) {
		NN("Critical Update, February 10, 2002");
		XX(rtm+"q315403_9eae12a1a1e27d7aea96b89ab793a5a28c680ed8.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _qmgr_dll>zero && _qmgr_dll<fver(6,0,2600,27)) {
		NN("Critical Update, February 9, 2002");
		XX(rtm+"Q314862_WXP_SP1_x86_ENU.exe"+a7);
	}
	if( sp<2 && (sku & XP_ALL) && _undo_guimode_txt ) {
		NN("Critical Update, November 19, 2001");
		XX(sw+p1+"q312369_37389f2393d35e685cf3818d75c6123365f5eef2.exe"+a2);
	}
	if( sp==0 && (sku & XP_ALL) && ( 
		                  ( _guitrn_dll   <fver(5,1,2600,14))
					  ||  ( _guitrn_a_dll <fver(5,1,2600,14))
					  ||  ( _migwiz_exe   <fver(5,1,2600,14))
					  ||  ( _migwiz_a_exe <fver(5,1,2600,14))
					  ||  ( _script_dll   <fver(5,1,2600,14))
					  ||  ( _script_a_dll <fver(5,1,2600,14))
					  ||  ( _sysmod_dll   <fver(5,1,2600,14))
					  ||  ( _sysmod_a_dll <fver(5,1,2600,14))
					  ||  ( _unctrn_dll   >zero && _unctrn_dll   <fver(5,1,2600,14))
					  ||  ( _unctrn_a_dll >zero && _unctrn_a_dll <fver(5,1,2600,14)) )) {
		NN("Files and Settings Transfer Wizard");
		XX(rtm+"q307869_f323efa52f460ea1e5f4201b011c071ea5b95110.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _udfs_sys>zero && _udfs_sys<fver(5,1,2600,19)) {
		NN("Q310510: Recommended Update");
		XX(rtm+"q310510_cee4ab9f58ef7892ad97ad74d9eb1717c8b1c760.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
	      (_wmiprvsd_dll >zero && _wmiprvsd_dll <fver(5,1,2600,42))
	   || (_wmiprvse_exe >zero && _wmiprvse_exe <fver(5,1,2600,42)) )) {
		NN("Q319322: Recommended Update");
		XX(rtm+"q319322_x86_b46cee9fd3e75a80a63491fdf1cd5665d37ae82a.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
	      (_imapi_exe >zero && _imapi_exe <fver(1,0,0,3))
	   || (_imapi_sys >zero && _imapi_sys <fver(5,1,2600,16)) )) {
		NN("Q320174: Recommended Update");
		XX(rtm+"q320174_1b5b353dd5a027b8d6ce1b8948cb8a80e7972ac9.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _qdvd_dll>zero && _qdvd_dll<fver(6,4,2600,44)) {
		NN("Q320552: Recommended Update");
		XX(rtm+"q320552_wxp_6aa28da67715a9796fbfc36ddd6ca6836d30582f.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _wbemess_dll>zero && _wbemess_dll<fver(5,1,2600,42)) {
		NN("Q320678: Recommended Update");
		XX(rtm+"q320678_x86_bd9044fa7d6e523f30f738effd9e39b8c05b117b.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
	      (_localspl_dll >zero && _localspl_dll <fver(5,1,2600,46))
	   || (_win32spl_dll >zero && _win32spl_dll <fver(5,1,2600,49)) )) {
		NN("Q320914: Recommended Update");
		XX(rtm+"Q320914_WXP_SP1_x86_ENU.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && _fxsclnt_exe>zero && _fxsclnt_exe<fver(5,2,1776,1024)) {
		NN("Q322011: Recommended Update");
		XX(p1+"q322011_wxp_c8e80cfc7414d922d9f5f9794d996cc86913e008.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _xactsrv_dll>zero && _xactsrv_dll<fver(5,1,2600,50)) {
		NN("Q326830: Security Update (Windows XP)");
		XX(rtm+"q326830_wxp_sp1_en_592a4e341f97da6d1bb0479ef1deecd.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_hccoin_dll  >zero && _hccoin_dll  <fver(5,1,2600,1243))
	   || (_usbccgp_sys >zero && _usbccgp_sys <fver(5,1,2600,1243))
	   || (_usbehci_sys >zero && _usbehci_sys <fver(5,1,2600,1243))
	   || (_usbhub_sys  >zero && _usbhub_sys  <fver(5,1,2600,1243))
	   || (_usbohci_sys >zero && _usbohci_sys <fver(5,1,2600,1243))
	   || (_usbport_sys >zero && _usbport_sys <fver(5,1,2600,1243))
	   || (_usbuhci_sys >zero && _usbuhci_sys <fver(5,1,2600,1243)) )) {
		NN("Recommended Update for Windows XP SP1 (KB822603)");
		XX(p1+"windowsxp-kb822603-x86-enu_d5007e34cf81d51e58b8eb4fb8912fb.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _pci_sys>zero && _pci_sys<fver(5,1,2600,21)) {
		NN("Resuming From Standby Update");
		XX(rtm+"q311542xpsp1x86_6897313d2eed9825fe937d670f389e97dca35664.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _shgina_dll==fver(6,0,2600,0)
		&& strncmp(_shgina_dll_md5,"584fb7de76afe6735dd12aabee568b34",32) != 0 ) {
		NN("Restarting Windows XP");
		XX(rtm+"Q307274_x86.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_dwwin_exe    >zero && _dwwin_exe    <fver(10,0,5403,0))
	   || (_faultrep_dll >zero && _faultrep_dll <fver(5,1,2600,115)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_dwwin_exe    >zero && _dwwin_exe    <fver(10,0,5403,0))
	   || (_faultrep_dll >zero && _faultrep_dll <fver(5,1,2600,1232)) ))) {
		NN("Windows Error Reporting: Recommended Update (Windows XP)");
		XX(p1+"windowsxp-kb821253-x86-enu_6700bff43173d425ee692ec1239b617.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _termsrv_dll>zero && _termsrv_dll<fver(5,1,2600,18)) {
		NN("Remote Assistance Connection");
		XX(rtm+"q311889_wxp_27dba07edd39b4cc0b3e2c6db90178e7396cff1a.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
	      (_rdchost_dll >zero && _rdchost_dll <fver(5,1,2600,15))
	   || (_sessmgr_exe >zero && _sessmgr_exe <fver(5,1,2600,15)) )) {
		NN("Remote Assistance Used with Network Cards and Firewalls");
		XX(rtm+"q308210_x86_9bf60ad45ad260ab449772bbdc337825796ac677.exe"+a7);
	}
	if( sp==1 && !(sku & XP_MCE) && (
	      (_encdec_dll >zero && _encdec_dll <fver(6,4,2600,1142))
	   || (_sbe_dll    >zero && _sbe_dll    <fver(6,4,2600,1142)) )) {
		NN("Update for Windows XP Service Pack 1 (KB810243)");
		XX(p1+"windowsxp-kb810243-x86-enu_f9221252ff64f016a59490bfacdd617.exe"+a7);
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_dhcpcsvc_dll >zero && _dhcpcsvc_dll <fver(5,1,2600,1264))
	   || (_ndis_sys     >zero && _ndis_sys     <fver(5,1,2600,1254))
	   || (_ndisuio_sys  >zero && _ndisuio_sys  <fver(5,1,2600,1254))
	   || (_netshell_dll >zero && _netshell_dll <fver(5,1,2600,1254))
	   || (_wzcdlg_dll   >zero && _wzcdlg_dll   <fver(5,1,2600,1276))
	   || (_wzcsapi_dll  >zero && _wzcsapi_dll  <fver(5,1,2600,1276))
	   || (_wzcsvc_dll   >zero && _wzcsvc_dll   <fver(5,1,2600,1276)) )) {
		NN("Update for Microsoft Windows XP (KB826942)");
		XX(p1+"windowsxp-kb826942-x86-enu_66c94466abeca8e6a2ce8d52a4e900f.exe"+a6);
	}
	if( sp==0 && (sku & XP_ALL) && _msctf_dll>zero && _msctf_dll<fver(5,1,2600,29)) {
		NN("Keyboard Layout Update");
		XX(rtm+"Q318388WXPSP1.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _gdiplus_dll_0<fver(5,1,3100,0)) {
		NN("Q318966: Recommended Update");
		XX(rtm+"Q318966_x86.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && _srchui_dll>zero && _srchui_dll<fver(1,0,0,2714)) {
		NN("Q319949: Recommended Update");
		XX(rtm+"Q319949_WXP_SP1_x86_ENU.exe"+a7);
	}
	if( sp<2 && (sku & (XP_PRO|XP_PRON)) && (
	    (_HscUpd_exe   >zero && _HscUpd_exe   <fver(5,1,2600,48))
	 || (regQueryValue(L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{abcdf74f-9a64-4e6e-b8eb-6e5a41de6550}\\0409",L"Version",&status)!=L"1.0.0.2")
	)) {
		NN("Q327405: Recommended Update (Windows XP Professional)");
		XX(sw+rtm+"hu1002_pro_d3adfeca5d27b538bfb5243dbf39a9034f2a5019.exe"+a2);
	}
	if( sp<2 && (sku & (XP_HOME|XP_HOMEN)) && (
	    (_HscUpd_exe   >zero && _HscUpd_exe   <fver(5,1,2600,48))
	 || (regQueryValue(L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{abcdf74f-9a64-4e6e-b8eb-6e5a41de6550}\\0409",L"Version",&status)!=L"1.0.0.2")
	)) {
		NN("Q327405: Recommended Update (Windows XP Home Edition)");
		XX(sw+rtm+"hu1002_per_f08bc10e7760c0d84cd27effd4c32e5965c2b142.exe"+a2);
	}
	if( sp>=0 && (sku & XP_ALL) && (
	      (_aec_sys      >zero && _aec_sys      <fver(5,1,2601,0))
	   || (_dxmrtp_dll   >zero && _dxmrtp_dll   <fver(5,1,2600,16))
	   || (_rtcdll_dll   >zero && _rtcdll_dll   <fver(5,1,2600,28))
	   || (_splitter_sys >zero && _splitter_sys <fver(5,1,2600,16))
	   || (_usbaudio_sys >zero && _usbaudio_sys <fver(5,1,2600,21))
	   || (_usbuhci_sys  >zero && _usbuhci_sys  <fver(5,1,2600,28)) )) {
		NN("Windows Messenger Audio Update");
		XX(rtm+"q316397_wxpx86_cb2ee9c9013d6a1a0db49d4353ff47dfb68310bd.exe"+a7);
	}
	if( sp==0 && (sku & XP_ALL) && (
	      (_dxmasf_dll   >zero && _dxmasf_dll   <fver(6,4,9,1121))
//	   || (_httpod51_dll >zero && _httpod51_dll <fver(5,1,2600,1))
	   || (_httpodbc_dll > zero && _httpodbc_dll<=fver(5,1,2600,1)
			&& strncmp(_httpodbc_dll_md5,"504486b0f3c976c26201ad03a86a35a3",32) != 0)
	   || (_lsasrv_dll   >zero && _lsasrv_dll   <fver(5,1,2600,14))
	   || (_msdxm_ocx    >zero && _msdxm_ocx    <fver(6,4,9,1121))
	   || (_sfcfiles_dll >zero && _sfcfiles_dll <fver(5,1,2600,16))
	   || (_ssdpapi_dll  >zero && _ssdpapi_dll  <fver(5,1,2600,15))
	   || (_ssdpsrv_dll	 >zero && _ssdpsrv_dll  <fver(5,1,2600,15))
//	   || (_ssinc51_dll  >zero && _ssinc51_dll  <fver(5,1,2600,1))
	   || (_ssinc_dll    >zero && _ssinc_dll<=fver(5,1,2600,1)
			&& strncmp(_ssinc_dll_md5,"45d1cacd02a55cb26d364c7e773441d6",32) != 0)
	   || (_url_dll      >zero && _url_dll      <fver(6,0,2600,1))
	   || (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2600,1)) )) {
		NN("Windows XP Update Package, October 25, 2001");
		XX(rtm+"q309521_x86_3c970d04ded697e0dc2daded5d10a9974534db79.exe"+a7);
	}
	if((sp==0 && (sku & XP_ALL) && (
	      (_crypt32_dll  >zero && _crypt32_dll  <fver(5,131,2600,1123))
	   || (_hh_exe       >zero && _hh_exe       <fver(5,2,3644,0))
	   || (_hhctrl_ocx   >zero && _hhctrl_ocx   <fver(5,2,3735,0))
	   || (_hhsetup_dll  >zero && _hhsetup_dll  <fver(5,2,3644,0))
	   || (_html32_cnv   >zero && _html32_cnv   <fver(2003,1100,5426,0))
	   || (_itircl_dll   >zero && _itircl_dll   <fver(5,2,3644,0))
	   || (_itss_dll     >zero && _itss_dll     <fver(5,2,3644,0))
	   || (_locator_exe  >zero && _locator_exe  <fver(5,1,2600,108))
	   || (_mrxsmb_sys   >zero && _mrxsmb_sys   <fver(5,1,2600,106))
	   || (_msconv97_dll >zero && _msconv97_dll <fver(2003,1100,5426,0))
	   || (_newdev_dll   >zero && _newdev_dll   <fver(5,1,2600,109))
	   || (_ntdll_dll    >zero && _ntdll_dll    <fver(5,1,2600,114))
	   || (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,108))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,108))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,108))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,108))
	   || (_ole32_dll    >zero && _ole32_dll    <fver(5,1,2600,118))
	   || (_raspptp_sys  >zero && _raspptp_sys  <fver(5,1,2600,101))
	   || (_rpcrt4_dll   >zero && _rpcrt4_dll   <fver(5,1,2600,109))
	   || (_rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,118))
	   || (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2600,115))
	   || (_shmedia_dll  >zero && _shmedia_dll  <fver(6,0,2600,101))
	   || (_srrstr_dll   >zero && _srrstr_dll   <fver(5,1,2600,101))
	   || (_srv_sys      >zero && _srv_sys      <fver(5,1,2600,112))
//	   || (_sysmain_sdb  >zero && _sysmain_sdb  <fver())
	   || (_user32_dll   >zero && _user32_dll   <fver(5,1,2600,104))
	   || (_win32k_sys   >zero && _win32k_sys   <fver(5,1,2600,104))
	   || (_winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,104))
	   || (_zipfldr_dll  >zero && _zipfldr_dll  <fver(6,0,2600,101)) ))
	 ||(sp==1 && (sku & XP_ALL) && (
	      (_accwiz_exe   >zero && _accwiz_exe   <fver(5,1,2600,1143))
	   || (_crypt32_dll  >zero && _crypt32_dll  <fver(5,131,2600,1123))
	   || (_cryptsvc_dll >zero && _cryptsvc_dll <fver(5,1,2600,1190))
	   || (_hh_exe       >zero && _hh_exe       <fver(5,2,3644,0))
	   || (_hhctrl_ocx   >zero && _hhctrl_ocx   <fver(5,2,3735,0))
	   || (_hhsetup_dll  >zero && _hhsetup_dll  <fver(5,2,3644,0))
	   || (_html32_cnv   >zero && _html32_cnv   <fver(2003,1100,5426,0))
	   || (_itircl_dll   >zero && _itircl_dll   <fver(5,2,3644,0))
	   || (_itss_dll     >zero && _itss_dll     <fver(5,2,3644,0))
	   || (_locator_exe  >zero && _locator_exe  <fver(5,1,2600,1147))
	   || (_magnify_exe  >zero && _magnify_exe  <fver(5,1,2600,1143))
	   || (_migwiz_exe   >zero && _migwiz_exe   <fver(5,1,2600,1143))
	   || (_mrxsmb_sys   >zero && _mrxsmb_sys   <fver(5,1,2600,1143))
	   || (_msconv97_dll >zero && _msconv97_dll <fver(2003,1100,5426,0))
	   || (_narrator_exe >zero && _narrator_exe <fver(5,1,2600,1143))
	   || (_newdev_dll   >zero && _newdev_dll   <fver(5,1,2600,1164))
	   || (_ntdll_dll    >zero && _ntdll_dll    <fver(5,1,2600,1217))
	   || (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,1151))
	   || (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,1151))
	   || (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,1151))
	   || (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,1151))
	   || (_ole32_dll    >zero && _ole32_dll    <fver(5,1,2600,1263))
	   || (_osk_exe      >zero && _osk_exe      <fver(5,1,2600,1143))
	   || (_pchshell_dll >zero && _pchshell_dll <fver(5,1,2600,1143))
	   || (_raspptp_sys  >zero && _raspptp_sys  <fver(5,1,2600,1129))
	   || (_rpcrt4_dll   >zero && _rpcrt4_dll   <fver(5,1,2600,1254))
	   || (_rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,1263))
	   || (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1145))
	   || (_shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1233))
	   || (_shmedia_dll  >zero && _shmedia_dll  <fver(6,0,2800,1125))
	   || (_srrstr_dll   >zero && _srrstr_dll   <fver(5,1,2600,1142))
	   || (_srv_sys      >zero && _srv_sys      <fver(5,1,2600,1193))
//	   || (_sysmain_sdb  >zero && _sysmain_sdb  <fver())
	   || (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1143))
	   || (_user32_dll   >zero && _user32_dll   <fver(5,1,2600,1134))
	   || (_win32k_sys   >zero && _win32k_sys   <fver(5,1,2600,1134))
	   || (_winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,1134))
	   || (_zipfldr_dll  >zero && _zipfldr_dll  <fver(6,0,2800,1126)) ))) {
		NN("Update Rollup 1 for Microsoft Windows XP (KB826939)");
		XX(p1+"WindowsXP-KB826939-x86-ENU.exe"+a6);
	}

	// Windows XP SP2 updates
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
	if((sp==1 && (sku & XP_ALL) && _srvsvc_dll>zero && _srvsvc_dll<fver(5,1,2600,1613))
	 ||(sp==2 && (sku & XP_ALL) && _srvsvc_dll>zero && _srvsvc_dll<fver(5,1,2600,2577))) {
		NN("Security Update for Windows XP (KB888302)");
		XX(p2+"windowsxp-kb888302-x86-enu_5c4ef905021d66aa78d9f5f112e5d05c40b1a909.exe"+a6);
	}
	/*if( sp==2 && (sku & XP_ALL) && _hhctrl_ocx>zero && _hhctrl_ocx<fver(5,2,3790,2847)) {
		// KB928843 is replaced with revised patch KB935448
		NN("Security Update for Windows XP (KB928843)");
		XX(p+"windowsxp-kb928843-x86-enu_80eb8779856aefa25bceab8926940fbdebabdc23.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _hhctrl_ocx>zero && (
				_hhctrl_ocx<fver(5,2,3790,2847)
				|| !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB935448")) ) {
		NN("Update for Windows XP (KB935448)");
		XX(p2+"windowsxp-kb935448-x86-enu_f7a4f05c7979ea17ea0300ba131e0655c66f8eab.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _snmp_exe>zero && _snmp_exe<fver(5,1,2600,3038)) {
		NN("Security Update for Windows XP (KB926247)");
		XX(p2+"windowsxp-kb926247-x86-enu_f465277dda6008156bb6f81bc77c72253a91d303.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && (
		           (sp==2 && _wordpad_exe>zero && _wordpad_exe<fver(5,1,2600,3355))
				|| (sp==2 && _mswrd8_wpc >zero && _mswrd8_wpc <fver(10,0,803,10))
				|| (sp==3 && _wordpad_exe>zero && _wordpad_exe<fver(5,1,2600,5584))
				|| (sp==3 && _mswrd8_wpc >zero && _mswrd8_wpc <fver(2007,10,31,10)) )) {
		NN("Security Update for Windows XP (KB923561)");
		XX(p3+"WindowsXP-KB923561-x86-ENU.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  (_cscdll_dll>zero && _cscdll_dll<fver(5,1,2600,1599))
					  ||  (_mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,1836))
					  ||  (_rdbss_sys >zero && _rdbss_sys <fver(5,1,2600,1836))))
	 ||(sp==2 && (sku & XP_ALL) && ( (_mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,2902))
					  ||  (_rdbss_sys>zero && _rdbss_sys<fver(5,1,2600,2902))))) {
		NN("Security Update for Windows XP (KB914389)");
		XX(p2+"windowsxp-kb914389-x86-enu_8c44336e9e4f287891ac384bee0219e9c2224523.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,3675)) {
	//if( sp==3 && (sku & XP_ALL) && _mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,5944)) {
		NN("Security Update for Windows XP (KB980232)");
		XX(p2+"windowsxp-kb980232-x86-enu_d137985d21958a9f3f277189287b9a71eee02421.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && ( (_dnsapi_dll>zero && _dnsapi_dll<fver(5,1,2600,1863))
					  ||  (_rasadhlp_dll>zero && _rasadhlp_dll<fver(5,1,2600,1863))))
	 ||(sp==2 && (sku & XP_ALL) && ( (_dnsapi_dll>zero && _dnsapi_dll<fver(5,1,2600,2938))
					  ||  (_rasadhlp_dll>zero && _rasadhlp_dll<fver(5,1,2600,2938))))) {
		NN("Security Update for Windows XP (KB920683)");
		XX(p2+"windowsxp-kb920683-x86-enu_ef1482c5b88557e56563dace9b7549ebf6d7f9c7.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _oleaut32_dll>zero && _oleaut32_dll<fver(5,1,2600,3266)) {
		NN("Security Update for Windows XP (KB943055)");
		XX(p2+"windowsxp-kb943055-x86-enu_e160e1b140872892d2d0050449b1606cc85b5654.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,3316)) {
		// KB948590 is replaced by KB956802 in SP2
		NN("Security Update for Windows XP (KB948590)");
		XX(p+"windowsxp-kb948590-x86-enu_f94e1dbe8e74d3da8f3979e1cae71360c0c11a8e.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,3466)) {
	//if( sp==3 && (sku & XP_ALL) && _gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,5698)) {
		NN("Security Update for Windows XP (KB956802)");
		XX(p2+"windowsxp-kb956802-x86-enu_bf0a8bfe0d01487a999fb8f66b3719957acf1f3a.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _msxml3_dll>zero && _msxml3_dll<fver(8,90,1101,0)) {
		// KB936021 is replaced by KB955069 on SP2
		NN("Security Update for Windows XP (KB936021)");
		XX(p+"windowsxp-kb936021-x86-enu_9952bfa53c32d996780807ca1c1ab1c9ca398237.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _msxml3_dll>zero && _msxml3_dll<fver(8,100,1048,0)) { // also installable on SP3, same payload
		NN("Security Update for Windows XP (KB955069)");
		XX(p2+"windowsxp-kb955069-x86-enu_fa864585a7d761ba0f940eff151672871d0e69f3.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && ( (_msdtcprx_dll>zero && _msdtcprx_dll<fver(2001,12,4414,65))
					  ||  (_msdtctm_dll>zero && _msdtctm_dll<fver(2001,12,4414,65))
					  ||  (_msdtcuiu_dll>zero && _msdtcuiu_dll<fver(2001,12,4414,65))
					  ||  (_mtxclu_dll>zero && _mtxclu_dll<fver(2001,12,4414,65))
					  ||  (_mtxoci_dll>zero && _mtxoci_dll<fver(2001,12,4414,65))
					  ||  (_xolehlp_dll>zero && _xolehlp_dll<fver(2001,12,4414,65))))
	 ||(sp==2 && (sku & XP_ALL) && ( (_msdtcprx_dll>zero && _msdtcprx_dll<fver(2001,12,4414,311))
					  ||  (_msdtctm_dll>zero && _msdtctm_dll<fver(2001,12,4414,311))
					  ||  (_msdtcuiu_dll>zero && _msdtcuiu_dll<fver(2001,12,4414,311))
					  ||  (_mtxclu_dll>zero && _mtxclu_dll<fver(2001,12,4414,311))
					  ||  (_mtxoci_dll>zero && _mtxoci_dll<fver(2001,12,4414,311))
					  ||  (_xolehlp_dll>zero && _xolehlp_dll<fver(2001,12,4414,311))))) {
		NN("Security Update for Windows XP (KB913580)");
		XX(p2+"windowsxp-kb913580-x86-enu_f57aa2fdaf623d8b0231fc928c00ad8498d37c76.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _netapi32_dll>zero && _netapi32_dll<fver(5,1,2600,3462)) {
	//if( sp==3 && (sku & XP_ALL) && _netapi32_dll>zero && _netapi32_dll<fver(5,1,2600,5694)) {
		NN("Security Update for Windows XP (KB958644)");
		XX(p2+"windowsxp-kb958644-x86-enu_5c135a8dae5721849430afe27af255f83e64f62b.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && ( (_mqac_sys>zero && _mqac_sys<fver(5,1,0,1109))
					  ||  (_mqad_dll    >zero && _mqad_dll    <fver(5,1,0,1109))
					  ||  (_mqdscli_dll >zero && _mqdscli_dll <fver(5,1,0,1109))
					  ||  (_mqise_dll   >zero && _mqise_dll   <fver(5,1,0,1109))
					  ||  (_mqqm_dll    >zero && _mqqm_dll    <fver(5,1,0,1109))
					  ||  (_mqrt_dll    >zero && _mqrt_dll    <fver(5,1,0,1109))
					  ||  (_mqsec_dll   >zero && _mqsec_dll   <fver(5,1,0,1109))
					  ||  (_mqupgrd_dll >zero && _mqupgrd_dll <fver(5,1,0,1109))
					  ||  (_mqutil_dll  >zero && _mqutil_dll  <fver(5,1,0,1109)) )) {
		// KB937894 is replaced by KB971032 on SP2
		NN("Security Update for Windows XP (KB937894)");
		XX(p+"windowsxp-kb937894-x86-enu_4546c1f99c7d0d557b5c454c961798eab01e3444.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && ( (_mqac_sys>zero && _mqac_sys<fver(5,1,0,1111))
					  ||  (_mqad_dll    >zero && _mqad_dll    <fver(5,1,0,1111))
					  ||  (_mqbkup_exe  >zero && _mqbkup_exe  <fver(5,1,0,1111))
					  ||  (_mqdscli_dll >zero && _mqdscli_dll <fver(5,1,0,1111))
					  ||  (_mqise_dll   >zero && _mqise_dll   <fver(5,1,0,1111))
					  ||  (_mqoa_dll    >zero && _mqoa_dll    <fver(5,1,0,1111))
					  ||  (_mqqm_dll    >zero && _mqqm_dll    <fver(5,1,0,1111))
					  ||  (_mqrt_dll    >zero && _mqrt_dll    <fver(5,1,0,1111))
					  ||  (_mqrtdep_dll >zero && _mqrtdep_dll <fver(5,1,0,1111))
					  ||  (_mqsec_dll   >zero && _mqsec_dll   <fver(5,1,0,1111))
					  ||  (_mqsnap_dll  >zero && _mqsnap_dll  <fver(5,1,0,1111))
					  ||  (_mqsvc_exe   >zero && _mqsvc_exe   <fver(5,1,0,1111))
					  ||  (_mqtgsvc_exe >zero && _mqtgsvc_exe <fver(5,1,0,1111))
					  ||  (_mqtrig_dll  >zero && _mqtrig_dll  <fver(5,1,0,1111))
					  ||  (_mqupgrd_dll >zero && _mqupgrd_dll <fver(5,1,0,1111))
					  ||  (_mqutil_dll  >zero && _mqutil_dll  <fver(5,1,0,1111))
					  ||  (_msmqocm_dll >zero && _msmqocm_dll <fver(5,1,0,1111)) )) {
		NN("Security Update for Windows XP (KB971032)");
		XX(p2+"windowsxp-kb971032-x86-enu_23f7ac835ed26257c74a7b1ef5caa6198182cf6c.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _quartz_dll>zero && _quartz_dll<fver(6,5,2600,3243)) {
		// KB941568 is replaced by KB975562 on SP2
		NN("Security Update for Windows XP (KB941568)");
		XX(p+"windowsxp-kb941568-x86-enu_e62eb6119afa388291cf71da243fb265afe31e60.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _quartz_dll>zero && _quartz_dll<fver(6,5,2600,3665)) {
	//if( sp==3 && (sku & XP_ALL) && _quartz_dll>zero && _quartz_dll<fver(6,5,2600,5933)) {
		NN("Security Update for Windows XP (KB975562)");
		XX(p2+"windowsxp-kb975562-x86-enu_6826ab2c0f8ce1e717e693036ae4662c7fcc7277.exe"+a1);
	}
	if(    (sp==2 && (sku & XP_ALL) && _mswebdvd_dll>zero && _mswebdvd_dll<fver(6,5,2600,3610))
		|| (sp==3 && (sku & XP_ALL) && _mswebdvd_dll>zero && _mswebdvd_dll<fver(6,5,2600,5857))) {
		NN("Security Update for Windows XP (KB973815)");
		XX(p3+"WindowsXP-KB973815-x86-ENU.exe"+a1);
	}
	if(    (sp==2 && (sku & XP_PLUS_FLP) && _winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,3494))
		|| (sp==3 && (sku & XP_PLUS_FLP) && _winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,5727))) {
		NN("Security Update for Windows XP (KB960803)");
		XX(p3+"WindowsXP-KB960803-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,3619)) {
	//if( sp==3 && (sku & XP_ALL) && _winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,5868)) {
		NN("Update for Windows XP (KB971737)");
		XX(p2+"windowsxp-kb971737-x86-enu_e2aa4340aaf68e97a5dbdd278edb9182c4383ed7.exe"+a1);
	}
	if(    (sp==2 && (sku & XP_ALL) && _asycfilt_dll>zero && _asycfilt_dll<fver(5,1,2600,3680))
		|| (sp==3 && (sku & XP_ALL) && _asycfilt_dll>zero && _asycfilt_dll<fver(5,1,2600,5949))) {
		NN("Security Update for Windows XP (KB979482)");
		XX(p3+"WindowsXP-KB979482-x86-ENU.exe"+a1);
	}
	if(    (sp==2 && (sku & XP_ALL) && _cabview_dll>zero && _cabview_dll<fver(6,0,2900,3663))
		|| (sp==3 && (sku & XP_ALL) && _cabview_dll>zero && _cabview_dll<fver(6,0,2900,5927))) {
		NN("Security Update for Windows XP (KB979309)");
		XX(p3+"WindowsXP-KB979309-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_dao360_dll>zero && _dao360_dll<fver(3,60,9512,0))
					  ||  (_msexch40_dll >zero && _msexch40_dll <fver(4,0,9502,0))
					  ||  (_msexcl40_dll >zero && _msexcl40_dll <fver(4,0,9502,0))
					  ||  (_msjet40_dll  >zero && _msjet40_dll  <fver(4,0,9511,0))
					  ||  (_msjetol1_dll >zero && _msjetol1_dll <fver(4,0,9502,0))
					  ||  (_msjint40_dll >zero && _msjint40_dll <fver(4,0,9502,0))
					  ||  (_msjter40_dll >zero && _msjter40_dll <fver(4,0,9502,0))
					  ||  (_msjtes40_dll >zero && _msjtes40_dll <fver(4,0,9502,0))
					  ||  (_msltus40_dll >zero && _msltus40_dll <fver(4,0,9502,0))
					  ||  (_mspbde40_dll >zero && _mspbde40_dll <fver(4,0,9502,0))
					  ||  (_msrd2x40_dll >zero && _msrd2x40_dll <fver(4,0,9502,0))
					  ||  (_msrd3x40_dll >zero && _msrd3x40_dll <fver(4,0,9502,0))
					  ||  (_msrepl40_dll >zero && _msrepl40_dll <fver(4,0,9502,0))
					  ||  (_mstext40_dll >zero && _mstext40_dll <fver(4,0,9502,0))
					  ||  (_mswdat10_dll >zero && _mswdat10_dll <fver(4,0,9502,0))
					  ||  (_mswstr10_dll >zero && _mswstr10_dll <fver(4,0,9502,0))
					  ||  (_msxbde40_dll >zero && _msxbde40_dll <fver(4,0,9502,0)) )) {
		NN("Security Update for Windows XP (KB950749)");
		XX(p2+"windowsxp-kb950749-x86-enu_52c183782f97ebcc7c92e0d71e0525a540898507.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _rpcrt4_dll>zero && _rpcrt4_dll<fver(5,1,2600,3173)) {
		// KB933729 is replaced by KB970238 on SP2
		NN("Security Update for Windows XP (KB933729)");
		XX(p+"windowsxp-kb933729-x86-enu_5b4ed1a7e5586b93a367156607ace191dc4bf53a.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _rpcrt4_dll>zero && _rpcrt4_dll<fver(5,1,2600,3555)) {
	//if( sp==3 && (sku & XP_ALL) && _rpcrt4_dll>zero && _rpcrt4_dll<fver(5,1,2600,5795)) {
		NN("Security Update for Windows XP (KB970238)");
		XX(p2+"windowsxp-kb970238-x86-enu_82bd58dd365d93afa746a4beaf2a1ad5b8d5181a.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _tcpip_sys>zero && _tcpip_sys<fver(5,1,2600,3244)) {
		// KB941644 is replaced by KB951748 on SP2
		NN("Security Update for Windows XP (KB941644)");
		XX(p+"windowsxp-kb941644-x86-enu_69bd9e4e22c97592dca087165e4fef304230f7a5.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && ( (_6to4svc_dll>zero && _6to4svc_dll<fver(5,1,2600,2975))
					  ||  (_afd_sys     >zero && _afd_sys     <fver(5,1,2600,3394))
					  ||  (_dnsapi_dll  >zero && _dnsapi_dll  <fver(5,1,2600,3394))
					  ||  (_mswsock_dll >zero && _mswsock_dll <fver(5,1,2600,3394))
					  ||  (_tcpip6_sys  >zero && _tcpip6_sys  <fver(5,1,2600,3394))
					  ||  (_tcpip_sys   >zero && _tcpip_sys   <fver(5,1,2600,3394)) )) {
	//if( sp==3 && (sku & XP_ALL) && ( (_afd_sys     >zero && _afd_sys     <fver(5,1,2600,5625))
	//				  ||  (_dnsapi_dll  >zero && _dnsapi_dll  <fver(5,1,2600,5625))
	//				  ||  (_mswsock_dll >zero && _mswsock_dll <fver(5,1,2600,5625))
	//				  ||  (_tcpip6_sys  >zero && _tcpip6_sys  <fver(5,1,2600,5625))
	//				  ||  (_tcpip_sys   >zero && _tcpip_sys   <fver(5,1,2600,5625)) )) {
		NN("Security Update for Windows XP (KB951748)");
		XX(p2+"windowsxp-kb951748-x86-enu_6b7598e550ef967b83e9c088f8cd3409632f5d23.exe"+a1);
	}
	if( (sp==2 && (sku & XP_ALL) && ( (_avifil32_dll>zero && _avifil32_dll<fver(5,1,2600,3649))
		              ||  (_iyuv_32_dll  >zero && _iyuv_32_dll  <fver(5,1,2600,3649))
					  ||  (_msrle32_dll  >zero && _msrle32_dll  <fver(5,1,2600,3649))
					  ||  (_msvidc32_dll >zero && _msvidc32_dll <fver(5,1,2600,3649))
					  ||  (_tsbyuv_dll   >zero && _tsbyuv_dll   <fver(5,1,2600,3649)) ))
	|| (sp==3 && (sku & XP_ALL) && ( (_avifil32_dll>zero && _avifil32_dll<fver(5,1,2600,5908))
		              ||  (_iyuv_32_dll  >zero && _iyuv_32_dll  <fver(5,1,2600,5908))
					  ||  (_msrle32_dll  >zero && _msrle32_dll  <fver(5,1,2600,5908))
					  ||  (_msvidc32_dll >zero && _msvidc32_dll <fver(5,1,2600,5908))
					  ||  (_tsbyuv_dll   >zero && _tsbyuv_dll   <fver(5,1,2600,5908)) ))) {
		NN("Security Update for Windows XP (KB977914)");
		XX(p3+"WindowsXP-KB977914-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_dnsapi_dll>zero && _dnsapi_dll<fver(5,1,2600,3316))
					  ||  (_dnsrslvr_dll>zero && _dnsrslvr_dll<fver(5,1,2600,3316)) )) {
		NN("Security Update for Windows XP (KB945553)");
		XX(p2+"windowsxp-kb945553-x86-enu_94dfff615b4f4fb15d3db5ef261a964d65f60492.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_agentdp2_dll>zero && _agentdp2_dll<fver(2,0,0,3424))
					  ||  (_agentdpv_dll>zero && _agentdpv_dll<fver(2,0,0,3424))
					  ||  (_agentsvr_exe>zero && _agentsvr_exe<fver(2,0,0,3424)) )) {
		NN("Security Update for Windows XP (KB920213)");
		XX(p2+"windowsxp-kb920213-x86-enu_02cb394147b09e8926b4f8334feeff4b8fa4b33b.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _agentdpv_dll>zero && _agentdpv_dll<fver(2,0,0,3425)) {
		NN("Security Update for Windows XP (KB932168)");
		XX(p2+"windowsxp-kb932168-x86-enu_646e8c2d535461396b8b53ce8fe04410327925a2.exe"+a1);
	}
	if(   (sp==2 && (sku & XP_ALL) && _oakley_dll>zero && _oakley_dll<fver(5,1,2600,3632))
		||(sp==3 && (sku & XP_ALL) && _oakley_dll>zero && _oakley_dll<fver(5,1,2600,5886))) {
		NN("Security Update for Windows XP (KB974392)");
		XX(p3+"WindowsXP-KB974392-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _sxs_dll>zero && _sxs_dll<fver(5,1,2600,3019)) {
		NN("Security Update for Windows XP (KB926255)");
		XX(p2+"windowsxp-kb926255-x86-enu_1737b8dde544dbbc79dddd6f123291b781313c04.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_msftedit_dll>zero && _msftedit_dll<fver(5,41,15,1514))
					  ||  (_riched20_dll>zero && _riched20_dll<fver(5,30,23,1228)) )) {
		NN("Security Update for Windows XP (KB918118)");
		XX(p2+"windowsxp-kb918118-x86-enu_8c38e06d795c410b8a072506af40ad81d64f1d6d.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		      (_mswrd632_wpc>zero && _mswrd632_wpc<fver(2004,10,25,0))
		   || (_wordpad_exe>zero && _wordpad_exe<fver(5,1,2600,1606))
	       || !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB885836")))
	 ||(sp==2 && (sku & XP_ALL) && ((_mswrd6_wpc>zero && _mswrd6_wpc<fver(10,0,803,2))
	       || !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB885836")))) {
		NN("Security Update for Windows XP (KB885836)");
		XX(p2+"windowsxp-kb885836-x86-enu_f87074f42947ee275445bdd34dda472871ed3b41.exe"+a6);
	}
	if( sp>=2 && (sku & XP_ALL) && ( (_html32_cnv>zero && _html32_cnv   <fver(2003,1100,8165,0))
		              ||  (_msconv97_dll         >zero && _msconv97_dll <fver(2003,1100,8165,0))
		              ||  (_mswrd6_wpc           >zero && _mswrd6_wpc   <fver(2009,10,31,10))
					  ||  (_mswrd8_wpc           >zero && _mswrd8_wpc   <fver(2009,10,31,10))  
					  ||  (_mswrd632_wpc         >zero && _mswrd632_wpc <fver(2009,10,31,10))
					  ||  (_mswrd832_cnv         >zero && _mswrd832_cnv <fver(2003,1100,8313,0))
					  ||  (_write32_wpc          >zero && _write32_wpc  <fver(2009,10,31,10))
					  ||  (_write_wpc            >zero && _write_wpc    <fver(2009,10,31,10)) )) {
		NN("Security Update for Windows XP (KB973904)");
		XX(p3+"WindowsXP-KB973904-x86-ENU.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2800,1579))
					  ||  ( _shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1873))
					  ||  ( _sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1579))
					  ||  ( _comctl32_dll >zero && _comctl32_dll <fver(5,82,2800,1891))
					  ||  ( _comctl32_dll_1891 < fver(6,0,2800,1891)) ))
	 ||(sp==2 && (sku & XP_ALL) && ( (_comctl32_dll>zero && _comctl32_dll<fver(5,82,2900,2982))
					  ||  ( _comctl32_dll_2982<fver(6,0,2900,2982))))) {
		NN("Security Update for Windows XP (KB923191)");
		XX(p2+"windowsxp-kb923191-x86-enu_9d2cfed124f1f50804c20a6e8a881f84c266745f.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,3335)) {
		// KB941693 is replaced by KB979559 on SP2
		NN("Security Update for Windows XP (KB941693)");
		XX(p+"windowsxp-kb941693-x86-enu_c2a97173c92d11672036e8a5e0a062f79460c6f1.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,3706)) {
	//if( sp==3 && (sku & XP_ALL) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,5976)) {
		NN("Security Update for Windows XP (KB979559)");
		XX(p2+"windowsxp-kb979559-x86-enu_4d87c29e02a9f212a012f3e035f91e68c8fc60be.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _wintrust_dll>zero && _wintrust_dll<fver(5,131,2600,3661)) {
	//if( sp==3 && (sku & XP_ALL) && _wintrust_dll>zero && _wintrust_dll<fver(5,131,2600,5922)) {
		NN("Security Update for Windows XP (KB978601)");
		XX(p2+"windowsxp-kb978601-x86-enu_49ae6ac4f6e0a609124f2ce6f80fcef249273c02.exe"+a1);
	}
	if(   (sp==2 && (sku & XP_ALL) && _rmcast_sys>zero && _rmcast_sys<fver(5,1,2600,3369))
		||(sp==3 && (sku & XP_ALL) && _rmcast_sys>zero && _rmcast_sys<fver(5,1,2600,5598))) {
		NN("Security Update for Windows XP (KB950762)");
		XX(p3+"windowsxp-kb950762-x86-enu_bfa04c9d2e62b4695d1bb8953486788c8a8c11e4.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
	             (_6to4svc_dll  >zero && _6to4svc_dll  <fver(5,1,2600,1847))
	          || (_dhcpcsvc_dll >zero && _dhcpcsvc_dll <fver(5,1,2600,1847))
	          || (_dnsapi_dll   >zero && _dnsapi_dll   <fver(5,1,2600,1847))
			  || (_inetmib1_dll >zero && _inetmib1_dll <fver(5,1,2600,1847))
			  || (_iphlpapi_dll >zero && _iphlpapi_dll <fver(5,1,2600,1847))
			  || (_ipv6_exe     >zero && _ipv6_exe     <fver(5,1,2600,1847))
			  || (_ipv6mon_dll  >zero && _ipv6mon_dll  <fver(5,1,2600,1847))
			  || (_netsh_exe    >zero && _netsh_exe    <fver(5,1,2600,1847))
			  || (_tcpip6_sys   >zero && _tcpip6_sys   <fver(5,1,2600,1847))
			  || (_tunmp_sys    >zero && _tunmp_sys    <fver(5,1,2600,1847))
			  || (_ws2_32_dll   >zero && _ws2_32_dll   <fver(5,1,2600,1847))
			  || (_wship6_dll   >zero && _wship6_dll   <fver(5,1,2600,1847))
			  || (_xpob2res_dll >zero && _xpob2res_dll <fver(5,1,2600,1847)) ))
	 ||(sp==2 && (sku & XP_ALL) && ( (_dhcpcsvc_dll>zero && _dhcpcsvc_dll<fver(5,1,2600,2912))
					  ||  (_dnsapi_dll>zero && _dnsapi_dll<fver(5,1,2600,2912))
					  ||  (_iphlpapi_dll>zero && _iphlpapi_dll<fver(5,1,2600,2912))))) {
		NN("Security Update for Windows XP (KB914388)");
		XX(p2+"windowsxp-kb914388-x86-enu_21992f2ad7f7fd8ab28e854ce364ebc4f8baf810.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _winsrv_dll>zero && _winsrv_dll<fver(5,1,2600,3103)) {
		NN("Security Update for Windows XP (KB930178)");
		XX(p2+"windowsxp-kb930178-x86-enu_5e02462613354e7679cdd79b0ee52083efd9aa30.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _atl_dll>zero && _atl_dll<fver(3,5,2284,2)) {
		NN("Security Update for Windows XP (KB973507)");
		XX(p3+"WindowsXP-KB973507-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _oledlg_dll>zero && _oledlg_dll<fver(5,1,2600,3016)) {
		NN("Security Update for Windows XP (KB926436)");
		XX(p2+"windowsxp-kb926436-x86-enu_98f46d49f189f01c14a7d5360d794da20edae885.exe"+a1);
	}
	if(    (sp==2 && (sku & XP_ALL) && ( (_msyuv_dll >zero && _msyuv_dll <fver(5,3,2600,3649))
					  ||                 (_quartz_dll>zero && _quartz_dll<fver(6,5,2600,3649))))
		|| (sp==3 && (sku & XP_ALL) && ( (_msyuv_dll >zero && _msyuv_dll <fver(5,3,2600,5908))
					  ||                 (_quartz_dll>zero && _quartz_dll<fver(6,5,2600,5908))))) {
		NN("Security Update for Windows XP (KB975560)");
		XX(p3+"WindowsXP-KB975560-x86-ENU.exe"+a1);
	}
	if(    (sp==2 && (sku & XP_ALL) && _msasn1_dll>zero && _msasn1_dll<fver(5,1,2600,3624))
		|| (sp==3 && (sku & XP_ALL) && _msasn1_dll>zero && _msasn1_dll<fver(5,1,2600,5875))) {
		NN("Security Update for Windows XP (KB974571)");
		XX(p3+"WindowsXP-KB974571-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_msdtclog_dll >zero && _msdtclog_dll <fver(2001,12,4414,320))
		              ||             (_msdtcprx_dll >zero && _msdtcprx_dll <fver(2001,12,4414,320))
					  ||             (_msdtctm_dll  >zero && _msdtctm_dll  <fver(2001,12,4414,320))
					  ||             (_msdtcuiu_dll >zero && _msdtcuiu_dll <fver(2001,12,4414,320))
					  ||             (_mtxclu_dll   >zero && _mtxclu_dll   <fver(2001,12,4414,320))
					  ||             (_mtxoci_dll   >zero && _mtxoci_dll   <fver(2001,12,4414,320))))
	|| (sp==3 && (sku & XP_ALL) && ( (_msdtclog_dll >zero && _msdtclog_dll <fver(2001,12,4414,706))
		              ||             (_msdtcprx_dll >zero && _msdtcprx_dll <fver(2001,12,4414,706))
					  ||             (_msdtctm_dll  >zero && _msdtctm_dll  <fver(2001,12,4414,706))
					  ||             (_msdtcuiu_dll >zero && _msdtcuiu_dll <fver(2001,12,4414,706))
					  ||             (_mtxclu_dll   >zero && _mtxclu_dll   <fver(2001,12,4414,706))
					  ||             (_mtxoci_dll   >zero && _mtxoci_dll   <fver(2001,12,4414,706))))) {
		NN("Security Update for Windows XP (KB952004)");
		XX(p3+"WindowsXP-KB952004-x86-ENU.exe"+a1);
	}
	if( (sp==1||sp==2) && (sku & XP_ALL) && _hlink_dll>zero && _hlink_dll<fver(5,2,3790,2748)) {
		NN("Security Update for Windows XP (KB920670)");
		XX(p2+"windowsxp-kb920670-x86-enu_a71ac163b276057101fe92739c06c6e6d143ccf8.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,3099))
					  ||  (_mf3216_dll>zero && _mf3216_dll<fver(5,1,2600,3099))
					  ||  (_user32_dll>zero && _user32_dll<fver(5,1,2600,3099))
					  ||  (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,3099)) )) {
		NN("Security Update for Windows XP (KB925902)");
		XX(p2+"windowsxp-kb925902-x86-enu_a7b984afc7cec6e351d4f1d176db202492ac0e0f.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _dhtmled_ocx>zero && _dhtmled_ocx<fver(6,1,0,9232)) {
//	if( sp>=2 && (sku & XP_ALL) && _dhtmled_ocx>zero && _dhtmled_ocx<fver(6,1,0,9232)) {
		// KB891781 is replaced by KB973869 on SP2
		NN("Security Update for Windows XP (KB891781)");
		XX(p1+"windowsxp-kb891781-x86-enu_32b11076df0189adeb0f36ce3bf7baa01cff1c29.exe"+a6);
	}
	if( sp>=2 && (sku & XP_ALL) && _dhtmled_ocx>zero && _dhtmled_ocx<fver(6,1,0,9247)) {
		NN("Security Update for Windows XP (KB973869)");
		XX(p3+"windowsxp-kb973869-x86-enu_c0679cb2d4c07f35f39ee5d8d7b973c5e4582d34.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _triedit_dll>zero && _triedit_dll<fver(6,1,0,9246)) {
		NN("Security Update for Windows XP (KB956844)");
		XX(p3+"windowsxp-kb956844-x86-enu_926002701569eabda17630d8f9cb45d8ced0ab71.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _localspl_dll>zero && _localspl_dll<fver(5,1,2600,3569))
	 ||(sp==3 && (sku & XP_ALL) && _localspl_dll>zero && _localspl_dll<fver(5,1,2600,5809))) {
		NN("Security Update for Windows XP (KB961501)");
		XX(p3+"windowsxp-kb961501-x86-enu_a5360324a1fe219a9aa9f0e513edffe7305176e4.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _lsasrv_dll>zero && _lsasrv_dll<fver(5,1,2600,3249)) {
		// KB943485 is replaced by KB956572 in SP2
		NN("Security Update for Windows XP (KB943485)");
		XX(p+"windowsxp-kb943485-x86-enu_cb8bf0aa18299dda9578a70411ade45cbd3ae619.exe"+a1);
	}*/
	if((sp==2 && (sku & XP_ALL) && (
		                  (_advapi32_dll >zero && _advapi32_dll <fver(5,1,2600,3520))
					  ||  (_colbact_dll  >zero && _colbact_dll  <fver(2001,12,4414,308))
		              ||  (_fastprox_dll >zero && _fastprox_dll <fver(5,1,2600,3520))
					  ||  (_lsasrv_dll   >zero && _lsasrv_dll   <fver(5,1,2600,3520))
					  ||  (_ntdll_dll    >zero && _ntdll_dll    <fver(5,1,2600,3520))
					  ||  (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,3520))
					  ||  (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,3520))
					  ||  (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,3520))
					  ||  (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,3520))
					  ||  (_pdh_dll      >zero && _pdh_dll      <fver(5,1,2600,3536))
					  ||  (_rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,3520))
					  ||  (_sc_exe       >zero && _sc_exe       <fver(5,1,2600,3520))
					  ||  (_services_exe >zero && _services_exe <fver(5,1,2600,3520))
					  ||  (_wmiprvsd_dll >zero && _wmiprvsd_dll <fver(5,1,2600,3520))
					  ||  (_wmiprvse_exe >zero && _wmiprvse_exe <fver(5,1,2600,3520))))
	||(sp==3 && (sku & XP_ALL) && (
		                  (_advapi32_dll >zero && _advapi32_dll <fver(5,1,2600,5755))
		              ||  (_fastprox_dll >zero && _fastprox_dll <fver(5,1,2600,5755))
					  ||  (_lsasrv_dll   >zero && _lsasrv_dll   <fver(5,1,2600,5755))
					  ||  (_ntdll_dll    >zero && _ntdll_dll    <fver(5,1,2600,5755))
					  ||  (_ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,5755))
					  ||  (_ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,5755))
					  ||  (_ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,5755))
					  ||  (_ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,5755))
					  ||  (_pdh_dll      >zero && _pdh_dll      <fver(5,1,2600,5773))
					  ||  (_rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,5755))
					  ||  (_sc_exe       >zero && _sc_exe       <fver(5,1,2600,5755))
					  ||  (_services_exe >zero && _services_exe <fver(5,1,2600,5755))
					  ||  (_wmiprvsd_dll >zero && _wmiprvsd_dll <fver(5,1,2600,5755))
					  ||  (_wmiprvse_exe >zero && _wmiprvse_exe <fver(5,1,2600,5755))))) {
		NN("Security Update for Windows XP (KB956572)");
		XX(p3+"WindowsXP-KB956572-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _strmdll_dll>zero && _strmdll_dll<fver(4,1,0,3938)) {
		NN("Security Update for Windows XP (KB974112)");
		XX(p3+"WindowsXP-KB974112-x86-ENU.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && ( (_fontsub_dll>zero && _fontsub_dll<fver(5,1,2600,1762))
					  ||  (_t2embed_dll>zero && _t2embed_dll<fver(5,1,2600,1762)) )) {
//	if( sp==2 && (sku & XP_ALL) && ( (_fontsub_dll>zero && _fontsub_dll<fver(5,1,2600,2777))
//					  ||  (_t2embed_dll>zero && _t2embed_dll<fver(5,1,2600,2777)) )) {
		// KB908519 is replaced by KB972270 on SP2
		NN("Security Update for Windows XP (KB908519)");
		XX(p1+"windowsxp-kb908519-x86-enu_ea7ea742f9a3632f1090eab8c66b3fe7735c084f.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_fontsub_dll>zero && _fontsub_dll<fver(5,1,2600,3634))
					  ||  (_t2embed_dll>zero && _t2embed_dll<fver(5,1,2600,3634))))
	 ||(sp==3 && (sku & XP_ALL) && ( (_fontsub_dll>zero && _fontsub_dll<fver(5,1,2600,5888))
					  ||  (_t2embed_dll>zero && _t2embed_dll<fver(5,1,2600,5888))))) {
		NN("Security Update for Windows XP (KB972270)");
		XX(p3+"WindowsXP-KB972270-x86-ENU.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _schannel_dll>zero && _schannel_dll<fver(5,1,2600,3126)) {
		// KB935840 is replaced by KB960225 on SP2
		NN("Security Update for Windows XP (KB935840)");
		XX(p+"windowsxp-kb935840-x86-enu_24d246dfa0195732d275b8f5206f9338f85a2113.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _schannel_dll>zero && _schannel_dll<fver(5,1,2600,3487)) {
	//if( sp==3 && (sku & XP_ALL) && _schannel_dll>zero && _schannel_dll<fver(5,1,2600,5721)) {
		NN("Security Update for Windows XP (KB960225)");
		XX(p2+"windowsxp-kb960225-x86-enu_bae2bc04b963c312a47f36bdea4a8236f7003d71.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _mrxdav_sys>zero && _mrxdav_sys<fver(5,1,2600,3276)) {
		NN("Security Update for Windows XP (KB946026)");
		XX(p2+"windowsxp-kb946026-x86-enu_09eb2e3ee66c5e1be768a4fee49f9f47a0f0b978.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && ( 
		                  (_6to4svc_dll  >zero && _6to4svc_dll  <fver(5,1,2600,1886))
					  ||  (_inetmib1_dll >zero && _inetmib1_dll <fver(5,1,2600,1886))
					  ||  (_iphlpapi_dll >zero && _iphlpapi_dll <fver(5,1,2600,1886))
					  ||  (_ipv6_exe     >zero && _ipv6_exe     <fver(5,1,2600,1886))
					  ||  (_ipv6mon_dll  >zero && _ipv6mon_dll  <fver(5,1,2600,1886))
					  ||  (_netsh_exe    >zero && _netsh_exe    <fver(5,1,2600,1886))
					  ||  (_tcpip6_sys   >zero && _tcpip6_sys   <fver(5,1,2600,1886))
					  ||  (_tunmp_sys    >zero && _tunmp_sys    <fver(5,1,2600,1886))
					  ||  (_ws2_32_dll   >zero && _ws2_32_dll   <fver(5,1,2600,1886))
					  ||  (_wship6_dll   >zero && _wship6_dll   <fver(5,1,2600,1886)) ))
	 ||(sp==2 && (sku & XP_ALL) && ( (_6to4svc_dll>zero && _6to4svc_dll<fver(5,1,2600,2975))
					  || (_tcpip6_sys>zero && _tcpip6_sys<fver(5,1,2600,2975))
					  || !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB922819")))) {
		// KB922819 is replaced by KB978338 on SP2, but is a prerequisite to KB920342
		NN("Security Update for Windows XP (KB922819)");
		XX(p2+"windowsxp-kb922819-x86-enu_e4dceecdd4a72e5ad91cc78fe5f4572f91ee5db0.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_6to4svc_dll>zero && _6to4svc_dll<fver(5,1,2600,3667))
					  ||  (_tcpip6_sys>zero && _tcpip6_sys<fver(5,1,2600,3667))))
	 ||(sp==3 && (sku & XP_ALL) && ( (_6to4svc_dll>zero && _6to4svc_dll<fver(5,1,2600,5935))
					  ||  (_tcpip6_sys>zero && _tcpip6_sys<fver(5,1,2600,5935))))) {
		NN("Security Update for Windows XP (KB978338)");
		XX(p3+"WindowsXP-KB978338-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( 
		                  (_p2p_dll      >zero && _p2p_dll      <fver(5,1,2600,3013))
					  ||  (_p2pgasvc_dll >zero && _p2pgasvc_dll <fver(5,1,2600,3013))
					  ||  (_p2pgraph_dll >zero && _p2pgraph_dll <fver(5,1,2600,3013))
					  ||  (_p2pnetsh_dll >zero && _p2pnetsh_dll <fver(5,1,2600,3013))
					  ||  (_p2psvc_dll   >zero && _p2psvc_dll   <fver(5,1,2600,3013))
					  ||  (_pnrpnsp_dll  >zero && _pnrpnsp_dll  <fver(5,1,2600,3013)) )) {
		// Update KB920342 requires KB922819 as a prerequisite
		NN("Update for Windows XP (KB920342)");
		XX(p2+"windowsxp-kb920342-x86-enu_6ffee4344d80833300220f461eff4d9c2d9d7d7f.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _wkssvc_dll>zero && _wkssvc_dll<fver(5,1,2600,3584))
	 ||(sp==3 && (sku & XP_ALL) && _wkssvc_dll>zero && _wkssvc_dll<fver(5,1,2600,5826))) {
		NN("Security Update for Windows XP (KB971657)");
		XX(p3+"WindowsXP-KB971657-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _shlwapi_dll>zero && _shlwapi_dll<fver(6,0,2900,3653))
	 ||(sp==3 && (sku & XP_ALL) && _shlwapi_dll>zero && _shlwapi_dll<fver(6,0,2900,5912))) {
		NN("Security Update for Windows XP (KB975713)");
		XX(p3+"WindowsXP-KB975713-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _csrsrv_dll>zero && _csrsrv_dll<fver(5,1,2600,3657)) {
	//if( sp==3 && (sku & XP_ALL) && _csrsrv_dll>zero && _csrsrv_dll<fver(5,1,2600,5915)) {
		NN("Security Update for Windows XP (KB978037)");
		XX(p2+"windowsxp-kb978037-x86-enu_0165498351f085f2cdcf407a23e2a85d08f7212e.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _es_dll>zero && _es_dll<fver(2001,12,4414,320))
	 ||(sp==3 && (sku & XP_ALL) && _es_dll>zero && _es_dll<fver(2001,12,4414,706))) {
		NN("Security Update for Windows XP (KB950974)");
		XX(p3+"WindowsXP-KB950974-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _helpsvc_exe>zero && _helpsvc_exe<fver(5,1,2600,3720))
	 ||(sp==3 && (sku & XP_ALL) && _helpsvc_exe>zero && _helpsvc_exe<fver(5,1,2600,5997))) {
		NN("Security Update for Windows XP (KB2229593)");
		XX(p3+"WindowsXP-KB2229593-x86-ENU.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && ( (_ciodm_dll>zero && _ciodm_dll<fver(5,1,2600,1860))
					  ||  (_query_dll>zero && _query_dll<fver(5,1,2600,1860))))
	 ||(sp==2 && (sku & XP_ALL) && ( (_ciodm_dll>zero && _ciodm_dll<fver(5,1,2600,2935))
					  ||  (_query_dll>zero && _query_dll<fver(5,1,2600,2935))))) {
		NN("Security Update for Windows XP (KB920685)");
		XX(p2+"windowsxp-kb920685-x86-enu_be0e9cea96e2ad48394aebe90d48edcc36ac38d5.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _query_dll>zero && _query_dll<fver(5,1,2600,3602))
	 ||(sp==3 && (sku & XP_ALL) && _query_dll>zero && _query_dll<fver(5,1,2600,5847))) {
		NN("Security Update for Windows XP (KB969059)");
		XX(p3+"WindowsXP-KB969059-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_raschap_dll>zero && _raschap_dll<fver(5,1,2600,3632))
					  ||  (_rastls_dll>zero && _rastls_dll<fver(5,1,2600,3632))))
	 ||(sp==3 && (sku & XP_ALL) && ( (_raschap_dll>zero && _raschap_dll<fver(5,1,2600,5886))
					  ||  (_rastls_dll>zero && _rastls_dll<fver(5,1,2600,5886))))) {
		NN("Security Update for Windows XP (KB974318)");
		XX(p3+"WindowsXP-KB974318-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _upnphost_dll>zero && _upnphost_dll<fver(5,1,2600,3077)) {
		NN("Security Update for Windows XP (KB931261)");
		XX(p2+"windowsxp-kb931261-x86-enu_76c76c0fbe6dd82bae54c22e353ea16f7bd4f95e.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_lsasrv_dll>zero && _lsasrv_dll<fver(5,1,2600,2976))
					  ||  (_netapi32_dll>zero && _netapi32_dll<fver(5,1,2600,2976))
					  ||  (_wkssvc_dll>zero && _wkssvc_dll<fver(5,1,2600,2976)) )) {
		NN("Security Update for Windows XP (KB924270)");
		XX(p2+"windowsxp-kb924270-x86-enu_3b1af30dc7a2f51f60a415eaf2cc01f9bf779dab.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && _msadco_dll>zero && _msadco_dll<fver(2,71,9053,0))
	 ||(sp==2 && (sku & XP_ALL) && _msadco_dll>zero && _msadco_dll<fver(2,81,1124,0))) {
		NN("Security Update for Windows XP (KB911562)");
		XX(p2+"windowsxp-kb911562-x86-enu_7d16ad9701607a354e0ca2602a3fef485c8d9929.exe"+a1);
	}
	/*if( sp==1 && (sku & XP_ALL) && (
	      (_gptext_dll   >zero && _gptext_dll   <fver(5,1,2600,1234))
	   || (_ipsec_sys    >zero && _ipsec_sys    <fver(5,1,2600,1240))
//	   || (_ipseccmd_exe >zero && _ipseccmd_exe <fver(5,1,2600,1240))
	   || (_ipsecsnp_dll >zero && _ipsecsnp_dll <fver(5,1,2600,1240))
	   || (_ipsecsvc_dll >zero && _ipsecsvc_dll <fver(5,1,2600,1240))
	   || (_ipsmsnap_dll >zero && _ipsmsnap_dll <fver(5,1,2600,1221))
	   || (_oakley_dll   >zero && _oakley_dll   <fver(5,1,2600,1240))
	   || (_polstore_dll >zero && _polstore_dll <fver(5,1,2600,1240))
	   || (_rasmans_dll  >zero && _rasmans_dll  <fver(5,1,2600,1221))
	   || (_winipsec_dll >zero && _winipsec_dll <fver(5,1,2600,1240)) )) {
	// KB818043 is replaced by KB911280v2 on SP1
		NN("Recommended Update for Windows XP (818043)");
		XX(p1+"windowsxp-kb818043-x86-enu_49e447d62b9abcf6b6482de5a668033.exe"+a7);
	}*/
	if(( sp==1 && (sku & XP_ALL) && (
	    (_gptext_dll   >zero && _gptext_dll   <fver(5,1,2600,1581))
	 || (_ipsec_sys    >zero && _ipsec_sys    <fver(5,1,2600,1842))
	 || (_ipsecsnp_dll >zero && _ipsecsnp_dll <fver(5,1,2600,1842))
	 || (_ipsecsvc_dll >zero && _ipsecsvc_dll <fver(5,1,2600,1842))
	 || (_ipsmsnap_dll >zero && _ipsmsnap_dll <fver(5,1,2600,1842))
	 || (_oakley_dll   >zero && _oakley_dll   <fver(5,1,2600,1842))
	 || (_polstore_dll >zero && _polstore_dll <fver(5,1,2600,1842))
	 || (_rasmans_dll  >zero && _rasmans_dll  <fver(5,1,2600,1861))
	 || (_winipsec_dll >zero && _winipsec_dll <fver(5,1,2600,1842)) ))
	 ||( sp==2 && (sku & XP_ALL) && _rasmans_dll>zero && _rasmans_dll<fver(5,1,2600,2936))) {
		NN("Security Update for Windows XP (KB911280)");
		XX(p2+"windowsxp-kb911280-v2-x86-enu_3a49ae105416eb7b37dbbaccbedc9c20069ef1d9.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (!regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP4\\KB980195")
		                        || !kb980195_installed())) {
		NN("Cumulative Security Update for ActiveX Killbits for Windows XP (KB980195)");
		XX(p2+"windowsxp-kb980195-x86-enu_5e9692a99ed5f78af0ce82f7aa5e87b1cd5bf958.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && (!regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP4\\KB2900986")
		                        || !kb2900986_installed())) {
		NN("Cumulative Security Update for ActiveX Killbits for Windows XP (KB2900986)");
		XX(p3+"WindowsXP-KB2900986-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _gdiplus_dll_22319<fver(5,2,6001,22319)) {
		NN("Security Update for Windows XP (KB958869)");
		XX(p2+"windowsxp-kb958869-x86-enu_ec59d655f78bbc751bb5d664530615ba53929548.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _vgx_dll>zero && _vgx_dll<fver(6,0,2900,3051)) {
		// KB929969 is replaced by KB938127 on SP2
		NN("Security Update for Windows XP (KB929969)");
		XX(p+"windowsxp-kb929969-x86-enu_1237547f1cb90f54269bdb73c9985f263e85d48c.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _vgx_dll>zero && _vgx_dll<fver(6,0,2900,3164)) {
		NN("Security Update for Windows XP Service Pack 2 (KB938127)");
		XX(p2+"WindowsXP-KB938127-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( (_shell32_dll>zero && _shell32_dll<fver(6,0,2900,3051))
					  ||  (_shsvcs_dll>zero && _shsvcs_dll<fver(6,0,2900,3051)) )) {
		NN("Security Update for Windows XP (KB928255)");
		XX(p2+"windowsxp-kb928255-x86-enu_d29dfbbe228e49f746a947eeb4880e980b76d53d.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2900,3241)) {
		NN("Security Update for Windows XP (KB943460)");
		XX(p2+"windowsxp-kb943460-x86-enu_ab4feaa4871f4c2512cad616479668f19e779c53.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2900,3402)) {
	//if( sp==3 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2900,5622)) {
		NN("Update for Windows XP (KB967715)");
		XX(p2+"windowsxp-kb967715-x86-enu_7d158e1f0dc6a7b5de75ca59bc89816017150ab4.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && ( 
		                  (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,3093))
					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,3093))
					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,3093))
					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,3093)) )) {
		// KB931784 is replaced by KB979683 on SP2
		NN("Security Update for Windows XP (KB931784)");
		XX(p+"windowsxp-kb931784-x86-enu_70c95793c76ac8746a1ca9abd1815a8f0b73944f.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && ( 
		                  (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,3670))
					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,3670))
					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,3670))
					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,3670)) )) {
//	if( sp==3 && (sku & XP_ALL) && (
//		                  (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,5938))
//					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,5938))
//					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,5938))
//					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,5938)) )) {
		NN("Security Update for Windows XP (KB979683)");
		XX(p2+"windowsxp-kb979683-x86-enu_8f9e90bf9b6ad58fd264cc1cf0f187f9a83223a5.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,1885)) {
		// KB923414 is replaced by KB971468 on SP2
		NN("Security Update for Windows XP (KB923414)");
		XX(p1+"windowsxp-kb923414-x86-enu_ed2b1047badbd832a971a76ca7ef4519d1a444f4.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,3662)) {
	//if( sp==3 && (sku & XP_ALL) && _srv_sys>zero && _srv_sys<fver(5,1,2600,5923)) {
		NN("Security Update for Windows XP (KB971468)");
		XX(p2+"windowsxp-kb971468-x86-enu_68d7899c8b8462219daf40f02c6fb9f362b1ee6b.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _telnet_exe>zero && _telnet_exe<fver(5,1,2600,1684)) {
	//if( sp==2 && (sku & XP_ALL) && _telnet_exe>zero && _telnet_exe<fver(5,1,2600,2674)) {
		// KB896428 is replaced by KB960859 on SP2
		NN("Security Update for Windows XP (KB896428)");
		XX(p1+"windowsxp-kb896428-x86-enu_24f66bc1e3b8107ec580ba2c53148a69dbc606a0.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_telnet_exe>zero && _telnet_exe<fver(5,1,2600,3587))
					  ||  (_tlntsess_exe>zero && _tlntsess_exe<fver(5,1,2600,3587))))
	 ||(sp==3 && (sku & XP_ALL) && ( (_telnet_exe>zero && _telnet_exe<fver(5,1,2600,5829))
					  ||  (_tlntsess_exe>zero && _tlntsess_exe<fver(5,1,2600,5829))))) {
		NN("Security Update for Windows XP (KB960859)");
		XX(p3+"WindowsXP-KB960859-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _afd_sys>zero && _afd_sys<fver(5,1,2600,3427)) {
	//if( sp==3 && (sku & XP_ALL) && _afd_sys>zero && _afd_sys<fver(5,1,2600,5657)) {
		NN("Security Update for Windows XP (KB956803)");
		XX(p2+"windowsxp-kb956803-x86-enu_d075d359a28ab8b058a35a2e7b466bd0bca8e9ef.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _msgsc_dll>=fver(4,7,0,0) && _msgsc_dll<fver(4,7,0,3002)) {
		NN("Security Update for Windows XP (KB946648)");
		XX(p3+"WindowsXP-KB946648-x86-ENU.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && _kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,3119)) {
		// KB935839 is replaced by KB959426 on SP2
		NN("Security Update for Windows XP (KB935839)");
		XX(p+"windowsxp-kb935839-x86-enu_891b3c6e312494ccf2dd0dab00dfcc4c49f2ca85.exe"+a1);
	}*/
	if((sp==2 && (sku & XP_ALL) && ( (_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,3541))
					  ||  (_secur32_dll>zero && _secur32_dll<fver(5,1,2600,3518))))
	 ||(sp==3 && (sku & XP_ALL) && ( (_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,5781))
					  ||  (_secur32_dll>zero && _secur32_dll<fver(5,1,2600,5753))))) {
		NN("Security Update for Windows XP (KB959426)");
		XX(p3+"WindowsXP-KB959426-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _mscms_dll>zero && _mscms_dll<fver(5,1,2600,3396))
	 ||(sp==3 && (sku & XP_ALL) && _mscms_dll>zero && _mscms_dll<fver(5,1,2600,5627))) {
		NN("Security Update for Windows XP (KB952954)");
		XX(p3+"WindowsXP-KB952954-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _wiaservc_dll>zero && _wiaservc_dll<fver(5,1,2600,3051)) {
		NN("Security Update for Windows XP (KB927802)");
		XX(p2+"windowsxp-kb927802-x86-enu_94703f4083a9d9d6633d9134d0d0a85bfc405f3a.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _bthport_sys<fver(5,1,2600,3389))
	 ||(sp==3 && (sku & XP_ALL) && _bthport_sys<fver(5,1,2600,5620))) {
		NN("Security Update for Windows XP (KB951376)");
		XX(p3+"WindowsXP-KB951376-v2-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( 
		                  (_msado15_dll>zero && _msado15_dll<fver(2,81,1128,0))
					  ||  (_msadomd_dll>zero && _msadomd_dll<fver(2,81,1128,0))
					  ||  (_msadox_dll >zero && _msadox_dll <fver(2,81,1128,0))
					  ||  (_msjro_dll  >zero && _msjro_dll  <fver(2,81,1128,0)) )) {
		NN("Security Update for Windows XP (KB927779)");
		XX(p2+"windowsxp-kb927779-x86-enu_ec7986f2b0afe9cc7f53a48b6582169a77d9515e.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _atmfd_dll>zero && _atmfd_dll<fver(5,1,2,228)) {
		NN("Security Update for Windows XP (KB980218)");
		XX(p2+"windowsxp-kb980218-x86-enu_8f11793773e54a3ff1f1b93e5ebfdd2592caba2f.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) /*&& _msctf_dll>zero*/ && _msctf_dll<fver(5,1,2600,3319)) {
		NN("Update for Windows XP (KB932823)");
		XX(p2+"windowsxp-kb932823-v3-x86-enu_d0806094569c5bbce9f6e0313cd67558316d048a.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _tzchange_exe<fver(5,1,2600,5971)) {
		NN("Update for Windows XP (KB981793)");
		XX(p2+"windowsxp-kb981793-x86-enu_e1363fb1d582b860c3e25b2bbfbc0b7f38a8b44b.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) /*&& _aclayers_dll>zero*/ && _aclayers_dll<fver(5,1,2600,3647)) {
	//if( sp==3 && (sku & XP_ALL) /*&& _aclayers_dll>zero*/ && _aclayers_dll<fver(5,1,2600,5906)) {
		NN("Update for Windows XP (KB955759)");
		XX(p2+"windowsxp-kb955759-x86-enu_32f91e0ecc35b55f6c9e51ad360b2adee1f74049.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _msxml3_dll>zero && _msxml3_dll<fver(8,100,1050,0)) {
//	if( sp==3 && (sku & XP_ALL) && ( 
//		                  (_msxml3_dll >zero && _msxml3_dll <fver(8,100,1051,0))
//					  ||  (_msxml6_dll >zero && _msxml6_dll <fver(6,20,1103,0)) )) {
		NN("Update for Windows XP (KB973687)");
		XX(p2+"windowsxp-kb973687-x86-enu_936b46b34291e12c6fe6b7cb19e93d52dda990d8.exe"+a1);
	}
	if( (sp==2 && (sku & XP_ALL) && _msadce_dll>zero && _msadce_dll<fver(2,81,1135,0))
	 || (sp==3 && (sku & XP_ALL) && _msadce_dll>zero && _msadce_dll<fver(2,81,3002,0)) ){
		NN("Update for Windows XP (KB952287)");
		XX(p3+"windowsxp-kb952287-x86-enu_5bee5928c09f4ee44bc41cab109022cec820e5ff.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _update_sys>zero && _update_sys<fver(5,1,2600,3124)) {
		NN("Update for Windows XP (KB936357)");
		XX(p2+"windowsxp-kb936357-x86-enu_50f76a151743f032f20d6a9f6e71ed0b73fe7781.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _explorer_exe>zero && _explorer_exe<fver(6,0,2900,3156)) {
		NN("Update for Windows XP (KB938828)");
		XX(p2+"windowsxp-kb938828-x86-enu_ba3f0cbe4ba5736d4254732e41fe058697b76ebc.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _ntfs_sys>zero && _ntfs_sys<fver(5,1,2600,3081)) {
		NN("Update for Windows XP (KB930916)");
		XX(p2+"windowsxp-kb930916-x86-enu_f077461cd094716eafd9cfe353d257a0f71a4af2.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( 
		                  (_fltlib_dll >zero && _fltlib_dll <fver(5,1,2600,2978))
					  ||  (_fltMc_exe  >zero && _fltMc_exe  <fver(5,1,2600,2978))
					  ||  (_fltMgr_sys >zero && _fltMgr_sys <fver(5,1,2600,2978)) )) {
		NN("Update for Windows XP (KB922582)");
		XX(p2+"windowsxp-kb922582-x86-enu_4d2c05da13ad8b38b98d290938bbb7de3fd6d985.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _http_sys>zero && _http_sys<fver(5,1,2600,2869)) {
		NN("Update for Windows XP (KB916595)");
		XX(p2+"windowsxp-kb916595-x86-enu_d2e2d4f421b6578b739717a98f84aa8fface741f.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( 
		                  (_kmixer_sys   >zero && _kmixer_sys   <fver(5,1,2600,2929))
					  ||  (_splitter_sys >zero && _splitter_sys <fver(5,1,2600,2929))
					  ||  (_wdmaud_sys   >zero && _wdmaud_sys   <fver(5,1,2600,2929)) )) {
		NN("Update for Windows XP (KB920872)");
		XX(p2+"windowsxp-kb920872-x86-enu_6f7ac7666023971f687e372581b6d40cfa8677f0.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _aec_sys>zero && _aec_sys<fver(5,1,2601,2180)) {
		NN("Update for Windows XP (KB900485)");
		XX(p2+"windowsxp-kb900485-v2-x86-enu_e30ec198dd9fbfa393940a630dfc641f49db727c.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( 
		                  ( /*_custsat_dll   >zero &&*/ _custsat_dll   <fver(9,0,2600,2921))
					  ||  ( /*_xpnetdg_exe   >zero &&*/ _xpnetdiag_exe <fver(5,1,2600,3012)) )) {
		NN("Network Diagnostic Tool (KB914440)");
		XX(p2+"windowsxp-kb914440-v12-x86-enu_74aec9f94a132216c271bf0aa3c5324979d12740.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( 
		                  ( _migload_exe  <fver(5,1,2600,2665))
					  ||  ( _migwiz_exe   <fver(5,1,2600,2665))
					  ||  ( _migwiza_exe  <fver(5,1,2600,2665))
					  ||  ( _cobramsg_dll <fver(5,1,2600,2665))
					  ||  ( _guitrn_dll   <fver(5,1,2600,2665))
					  ||  ( _guitrna_dll  <fver(5,1,2600,2665))
					  ||  ( _iconlib_dll  <fver(5,1,2600,2665))
					  ||  ( _log_dll      <fver(5,1,2600,2665))
					  ||  ( _migism_dll   <fver(5,1,2600,2665))
					  ||  ( _migisma_dll  <fver(5,1,2600,2665))
					  ||  ( _script_dll   <fver(5,1,2600,2665))
					  ||  ( _scripta_dll  <fver(5,1,2600,2665))
					  ||  ( _sysmod_dll   <fver(5,1,2600,2665))
					  ||  ( _sysmoda_dll  <fver(5,1,2600,2665)) )) {
		NN("Update for Windows XP (KB896344)");
		XX(p2+"windowsxp-kb896344-v2-x86-enu_3ab0b06345f79bbcd426f4f57834753210716441.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && ( 
		                  ( _photometadatahandler_dll <fver(6,0,5840,16388))
					  ||  ( _windowscodecs_dll        <fver(6,0,5840,16388))
					  ||  ( _windowscodecsext_dll     <fver(6,0,5840,16388))
					  ||  ( _wmphoto_dll              <fver(6,0,5840,16388)) )) {
		if(    _photometadatahandler_dll <=fver(6,0,5840,16388)
			&& _windowscodecs_dll        <=fver(6,0,5840,16388)
			&& _windowscodecsext_dll     <=fver(6,0,5840,16388)
			&& _wmphoto_dll              <=fver(6,0,5840,16388)) {
			NN("Windows Imaging Component for Windows XP and Server 2003");
			XX(p3+"wic_x86_enu.exe"+a1);
		} else {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Windows Imaging Component is missing, but")
			                               +"|cannot be installed because newer parts of"
							               +"|it are already present on the system.");
		}
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _authz_dll    >zero && _authz_dll    <fver(5,1,2600,1634))
					  ||  ( _ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,1634))
					  ||  ( _ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,1634))
					  ||  ( _ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,1634))
					  ||  ( _ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,1634))
					  ||  ( _user32_dll   >zero && _user32_dll   <fver(5,1,2600,1634))
					  ||  ( _win32k_sys   >zero && _win32k_sys   <fver(5,1,2600,1634))
					  ||  ( _winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,1634))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _authz_dll    >zero && _authz_dll    <fver(5,1,2600,2622))
					  ||  ( _ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,2622))
					  ||  ( _ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,2622))
					  ||  ( _ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,2622))
					  ||  ( _ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,2622))
					  ||  ( _user32_dll   >zero && _user32_dll   <fver(5,1,2600,2622))
					  ||  ( _win32k_sys   >zero && _win32k_sys   <fver(5,1,2600,2622))
					  ||  ( _winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,2622))))) {
		NN("Security Update for Windows XP (KB890859)");
		XX(p2+"windowsxp-kb890859-x86-enu_813f47d987b772bacae20e7dec9b5f6f16079303.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (
		                  ( /*_secdrv_sys    >zero*/ _secdrv_sys_exist
						   && _secdrv_sys    <fver(4,3,86,0)) )) {
		NN("Security Update for Windows XP (KB944653)");
		XX(p2+"windowsxp-kb944653-x86-enu_e08db12cc3442f904f2a1d9a51c7c5662e15ef3b.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && (_flash_ocx>zero || _swflash_ocx>zero) && _flash6_ocx<fver(6,0,88,0)) {
		NN("Security Update for Flash Player (KB923789)");
		XX(p3+"WindowsXP-KB923789-x86-ENU.exe"+a2);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _umpnpmgr_dll    >zero && _umpnpmgr_dll    <fver(5,1,2600,1734))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _umpnpmgr_dll    >zero && _umpnpmgr_dll    <fver(5,1,2600,2744))))) {
		NN("Security Update for Windows XP (KB905749)");
		XX(p2+"windowsxp-kb905749-x86-enu_3f44b68f7e0a0e6332dd18997e134beab1027c73.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2800,1579))
					  ||  ( _shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1816))
					  ||  ( _sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1579))
					  ||  ( _verclsid_exe >zero && _verclsid_exe <fver(5,1,2600,1816))
					  ||  ( _comctl32_dll_1816 < fver(6,0,2800,1816)) ))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _shell32_dll    >zero && _shell32_dll    <fver(6,0,2900,2869))
					  ||  ( _verclsid_exe   >zero && _verclsid_exe   <fver(5,1,2600,2869))))) {
		NN("Security Update for Windows XP (KB908531)");
		XX(p2+"windowsxp-kb908531-v2-x86-enu_0f04352bbc21b3c173cc8dd8c9e63c082b34b676.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _mspaint_exe>zero && _mspaint_exe<fver(5,1,2600,3660))
	 ||(sp==3 && (sku & XP_ALL) && _mspaint_exe>zero && _mspaint_exe<fver(5,1,2600,5918))) {
		NN("Security Update for Windows XP (KB978706)");
		XX(p3+"WindowsXP-KB978706-x86-ENU.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _fldrclnr_dll >zero && _fldrclnr_dll <fver(6,0,2800,1579))
					  ||  ( _linkinfo_dll >zero && _linkinfo_dll <fver(5,1,2600,1740))
					  ||  ( _shell32_dll  >zero && _shell32_dll  <fver(6,0,2800,1751))
					  ||  ( _shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1740))
					  ||  ( _sxs_dll      >zero && _sxs_dll      <fver(5,1,2600,1579))
					  ||  ( _winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,1740))
					  ||  ( _comctl32_dll_1740 < fver(6,0,2800,1740)) ))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _linkinfo_dll >zero && _linkinfo_dll <fver(5,1,2600,2751))
					  ||  ( _shell32_dll  >zero && _shell32_dll  <fver(6,0,2900,2763))
					  ||  ( _shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2900,2753))
					  ||  ( _winsrv_dll   >zero && _winsrv_dll   <fver(5,1,2600,2751))))) {
		NN("Security Update for Windows XP (KB900725)");
		XX(p2+"windowsxp-kb900725-x86-enu_21b409882b7f51a9d09c32bd698504fadb9fc433.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _icm32_dll >zero && _icm32_dll <fver(5,1,2600,1710))
					  ||  ( _mscms_dll >zero && _mscms_dll <fver(5,1,2600,1710))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _icm32_dll >zero && _icm32_dll <fver(5,1,2600,2709))
					  ||  ( _mscms_dll >zero && _mscms_dll <fver(5,1,2600,2709))))) {
		NN("Security Update for Windows XP (KB901214)");
		XX(p2+"windowsxp-kb901214-x86-enu_2838831de819dad80ea0edaf5fb1e0bfb3c026c0.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _netman_dll    >zero && _netman_dll    <fver(5,1,2600,1733))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _netman_dll    >zero && _netman_dll    <fver(5,1,2600,2743))))) {
		NN("Security Update for Windows XP (KB905414)");
		XX(p2+"windowsxp-kb905414-x86-enu_9e8fa8909332653de951edcfdb691f2aa148eb1b.exe"+a1);
	}
	if( (sp==1||sp==2) && (sku & XP_ALL) && (
		                  ( _agentdpv_dll    >zero && _agentdpv_dll    <fver(2,0,0,3423)) )) {
		NN("Security Update for Windows XP (KB890046)");
		XX(p2+"windowsxp-kb890046-x86-enu_432bf46f62aeaecc0e519af31d74723096f9b201.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && (
		                  ( _catsrv_dll   >zero && _catsrv_dll   <fver(2001,12,4414,308))
					  ||  ( _catsrvut_dll >zero && _catsrvut_dll <fver(2001,12,4414,308))
					  ||  ( _clbcatex_dll >zero && _clbcatex_dll <fver(2001,12,4414,308))
					  ||  ( _clbcatq_dll  >zero && _clbcatq_dll  <fver(2001,12,4414,308))
					  ||  ( _colbact_dll  >zero && _colbact_dll  <fver(2001,12,4414,308))
					  ||  ( _comadmin_dll >zero && _comadmin_dll <fver(2001,12,4414,308))
					  ||  ( _comrepl_dll  >zero && _comrepl_dll  <fver(2001,12,4414,308))
					  ||  ( _comsvcs_dll  >zero && _comsvcs_dll  <fver(2001,12,4414,308))
					  ||  ( _comuid_dll   >zero && _comuid_dll   <fver(2001,12,4414,308))
					  ||  ( _es_dll       >zero && _es_dll       <fver(2001,12,4414,308))
					  ||  ( _migregdb_exe >zero && _migregdb_exe <fver(2001,12,4414,308))
					  ||  ( _msdtcprx_dll >zero && _msdtcprx_dll <fver(2001,12,4414,308))
					  ||  ( _msdtctm_dll  >zero && _msdtctm_dll  <fver(2001,12,4414,308))
					  ||  ( _msdtcuiu_dll >zero && _msdtcuiu_dll <fver(2001,12,4414,308))
					  ||  ( _mtxclu_dll   >zero && _mtxclu_dll   <fver(2001,12,4414,308))
					  ||  ( _mtxoci_dll   >zero && _mtxoci_dll   <fver(2001,12,4414,308))
					  ||  ( _ole32_dll    >zero && _ole32_dll    <fver(5,1,2600,2726))
					  ||  ( _olecli32_dll >zero && _olecli32_dll <fver(5,1,2600,2726))
					  ||  ( _olecnv32_dll >zero && _olecnv32_dll <fver(5,1,2600,2726))
					  ||  ( _rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,2726))
					  ||  ( _txflog_dll   >zero && _txflog_dll   <fver(2001,12,4414,308))
					  ||  ( _xolehlp_dll  >zero && _xolehlp_dll  <fver(2001,12,4414,308)) )) {
		// KB902400 is replaced by KB912817 on SP1 and SP2
		NN("Security Update for Windows XP (KB902400)");
		XX(p+"windowsxp-kb902400-x86-enu_a51d743a1925dd0216160daaf9fc4c7a42a27e53.exe"+a1);
	}*/
	if( sp==1 && (sku & XP_ALL) && _srrstr_dll>zero && _srrstr_dll<fver(5,1,2600,1765)) {
		NN("Update for Windows XP (KB835409)");
		XX(p1+"windowsxp-kb835409-x86-enu_bfb2828c3da695a07dfd25d76110cd35da03e044.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _catsrv_dll   >zero && _catsrv_dll   <fver(2001,12,4414,64))
					  ||  ( _catsrvut_dll >zero && _catsrvut_dll <fver(2001,12,4414,64))
					  ||  ( _clbcatex_dll >zero && _clbcatex_dll <fver(2001,12,4414,64))
					  ||  ( _clbcatq_dll  >zero && _clbcatq_dll  <fver(2001,12,4414,64))
					  ||  ( _colbact_dll  >zero && _colbact_dll  <fver(2001,12,4414,64))
					  ||  ( _comadmin_dll >zero && _comadmin_dll <fver(2001,12,4414,64))
					  ||  ( _comrepl_dll  >zero && _comrepl_dll  <fver(2001,12,4414,64))
					  ||  ( _comsvcs_dll  >zero && _comsvcs_dll  <fver(2001,12,4414,64))
					  ||  ( _comuid_dll   >zero && _comuid_dll   <fver(2001,12,4414,64))
					  ||  ( _es_dll       >zero && _es_dll       <fver(2001,12,4414,64))
					  ||  ( _migregdb_exe >zero && _migregdb_exe <fver(2001,12,4414,64))
					  ||  ( _msdtcprx_dll >zero && _msdtcprx_dll <fver(2001,12,4414,64))
					  ||  ( _msdtctm_dll  >zero && _msdtctm_dll  <fver(2001,12,4414,64))
					  ||  ( _msdtcuiu_dll >zero && _msdtcuiu_dll <fver(2001,12,4414,64))
					  ||  ( _mtxclu_dll   >zero && _mtxclu_dll   <fver(2001,12,4414,64))
					  ||  ( _mtxoci_dll   >zero && _mtxoci_dll   <fver(2001,12,4414,64))
					  ||  ( _ole32_dll    >zero && _ole32_dll    <fver(5,1,2600,1801))
					  ||  ( _olecli32_dll >zero && _olecli32_dll <fver(5,1,2600,1801))
					  ||  ( _olecnv32_dll >zero && _olecnv32_dll <fver(5,1,2600,1801))
					  ||  ( _rpcrt4_dll   >zero && _rpcrt4_dll   <fver(5,1,2600,1361))
					  ||  ( _rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,1801))
					  ||  ( _srrstr_dll   >zero && _srrstr_dll   <fver(5,1,2600,1765))
					  ||  ( _stclient_dll >zero && _stclient_dll <fver(2001,12,4414,64))
					  ||  ( _txflog_dll   >zero && _txflog_dll   <fver(2001,12,4414,64))
					  ||  ( _xolehlp_dll  >zero && _xolehlp_dll  <fver(2001,12,4414,64))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _catsrv_dll   >zero && _catsrv_dll   <fver(2001,12,4414,310))
					  ||  ( _catsrvut_dll >zero && _catsrvut_dll <fver(2001,12,4414,310))
					  ||  ( _clbcatex_dll >zero && _clbcatex_dll <fver(2001,12,4414,310))
					  ||  ( _clbcatq_dll  >zero && _clbcatq_dll  <fver(2001,12,4414,310))
					  ||  ( _colbact_dll  >zero && _colbact_dll  <fver(2001,12,4414,310))
					  ||  ( _comadmin_dll >zero && _comadmin_dll <fver(2001,12,4414,310))
					  ||  ( _comrepl_dll  >zero && _comrepl_dll  <fver(2001,12,4414,310))
					  ||  ( _comsvcs_dll  >zero && _comsvcs_dll  <fver(2001,12,4414,310))
					  ||  ( _comuid_dll   >zero && _comuid_dll   <fver(2001,12,4414,310))
					  ||  ( _es_dll       >zero && _es_dll       <fver(2001,12,4414,310))
					  ||  ( _migregdb_exe >zero && _migregdb_exe <fver(2001,12,4414,310))
					  ||  ( _msdtcprx_dll >zero && _msdtcprx_dll <fver(2001,12,4414,310))
					  ||  ( _msdtctm_dll  >zero && _msdtctm_dll  <fver(2001,12,4414,310))
					  ||  ( _msdtcuiu_dll >zero && _msdtcuiu_dll <fver(2001,12,4414,310))
					  ||  ( _mtxclu_dll   >zero && _mtxclu_dll   <fver(2001,12,4414,310))
					  ||  ( _mtxoci_dll   >zero && _mtxoci_dll   <fver(2001,12,4414,310))
					  ||  ( _ole32_dll    >zero && _ole32_dll    <fver(5,1,2600,2846))
					  ||  ( _olecli32_dll >zero && _olecli32_dll <fver(5,1,2600,2846))
					  ||  ( _olecnv32_dll >zero && _olecnv32_dll <fver(5,1,2600,2846))
					  ||  ( _rpcss_dll    >zero && _rpcss_dll    <fver(5,1,2600,2846))
					  ||  ( _stclient_dll >zero && _stclient_dll <fver(2001,12,4414,310))
					  ||  ( _txflog_dll   >zero && _txflog_dll   <fver(2001,12,4414,310))
					  ||  ( _xolehlp_dll  >zero && _xolehlp_dll  <fver(2001,12,4414,310))))) {
		NN("Windows Communication Foundation (KB912817)");
		XX(p2+"WindowsXP-KB912817-v2-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _moviemk_exe>zero && _moviemk_exe<fver(2,1,4027,0)) {
	//if( sp==3 && (sku & XP_ALL) && _moviemk_exe>zero && _moviemk_exe<fver(2,1,4027,0)) {
		NN("Security Update for Windows XP (KB975561)");
		XX(p2+"windowsxp-kb975561-x86-enu_9d1f72aba7bfd6670433e4b997a88860f217e520.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _moviemk_exe>zero && _moviemk_exe<fver(2,1,4028,0)) {
		NN("Security Update for Windows XP (KB981997)");
		XX(p3+"WindowsXP-KB981997-x86-ENU.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _hh_exe      >zero && _hh_exe      <fver(5,2,3790,315))
					  ||  ( _hhctrl_ocx  >zero && _hhctrl_ocx  <fver(5,2,3790,315))
					  ||  ( _hhsetup_dll >zero && _hhsetup_dll <fver(5,2,3790,315))
					  ||  ( _itircl_dll  >zero && _itircl_dll  <fver(5,2,3790,315))
					  ||  ( _itss_dll    >zero && _itss_dll    <fver(5,2,3790,315))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _hh_exe      >zero && _hh_exe      <fver(5,2,3790,2453))
					  ||  ( _hhctrl_ocx  >zero && _hhctrl_ocx  <fver(5,2,3790,2453))
					  ||  ( _hhsetup_dll >zero && _hhsetup_dll <fver(5,2,3790,2453))
					  ||  ( _itircl_dll  >zero && _itircl_dll  <fver(5,2,3790,2453))
					  ||  ( _itss_dll    >zero && _itss_dll    <fver(5,2,3790,2453))))) {
		NN("Security Update for Windows XP (KB896358)");
		XX(p2+"windowsxp-kb896358-x86-enu_42b05278a6f2ee006072af8830c103eab2ce045f.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && ( 
                  ( _msgsc_dll    >zero && _msgsc_dll    <fver(4,7,0,2009))
			  ||  ( _msgslang_dll >zero && _msgslang_dll <fver(4,7,0,2009))
			  ||  ( _msmsgs_exe   >zero && _msmsgs_exe   <fver(4,7,0,2010))
//			  ||  ( _msmsgsin_exe >zero && _msmsgsin_exe <fver(4,7,0,2009))
			  ||  ( _rtcimsp_dll  >zero && _rtcimsp_dll  <fver(4,0,3599,0)) )) {
		NN("Security Update for Windows Messenger (KB887472)");
		XX(sw+p1+"windowsmessenger-kb887472-prexpsp2-enu_15756113f5ce1562fa277d6473ad77e5b54bc00c.exe"+a8);
	}
	/*if( sp==1 && (sku & XP_ALL) && _msmsgs_exe>zero && _msmsgs_exe<fver(4,7,0,2010)) {
		// This is an updated release for KB887472 on Windows XP SP1
		// According to Bulletin MS05-009:
		// Subsequent to the release of this bulletin, it was determined that the update for Windows Messenger
		// version 4.7.0.2009 (when running on Windows XP Service Pack 1) was failing to install when
		// distributed via SMS [Systems Management Server] or AutoUpdate. The updated package corrects this behavior.
		//
		// Customers who have successfully installed the previous update and are now running the 4.7.0.2010
		// version of Windows Messenger are protected from the vulnerability and do not need to take any action
		// at this time.
		//
		// Administrative note: The older version of the security update installs a digitally signed
		// binary while the new version does not, so I made the decision to retain the older one in
		// this code.
		NN("Security Update for Windows Messenger (KB887472)");
		XX(p1+"windowsxp-kb887472-x86-sp1-enu_6d8ca0284e1d032c3ba4e11e2730c8d16988ae15.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && _msmsgs_exe>zero && _msmsgs_exe<fver(4,7,0,3001)) {
		NN("Security Update for Windows Messenger (KB887472)");
		XX(p2+"windowsxp-kb887472-x86-enu_5edc4ccc759d65f4afba8542435172ed54515135.exe"+a6);
	}
	if((sp==1 && (sku & XP_ALL) && _hypertrm_dll>zero && _hypertrm_dll<fver(5,1,2600,1609))
	 ||(sp==2 && (sku & XP_ALL) && _hypertrm_dll>zero && _hypertrm_dll<fver(5,1,2600,2563))) {
		NN("Security Update for Windows XP (KB873339)");
		XX(p2+"windowsxp-kb873339-x86-enu_fd28098f5f0e8e629e5b7f64e5cd6b6b722a35a7.exe"+a6);
	}
	if(( sp==1 && (sku & XP_ALL) && (
		                  ( _spoolsv_exe  >zero && _spoolsv_exe  <fver(5,1,2600,1699))
					  ||  ( _win32spl_dll >zero && _win32spl_dll <fver(5,1,2600,1699))))
	 ||(sp==2 && (sku & XP_ALL) && _spoolsv_exe>zero && _spoolsv_exe<fver(5,1,2600,2696)) ) {
		NN("Security Update for Windows XP (KB896423)");
		XX(p2+"windowsxp-kb896423-x86-enu_baba29a9d96e44e3f55045f749cc82cfa4038f0b.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (
		                  ( _nwapi32_dll  >zero && _nwapi32_dll  <fver(5,1,2600,3015))
					  ||  ( _nwprovau_dll >zero && _nwprovau_dll <fver(5,1,2600,3015))
					  ||  ( _nwrdr_sys    >zero && _nwrdr_sys    <fver(5,1,2600,3015))
					  ||  ( _nwwks_dll    >zero && _nwwks_dll    <fver(5,1,2600,3015)) )) {
		NN("Security Update for Windows XP (KB923980)");
		XX(p2+"windowsxp-kb923980-x86-enu_1f04bf1859d5ba3761e482dbbd48f3795001e391.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _remotesp_tsp  >zero && _remotesp_tsp  <fver(5,1,2600,1715))
					  ||  ( _tapisrv_dll   >zero && _tapisrv_dll   <fver(5,1,2600,1715))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _remotesp_tsp  >zero && _remotesp_tsp  <fver(5,1,2600,2716))
					  ||  ( _tapisrv_dll   >zero && _tapisrv_dll   <fver(5,1,2600,2716))))) {
		NN("Security Update for Windows XP (KB893756)");
		XX(p2+"windowsxp-kb893756-x86-enu_0fff59c5188cc15ec8f138fda97cb8be1e22da66.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && _rdpwd_sys>zero && _rdpwd_sys<fver(5,1,2600,1698))
	 ||(sp==2 && (sku & XP_ALL) && _rdpwd_sys>zero && _rdpwd_sys<fver(5,1,2600,2695))) {
		NN("Security Update for Windows XP (KB899591)");
		XX(p2+"windowsxp-kb899591-x86-enu_3022d995581878f4017098b693b4ba35f99dee5c.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && _cdosys_dll>zero && _cdosys_dll<fver(6,1,1002,0))
	 ||(sp==2 && (sku & XP_ALL) && _cdosys_dll>zero && _cdosys_dll<fver(6,2,4,0))) {
		NN("Security Update for Windows XP (KB901017)");
		XX(p2+"windowsxp-kb901017-x86-enu_04c459695f9018fd31c762bb0a8250cb0506061b.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _mrxdav_sys  >zero && _mrxdav_sys  <fver(5,1,2600,1673))
					  ||  ( _webclnt_dll >zero && _webclnt_dll <fver(5,1,2600,1790)) ))
	 ||(sp==2 && (sku & XP_ALL) && _webclnt_dll>zero && _webclnt_dll<fver(5,1,2600,2821))) {
		NN("Security Update for Windows XP (KB911927)");
		XX(p2+"windowsxp-kb911927-x86-enu_db8cbad537f3f0453deac488f8eb629b3c3a832b.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		                  ( _cscdll_dll   >zero && _cscdll_dll   <fver(5,1,2600,1599))
					  ||  ( _lsasrv_dll   >zero && _lsasrv_dll   <fver(5,1,2600,1597))
					  ||  ( _mrxsmb_sys   >zero && _mrxsmb_sys   <fver(5,1,2600,1599))
					  ||  ( _ntkrnlmp_exe >zero && _ntkrnlmp_exe <fver(5,1,2600,1605))
					  ||  ( _ntkrnlpa_exe >zero && _ntkrnlpa_exe <fver(5,1,2600,1605))
					  ||  ( _ntkrpamp_exe >zero && _ntkrpamp_exe <fver(5,1,2600,1605))
					  ||  ( _ntoskrnl_exe >zero && _ntoskrnl_exe <fver(5,1,2600,1605))
					  ||  ( _rdbss_sys    >zero && _rdbss_sys    <fver(5,1,2600,1599))
					  ||  ( _shsvcs_dll   >zero && _shsvcs_dll   <fver(6,0,2800,1605))))
	 ||(sp==2 && (sku & XP_ALL) && (
		                  ( _lsasrv_dll >zero && _lsasrv_dll <fver(5,1,2600,2525))
					  ||  ( _mrxsmb_sys >zero && _mrxsmb_sys <fver(5,1,2600,2541))
					  ||  ( _rdbss_sys  >zero && _rdbss_sys  <fver(5,1,2600,2541))))) {
		NN("Security Update for Windows XP (KB885835)");
		XX(p2+"windowsxp-kb885835-x86-enu_920ecebf9cd90610ec67a305820f98e4186ca748.exe"+a6);
	}
	if((sp==1 && (sku & XP_ALL) && _kerberos_dll>zero && _kerberos_dll<fver(5,1,2600,1701))
	 ||(sp==2 && (sku & XP_ALL) && _kerberos_dll>zero && _kerberos_dll<fver(5,1,2600,2698))) {
		NN("Security Update for Windows XP (KB899587)");
		XX(p2+"windowsxp-kb899587-x86-enu_95ef03f0da9761b044b9a98d445af90266777ea8.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _ipnat_sys>zero && _ipnat_sys<fver(5,1,2600,2524)) {
		NN("Critical Update for Windows XP (KB886185)");
		XX(p2+"windowsxp-kb886185-x86-enu_88123f8c8a9a8147529eac8ae4656b86c1018a8f.exe"+a6);
	}
	if( sp==2 && (sku & XP_ALL) && _imekr61_ime>zero && _imekr61_ime<fver(6,1,2600,3)) {
		NN("Security Update for Windows XP (KB901190)");
		XX(p2+"windowsxp-kb901190-x86-enu_2497a4e9957ddf13e2343858608f89ef6132efb2.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && _esent_dll>zero && _esent_dll<fver(5,1,2600,1763))
	 ||(sp==2 && (sku & XP_ALL) && _esent_dll>zero && _esent_dll<fver(5,1,2600,2780))) {
		NN("Update for Windows XP (KB910437)");
		XX(p2+"windowsxp-kb910437-x86-enu_6045df96253a6130f973f5f6ac9d10988ba1b01c.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) 
		&& regQueryDWORD(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Setup\\Oc Manager\\Subcomponents",L"iis_www",&status)==1
		&& _w3svc_dll > zero && _w3svc_dll <fver(5,1,2600,3163)) {
		NN("Security Update for Windows XP (KB939373)");
		XX(p2+"windowsxp-kb939373-x86-enu_1060604efc56284ff8208d265f72be4d5394ba32.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && (
		   (_asp_dll            > zero && _asp_dll            <fver(5,1,2600,1829))
		|| (_asp51_dll_dllcache > zero && _asp51_dll_dllcache <fver(5,1,2600,1829)) ))
	 ||(sp==2 && (sku & XP_ALL) && (
		   (_asp_dll            > zero && _asp_dll            <fver(5,1,2600,2889))
		|| (_asp51_dll_dllcache > zero && _asp51_dll_dllcache <fver(5,1,2600,2889)) ))) {
		NN("Security Update for Windows XP (KB917537)");
		XX(p2+"windowsxp-kb917537-x86-enu_a4dbb2338b97e63f46d45f1d69aa6a7908269b13.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL)
		&& regQueryDWORD(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Setup\\Oc Manager\\Subcomponents",L"iis_www",&status)==1
		&& ((_asp_dll > zero && _asp_dll <fver(5,1,2600,3291))
		||   _asp51_dll_dllcache < fver(5,1,2600,3291)) ) {
		NN("Security Update for Windows XP (KB942830)");
		XX(p2+"windowsxp-kb942830-x86-enu_3b34ded0cc8d3c0de48c3c31f4fae006a2eae923.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) 
		&& regTestKey(L"SYSTEM\\CurrentControlSet\\Services\\IISADMIN")
		&& _infocomm_dll > zero && _infocomm_dll <fver(6,0,2600,3290)) {
		NN("Security Update for Windows XP (KB942831)");
		XX(p2+"windowsxp-kb942831-x86-enu_0a8500f0300505cecc06c90840c75796c5e3c9a5.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _xmllite_dll <fver(1,0,1018,0)) {
		NN("XMLLite for Windows XP (KB915865)");
		XX(p3+"WindowsXP-KB915865-v11-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _gpprefcl_dll <fver(5,2,3790,4568)) {
		// KB943729 requires KB915865 (which is an upgraded version of KB914783)
		NN("Group Policy Preference Client Side Extensions for Windows XP (KB943729)");
		XX(p3+"windows-kb943729-x86-enu_e174c41ce3dcbd5c8922d6d1c39df1be425a70e0.exe"+a1);
	}


	// Windows XP Media Center
	if( sp==1 && (sku & XP_MCE) && _dxg_sys>zero && _dxg_sys <fver(5,1,2600,1152)) {
		NN("Q811009: Windows XP Media Center Edition Fast User Switching Hotfix");
		XX(p1+"q811009_wxp_84709768aaaf96cf2de82f15b2db5ffd49e6ca3e.exe"+a7);
	}
	if( sp==1 && (sku & XP_MCE) && _dpcdll_dll>zero && _dpcdll_dll <fver(5,1,2600,1165)) {
		NN("Q811632: Critical Update (Windows XP Media Center Edition Product Activation Hotfix)");
		XX(p1+"q811632_xp_eec81e99c580646b731e53c7186d7e9c9741ef42.exe"+a7);
	}
	if( sp==1 && (sku & XP_MCE) && (
		                 ( /*_hidir_sys >zero &&*/ _hidir_sys <fver(5,1,2600,1321))
					  || ( /*_irbus_sys >zero &&*/ _irbus_sys <fver(5,1,2600,1321)) )) {
		NN("Critical Update for Microsoft Windows XP Media Center Edition Infrared Receiver (KB832418)");
		XX(p1+"windowsxp-kb832418-x86-enu_7f380619f4113b382546291911af7d1.exe"+a6);
	}
	if( sp>=1 && (sku & XP_MCE)
		&& regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"2.7"
		&& (( _ehshell_exe >zero && _ehshell_exe <fver(5,1,2600,1378)) )) {
		NN("Critical Update for Windows XP Media Center Edition 2004 (KB838358)");
		XX(p1+"kb838358_mce2004_x86_enu_8468b88ac30fa21b96f12588ed76278cb865ba8a.exe"+a6);
	}
	if( sp==2 && (sku & XP_ALL) && _wmp_dll>=fver(9,0,0,0) && (
		                  (_blackbox_dll >zero && _blackbox_dll <fver(10,0,0,3802))
		              ||  (_cewmdm_dll   >zero && _cewmdm_dll   <fver(10,0,3790,3802))
					  ||  (_drmclien_dll >zero && _wmp_dll<fver(11,0,0,0) && _drmclien_dll <fver(10,0,0,3802))
					  ||  (_drmstor_dll  >zero && _wmp_dll<fver(11,0,0,0) && _drmstor_dll  <fver(10,0,0,3802))
					  ||  (_drmupgds_exe >zero && _drmupgds_exe <fver(10,0,0,3802))
					  ||  (_drmv2clt_dll >zero && _drmv2clt_dll <fver(10,0,0,3802))
					  ||  (_LAPRXY_dll   >zero && _LAPRXY_dll   <fver(10,0,0,3802))
					  ||  (_logagent_exe >zero && _logagent_exe <fver(10,0,0,3802))
					  ||  (_msnetobj_dll >zero && _msnetobj_dll <fver(10,0,0,3802))
					  ||  (_mspmsnsv_dll >zero && _mspmsnsv_dll <fver(10,0,3790,3802))
					  ||  (_mspmsp_dll   >zero && _mspmsp_dll   <fver(10,0,3790,3802))
					  ||  (_msscp_dll    >zero && _msscp_dll    <fver(10,0,0,3802))
					  ||  (_mswmdm_dll   >zero && _mswmdm_dll   <fver(10,0,3790,3802))
					  ||  (_qasf_dll     >zero && _qasf_dll     <fver(10,0,0,3802))
					  ||  (_uwdf_exe     >zero && _uwdf_exe     <fver(5,2,3790,1230))
					  ||  (_wdfapi_dll   >zero && _wdfapi_dll   <fver(5,2,3790,1230))
					  ||  (_wdfmgr_exe   >zero && _wdfmgr_exe   <fver(5,2,3790,1230))
					  ||  (_WMADMOD_dll  >zero && _WMADMOD_dll  <fver(10,0,0,3802))
					  ||  (_WMADMOE_dll  >zero && _WMADMOE_dll  <fver(10,0,0,3802))
					  ||  (_wmasf_dll    >zero && _wmasf_dll    <fver(10,0,0,3802))
					  ||  (_wmdmlog_dll  >zero && _wmdmlog_dll  <fver(10,0,3790,3802))
					  ||  (_wmdmps_dll   >zero && _wmdmps_dll   <fver(10,0,3790,3802))
					  ||  (_wmdrmdev_dll >zero && _wmdrmdev_dll <fver(10,0,0,3802))
					  ||  (_wmdrmnet_dll >zero && _wmdrmnet_dll <fver(10,0,0,3802))
					  ||  (_wmidx_dll    >zero && _wmidx_dll    <fver(10,0,0,3802))
					  ||  (_wmnetmgr_dll >zero && _wmnetmgr_dll <fver(10,0,0,3802))
					  ||  (_wmsdmod_dll  >zero && _wmsdmod_dll  <fver(10,0,0,3802))
					  ||  (_wmsdmoe2_dll >zero && _wmsdmoe2_dll <fver(10,0,0,3802))
					  ||  (_wmsetsdk_exe >zero && _wmsetsdk_exe <fver(10,0,0,3802))
					  ||  (_wmspdmod_dll >zero && _wmspdmod_dll <fver(10,0,0,3802))
					  ||  (_WMSPDMOE_dll >zero && _WMSPDMOE_dll <fver(10,0,0,3802))
					  ||  (_WMVADVD_dll  >zero && _WMVADVD_dll  <fver(10,0,0,3802))
					  ||  (_WMVADVE_DLL  >zero && _WMVADVE_DLL  <fver(10,0,0,3802))
					  ||  (_wmvcore_dll  >zero && _wmvcore_dll  <fver(10,0,0,3802))
					  ||  (_wmvdmod_dll  >zero && _wmvdmod_dll  <fver(10,0,0,3802))
					  ||  (_wmvdmoe2_dll >zero && _wmvdmoe2_dll <fver(10,0,0,3802))
//					  ||  (_wpdconns_dll >zero && _wpdconns_dll <fver(5,2,3802,3802))
//					  ||  (_wpdmtp_dll   >zero && _wpdmtp_dll   <fver(5,2,3802,3802))
//					  ||  (_wpdmtpdr_dll >zero && _wpdmtpdr_dll <fver(5,2,3802,3802))
//					  ||  (_wpdmtpus_dll >zero && _wpdmtpus_dll <fver(5,2,3802,3802))
//					  ||  (_wpdsp_dll    >zero && _wpdsp_dll    <fver(5,2,3802,3802))
//					  ||  (_wpdtrace_dll >zero && _wpdtrace_dll <fver(5,2,3802,3802))
//					  ||  (_wpdusb_sys   >zero && _wpdusb_sys   <fver(5,2,3802,3802))
//					  ||  (_wpd_ci_dll   >zero && _wpd_ci_dll   <fver(5,2,3802,3802))
					      )) {
		kb891122=true;
		NN("Update for WMDRM-enabled Media Players (KB891122)");
		XX(p2+"windowsmedia-kb891122-x86-enu_c1b14007407ee75d546dfd820af4f52eaf5e18f9.exe"+a1);
	}
//	if( sp>=2 && (sku & XP_ALL) && _wmp_dll>=fver(10,0,0,3646) && _wmp_dll<fver(10,0,0,3909)) {
//		NN("Update for Windows XP (KB895572)");
//		XX(p+"windowsmedia10-kb895572-x86-enu.exe"+a1);
//	}
	if( sp==2 && (sku & XP_ALL) && (
		                  ( _qdvd_dll   >zero && _qdvd_dll   <fver(6,5,2600,2709))
					  ||  ( _quartz_dll >zero && _quartz_dll <fver(6,5,2600,2709)) )) {
		NN("Update for Windows XP (KB891593)");
		XX(p2+"WindowsXP-KB891593-v2-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _termsrv_dll>zero && _termsrv_dll<fver(5,1,2600,3352))
	 ||(sp==3 && (sku & XP_ALL) && _termsrv_dll>zero && _termsrv_dll<fver(5,1,2600,5581))) {
		NN("Update for Windows XP (KB895961)");
		XX(p3+"WindowsXP-KB895961-v4-x86-ENU.exe"+a1);
	}
//	if( sp==2 && (sku & XP_ALL) && (
//		                  ( _ndisuio_sys  >zero && _ndisuio_sys  <fver(5,1,2600,2626))
//					  ||  ( _netshell_dll >zero && _netshell_dll <fver(5,1,2600,2626))
//					  ||  ( _wzcdlg_dll   >zero && _wzcdlg_dll   <fver(5,1,2600,2676))
//					  ||  ( _wzcsapi_dll  >zero && _wzcsapi_dll  <fver(5,1,2600,2626))
//					  ||  ( _wzcsvc_dll   >zero && _wzcsvc_dll   <fver(5,1,2600,2626)) )) {
//		// KB89937 is replaced by KB915428
//		NN("Update for Windows XP (KB899337)");
//		XX(p+"WindowsXP-KB899337-v2-x86-ENU.exe"+a1);
//	}
//	if( sp==2 && (sku & XP_MCE2005) && (
//			(    _sbp2port_sys       <fver(5,1,2600,2688)
//		      && _sbp2port_sys_cache <fver(5,1,2600,2688))
//			|| (_sbp2port_sys>zero && _sbp2port_sys <fver(5,1,2600,2688))
//			|| !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB899510"))) {
//		NN("Update for Windows XP (KB899510)");
//		XX(p+"WindowsXP-KB899510-x86.exe"+a1);
//	}
//	if( sp==2 && (sku & XP_MCE2005) && (
//		                  ( _hidir_sys >zero && _hidir_sys <fver(5,1,2600,2709))
//					  ||  ( _irbus_sys >zero && _irbus_sys <fver(5,1,2600,2709)) )) {
//		// Included in KB900325, but also KB888795 is replaced by KB912024
//		NN("Update Rollup 1 for eHome Infrared Receiver for Windows XP Media Center Edition (KB888795)");
//		XX(p+"windowsxp-kb888795-v3-x86-enu_35a20ee3889859f9e0db3d9899c0a679664726be.exe"+a1);
//	}
//	if( sp==2 && (sku & XP_MCE2005) && (
//			(    _powerfil_sys       <fver(5,1,2600,2707)
//		      && _powerfil_sys_cache <fver(5,1,2600,2707))
//			|| (_powerfil_sys>zero && _powerfil_sys <fver(5,1,2600,2707))
//			|| !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB902841"))) {
//		NN("Update for Windows XP (KB902841)");
//		XX(p+"WindowsXP-KB902841-x86.exe"+a1);
//	}
	if(sp>=2 && (sku & XP_MCE2005) && _ehkeyctl_dll>zero && _ehkeyctl_dll<fver(5,1,2715,5512)) {
		NN("Security Update for Windows XP (KB973768)");
		XX(p3+"windowsxpmediacenter2005-kb973768-enu_05fb491b522e4265834136d7288711e5fdb827e4.exe"+a1);
	}
	if(sp>=2 && (sku & XP_MCE2005) && *nfx11<0) {
		// Update Rollup 2 (KB900325) requires .NET Framework 1.1 SP1
		NN("Microsoft .NET Framework 1.1");
		XX(np+"dotnetf11.exe /q:a /c:\"install.exe /qb /l\"");
	}
	if(sp>=2 && (sku & XP_MCE2005) && *nfx11<1){
		// Update Rollup 2 (KB900325) requires .NET Framework 1.1 SP1
		NN("Microsoft .NET Framework 1.1 Service Pack 1 (KB867460)");
		XX(p3+"NDP1.1sp1-KB867460-X86.exe"+a2);
	}
	std::string temp;
	if( sp>=2 && (sku & XP_MCE2005) && (
		    ( _audiodepthconverter_ax    <fver(6,0,0,1))
		||  ( _authorscript_dll          <fver(6,0,0,1))
		||  ( _bdatunepia_dll            <fver(5,1,2710,2732))
		||  ( _createdisc_dll            <fver(2,0,0,0))
		||  ( _directshowtap_ax          <fver(6,0,0,1))
		||  ( _ehcett_dll                <fver(5,1,2710,2732))
		||  ( _ehchhime_dll              <fver(5,1,2710,2732))
		||  ( _ehchtime_dll              <fver(5,1,2710,2732))
		||  ( _ehcir_dll                 <fver(5,1,2710,2732))
		||  ( _ehcm_dll                  <fver(5,1,2710,2732))
		||  ( _ehcommon_dll              <fver(5,1,2710,2732))
		||  ( _ehcyrtt_dll               <fver(5,1,2710,2732))
		||  ( _ehdebug_dll               <fver(5,1,2710,2732))
		||  ( _ehdrop_dll                <fver(5,1,2710,2732))
		||  ( _ehentt_dll                <fver(5,1,2710,2732))
		||  ( _ehepg_dll                 <fver(5,1,2710,2732))
		||  ( _ehepgdat_dll              <fver(5,1,2710,2732))
		||  ( _ehextcom_dll              <fver(5,1,2710,2732))
		||  ( _ehexthost_exe             <fver(5,1,2710,2732))
		||  ( _ehglid_dll                <fver(5,1,2710,2732))
		||  ( _ehiextcom_dll             <fver(5,1,2710,2732))
		||  ( _ehiextens_dll             <fver(5,1,2710,2732))
		||  ( _ehiplay_dll               <fver(5,1,2710,2732))
		||  ( _ehiproxy_dll              <fver(5,1,2710,2732))
		||  ( _ehiuserxp_dll             <fver(5,1,2710,2732))
		||  ( _ehividctl_dll             <fver(5,1,2710,2732))
		||  ( _ehiwuapi_dll              <fver(5,1,2710,2732))
		||  ( _ehjpnime_dll              <fver(5,1,2710,2732))
		||  ( _ehkorime_dll              <fver(5,1,2710,2732))
		||  ( _ehmcxins_exe              <fver(6,1,2710,2732))
		||  ( _ehmsas_exe                <fver(5,1,2710,2732))
		||  ( _ehplayer_dll              <fver(5,1,2710,2732))
		||  ( _ehproxy_dll               <fver(5,1,2710,2732))
		||  ( _ehrec_exe                 <fver(5,1,2710,2732))
		||  ( _ehrecobj_dll              <fver(5,1,2710,2732))
		||  ( _ehrecvr_exe               <fver(5,1,2710,2732))
		||  ( _ehres_dll                 <fver(5,1,2710,2732))
		||  ( _ehreschs_dll              <fver(5,1,2710,2732))
		||  ( _ehrescht_dll              <fver(5,1,2710,2732))
		||  ( _ehrescs_dll               <fver(5,1,2710,2732))
		||  ( _ehresda_dll               <fver(5,1,2710,2732))
		||  ( _ehresde_dll               <fver(5,1,2710,2732))
		||  ( _ehreses_dll               <fver(5,1,2710,2732))
		||  ( _ehresfi_dll               <fver(5,1,2710,2732))
		||  ( _ehresfr_dll               <fver(5,1,2710,2732))
		||  ( _ehresit_dll               <fver(5,1,2710,2732))
		||  ( _ehresja_dll               <fver(5,1,2710,2732))
		||  ( _ehresko_dll               <fver(5,1,2710,2732))
		||  ( _ehresnl_dll               <fver(5,1,2710,2732))
		||  ( _ehresno_dll               <fver(5,1,2710,2732))
		||  ( _ehrespl_dll               <fver(5,1,2710,2732))
		||  ( _ehrespt_dll               <fver(5,1,2710,2732))
		||  ( _ehresru_dll               <fver(5,1,2710,2732))
		||  ( _ehressv_dll               <fver(5,1,2710,2732))
		||  ( _ehrestr_dll               <fver(5,1,2710,2732))
		||  ( _ehsched_exe               <fver(5,1,2710,2732))
		||  ( _ehshell_exe               <fver(5,1,2710,2732))
		||  ( _sqldb20_dll               <fver(2,2,6712,0))
		||  ( _sqlqp20_dll               <fver(2,2,6712,0))
		||  ( _sqlse20_dll               <fver(2,2,6712,0))
		||  ( _ehtktt_dll                <fver(5,1,2710,2732))
		||  ( _ehtrace_dll               <fver(5,1,2710,2732))
		||  ( _ehtray_exe                <fver(5,1,2710,2732))
		||  ( _ehui_dll                  <fver(5,1,2710,2732))
		||  ( _ehuihlp_dll               <fver(5,1,2710,2732))
		||  ( _encdec_dll                <fver(6,5,2710,2732))
		||  ( _mce_wmadrm_dll            <fver(2,0,0,0))
		||  ( _mcrdsvc_exe               <fver(4,1,2710,2732))
		||  ( _mcrmgr_dll                <fver(6,1,2710,2732))
		||  ( _mcrmgr_exe                <fver(6,1,2710,2732))
		||  ( _medctrro_exe              <fver(5,1,2710,2732))
		||  ( _microsoft_mediacenter_dll <fver(5,1,2710,2732))
		||  ( _mpeg2data_ax              <fver(6,5,2710,2732))
		||  ( _mpg2splt_ax               <fver(6,5,2710,2732))
		||  ( _msdvbnp_ax                <fver(6,5,2710,2732))
		||  ( _msnp_ax                   <fver(6,5,2710,2732))
		||  ( _mstvcapn_dll              <fver(5,1,2710,2732))
		||  ( _msvidctl_dll              <fver(6,5,2710,2732))
		||  ( _primosdk_dll              <fver(2,2,45,500))
		||  ( _psisdecd_dll              <fver(6,5,2710,2732))
		||  ( _psisrndr_ax               <fver(6,5,2710,2732))
		||  ( _px_dll                    <fver(2,2,45,500))
		||  ( _pxdrv_dll                 <fver(1,1,33,0))
		||  ( _pxhelp20_sys              <fver(2,3,26,0))
		||  ( _pxhpinst_exe              <fver(2,3,0,0))
		||  ( _pxmas_dll                 <fver(2,2,45,500))
		||  ( _pxwavedec_dll             <fver(2,2,45,0))
		||  ( _pxwma_dll                 <fver(1,0,0,3))
		||  ( _rtstreamsink_ax           <fver(6,0,0,1))
		||  ( _sbe_dll                   <fver(6,5,2710,2732))
		||  ( _sbeserver_exe             <fver(2,0,0,0))
		||  ( _sbeserverps_dll           <fver(2,0,0,0))
		||  ( _simpleburnuires_dll       <fver(0,9,0,0))
		||  ( _sonicmceburnengine_dll    <fver(0,9,0,0))
		||  ( _sonicmmburnengine_exe     <fver(2,0,0,0))
		||  ( _sonicmpegaudio_dll        <fver(3,0,0,14))
		||  ( _sonicmpegsplitter_dll     <fver(3,0,0,19))
		||  ( _sonicmpegvideo_dll        <fver(3,0,0,19))
//		||  ( _spuninst_exe              <fver(5,6,1002,0))
//		||  ( _spupdsvc_exe              <fver(5,6,1002,0))
		||  ( _standardfx_plugin_dll     <fver(2,0,0,0))
		||  ( _tenfoothelp_exe           <fver(5,6,1000,0))
		||  ( _vbicodec_ax               <fver(6,5,2710,2732))
		||  ( _vxblock_dll               <fver(1,0,62,0))
		||  ( _wstpager_ax               <fver(6,5,2710,2732))
		||  ( _wstrenderer_ax            <fver(6,5,2710,2732)) )) {
	kb900325=true;
	NN("Update Rollup 2 for Windows XP Media Center Edition 2005 (KB900325)");
	//XX(p3+"WindowsXPMediaCenter2005-KB900325-usa.exe /quiet /norestart");
	temp=p3+"WindowsXPMediaCenter2005-KB900325-usa.exe /Q /x:%TEMP%\\KB900325\n"
		  +"pushd %TEMP%\\KB900325\n"
		  +"md install\n"
		  +"copy bin\\WindowsXP-KB891593-x86.exe install\\\n"
		  +"copy bin\\WindowsXP-KB899337-v2-x86.exe install\\\n"
		  +"copy bin\\WindowsXP-KB899510-x86.exe install\\\n"
		  +"copy bin\\WindowsXP-KB888795-x86.exe install\\\n"
		  +"copy bin\\WindowsXP-KB902841-x86.exe install\\\n";
		if( sp>=2 && (sku & XP_ALL) && _wmp_dll>=fver(10,0,0,3646) && _wmp_dll<fver(10,0,0,3937)) {
			//NN("Update for Windows XP (KB895572)"); // Actually it is KB903157
			temp+="bin\\WindowsMedia10-KB895572-x86.exe"+a1+"\n";
		}
//		if( sp==2 && (sku & XP_ALL) && (
//							  ( _qdvd_dll   >zero && _qdvd_dll   <fver(6,5,2600,2709))
//						  ||  ( _quartz_dll >zero && _quartz_dll <fver(6,5,2600,2709)) )) {
//			//NN("Update for Windows XP (KB891593)");
//			temp+="install\\WindowsXP-KB891593-x86.exe"+a1+"\n";
//		}
		if( sp==2 && (sku & XP_ALL) && (
							  ( _ndisuio_sys  >zero && _ndisuio_sys  <fver(5,1,2600,2626))
						  ||  ( _netshell_dll >zero && _netshell_dll <fver(5,1,2600,2626))
						  ||  ( _wzcdlg_dll   >zero && _wzcdlg_dll   <fver(5,1,2600,2676))
						  ||  ( _wzcsapi_dll  >zero && _wzcsapi_dll  <fver(5,1,2600,2626))
						  ||  ( _wzcsvc_dll   >zero && _wzcsvc_dll   <fver(5,1,2600,2626)) )) {
			//NN("Update for Windows XP (KB899337)");
			temp+="install\\WindowsXP-KB899337-v2-x86.exe"+a1+"\n";
		}
		if( sp==2 && (sku & XP_MCE2005) && (
				(    _sbp2port_sys       <fver(5,1,2600,2688)
				  && _sbp2port_sys_cache <fver(5,1,2600,2688))
				|| (_sbp2port_sys>zero && _sbp2port_sys <fver(5,1,2600,2688))
				|| !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB899510"))) {
			//NN("Update for Windows XP (KB899510)");
			temp+="install\\WindowsXP-KB899510-x86.exe"+a1+"\n";
		}
		if( sp==2 && (sku & XP_MCE2005) && (
							  ( _hidir_sys >zero && _hidir_sys <fver(5,1,2600,2709))
						  ||  ( _irbus_sys >zero && _irbus_sys <fver(5,1,2600,2709))
						  ||    _hidir_sys_cache < fver(5,1,2600,2709)
						  ||    _irbus_sys_cache < fver(5,1,2600,2709) )) {
			//NN("Update Rollup 1 for eHome Infrared Receiver for Windows XP Media Center Edition (KB888795)");
			temp+="install\\WindowsXP-KB888795-x86.exe"+a1+"\n";
		}
		if( sp==2 && (sku & XP_MCE2005) && (
				(    _powerfil_sys       <fver(5,1,2600,2707)
				  && _powerfil_sys_cache <fver(5,1,2600,2707))
				|| (_powerfil_sys>zero && _powerfil_sys <fver(5,1,2600,2707))
				|| !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB902841"))) {
			//NN("Update for Windows XP (KB902841)");
			temp+="install\\WindowsXP-KB902841-x86.exe"+a1+"\n";
		}
		temp+="bin\\KB900325.exe /passive /norestart\n";
//		temp+="bin\\wmfdist95.exe /Q:A /R:N /c:\"wmsetsdk.exe /WMFDIST /Q /R:N /DisallowSystemRestore\"\n";
		temp+="bin\\wmfdist95.exe /Q:U /R:N /c:\"wmsetsdk.exe /WMFDIST /Q:U /R:N /DisallowSystemRestore\"\n";
		temp+="popd\nrd /S /Q %TEMP%\\KB900325";
//	XX(p+"WindowsXPMediaCenter2005-KB900325-usa.exe /Q /x:%TEMP%\\KB900325\n"
//		+"pushd %TEMP%\\KB900325\\bin\n"
//		+"WindowsMedia10-KB895572-x86.exe /passive /norestart\n"
//		+"WindowsXP-KB891593-x86.exe /passive /norestart\n"
//		+"WindowsXP-KB895961-x86.exe /passive /norestart\n"
//		+"WindowsXP-KB899337-v2-x86.exe /passive /norestart\n"
//		+"WindowsXP-KB899510-x86.exe /passive /norestart\n"
//		+"WindowsXP-KB888795-x86.exe /passive /norestart\n"
//		+"WindowsXP-KB902841-x86.exe /passive /norestart\n"
//		+"KB900325.exe /passive /norestart\n"
//		+"wmfdist95.exe /Q:A /R:N /c:\"wmsetsdk.exe /WMFDIST /Q /R:N /DisallowSystemRestore\"\n"
//		+"popd\nrd /S /Q %TEMP%\\KB900325");
		XX(temp);
	}
	if(sp>=2 && (sku & XP_MCE2005) && ( kb900325
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.0"
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.1" )
		 && (/*_mstvcapn_dll>zero &&*/ _mstvcapn_dll<fver(5,1,2715,5512))) {
		NN("Security Update for Windows XP Media Center Edition 2005 Update Rollup 2 X86 Edition (KB2628259)");
		XX(p3+"windowsxpmediacenter2005-kb2628259-enu_78c8b91dfe474d076c66d74e8454960e5bc23fa3.exe"+a1);
	}
	if(sp>=2 && (sku & XP_MCE2005) && ( kb900325
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.0"
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.1" )
		 && ((/*_encdec_dll>zero &&*/ _encdec_dll<fver(6,5,2715,5512))
		  || !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP3\\KB2619340") )) {
		NN("Security Update for Windows XP Media Center Edition 2005 Update Rollup 2 X86 Edition (KB2619340)");
		XX(p3+"windowsxpmediacenter2005-kb2619340-enu_074452832167002c2b8388847294a6a5950d78f1.exe"+a1);
	}
	if(sp>=2 && (sku & XP_MCE2005) && ( kb900325
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.0"
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.1" )
		 && ((/*_encdec_dll>zero &&*/ _encdec_dll<fver(6,5,2715,5512))
		  || (_sbe_dll   >zero && _sbe_dll   <fver(6,5,2715,5512)) )) {
		NN("Security Update for Windows XP Media Center Edition 2005 Update Rollup 2 X86 Edition (KB2502898)");
		XX(p3+"windowsxpmediacenter2005-kb2502898-enu_6449e0bc751c138c0a5a31fe0394677038616056.exe"+a1);
	}
	if(sp>=2 && (sku & XP_MCE2005)
		&& ( kb900325 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.0" )
		&& (    (_blackbox_dll >zero && _blackbox_dll <fver(11,0,0,4346))
		     || (_cewmdm_dll   >zero && _cewmdm_dll   <fver(10,0,3790,4332))
		     || (_drmupgds_exe >zero && _drmupgds_exe <fver(11,0,0,4332))
		     || (_drmv2clt_dll >zero && _drmv2clt_dll <fver(11,0,0,4346))
		     || (_kb913800_exe >zero && _kb913800_exe <fver(11,0,0,4350))
		     || (_LAPRXY_dll   >zero && _LAPRXY_dll   <fver(10,0,0,4332))
		     || (_logagent_exe >zero && _logagent_exe <fver(10,0,0,4332))
		     || (_mfplat_dll   >zero && _mfplat_dll   <fver(11,0,0,4332))
		     || (_msnetobj_dll >zero && _msnetobj_dll <fver(11,0,0,4332))
		     || (_mspmsnsv_dll >zero && _mspmsnsv_dll <fver(10,0,3790,4332))
		     || (_mspmsp_dll   >zero && _mspmsp_dll   <fver(10,0,3790,4332))
		     || (_msscp_dll    >zero && _msscp_dll    <fver(11,0,0,4332))
		     || (_mswmdm_dll   >zero && _mswmdm_dll   <fver(10,0,3790,4332))
		     || (_qasf_dll     >zero && _qasf_dll     <fver(10,0,0,4332))
//		     || (_spmsg_dll    >zero && _spmsg_dll    <fver(6,2,29,0))
//		     || (_spuninst_exe >zero && _spuninst_exe <fver(6,2,29,0))
//		     || (_spupdsvc_exe >zero && _spupdsvc_exe <fver(6,2,29,0))
		     || (_uwdf_exe     >zero && _uwdf_exe     <fver(5,2,3790,1230))
		     || (_wdfapi_dll   >zero && _wdfapi_dll   <fver(5,2,3790,1230))
		     || (_wdfmgr_exe   >zero && _wdfmgr_exe   <fver(5,2,3790,1230))
		     || (_WMADMOD_dll  >zero && _WMADMOD_dll  <fver(10,0,0,4332))
		     || (_WMADMOE_dll  >zero && _WMADMOE_dll  <fver(10,0,0,4332))
		     || (_wmasf_dll    >zero && _wmasf_dll    <fver(10,0,0,4332))
		     || (_wmdmlog_dll  >zero && _wmdmlog_dll  <fver(10,0,3790,4332))
		     || (_wmdmps_dll   >zero && _wmdmps_dll   <fver(10,0,3790,4332))
		     || (_wmdrmdev_dll >zero && _wmdrmdev_dll <fver(10,0,0,4332))
		     || (_wmdrmnet_dll >zero && _wmdrmnet_dll <fver(10,0,0,4332))
		     || (_wmdrmsdk_dll >zero && _wmdrmsdk_dll <fver(11,0,0,4332))
		     || (_wmidx_dll    >zero && _wmidx_dll    <fver(10,0,0,4332))
		     || (_wmnetmgr_dll >zero && _wmnetmgr_dll <fver(10,0,0,4332))
		     || (_wmp_dll      >zero && _wmp_dll      <fver(10,0,0,3937))
		     || (_wmplayer_exe >zero && _wmplayer_exe <fver(10,0,0,3931))
		     || (_wmploc_dll   >zero && _wmploc_dll   <fver(10,0,0,3931))
		     || (_wmsdmod_dll  >zero && _wmsdmod_dll  <fver(10,0,0,4332))
		     || (_wmsdmoe2_dll >zero && _wmsdmoe2_dll <fver(10,0,0,4332))
		     || (_wmsetsdk_exe >zero && _wmsetsdk_exe <fver(10,0,0,4332))
		     || (_wmspdmod_dll >zero && _wmspdmod_dll <fver(10,0,0,4332))
		     || (_WMSPDMOE_dll >zero && _WMSPDMOE_dll <fver(10,0,0,4332))
		     || (_WMVADVD_dll  >zero && _WMVADVD_dll  <fver(10,0,0,4332))
		     || (_WMVADVE_DLL  >zero && _WMVADVE_DLL  <fver(10,0,0,4332))
		     || (_wmvcore_dll  >zero && _wmvcore_dll  <fver(10,0,0,4332))
		     || (_wmvdmod_dll  >zero && _wmvdmod_dll  <fver(10,0,0,4332))
		     || (_wmvdmoe2_dll >zero && _wmvdmoe2_dll <fver(10,0,0,4332))
		     || (_wpdconns_dll >zero && _wpdconns_dll <fver(5,2,3811,4346))
		     || (_wpdmtp_dll   >zero && _wpdmtp_dll   <fver(5,2,3811,4346))
		     || (_wpdmtpdr_dll >zero && _wpdmtpdr_dll <fver(5,2,3811,4346))
		     || (_wpdmtpus_dll >zero && _wpdmtpus_dll <fver(5,2,3811,4346))
		     || (_wpdsp_dll    >zero && _wpdsp_dll    <fver(5,2,3811,4346))
		     || (_wpdtrace_dll >zero && _wpdtrace_dll <fver(5,2,3811,4346))
		     || (_wpdusb_sys   >zero && _wpdusb_sys   <fver(5,2,3811,4346))
		     || (_wpd_ci_dll   >zero && _wpd_ci_dll   <fver(5,2,3811,4346)) )) {
		NN("Update for Windows XP Media Center Edition 2005 (KB913800)");
		XX(p3+"windowsmedia10-kb913800-x86-enu_d92cb6ac9b52c7926f8f94d65217ed297c45fecc.exe"+a1);
		kb913800=true;
	}
	if(sp==2 && (sku & XP_MCE2005) &&  (_wmploc_dll==fver(10,0,0,3931) ||
				( _wmploc_dll<=fver(10,0,0,3931) && kb913800 ))
			&& ((_setup_wm_exe >zero && _setup_wm_exe <fver(10,0,0,3931)) )) {
		NN("Update for Windows XP Media Center Edition 2005 (KB926251)");
		XX(p2+"windowsmedia10-kb926251-x86-enu_1e6ce415f8de5a8a3b43e9917daf0f501493dde5.exe"+a1);
	}
	/*if(sp>=2 && (sku & XP_MCE2005) && ((_ehepg_dll >zero && _ehepg_dll <fver(5,1,2700,2567)) )) {
		// KB888316 is replaced by KB925766
		NN("Update for Windows XP Media Center Edition 2005 (KB888316)");
		XX(p3+"KB888316_enu.exe"+a6);
	}*/
	if(sp>=2 && (sku & XP_MCE2005)
		&& ( kb900325 || regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Media Center",L"Ident",&status)==L"4.0" )
		&& (    (_ehcm_dll     >zero && _ehcm_dll     <fver(5,1,2715,3011))
		     || (_ehepg_dll    >zero && _ehepg_dll    <fver(5,1,2715,3011))
		     || (_ehglid_dll   >zero && _ehglid_dll   <fver(5,1,2715,3011))
		     || (_ehkeyctl_dll >zero && _ehkeyctl_dll <fver(5,1,2715,3011))
		     || (_ehrecvr_exe  >zero && _ehrecvr_exe  <fver(5,1,2715,3011))
		     || (_ehshell_exe  >zero && _ehshell_exe  <fver(5,1,2715,3011))
		     || (_ehui_dll     >zero && _ehui_dll     <fver(5,1,2715,3011))
		     || (_encdec_dll   >zero && _encdec_dll   <fver(6,5,2715,3011))
		     || (_mstvcapn_dll >zero && _mstvcapn_dll <fver(5,1,2715,3011))
		     || (_msvidctl_dll >zero && _msvidctl_dll <fver(6,5,2715,3011))
		     || (_psisdecd_dll >zero && _psisdecd_dll <fver(6,5,2715,3011))
		     || (_psisrndr_ax  >zero && _psisrndr_ax  <fver(6,5,2715,3011))
		     || (_pxdrv_dll    >zero && _pxdrv_dll    <fver(1,1,63,0))
		     || (_sbe_dll      >zero && _sbe_dll      <fver(6,5,2715,3011)) )) {
		NN("Update Rollup for Windows XP Media Center Edition 2005 (KB925766)");
		XX(p3+"windowsxpmediacenter2005-kb925766-enu_30e90395a057a5e974768536ee7eaf014670ed2c.exe"+a1);
	}
	if( sp==2 && (sku & XP_MCE2005) && ( kb900325
		              || ( _hidir_sys >zero && _hidir_sys <fver(5,1,2600,2825))
					  || ( _irbus_sys >zero && _irbus_sys <fver(5,1,2600,2825)) )) {
		NN("Update for Windows XP Media Center Edition 2005 (KB912024)");
		XX(p2+"windowsxp-kb912024-v2-x86-enu_0e305ca77d97d5ee54ea79e3f43bedaba844705b.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (
		  ( _ndisuio_sys  >zero && _ndisuio_sys  <fver(5,1,2600,2858))
	  ||  ( _netshell_dll >zero && _netshell_dll <fver(5,1,2600,2858))
	  ||  ( _wzcdlg_dll   >zero && _wzcdlg_dll   <fver(5,1,2600,2858))
	  ||  ( _wzcsapi_dll  >zero && _wzcsapi_dll  <fver(5,1,2600,2858))
	  ||  ( _wzcsvc_dll   >zero && _wzcsvc_dll   <fver(5,1,2600,2858)) )) {
		NN("Update for Windows XP (KB915428)");
		XX(p2+"WindowsXP-KB915428-x86-ENU.exe"+a1);
	}

	// Windows XP SP3 updates;
	if( sp==3 && (sku & XP_ALL) && _packager_exe>zero && _packager_exe<fver(5,1,2600,6176)) {
		NN("Security Update for Windows XP (KB2584146)");
		XX(p3+"WindowsXP-KB2584146-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _ndproxy_sys>zero && _ndproxy_sys<fver(5,1,2600,6484)) {
		NN("Security Update for Windows XP (KB2914368)");
		XX(p3+"WindowsXP-KB2914368-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _iacenc_dll<fver(1,0,0,0)) {
		NN("Security Update for Windows XP (KB2661637)");
		XX(p3+"WindowsXP-KB2661637-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _ndistapi_sys>zero && _ndistapi_sys<fver(5,1,2600,6132)) {
		NN("Security Update for Windows XP (KB2566454)");
		XX(p3+"WindowsXP-KB2566454-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _csrsrv_dll>zero && _csrsrv_dll<fver(5,1,2600,6166)) {
		NN("Security Update for Windows XP (KB2620712)");
		XX(p3+"WindowsXP-KB2620712-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,6055))
		              ||  (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,6055))
					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,6055))
					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,6055))
					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,6055)) )) {
		NN("Security Update for Windows XP (KB2393802)");
		XX(p3+"WindowsXP-KB2393802-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _lsasrv_dll>zero && _lsasrv_dll<fver(5,1,2600,6058)) {
		NN("Security Update for Windows XP (KB2478960)");
		XX(p3+"WindowsXP-KB2478960-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _iccvid_dll>zero && _iccvid_dll<fver(1,10,0,13)) {
		NN("Security Update for Windows XP (KB982665)");
		XX(p3+"WindowsXP-KB982665-x86-ENU.exe"+a1);
	}
	if( sp>=2 && !checkCertificates("Root")) {
		NN("Update for Root Certificates for Windows XP [March 2014] (KB931125)");
		XX(sw+p3+"rootsupd_01a26e5c75ff5b3e34fb6b763ace486fe6836aac.exe");
	}
	if( sp>=2 && !checkCertificates("Disallowed")) {
		NN("Update for Certificate Revocation List [March 2015] (KB3050995)");
		XX(sw+p3+"rvkroots_3f2ce4676450c06f109b5b4e68bec252873ccc21.exe");
	}
	if( sp==3 && (sku & XP_ALL) && ( (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,6206))
		              ||  (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,6206))
					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,6206))
					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,6206))
					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,6206)) )) {
		NN("Security Update for Windows XP (KB2676562)");
		XX(p3+"WindowsXP-KB2676562-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_afd_sys>zero && _afd_sys<fver(5,1,2600,5695))
		              ||  (_dnsapi_dll>zero && _dnsapi_dll<fver(5,1,2600,6089))
					  ||  (_dnsrslvr_dll>zero && _dnsrslvr_dll<fver(5,1,2600,5797))
					  ||  (_mswsock_dll>zero && _mswsock_dll<fver(5,1,2600,5625))
					  ||  (_tcpip_sys>zero && _tcpip_sys<fver(5,1,2600,5625))
					  ||  (_tcpip6_sys>zero && _tcpip6_sys<fver(5,1,2600,5625)) )) {
		NN("Security Update for Windows XP (KB2509553)");
		XX(p3+"WindowsXP-KB2509553-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_usbccgp_sys>zero && _usbccgp_sys<fver(5,1,2600,6437))
		              ||  (_usbd_sys>zero && _usbd_sys<fver(5,1,2600,6437))
					  ||  (_usbehci_sys>zero && _usbehci_sys<fver(5,1,2600,5778))
					  ||  (_usbport_sys>zero && _usbport_sys<fver(5,1,2600,6437)) )) {
		NN("Security Update for Windows XP (KB2862330)");
		XX(p3+"WindowsXP-KB2862330-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _rdpwd_sys>zero && _rdpwd_sys<fver(5,1,2600,6258)) {
		NN("Security Update for Windows XP (KB2723135)");
		XX(p3+"WindowsXP-KB2723135-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _synceng_dll>zero && _synceng_dll<fver(5,1,2600,6293)) {
		NN("Security Update for Windows XP (KB2727528)");
		XX(p3+"WindowsXP-KB2727528-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_browser_dll>zero && _browser_dll<fver(5,1,2600,6260))
					  ||  (_netapi32_dll>zero && _netapi32_dll<fver(5,1,2600,6260)) )) {
		NN("Security Update for Windows XP (KB2705219)");
		XX(p3+"WindowsXP-KB2705219-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_msado15_dll>zero && !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP4\\KB2698365"))
		            //||  (_msadco_dll>zero && _msadco_dll<fver(2,81,3014,0))
		              ||  (_msado15_dll>zero && _msado15_dll<fver(2,81,3014,0))
		// NOTE: A lot of these files are QFE only (not GDR), so testing for them is misleading.
					/*||  (_msadomd_dll>zero && _msadomd_dll<fver(2,81,3014,0))
					  ||  (_msador15_dll>zero && _msador15_dll<fver(2,81,3014,0))
					  ||  (_msadox_dll>zero && _msadox_dll<fver(2,81,3014,0))
					  ||  (_msjro_dll>zero && _msjro_dll<fver(2,81,3014,0))
					  ||  (_msado20_tlb>zero && _msado20_tlb<fver(2,81,3014,0))
					  ||  (_msado21_tlb>zero && _msado21_tlb<fver(2,81,3014,0))
					  ||  (_msado25_tlb>zero && _msado25_tlb<fver(2,81,3014,0))
					  ||  (_msado26_tlb>zero && _msado26_tlb<fver(2,81,3014,0))
					  ||  (_msado27_tlb>zero && _msado27_tlb<fver(2,81,3014,0))
					  ||  (_msado28_tlb>zero && _msado28_tlb<fver(2,81,3014,0))*/ )) {
		NN("Security Update for Windows XP (KB2698365)");
		XX(p3+"WindowsXP-KB2698365-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _scrrun_dll>zero && _scrrun_dll<fver(5,7,6002,18960)) {
		NN("Security Update for Windows XP (KB2892075)");
		XX(p3+"WindowsXP-KB2892075-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_mfc42_dll>zero && _mfc42_dll<fver(6,2,8081,0))
					  ||  (_mfc42u_dll>zero && _mfc42u_dll<fver(6,2,8081,0)) )) {
		NN("Security Update for Windows XP (KB2506212)");
		XX(p3+"WindowsXP-KB2506212-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_imagehlp_dll>zero && _imagehlp_dll<fver(5,1,2600,6198))
					  ||  (_wintrust_dll>zero && _wintrust_dll<fver(5,131,2600,6198)) )) {
		NN("Security Update for Windows XP (KB2653956)");
		XX(p3+"WindowsXP-KB2653956-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_srv_sys>zero && _srv_sys<fver(5,1,2600,6082))
					  ||  ( _xpsp4res_dll<fver(5,1,2600,6082)) )) {
		NN("Security Update for Windows XP (KB2508429)");
		XX(p3+"WindowsXP-KB2508429-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_msadco_dll>zero && _msadco_dll<fver(2,81,3012,0))
		              ||  (_msado15_dll>zero && _msado15_dll<fver(2,81,3012,0))
					  ||  (_msadomd_dll>zero && _msadomd_dll<fver(2,81,3012,0))
					  ||  (_msadox_dll>zero && _msadox_dll<fver(2,81,3012,0))
					  ||  (_msjro_dll>zero && _msjro_dll<fver(2,81,3012,0))
					  ||  (_odbc32_dll>zero && _odbc32_dll<fver(3,525,3012,0))
					  ||  (_msado20_tlb>zero && _msado20_tlb<fver(2,81,3012,0))
					  ||  (_msado21_tlb>zero && _msado21_tlb<fver(2,81,3012,0))
					  ||  (_msado25_tlb>zero && _msado25_tlb<fver(2,81,3012,0))
					  ||  (_msado26_tlb>zero && _msado26_tlb<fver(2,81,3012,0))
					  ||  (_msado27_tlb>zero && _msado27_tlb<fver(2,81,3012,0)) )) {
	// NOTE: GDR and QFE file lists and versions are identical for this patch.
		NN("Security Update for Windows XP (KB2419632)");
		XX(p3+"WindowsXP-KB2419632-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _msxml6_dll>zero && _msxml6_dll<fver(6,20,2502,0)) {
		NN("Security Update for Windows XP (KB2757638)");
		XX(p3+"WindowsXP-KB2757638-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _imagehlp_dll>zero && _imagehlp_dll<fver(5,1,2600,6479)) {
		NN("Security Update for Windows XP (KB2893294)");
		XX(p3+"WindowsXP-KB2893294-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP4\\KB2603381")) {
		NN("Security Update for Windows XP (KB2603381)");
		XX(p3+"WindowsXP-KB2603381-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _winsrv_dll>zero && _winsrv_dll<fver(5,1,2600,6368)) {
		NN("Security Update for Windows XP (KB2820917)");
		XX(p3+"WindowsXP-KB2820917-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP4\\KB2570947")) {
		NN("Security Update for Windows XP (KB2570947)");
		XX(p3+"WindowsXP-KB2570947-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_usb8023_sys>zero && _usb8023_sys<fver(5,1,2600,6352))
					  ||  (_usb8023x_sys>zero && _usb8023x_sys<fver(5,1,2600,6352)) )) {
		NN("Security Update for Windows XP (KB2807986)");
		XX(p3+"WindowsXP-KB2807986-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,6419))
					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,6419))
					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,6419))
					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,6419)) )) {
		NN("Security Update for Windows XP (KB2859537)");
		XX(p3+"WindowsXP-KB2859537-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,6460)) {
		NN("Security Update for Windows XP (KB2876331)");
		XX(p3+"WindowsXP-KB2876331-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _usp10_dll>zero && _usp10_dll<fver(1,420,2600,6421)) {
		NN("Security Update for Windows XP (KB2850869)");
		XX(p3+"WindowsXP-KB2850869-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _mup_sys>zero && _mup_sys<fver(5,1,2600,6103)) {
		NN("Security Update for Windows XP (KB2535512)");
		XX(p3+"WindowsXP-KB2535512-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _dpnet_dll>zero && _dpnet_dll<fver(5,3,2600,6311)) {
		NN("Security Update for Windows XP (KB2770660)");
		XX(p3+"WindowsXP-KB2770660-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _afd_sys>zero && _afd_sys<fver(5,1,2600,6142)) {
		NN("Security Update for Windows XP (KB2592799)");
		XX(p3+"WindowsXP-KB2592799-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _oakley_dll>zero && _oakley_dll<fver(5,1,2600,6462)) {
		NN("Security Update for Windows XP (KB2862152)");
		XX(p3+"WindowsXP-KB2862152-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_msxml3_dll>zero && _msxml3_dll<fver(8,100,1053,0))
					  ||  (_msxml6_dll>zero && _msxml6_dll<fver(6,20,2501,0)) )) {
		NN("Security Update for Windows XP (KB2719985)");
		XX(p3+"WindowsXP-KB2719985-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _themeui_dll>zero && _themeui_dll<fver(6,0,2900,6437)) {
		NN("Security Update for Windows XP (KB2864063)");
		XX(p3+"WindowsXP-KB2864063-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,6514)) {
		NN("Security Update for Windows XP (KB2930275)");
		XX(p3+"WindowsXP-KB2930275-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,6010))
		              ||  (_wordpad_exe>zero && _wordpad_exe<fver(5,1,2600,6010)) )) {
		NN("Security Update for Windows XP (KB979687)");
		XX(p3+"WindowsXP-KB979687-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_shell32_dll>zero && _shell32_dll<fver(6,0,2900,6072))
					  ||  (_shimgvw_dll>zero && _shimgvw_dll<fver(6,0,2900,6072)) )) {
		NN("Security Update for Windows XP (KB2483185)");
		XX(p3+"WindowsXP-KB2483185-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _ole32_dll>zero && _ole32_dll<fver(5,1,2600,6435)) {
		NN("Security Update for Windows XP (KB2876217)");
		XX(p3+"WindowsXP-KB2876217-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _spoolsv_exe>zero && _spoolsv_exe<fver(5,1,2600,6024)) {
		NN("Security Update for Windows XP (KB2347290)");
		XX(p3+"WindowsXP-KB2347290-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_CLIENT) && !regTestKey(L"SOFTWARE\\Microsoft\\\Updates\\Windows XP\\SP4\\KB2686509")) {
		NN("Security Update for Windows XP (KB2686509)");
		XX(p3+"WindowsXP-KB2686509-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_EMBEDDED_PLUS_FLP) && !regTestKey(L"SOFTWARE\\Microsoft\\\Updates\\Windows XP\\SP4\\KB2686509")) {
		NN("Security Update for Windows XP Embedded (KB2686509)");
		XX(p3+"WindowsXPEmbedded-KB2686509-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_mpg2splt_ax>zero && _mpg2splt_ax<fver(6,5,2600,6333))
					  ||  (_quartz_dll>zero && _quartz_dll<fver(6,5,2600,6333)) )) {
		NN("Security Update for Windows XP (KB2780091)");
		XX(p3+"WindowsXP-KB2780091-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_csrsrv_dll>zero && _csrsrv_dll<fver(5,1,2600,6104))
					  ||  (_winsrv_dll>zero && _winsrv_dll<fver(5,1,2600,6104)) )) {
		NN("Security Update for Windows XP (KB2507938)");
		XX(p3+"WindowsXP-KB2507938-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_hidparse_sys>zero && _hidparse_sys<fver(5,1,2600,6418))
					  ||  (_usbscan_sys>zero && _usbscan_sys<fver(5,1,2600,6418)) )) {
		NN("Security Update for Windows XP (KB2862335)");
		XX(p3+"WindowsXP-KB2862335-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _t2embed_dll>zero && _t2embed_dll<fver(5,1,2600,6031)) {
		NN("Security Update for Windows XP (KB982132)");
		XX(p3+"WindowsXP-KB982132-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _qedit_dll>zero && _qedit_dll<fver(6,5,2600,6512)) {
		NN("Security Update for Windows XP (KB2929961)");
		XX(p3+"WindowsXP-KB2929961-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_mciseq_dll>zero && _mciseq_dll<fver(5,1,2600,6160))
					  ||  (_winmm_dll>zero && _winmm_dll<fver(5,1,2600,6160)) )) {
		NN("Security Update for Windows XP (KB2598479)");
		XX(p3+"WindowsXP-KB2598479-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _mswrd8_wpc>zero && _mswrd8_wpc<fver(2011,1,31,10)) {
		NN("Security Update for Windows XP (KB2485663)");
		XX(p3+"WindowsXP-KB2485663-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_rpcrt4_dll>zero && _rpcrt4_dll<fver(5,1,2600,6477))
					  ||  (_xpsp4res_dll<fver(5,1,2600,6477)) )) {
		NN("Security Update for Windows XP (KB2898715)");
		XX(p3+"WindowsXP-KB2898715-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _oleaut32_dll>zero && _oleaut32_dll<fver(5,1,2600,6341)) {
		NN("Security Update for Windows XP (KB2802968)");
		XX(p3+"WindowsXP-KB2802968-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _schannel_dll>zero && _schannel_dll<fver(5,1,2600,6239)) {
		NN("Security Update for Windows XP (KB2655992)");
		XX(p3+"WindowsXP-KB2655992-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _isign32_dll>zero && _isign32_dll<fver(6,0,2900,6052)) {
		NN("Security Update for Windows XP (KB2443105)");
		XX(p3+"WindowsXP-KB2443105-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _atmfd_dll>zero && _atmfd_dll<fver(5,1,2,236)) {
		NN("Security Update for Windows XP (KB2847311)");
		XX(p3+"WindowsXP-KB2847311-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _shell32_dll>zero && _shell32_dll<fver(6,0,2900,6242)) {
		NN("Security Update for Windows XP (KB2691442)");
		XX(p3+"WindowsXP-KB2691442-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _comctl32_dll>zero && _comctl32_dll<fver(5,82,2900,6028)) {
		NN("Security Update for Windows XP (KB2296011)");
		XX(p3+"WindowsXP-KB2296011-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_qdvd_dll>zero && _qdvd_dll<fver(6,5,2600,6169))
					  ||  (_quartz_dll>zero && _quartz_dll<fver(6,5,2600,6169)) )) {
		NN("Security Update for Windows XP (KB2631813)");
		XX(p3+"WindowsXP-KB2631813-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_schannel_dll>zero && _schannel_dll<fver(5,1,2600,6175))
					  ||  (_winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,6175)) )) {
		NN("Security Update for Windows XP (KB2585542)");
		XX(p3+"WindowsXP-KB2585542-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,6133)) {
		NN("Security Update for Windows XP (KB2536276)");
		XX(p3+"WindowsXP-KB2536276-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _gdiplus_dll_23084<fver(5,2,6002,23084)) {
		NN("Security Update for Windows XP (KB2834886)");
		XX(p3+"WindowsXP-KB2834886-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _inetcomm_dll>zero && _inetcomm_dll<fver(6,0,2900,6157)) {
		NN("Security Update for Windows XP (KB2544893)");
		XX(p3+"WindowsXP-KB2544893-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _msxml3_dll>zero && _msxml3_dll<fver(8,100,1054,0)) {
		NN("Security Update for Windows XP (KB2916036)");
		XX(p3+"WindowsXP-KB2916036-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_oleacc_dll>zero && _oleacc_dll<fver(7,0,2600,6153))
		              ||  (_oleaccrc_dll>zero && _oleaccrc_dll<fver(7,0,2600,6153))
					  ||  (_uiautomationcore_dll<fver(7,0,2600,6153)) )) {
		NN("Security Update for Windows XP (KB2564958)");
		XX(p3+"Windows-en-US-KB2564958.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _gdiplus_dll_22791<fver(5,2,6002,22791)) {
		NN("Security Update for Windows XP (KB2659262)");
		XX(p3+"WindowsXP-KB2659262-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( ( /*_encdec_dll>zero &&*/ _encdec_dll<fver(6,5,2600,6076))
					  ||  (_sbe_dll>zero && _sbe_dll<fver(6,5,2600,6076)) )) {
		NN("Security Update for Windows XP (KB2479943)");
		XX(p3+"WindowsXP-KB2479943-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) /*&& _encdec_dll>zero*/ && _encdec_dll<fver(6,5,2600,6161)) {
		NN("Security Update for Windows XP (KB2619339)");
		XX(p3+"WindowsXP-KB2619339-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_mfc40_dll>zero && _mfc40_dll<fver(4,1,0,6151))
		              ||  (_mfc40u_dll>zero && _mfc40u_dll<fver(4,1,0,6151))
					  ||  (_mfc42_dll>zero && _mfc42_dll<fver(6,2,8073,0))
					  ||  (_mfc42u_dll>zero && _mfc42u_dll<fver(6,2,8073,0)) )) {
		NN("Security Update for Windows XP (KB2387149)");
		XX(p3+"WindowsXP-KB2387149-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,6532)) {
		NN("Security Update for Windows XP (KB2922229)");
		XX(p3+"WindowsXP-KB2922229-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_crypt32_dll>zero && _crypt32_dll<fver(5,131,2600,6459))
					  ||  ( _xpsp4res_dll<fver(5,1,2600,6459)) )) {
		NN("Security Update for Windows XP (KB2868626)");
		XX(p3+"WindowsXP-KB2868626-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _tzchange_exe<fver(5,1,2600,6479)) {
		NN("Update for Windows XP (KB2904266)");
		XX(p3+"WindowsXP-KB2904266-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _wab_exe>zero && _wab_exe<fver(6,0,2900,6040)) {
		NN("Security Update for Windows XP (KB2423089)");
		XX(p3+"WindowsXP-KB2423089-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( ( _exfat_sys   < fver(5,1,2600,3453))
		||   ( _fmifs_dll   < fver(5,1,2600,3453))
		||   ( _format_com > zero && _format_com  < fver(5,1,2600,3453))
		||   ( _fs_rec_sys  < fver(5,1,2600,3453))
		||   ( _ifsutil_dll < fver(5,1,2600,3453))
		||   ( _shell32_dll < fver(6,0,2900,3453))
		||   ( _uexfat_dll  < fver(5,1,2600,3453))
		||   ( _ulib_dll    < fver(5,1,2600,3453))))
	 ||(sp==3 && (sku & XP_ALL) && ( ( _exfat_sys   < fver(5,1,2600,5686))
		||   ( _fmifs_dll   < fver(5,1,2600,5686))
		||   ( _format_com > zero && _format_com  < fver(5,1,2600,5686))
		||   ( _fs_rec_sys  < fver(5,1,2600,5686))
		||   ( _ifsutil_dll < fver(5,1,2600,5686))
		||   ( _shell32_dll < fver(6,0,2900,5686))
		||   ( _uexfat_dll  < fver(5,1,2600,5686))
		||   ( _ulib_dll    < fver(5,1,2600,5686))))) {
		NN("Update for Windows XP (KB955704)"); // exFAT File System Driver
		XX(p3+"WindowsXP-KB955704-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_wintrust_dll>zero && _wintrust_dll<fver(5,131,2600,6285)) )) {
		NN("Update for Windows XP (KB2749655)");
		XX(p3+"WindowsXP-KB2749655-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_shell32_dll>zero && _shell32_dll<fver(6,0,2900,5853))
					  ||  ( _shsvcs_dll >zero && _shsvcs_dll <fver(6,0,2900,5853)) )) {
		NN("Update for Windows XP (KB971029)");
		XX(p3+"WindowsXP-KB971029-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && (
		     ( _kerberos_dll > zero && _kerberos_dll < fver(5,1,2600,3592))
		||   ( _ksecdd_sys   < fver(5,1,2600,3592))
		||   ( _lsasrv_dll   < fver(5,1,2600,3592))
		||   ( _msv1_0_dll   > zero && _msv1_0_dll   < fver(5,1,2600,3592))
		||   ( _schannel_dll > zero && _schannel_dll < fver(5,1,2600,3592))
		||   ( _secur32_dll  > zero && _secur32_dll  < fver(5,1,2600,3592))
		||   ( _wdigest_dll  > zero && _wdigest_dll  < fver(5,1,2600,3592))))
	 ||(sp==3 && (sku & XP_ALL) && (
		     ( _kerberos_dll > zero && _kerberos_dll < fver(5,1,2600,5834))
		||   ( _ksecdd_sys   < fver(5,1,2600,5834))
		||   ( _lsasrv_dll   < fver(5,1,2600,5834))
		||   ( _msv1_0_dll   > zero && _msv1_0_dll   < fver(5,1,2600,5834))
		||   ( _schannel_dll > zero && _schannel_dll < fver(5,1,2600,5834))
		||   ( _secur32_dll  > zero && _secur32_dll  < fver(5,1,2600,5834))
		||   ( _wdigest_dll  > zero && _wdigest_dll  < fver(5,1,2600,5834))))) {
		NN("Update for Windows XP (KB968389)");
		XX(p3+"WindowsXP-KB968389-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _msv1_0_dll>zero && _msv1_0_dll<fver(5,1,2600,3625))
	 ||(sp==3 && (sku & XP_ALL) && _msv1_0_dll>zero && _msv1_0_dll<fver(5,1,2600,5876))) {
		NN("Security Update for Windows XP (KB975467)");
		XX(p3+"windowsxp-kb975467-x86-enu_703673b065074bb9dd7d849f420e52ea0d343f8d.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_crypt32_dll  >zero && _crypt32_dll  <fver(5,131,2600,6369))
					  ||  ( _cryptdlg_dll >zero && _cryptdlg_dll <fver(5,1,2600,6369)) )) {
		NN("Update for Windows XP (KB2808679)");
		XX(p3+"WindowsXP-KB2808679-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (/*_aclayers_dll>zero &&*/ _aclayers_dll<fver(5,1,2600,6093)) )) {
		NN("Update for Windows XP (KB2492386)");
		XX(p3+"WindowsXP-KB2492386-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & (XP_ALL)) && (
		     ( _irbus_sys    > zero && _irbus_sys    < fver(5,1,2600,6425))
		||   ( _usbaudio_sys > zero && _usbaudio_sys < fver(5,1,2600,6425))
		||   ( _usbvideo_sys > zero && _usbvideo_sys < fver(5,1,2600,6425)) )) {
		NN("Security Update for Windows XP (KB2868038)");
		XX(p3+"WindowsXP-KB2868038-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_PLUS_FLP) && (
		     ( _kerberos_dll > zero && _kerberos_dll < fver(5,1,2600,6059)) )) {
		NN("Security Update for Windows XP (KB2478971)");
		XX(p3+"WindowsXP-KB2478971-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_PLUS_FLP) && (
		     ( _localspl_dll > zero && _localspl_dll < fver(5,1,2600,6226)) )) {
		 NN("Security Update for Windows XP (KB2712808)");
		XX(p3+"WindowsXP-KB2712808-x86-ENU.exe"+a1);
	}
	
	if( sp>=2 && (sku & XP_ALL) && (
		     ( _axaltocm_dll < fver(6,0,5066,164))
		||   ( _basecsp_dll  < fver(6,0,5066,160))
		||   ( _bcsprsrc_dll < fver(6,0,5066,164))
		||   ( _ifxcardm_dll < fver(6,0,5066,164))
		||   ( _pintool_exe  < fver(6,0,5066,164)) )) {
		NN("Microsoft Base Smart Card Cryptographic Service Provider Package: x86 (KB909520)");
		XX(p3+"Windows-KB909520-v1.000-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( ( _srv_sys    >zero && _srv_sys    <fver(5,1,2600,6031))
					  ||   ( _srvsvc_dll >zero && _srvsvc_dll <fver(5,1,2600,6031)) )) {
		// To use the features that this update includes, you must have Windows 7, Windows 2008 R2, or
		// update 968389 installed on the computer.
		NN("Update for Windows XP (KB2345886)");
		XX(p3+"WindowsXP-KB2345886-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_http_sys    >zero && _http_sys     <fver(5,1,2600,3637))
		              ||  (_httpapi_dll  >zero && _httpapi_dll  <fver(5,1,2600,3637))
					  ||  (_strmfilt_dll >zero && _strmfilt_dll <fver(6,0,2600,3637))))
	 ||(sp==3 && (sku & XP_ALL) && ( (_http_sys    >zero && _http_sys     <fver(5,1,2600,5891))
		              ||  (_httpapi_dll  >zero && _httpapi_dll  <fver(5,1,2600,5891))
					  ||  (_strmfilt_dll >zero && _strmfilt_dll <fver(6,0,2600,5891))))) {
		// To use the features that this update includes, you must have the update 968389 installed
		// on the computer.
		NN("Update for Windows XP (KB970430)");
		XX(p3+"WindowsXP-KB970430-x86-ENU.exe"+a1);
	}

	// Updates for Microsoft Visual C++ Runtime Versions
	if( sp==2 && (sku & XP_ALL) && ( (_mfc40u_dll>zero && _mfc40u_dll<fver(4,1,0,6141))
					  ||  (_mfc42u_dll>zero && _mfc42u_dll<fver(6,2,8071,0))
					  ||  (_atl_dll_9792     <fver(3,0,9238,0))
					  ||  (_mfc42_dll_9792   <fver(6,0,8665,0))
					  ||  (_mfc42u_dll_9792  <fver(6,0,9792,0))
					  ||  (_msvcp60_dll_9792 <fver(6,0,8972,0)) )) {
		NN("Security Update for Windows XP (KB924667)");
		XX(p2+"windowsxp-kb924667-x86-enu_9016dbabca407c3278219baba256111e131330a3.exe"+a1);
	}
	if( sp>=0 && (sku & XP_ALL) && (
			(vcr2005(false)>zero && vcr2005(false)<fver(8,0,50727,6195)) )) {
		NN("Security Update for Microsoft Visual C++ 2005 Service Pack 1 Redistributable Package (KB2538242)");
		XX(p+"vcredist_x86_b8fab0bb7f62a24ddfe77b19cd9a1451abd7b847.exe"+a2);
	}
	if( sp>=0 && (sku & XP_ALL) && (
			(vcr2008(false)>zero && vcr2008(false)<fver(9,0,30729,6161)) )) {
		NN("Security Update for Microsoft Visual C++ 2008 Service Pack 1 Redistributable Package (KB2538243)");
		XX(p+"vcredist_x86_470640aa4bb7db8e69196b5edb0010933569e98d.exe /qb");
	}
	if( sp==3 && (sku & XP_ALL) && (
			(vcr2010(false)>zero && vcr2010(false)<fver(10,0,30319,415)) )) {
		NN("Security Update for Microsoft Visual C++ 2010 Redistributable Package (KB2467173)");
		XX(p+"vcredist_x86_f5420aa2ac335ce211623a1aa8e16fcf547cfe9a.exe"+a3);
	}
	else if( sp==3 && (sku & XP_ALL) && (
			(vcr2010(false)>fver(10,0,30319,415) && vcr2010(false)<fver(10,0,40219,325)) )) {
		NN("Security Update for Microsoft Visual C++ 2010 Service Pack 1 Redistributable Package (KB2565063)");
		XX(p+"vcredist_x86_28c54491be70c38c97849c3d8cfbfdd0d3c515cb.exe"+a3);
	}
	if( sp==3 && (sku & XP_ALL) && (
			(vcr2012(false)>zero && vcr2012(false)<fver(11,0,61030,0)) )) {
		NN("Update for Microsoft Visual C++ 2012 Update 4 Redistributable Package (KB3119142)");
		XX(p+"vcredist_x86_96b377a27ac5445328cbaae210fc4f0aaa750d3f.exe"+a3);
	}
	if( sp==3 && (sku & XP_ALL) && (
			(vcr2013(false)>zero && vcr2013(false)<fver(12,0,40664,0)) )) {
		NN("Update for Microsoft Visual C++ 2013 Update 5 Redistributable Package (KB4032938)");
		XX(p+"vcredist_x86_0f5d66bcaf120f2d3f340e448a268fe4bbf7709d.exe"+a3);
	}
	if( sp==3 && (sku & XP_ALL) && (
			(vcr2015(false)>zero && vcr2015(false)<fver(14,0,24215,1)) )) {
		NN("Update for Microsoft Visual C++ 2015 Update 3 Redistributable Package");
		XX(p+"vcredist_x86_72211bd2e7dfc91ea7c8fac549c49c0543ba791b.exe"+a3);
	}

	// Updates for Windows Embedded Standard and POSReady 2009
	if( sp==3 && (sku & XPE_2009) && _shlwapi_dll>zero && _shlwapi_dll<fver(6,0,2900,6540)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB2926765)");
		XX(p3+"windowsxp-kb2926765-x86-embedded-enu_fd1b4b0791354fab04ae68907436cab0c8676665.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_23386<fver(5,2,6002,23386)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB2957503)");
		XX(p3+"windowsxp-kb2957503-x86-embedded-enu_34e002ae983c5a61dced1e5cb41d81e714a6cfbd.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,5781))
					  ||  (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,5753)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB2993651)");
		XX(p3+"windowsxp-kb2993651-x86-embedded-enu_60107e4eed4850513f41778ad3a9caef35d893b8.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_mqac_sys>zero && _mqac_sys<fver(5,1,0,1111))
					  ||  (_mqqm_dll>zero && _mqqm_dll<fver(5,1,0,1111)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB2993254)");
		XX(p3+"windowsxp-kb2993254-x86-embedded-enu_a56e76a2cc8404722e44a938849f114431788829.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _fastfat_sys>zero && _fastfat_sys<fver(5,1,2600,6630)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB2998579)");
		XX(p3+"windowsxp-kb2998579-x86-embedded-enu_166f21272c3e54fa2430ca1231b65d870ac1a1bc.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_23535<fver(5,2,6002,23535)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3013126)");
		XX(p3+"windowsxp-kb3013126-x86-embedded-enu_a7485236ec7df12c907ac232c4d8778df1936f43.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _tlntsess_exe>zero && _tlntsess_exe<fver(5,1,2600,6705)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3020393)");
		XX(p3+"windowsxp-kb3020393-v2-x86-embedded-enu_8eed034b5b679808bfb76c522cede9ab48e681e5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _userenv_dll>zero && _userenv_dll<fver(5,1,2600,6689)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3021674)");
		XX(p3+"windowsxp-kb3021674-x86-embedded-enu_3fc15ebe581b61c1a30ab790f97d2fb2d7d41926.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _scesrv_dll>zero && _scesrv_dll<fver(5,1,2600,6690)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3004361)");
		XX(p3+"windowsxp-kb3004361-x86-embedded-enu_47e4ec000eca37d76a17a45d1f17ac181849b45b.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_23588<fver(5,2,6002,23588)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3029944)");
		XX(p3+"windowsxp-kb3029944-x86-embedded-enu_ecdc1784751745ae36f14c6a964f1d9701baefad.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msctf_dll>zero && _msctf_dll<fver(5,1,2600,6727)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3033889)");
		XX(p3+"windowsxp-kb3033889-x86-embedded-enu_97084ab0bd0fe1c7097c6d176d61d85cc28a3609.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_23609<fver(5,2,6002,23609)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3035132)"); 
		XX(p3+"windowsxp-kb3035132-x86-embedded-enu_8e7607ac53706be4b440013f5c82725c2a94a6d2.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_atmfd_dll>zero && _atmfd_dll<fver(5,1,2,245))
					  ||  ( _gdiplus_dll_23746<fver(5,2,6002,23746)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3078601)");
		XX(p3+"windowsxp-kb3078601-x86-embedded-enu_9f428ed1e710bced0702b4097846d36429a93202.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_atmfd_dll>zero && _atmfd_dll<fver(5,1,2,246))
					  ||  (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,6908)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3087039)");
		XX(p3+"windowsxp-kb3087039-x86-embedded-enu_06ac33315eaa9c1d4728cb19651e72cad6186a97.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_tcpip6_sys>zero && _tcpip6_sys<fver(5,1,2600,6935))
					  ||  (_tcpip_sys>zero && _tcpip_sys<fver(5,1,2600,6935)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3092601)");
		XX(p3+"windowsxp-kb3092601-x86-embedded-enu_8ddf39236405ff94407350f62daf3ab6e0fbca65.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_dssenh_dll>zero && _dssenh_dll<fver(5,1,2600,6924))
		              ||  (_rsaenh_dll>zero && _rsaenh_dll<fver(5,1,2600,6924))
					  ||  (_schannel_dll>zero && _schannel_dll<fver(5,1,2600,6926)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3081320)");
		XX(p3+"windowsxp-kb3081320-x86-embedded-enu_d8e991e08445605d85c48425684c7850d1d63a36.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_devenum_dll>zero && _devenum_dll<fver(6,5,2600,6971))
					  ||  (_qedit_dll>zero && _qedit_dll<fver(6,5,2600,6971)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3109560)");
		XX(p3+"windowsxp-kb3109560-x86-embedded-enu_ec8a3e049d5b27bbdbb091cd3ea2ffc6c725ffc5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _advapi32_dll>zero && _advapi32_dll<fver(5,1,2600,6987)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3121918)");
		XX(p3+"windowsxp-kb3121918-x86-embedded-enu_0415f43e728e56f42bdc3171ec7d0f59221cafc7.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_msdaora_dll>zero && _msdaora_dll<fver(2,81,3014,0))
		              ||  (_msorcl32_dll>zero && _msorcl32_dll<fver(2,575,3014,0))
					  ||  (_mtxoci_dll>zero && _mtxoci_dll<fver(2001,12,4414,707)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3126587)");
		XX(p3+"windowsxp-kb3126587-x86-embedded-enu_b638c3dc6cc42839e8f73e46306791ebc7fc6709.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,7013))
		              ||  (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,7013))
					  ||  (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,7013)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3126593)");
		XX(p3+"windowsxp-kb3126593-x86-embedded-enu_28707de25730ebbb4d0e5f02d156dff431bc6e6d.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _seclogon_dll>zero && _seclogon_dll<fver(5,1,2600,7020)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3139914)");
		XX(p3+"windowsxp-kb3139914-x86-embedded-enu_44ee887d5f8da7d99a521b41e3c5ba9e4954de62.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _usbstor_sys>zero && _usbstor_sys<fver(5,1,2600,7012)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3139398)");
		XX(p3+"windowsxp-kb3139398-x86-embedded-enu_1a601a891b38acd2b4e92ed4695646b74b66e1d9.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_wabfind_dll>zero && _wabfind_dll<fver(6,0,2900,7019))
		              ||  (_wabimp_dll>zero && _wabimp_dll<fver(6,0,2900,7019))
					  ||  (_wabmig_exe>zero && _wabmig_exe<fver(6,0,2900,7019)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3140709)");
		XX(p3+"windowsxp-kb3140709-x86-embedded-enu_26891d26d98b012dd5b42112b7e692a5cae56cdf.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,7051))
					  ||  ( _gdiplus_dll_23930<fver(5,2,6002,23930)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3145739)");
		XX(p3+"windowsxp-kb3145739-x86-embedded-enu_e958811dbe4188202eda69cdc9e797a4ae1dc923.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_lsasrv_dll>zero && _lsasrv_dll<fver(5,1,2600,7049))
					  ||  (_samsrv_dll>zero && _samsrv_dll<fver(5,1,2600,7049)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3149090)");
		XX(p3+"windowsxp-kb3149090-x86-embedded-enu_ee6360c61fb2bc49278a0fcbcfb9c299af436857.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_kerberos_dll>zero && _kerberos_dll<fver(5,1,2600,7075))
		              ||  (_ksecdd_sys>zero && _ksecdd_sys<fver(5,1,2600,7075))
					  ||  (_msv1_0_dll>zero && _msv1_0_dll<fver(5,1,2600,7075))
					  ||  (_srv_sys>zero && _srv_sys<fver(5,1,2600,7075)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3161561)");
		XX(p3+"windowsxp-kb3161561-x86-embedded-enu_cf0ce9294706376c9c16f927ec915f5892f31213.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_mswsock_dll>zero && _mswsock_dll<fver(5,1,2600,7074))
		              ||  (_netbt_sys>zero && _netbt_sys<fver(5,1,2600,7074))
					  ||  (_winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,7074))
					  ||  (_ws2_32_dll>zero && _ws2_32_dll<fver(5,1,2600,7074)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3161949)");
		XX(p3+"windowsxp-kb3161949-x86-embedded-enu_0973436e841ac9a6697c9439f97c7d6ab66484f6.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_inetpp_dll>zero && _inetpp_dll<fver(5,1,2600,7094))
		              ||  (_localspl_dll>zero && _localspl_dll<fver(5,1,2600,7094))
					  ||  (_printui_dll>zero && _printui_dll<fver(5,1,2600,7094)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3170455)");
		XX(p3+"windowsxp-kb3170455-x86-embedded-enu_c49f5af4f42870c20dc1f5bc1c848405dc8ad197.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_23984<fver(5,2,6002,23984)) {
		NN("Security Update for WES09 and POSReady 2009 (KB3178034)");
		XX(p3+"windowsxp-kb3178034-x86-embedded-enu_cb20691577ade771359e8a680e75d8f0872ff5f4.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_user32_dll>zero && _user32_dll<fver(5,1,2600,7121))
					  ||  (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,7121)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3185911)");
		XX(p3+"windowsxp-kb3185911-x86-embedded-enu_2ca7c76b25bdb5024f89b4754048d0b5f884c12e.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_mrxdav_sys>zero && _mrxdav_sys<fver(5,1,2600,7128))
					  ||  (_mup_sys>zero && _mup_sys<fver(5,1,2600,7128)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3183431)");
		XX(p3+"windowsxp-kb3183431-x86-embedded-enu_e1867d355002ce685fbffb06f364d2f003b9afcd.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_asycfilt_dll>zero && _asycfilt_dll<fver(5,1,2600,7149))
		              ||  (_oleaut32_dll>zero && _oleaut32_dll<fver(5,1,2600,7149))
					  ||  (_olepro32_dll>zero && _olepro32_dll<fver(5,1,2600,7149)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3196718)");
		XX(p3+"windowsxp-kb3196718-x86-embedded-enu_6b1d637604907623f8989b3d98c9e531b122c6cd.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_lsasrv_dll>zero && _lsasrv_dll<fver(5,1,2600,7151))
					  ||  (_msv1_0_dll>zero && _msv1_0_dll<fver(5,1,2600,7151)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB3198510)");
		XX(p3+"windowsxp-kb3198510-x86-embedded-enu_237a0e55e051d8e10cd5510b89e25bf2da64d508.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _icm32_dll>zero && _icm32_dll<fver(5,1,2600,7197)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4012584)");
		XX(p3+"windowsxp-kb4012584-x86-embedded-enu_04053061cb8d03ee685554dd351594fc6f0a12ab.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_qedit_dll>zero && _qedit_dll<fver(6,5,2600,7218))
					  ||  (_quartz_dll>zero && _quartz_dll<fver(6,5,2600,7218)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB4014652)");
		XX(p3+"windowsxp-kb4014652-x86-embedded-enu_c2702e4f1e63d06a320c139a853fd3a578d5e228.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _asycfilt_dll>zero && _asycfilt_dll<fver(5,1,2600,7225)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4014794)");
		XX(p3+"windowsxp-kb4014794-x86-embedded-enu_ffefa30f0b989917a75cd5474576c6155d17d420.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_24072<fver(5,2,6002,24072)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4015383)");
		XX(p3+"windowsxp-kb4015383-x86-embedded-enu_f2891bd693ccc72eac9aba102aed25687df5a85c.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _kerberos_dll>zero && _kerberos_dll<fver(5,1,2600,7292)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4022746)");
		XX(p3+"windowsxp-kb4022746-x86-embedded-enu_c8cee9e23429d15d9a21529b7eaedde935b36699.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _wldap32_dll>zero && _wldap32_dll<fver(5,1,2600,7313)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4025409)");
		XX(p3+"windowsxp-kb4025409-x86-embedded-enu_3afe33a68dfc323b223c6573670bbafddcdf82f4.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _browseui_dll>zero && _browseui_dll<fver(6,0,2900,7288)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4025497)");
		XX(p3+"windowsxp-kb4025497-x86-embedded-enu_4a37e35189bb8bbdc561962c3ea6e500638fccf5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _wordpad_exe>zero && _wordpad_exe<fver(5,1,2600,7297)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4026061)");
		XX(p3+"windowsxp-kb4026061-x86-embedded-enu_c9e64b9686dae23b9e299fd13669019f174e26b8.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_msado15_dll>zero && _msado15_dll<fver(2,81,3015,0))
					  ||  (_msado28_tlb>zero && _msado28_tlb<fver(2,81,3015,0)) )) {
		NN("2018-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4034044)");
		XX(p3+"windowsxp-kb4034044-x86-embedded-enu_2eed2f8234cf7d6326ece957d4309979faf9d84a.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_msexcl40_dll>zero && _msexcl40_dll<fver(4,0,9801,0))
		              ||  (_msjet40_dll>zero && _msjet40_dll<fver(4,0,9801,0))
					  ||  (_msjetol1_dll>zero && _msjetol1_dll<fver(4,0,9801,0))
					  ||  (_msjint40_dll>zero && _msjint40_dll<fver(4,0,9801,0))
					  ||  (_msjter40_dll>zero && _msjter40_dll<fver(4,0,9801,0))
					  ||  (_msjtes40_dll>zero && _msjtes40_dll<fver(4,0,9801,0))
					  ||  (_msltus40_dll>zero && _msltus40_dll<fver(4,0,9801,0))
					  ||  (_mspbde40_dll>zero && _mspbde40_dll<fver(4,0,9801,0))
					  ||  (_msrd2x40_dll>zero && _msrd2x40_dll<fver(4,0,9801,0))
					  ||  (_msrd3x40_dll>zero && _msrd3x40_dll<fver(4,0,9801,0))
					  ||  (_msrepl40_dll>zero && _msrepl40_dll<fver(4,0,9801,0))
					  ||  (_mstext40_dll>zero && _mstext40_dll<fver(4,0,9801,0))
					  ||  (_mswdat10_dll>zero && _mswdat10_dll<fver(4,0,9801,0))
					  ||  (_mswstr10_dll>zero && _mswstr10_dll<fver(4,0,9801,0))
					  ||  (_msxbde40_dll>zero && _msxbde40_dll<fver(4,0,9801,0)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB4034775)");
		XX(p3+"windowsxp-kb4034775-x86-embedded-enu_0c5c2f6822400aaf9fb2daf450e6373be2255e75.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_usp10_dll>zero && _usp10_dll<fver(1,420,2600,7334))
		              ||  (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,7334))
					  ||  (_gdiplus_dll_24180<fver(5,2,6002,24180)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB4039384)");
		XX(p3+"windowsxp-kb4039384-x86-embedded-enu_6aabfe977264b12c8331500d97053134de7d0738.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,7348))
					  ||    (_srv_sys   >zero && _srv_sys   <fver(5,1,2600,7348)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB4041995)");
		XX(p3+"windowsxp-kb4041995-x86-embedded-enu_acee429ab4e7445545ac73a97f390628e647c1cf.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _query_dll>zero && _query_dll<fver(5,1,2600,7363)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4047211)");
		XX(p3+"windowsxp-kb4047211-x86-embedded-enu_217f80951ee111cf78ea67a854c2919ddc94dceb.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _iprtrmgr_dll>zero && _iprtrmgr_dll<fver(5,1,2600,7377)) {
		NN("2017-12 Security Update for Windows XP Embedded SP3 for x86-based Systems (KB4052303)");
		XX(p3+"windowsxp-kb4052303-x86-embedded-enu_04ab266aa0752613a9b6c68ff33d40699a0045e5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (/*_credssp_dll>zero &&*/ _credssp_dll<fver(5,1,2600,7471))
					  ||  (/*_tspkg_dll>zero &&*/ _tspkg_dll<fver(5,1,2600,7471)) )) {
		NN("2018-05 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4056564)");
		XX(p3+"windowsxp-kb4056564-v2-x86-embedded-enu_964310efb26058c35b0233f008f8b5f6ffd102af.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _atmfd_dll>zero && _atmfd_dll<fver(5,1,2,253)) {
		NN("2018-01 Security Update for Windows XP Embedded SP3 for x86-based Systems (KB4056941)");
		XX(p3+"windowsxp-kb4056941-x86-embedded-enu_24ebcc41e3f03048c25cf02d8d8ab6107479aabd.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msvidctl_dll>zero && _msvidctl_dll<fver(6,5,2600,7431)) {
		NN("2018-03 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4089082)");
		XX(p3+"windowsxp-kb4089082-x86-embedded-enu_56a4c587452f40f8e91f3192168f4cf1193e801c.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _zipfldr_dll>zero && _zipfldr_dll<fver(6,0,2900,7443)) {
		NN("2018-03 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4089175)");
		XX(p3+"windowsxp-kb4089175-x86-embedded-enu_1caf7f6ec0b27d38070eb0385f1548776bc65e6d.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_helpctr_exe>zero && _helpctr_exe<fver(5,1,2600,7431))
		              ||  (_helphost_exe>zero && _helphost_exe<fver(5,1,2600,7431))
					  ||  (_helpsvc_exe>zero && _helpsvc_exe<fver(5,1,2600,7431))
					  ||  (_pchsvc_dll>zero && _pchsvc_dll<fver(5,1,2600,7431))
					  ||  (_safrslv_dll>zero && _safrslv_dll<fver(5,1,2600,7431))
					  ||  (_srdiag_exe>zero && _srdiag_exe<fver(5,1,2600,7431))
					  ||  (_uploadm_exe>zero && _uploadm_exe<fver(5,1,2600,7431)) )) {
		NN("2018-05 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4089453)");
		XX(p3+"windowsxp-kb4089453-v2-x86-embedded-enu_23b1aec8d9e41242c20799e66045dc75c089df61.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _wzcsvc_dll>zero && _wzcsvc_dll<fver(5,1,2600,7432)) {
		NN("2018-03 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4089694)");
		XX(p3+"windowsxp-kb4089694-x86-embedded-enu_f0698bdbb2c0dbaf56d7c4071061046865ec3526.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _wsnmp32_dll>zero && _wsnmp32_dll<fver(5,1,2600,7450)) {
		NN("2018-04 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4091756)");
		XX(p3+"windowsxp-kb4091756-x86-embedded-enu_4084e5a3bf76e0a22d8d33b0c5e8ff3b57e3faf3.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _comsvcs_dll>zero && _comsvcs_dll<fver(2001,12,4414,7477)) {
		NN("2018-05 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4101477)");
		XX(p3+"windowsxp-kb4101477-x86-embedded-enu_867ee71e20e05bcdcab83b86466ab20771f74ac5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _itss_dll>zero && _itss_dll<fver(5,2,3790,6349)) {
		NN("2018-05 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4130957)");
		XX(p3+"windowsxp-kb4130957-x86-embedded-enu_dab532721ce76725b35909e6d95c9d430edc1e13.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,7494))
		              ||  (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,7494))
					  ||  (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,7494))
					  ||  (_oleaut32_dll>zero && _oleaut32_dll<fver(5,1,2600,7494))
					  ||  (_rpcss_dll>zero && _rpcss_dll<fver(5,1,2600,7494)) )) {
		NN("2018-07 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4134651)");
		XX(p3+"windowsxp-kb4134651-v2-x86-embedded-enu_df9272e617574b8645fdca35b58627930c394999.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_afd_sys>zero && _afd_sys<fver(5,1,2600,7510))
		              ||  (_dnsapi_dll>zero && _dnsapi_dll<fver(5,1,2600,7510))
					  ||  (_dnsrslvr_dll>zero && _dnsrslvr_dll<fver(5,1,2600,7510))
					  ||  (_mswsock_dll>zero && _mswsock_dll<fver(5,1,2600,7510))
					  ||  (_tcpip6_sys>zero && _tcpip6_sys<fver(5,1,2600,7510))
					  ||  (_tcpip_sys>zero && _tcpip_sys<fver(5,1,2600,7510)) )) {
		NN("2018-07 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4291391)");
		XX(p3+"windowsxp-kb4291391-x86-embedded-enu_a48bc4ba244b5a49d0191900c5f65f60e1568579.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _hidparse_sys>zero && _hidparse_sys<fver(5,1,2600,7494)) {
		NN("2018-06 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4294413)");
		XX(p3+"windowsxp-kb4294413-x86-embedded-enu_b211e66c425fcef8980fd16953483282cb4e6b14.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,7512))
		              ||  (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,7512))
					  ||  (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,7512))
					  ||  (_rpcss_dll>zero && _rpcss_dll<fver(5,1,2600,7512))
					  ||  (_wordpad_exe>zero && _wordpad_exe<fver(5,1,2600,7512)) )) {
		NN("2018-07 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4339291)");
		XX(p3+"windowsxp-kb4339291-x86-embedded-enu_093848f3c816e2814c7cda1baab07f00e0ac5917.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( ( _mf3216_dll>zero && _mf3216_dll<fver(5,1,2600,7535))
					  ||  ( _gdiplus_dll_24439<fver(5,2,6002,24439)) )) {
		NN("2018-08 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4343674)");
		XX(p3+"windowsxp-kb4343674-x86-embedded-enu_ca27aebe8d7a210ffbc67934c144627a9d6f5463.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _shell32_dll>zero && _shell32_dll<fver(6,0,2900,7551)) {
		NN("2018-09 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4457163)");
		XX(p3+"windowsxp-kb4457163-x86-embedded-enu_6079ca40413cb788b625ebc6c4b8b802fa3bca57.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,7553))
		              ||  ( _mf3216_dll>zero && _mf3216_dll<fver(5,1,2600,7553))
					  ||  ( _gdiplus_dll_24465<fver(5,2,6002,24465)) )) {
		NN("2018-09 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4458000)");
		XX(p3+"windowsxp-kb4458000-x86-embedded-enu_cd9e39263c1b198b9d0a22529100434d3bd20e90.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_fontsub_dll>zero && _fontsub_dll<fver(5,1,2600,7563))
					  ||  (_t2embed_dll>zero && _t2embed_dll<fver(5,1,2600,7563)) )) {
		NN("2018-09 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4458001)");
		XX(p3+"windowsxp-kb4458001-x86-embedded-enu_b6ae71b9b58128d4c6a3221208c1e42d0d434f79.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_msxml3_dll>zero && _msxml3_dll<fver(8,100,1063,0))
					  ||  (_msxml6_dll>zero && _msxml6_dll<fver(6,20,2506,0)) )) {
		NN("2018-09 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4458003)");
		XX(p3+"windowsxp-kb4458003-x86-embedded-enu_a17d87d408201c9d690d726da04d09f717bc661e.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_msexcl40_dll>zero && _msexcl40_dll<fver(4,0,9801,4))
					  ||  (_msjet40_dll>zero && _msjet40_dll<fver(4,0,9801,4)) )) {
		NN("2018-09 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4458006)");
		XX(p3+"windowsxp-kb4458006-x86-embedded-enu_e96b950633bde3636cffe6629b30f424acc90daf.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msxml6_dll>zero && _msxml6_dll<fver(6,20,2507,0)) {
		NN("2018-10 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4458008)");
		XX(p3+"windowsxp-kb4458008-x86-embedded-enu_abc9e19593fcbcf30212a77493af05b144da8bb6.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _ks_sys>zero && _ks_sys<fver(5,3,2600,7562)) {
		NN("2018-09 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4458318)");
		XX(p3+"windowsxp-kb4458318-x86-embedded-enu_d2751e7d4cd301c51c010f1d193d909e64ba8466.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_dssenh_dll>zero && _dssenh_dll<fver(5,1,2600,7345))
		              ||  (_rsaenh_dll>zero && _rsaenh_dll<fver(5,1,2600,7345))
					  ||  (_schannel_dll>zero && _schannel_dll<fver(5,1,2600,7567)) )) {
		NN("Update for WES09 and POSReady 2009 (KB4459091)");
		XX(p3+"windowsxp-kb4459091-x86-embedded-enu_6f260244fafd69643591ded6624997eb2211b627.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _mrxsmb_sys>zero && _mrxsmb_sys<fver(5,1,2600,7570)) {
		NN("2018-10 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4462935)");
		XX(p3+"windowsxp-kb4462935-x86-embedded-enu_b5de399aa37284c81ca1e26ffb93893db035ec17.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_24489<fver(5,2,6002,24489)) {
		NN("2018-10 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4462987)");
		XX(p3+"windowsxp-kb4462987-x86-embedded-enu_2ca3c3b7b4e2f9bee24af4e46eeb7e4624aabd3d.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_ntfs_sys>zero && _ntfs_sys<fver(5,1,2600,7581))
		              ||  (_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,7581))
					  ||  (_ntkrnlpa_exe>zero && _ntkrnlpa_exe<fver(5,1,2600,7581))
					  ||  (_ntkrpamp_exe>zero && _ntkrpamp_exe<fver(5,1,2600,7581))
					  ||  (_ntoskrnl_exe>zero && _ntoskrnl_exe<fver(5,1,2600,7581)) )) {
		NN("2018-10 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4463103)");
		XX(p3+"windowsxp-kb4463103-x86-embedded-enu_1d7cc48a7f1902bdd5953a1ab980db5832631339.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msrd3x40_dll>zero && _msrd3x40_dll<fver(4,0,9801,5)) {
		NN("2018-10 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4463573)");
		XX(p3+"windowsxp-kb4463573-x86-embedded-enu_b0e76ad95da29d0ce8ba122a6b703660b93ed49b.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,7594))
		              ||  (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,7594))
					  ||  (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,7594))
					  ||  (_oleaut32_dll>zero && _oleaut32_dll<fver(5,1,2600,7594))
					  ||  (_rpcss_dll>zero && _rpcss_dll<fver(5,1,2600,7594)) )) {
		NN("2018-11 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4466388)");
		XX(p3+"windowsxp-kb4466388-x86-embedded-enu_b177578d5bc6c95cf31dd1848a22ddbe10b9501b.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,7594))
		              ||  (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,7594))
					  ||  (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,7594))
					  ||  (_rpcrt4_dll>zero && _rpcrt4_dll<fver(5,1,2600,7594))
					  ||  (_rpcss_dll>zero && _rpcss_dll<fver(5,1,2600,7594)) )) {
		NN("2018-11 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4467689)");
		XX(p3+"windowsxp-kb4467689-x86-embedded-enu_b974cbc4675474ddb38c41f7926f493cde207eee.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _tzchange_exe<fver(5,1,2600,7607)) {
		NN("Update for WES09 and POSReady 2009 (KB4468323)");
		XX(p3+"windowsxp-kb4468323-x86-embedded-enu_6a46468fe73b3b793934d70768ab2c4ce11a03ab.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _scrrun_dll>zero && _scrrun_dll<fver(5,7,6002,24514)) {
		NN("2018-11 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4470118)");
		XX(p3+"windowsxp-kb4470118-x86-embedded-enu_eb3fe70a186545d1745f48eda37ecf21a1d330d9.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_24533<fver(5,2,6002,24533)) {
		NN("2018-12 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4473077)");
		XX(p3+"windowsxp-kb4473077-x86-embedded-enu_2472bfb0a1921bccc30621166b26004b22203e63.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msrd3x40_dll>zero && _msrd3x40_dll<fver(4,0,9801,7)) {
		NN("2019-01 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4481275)");
		XX(p3+"windowsxp-kb4481275-x86-embedded-enu_5160cf8226e525f266f39e7279ce39e6176cece9.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_ntkrnlmp_exe>zero && _ntkrnlmp_exe<fver(5,1,2600,7649))
		                || !regTestKey(L"SOFTWARE\\Microsoft\\Updates\\Windows XP\\SP4\\KB4486463"))) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4486463)");
		XX(p3+"windowsxp-kb4486463-x86-embedded-enu_8ab6c1cbd442dbdd8667993af4ee093abf09fb41.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _npfs_sys>zero && _npfs_sys<fver(5,1,2600,7661)) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4486468)");
		XX(p3+"windowsxp-kb4486468-x86-embedded-enu_491ec2dcd578becacf2ec08f65b6c6f62ca3822c.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msv1_0_dll>zero && _msv1_0_dll<fver(5,1,2600,7666)) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4486536)");
		XX(p3+"windowsxp-kb4486536-x86-embedded-enu_4dd89d4cfab9ba6526747646e320adf407b6ece4.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _kerberos_dll>zero && _kerberos_dll<fver(5,1,2600,7663)) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4486538)");
		XX(p3+"windowsxp-kb4486538-x86-embedded-enu_b46b867293bb21b12fe5867ff98806a98ae50fa2.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_mf3216_dll>zero && _mf3216_dll<fver(5,1,2600,7663))
					  ||  (_gdiplus_dll_24561<fver(5,2,6002,24561)) )) {
		NN("Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4487085)");
		XX(p3+"windowsxp-kb4487085-v2-x86-embedded-enu_667f051ffe98ff99495e9b6ede2b8c321aba1ca3.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _hidparse_sys>zero && _hidparse_sys<fver(5,1,2600,7640)) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4487086)");
		XX(p3+"windowsxp-kb4487086-x86-embedded-enu_d87dffd2310c8df09892de204e6216bb447ad0db.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msjet40_dll>zero && _msjet40_dll<fver(4,0,9801,8)) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4487385)");
		XX(p3+"windowsxp-kb4487385-x86-embedded-enu_2556bb7bd7217d6295d877c731c07acd62c9c59c.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _tzchange_exe<fver(5,1,2600,7654)) {
		NN("2019-03 Update for POSReady 2009 for x86-based Systems (KB4487989)");
		XX(p3+"windowsxp-kb4487989-x86-embedded-enu_3c32c4f284fcd7208b3cf4428320e95a27513c3a.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && _tzchange_exe<fver(5,1,2600,7676)) {
		NN("2019-04 Update for POSReady 2009 for x86-based Systems (KB4487990)");
		XX(p3+"windowsxp-kb4487990-x86-embedded-enu_8a1f84ccfc4cdcb9536864fa91ae82ef62cb3e79.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && (_msfs_sys>zero && _msfs_sys<fver(5,1,2600,7657))) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4489493)");
		XX(p3+"windowsxp-kb4489493-x86-embedded-enu_bf5786db231541e562a8f4dececac9b8fdc3cdce.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_msado15_dll>zero && _msado15_dll<fver(2,81,3016,0))
					  ||  (_msado28_tlb>zero && _msado28_tlb<fver(2,81,3016,0)) )) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4489973)");
		XX(p3+"windowsxp-kb4489973-x86-embedded-enu_1c50f081b27c440d89300b59cd5df9ed689390b0.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_msxml3_dll>zero && _msxml3_dll<fver(8,100,1064,0))
					  ||  (_msxml6_dll>zero && _msxml6_dll<fver(6,20,2508,0)) )) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4489974)");
		XX(p3+"windowsxp-kb4489974-x86-embedded-enu_fb9d178e51321bf47246a752c2d79dcfe6bada40.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && (_msrd2x40_dll>zero && _msrd2x40_dll<fver(4,0,9801,12))) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4489977)");
		XX(p3+"windowsxp-kb4489977-x86-embedded-enu_e1b835d2c684f2953946a414fddc7fe40ef6eec2.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _comctl32_dll_7660<fver(6,0,2900,7660)) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4490228)");
		XX(p3+"windowsxp-kb4490228-x86-embedded-enu_a81c03f3f775390169d890d6f99cd9d3e58cc62e.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( ( _mf3216_dll>zero && _mf3216_dll<fver(5,1,2600,7666))
					  ||  ( _gdiplus_dll_24563<fver(5,2,6002,24563)) )) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4490385)");
		XX(p3+"windowsxp-kb4490385-x86-embedded-enu_47b193ff117da1c86efc4d275b52a829c5cb00b9.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_localspl_dll>zero && _localspl_dll<fver(5,1,2600,7661))
		              ||  (_win32spl_dll>zero && _win32spl_dll<fver(5,1,2600,7661))
					  ||  (_winspool_drv>zero && _winspool_drv<fver(5,1,2600,7661)) )) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4490501)");
		XX(p3+"windowsxp-kb4490501-x86-embedded-enu_c0b31474df5c52c3f1794914050fdf27b0692c65.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && (_netbt_sys>zero && _netbt_sys<fver(5,1,2600,7675))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4491443)");
		XX(p3+"windowsxp-kb4491443-x86-embedded-enu_38dcb7ca4adee2236e202651b1a9feff15123fc5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && (_hidparse_sys>zero && _hidparse_sys<fver(5,1,2600,7667))) {
		NN("2019-03 Security Update for POSReady 2009 for x86-based Systems (KB4493341)");
		XX(p3+"windowsxp-kb4493341-x86-embedded-enu_cfd32e1875c216557e94e1763fbc114c216d6b0c.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && ((_kernel32_dll>zero && _kernel32_dll<fver(5,1,2600,7682))
		              ||  (_ntdll_dll>zero && _ntdll_dll<fver(5,1,2600,7682))
					  ||  (_ole32_dll>zero && _ole32_dll<fver(5,1,2600,7682))
					  ||  (_oleaut32_dll>zero && _oleaut32_dll<fver(5,1,2600,7682))
					  ||  (_rpcss_dll>zero && _rpcss_dll<fver(5,1,2600,7682)) )) {
		// The version of ntdll.dll installed by KB4493563 has an alleged bug in non-English languages
		// that causes error messages to display improperly?
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493563)");
		XX(p3+"windowsxp-kb4493563-x86-embedded-enu_b7acbe01322ca903c920fa8a85328057ba2d9703.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && ( (_msxml3_dll>zero && _msxml3_dll<fver(8,100,1065,0))
					  ||  (_msxml6_dll>zero && _msxml6_dll<fver(6,20,2509,0)) )) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493790)");
		XX(p3+"windowsxp-kb4493790-x86-embedded-enu_e078da675e389cbfaba2354eae8040e438a42476.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_msxbde40_dll>zero && _msxbde40_dll<fver(4,0,9801,14))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493793)");
		XX(p3+"windowsxp-kb4493793-x86-embedded-enu_c238b0eab410f55d9b8fdc7485eba2ba0459559a.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_mspbde40_dll>zero && _mspbde40_dll<fver(4,0,9801,14))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493794)");
		XX(p3+"windowsxp-kb4493794-x86-embedded-enu_89c4913943e563851bd4c458f222aee21edb7bb3.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_msexcl40_dll>zero && _msexcl40_dll<fver(4,0,9801,14))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493795)");
		XX(p3+"windowsxp-kb4493795-x86-embedded-enu_9a084800b33b40776da30e70293cccfe4b0342f5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_msjet40_dll>zero && _msjet40_dll<fver(4,0,9801,14))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493796)");
		XX(p3+"windowsxp-kb4493796-x86-embedded-enu_92495fc42b0bf0fd51baa584af55f02b7d8deaa5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_msrd3x40_dll>zero && _msrd3x40_dll<fver(4,0,9801,14))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493797)");
		XX(p3+"windowsxp-kb4493797-x86-embedded-enu_40d780178b80a6bd6b4398ca1ca3e47973997b0b.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_oleprn_dll>zero && _oleprn_dll<fver(5,1,2600,7680))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4494059)");
		XX(p3+"windowsxp-kb4494059-x86-embedded-enu_58109e2522cd4e5b41840ce5d24931d52e421f72.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && ( (_mf3216_dll>zero && _mf3216_dll<fver(5,1,2600,7683))
					  ||  (_gdiplus_dll_20488<fver(5,2,6003,20488)) )) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4495022)");
		XX(p3+"windowsxp-kb4495022-x86-embedded-enu_6fce82c569aa5a6af6ff63769b2e901032d401ee.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _gdiplus_dll_23693<fver(5,2,6002,23693)) {
		NN("Update for WEPOS and POSReady 2009 (KB3065979)");
		XX(p3+"windowsxp-kb3065979-x86-embedded-enu_fe0a4fbb1fcb7c6062ae3ff88dcbf388f1ec4b07.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ((_dssenh_dll>zero && _dssenh_dll<fver(5,1,2600,7345))
		              ||  (_ksecdd_sys>zero && _ksecdd_sys<fver(5,1,2600,7346))
					  ||  (_lsasrv_dll>zero && _lsasrv_dll<fver(5,1,2600,7346))
					  ||  (_rsaenh_dll>zero && _rsaenh_dll<fver(5,1,2600,7345))
					  ||  (_schannel_dll>zero && _schannel_dll<fver(5,1,2600,7346))
					  ||  (_secur32_dll>zero && _secur32_dll<fver(5,1,2600,7346)) )) {	
		NN("Update for WES09 and POSReady 2009 (KB4019276)");
		XX(p3+"windowsxp-kb4019276-x86-embedded-enu_3822fc1692076429a7dc051b00213d5e1240ce3d.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _winhttp_dll>zero && _winhttp_dll<fver(5,1,2600,7587)) {
		NN("Update for WES09 and POSReady 2009 (KB4467770)");
		XX(p3+"windowsxp-kb4467770-x86-embedded-enu_f61e16be7e32887953b29ae1b8ba90064f3ef0a5.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && _msrd2x40_dll>zero && _msrd2x40_dll<fver(4,0,9801,8)) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4487396)");
		XX(p3+"windowsxp-kb4487396-x86-embedded-enu_ab24ff1d4e7592d3642accd2557bf48c1ee4e4e2.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _httpext_dll>zero && _httpext_dll<fver(6,0,2600,7150)) {
		NN("Security Update for Windows XP Service Pack 3 (KB3197835)");
		XX(p3+"windowsxp-kb3197835-x86-custom-enu_a65f07e2c62156b47d3940873c516b05ad775086.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && _httpext_dll>zero && _httpext_dll<fver(6,0,2600,7150)) {
		NN("Security Update for WES09 and POSReady 2009 (KB3197835)");
		XX(p3+"windowsxp-kb3197835-x86-embedded-enu_96f4567c9e6d92956a85f4c8cbddb66cb6d8c8b3.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _httpext_dll>zero && _httpext_dll<fver(6,0,2600,7150)) {
		NN("Security Update for Windows XP SP3 for XPe and WEPOS (KB3197835)");
		XX(p3+"windowsxp-kb3197835-x86-embedded-custom-enu_f5838b8f79ab9cc882f5e729c7ec6a5452f517d6.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && ((_gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,7209))
		              ||  (_usp10_dll>zero && _usp10_dll<fver(1,420,2600,7209))
					  ||  (_gdiplus_dll_24064<fver(5,2,6002,24064)) )) {
		NN("Security Update for Windows XP Service Pack 3 (KB4012583)");
		XX(p3+"windowsxp-kb4012583-x86-custom-enu_36fa3aa9ed1507506cc79e6bb2e4d0fd774dd941.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && ((_gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,7209))
		              ||  (_usp10_dll>zero && _usp10_dll<fver(1,420,2600,7209))
					  ||  (_gdiplus_dll_24064<fver(5,2,6002,24064)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB4012583)");
		XX(p3+"windowsxp-kb4012583-x86-embedded-enu_cd150712980ded7ca5765a8fdd9a9dda860877b8.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && ((_gdi32_dll>zero && _gdi32_dll<fver(5,1,2600,7209))
		              ||  (_usp10_dll>zero && _usp10_dll<fver(1,420,2600,7209))
					  ||  (_gdiplus_dll_24064<fver(5,2,6002,24064)) )) {
		NN("Security Update for Windows XP SP3 for XPe (KB4012583)");
		XX(p3+"windowsxp-kb4012583-x86-embedded-custom-enu_b4ed0f4e2ec4ce15b7c699aeb5fd9547a64c1b4c.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _srv_sys>zero && _srv_sys<fver(5,1,2600,7208)) {
		NN("Security Update for Windows XP SP3 (KB4012598)");
		XX(p3+"windowsxp-kb4012598-x86-custom-enu_eceb7d5023bbb23c0dc633e46b9c2f14fa6ee9dd.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && ( (_srv_sys>zero && _srv_sys<fver(5,1,2600,7651))
					  /*||  ( _xpsp4res_dll<fver(5,1,2600,7651))*/ )) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4486465)");
		XX(p3+"windowsxp-kb4486465-x86-embedded-enu_80c4f32abd257968871a840fd4d87cfedd0b3d4b.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _srv_sys>zero && _srv_sys<fver(5,1,2600,7210)) {
		NN("Security Update for Windows XP SP3 for XPe (KB4012598)");
		XX(p3+"windowsxp-kb4012598-x86-embedded-custom-enu_8f2c266f83a7e1b100ddb9acd4a6a3ab5ecd4059.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _srv_sys>zero && _srv_sys<fver(5,1,2600,7238)) {
		NN("Security Update for Windows XP SP3 (KB4018466)");
		XX(p3+"windowsxp-kb4018466-x86-custom-enu_97472f796e84cd9869c02c32178886115a77daf6.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && _srv_sys>zero && _srv_sys<fver(5,1,2600,7238)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4018466)");
		XX(p3+"windowsxp-kb4018466-x86-embedded-enu_9152d114029dd47afbf6d5108012ac513a720cf7.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _srv_sys>zero && _srv_sys<fver(5,1,2600,7239)) {
		NN("Security Update for Windows XP SP3 for XPe (KB4018466)");
		XX(p3+"windowsxp-kb4018466-x86-embedded-custom-enu_52d9228ae013396d3fda20dbd68fe2453c11a6a2.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,7258)) {
		NN("Security Update for Windows XP SP3 (KB4019204)");
		XX(p3+"windowsxp-kb4019204-x86-custom-enu_4498d4a6c2faf1232afa6595b9a0bdb9e39526c5.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_POSREADY2009) && (_win32k_sys>zero && _win32k_sys<fver(5,1,2600,7684))) {
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4493927)");
		XX(p3+"windowsxp-kb4493927-x86-embedded-enu_4123fcdc99a7976cd5791f78536a0585e0264462.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,7649)) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4486924)");
		XX(p3+"windowsxp-kb4486924-x86-embedded-enu_49ea7629701a23dd3dbf3c21e60af54eec3baf19.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _win32k_sys>zero && _win32k_sys<fver(5,1,2600,7240)) {
		NN("Security Update for Windows XP SP3 for XPe (KB4019204) ");
		XX(p3+"windowsxp-kb4019204-x86-embedded-custom-enu_ee9107e4af833ca674946fadef85e0f138ce4201.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _gpkcsp_dll>zero && _gpkcsp_dll<fver(5,1,2600,7264)) {
		NN("Security Update for Windows XP SP3 (KB4022747)");
		XX(p3+"windowsxp-kb4022747-x86-custom-enu_79307511c6d11bf713a33e53683b594c8876e78e.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && _gpkcsp_dll>zero && _gpkcsp_dll<fver(5,1,2600,7263)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4022747)");
		XX(p3+"windowsxp-kb4022747-x86-embedded-enu_9ee3ca3c99988ec3a9cade2073f461b39706c82c.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _gpkcsp_dll>zero && _gpkcsp_dll<fver(5,1,2600,7263)) {
		NN("Security Update for Windows XP SP3 for XPe (KB4022747)");
		XX(p3+"windowsxp-kb4022747-x86-embedded-custom-enu_dbdadefb4204e0b5e63542c0ac09e351fc65063a.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && (
		                  (_iprtrmgr_dll >zero && _iprtrmgr_dll <fver(5,1,2600,7272))
		              ||  (_ndiswan_sys  >zero && _ndiswan_sys  <fver(5,1,2600,7272))
					  ||  (_rasctrs_dll  >zero && _rasctrs_dll  <fver(5,1,2600,7272))
					  ||  (_rasdlg_dll   >zero && _rasdlg_dll   <fver(5,1,2600,7272))
					  ||  (_rasmxs_dll   >zero && _rasmxs_dll   <fver(5,1,2600,7272)) )) {	
		NN("Security Update for Windows XP SP3 (KB4024323)");
		XX(p3+"windowsxp-kb4024323-x86-custom-enu_edb5a850bcbcc640c2b8c8aad3ab4d36a3b6fb26.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && (
		                  (_iprtrmgr_dll >zero && _iprtrmgr_dll <fver(5,1,2600,7271))
		              ||  (_ndiswan_sys  >zero && _ndiswan_sys  <fver(5,1,2600,7271))
					  ||  (_rasctrs_dll  >zero && _rasctrs_dll  <fver(5,1,2600,7271))
					  ||  (_rasdlg_dll   >zero && _rasdlg_dll   <fver(5,1,2600,7271))
					  ||  (_rasmxs_dll   >zero && _rasmxs_dll   <fver(5,1,2600,7271)) )) {
		NN("Security Update for WES09 and POSReady 2009 (KB4024323)");
		XX(p3+"windowsxp-kb4024323-x86-embedded-enu_42d69f9c232fb86657c938a507388bcdf5043483.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && (
		                  (_iprtrmgr_dll >zero && _iprtrmgr_dll <fver(5,1,2600,7286))
		              ||  (_ndiswan_sys  >zero && _ndiswan_sys  <fver(5,1,2600,7286))
					  ||  (_rasctrs_dll  >zero && _rasctrs_dll  <fver(5,1,2600,7286))
					  ||  (_rasdlg_dll   >zero && _rasdlg_dll   <fver(5,1,2600,7286))
					  ||  (_rasmxs_dll   >zero && _rasmxs_dll   <fver(5,1,2600,7286)) )) {
		NN("Security Update for Windows XP SP3 for XPe (KB4024323)");
		XX(p3+"windowsxp-kb4024323-x86-embedded-custom-enu_7cdcf5dbb7f304b765656461c182a2492a5a6596.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _query_dll>zero && _query_dll<fver(5,1,2600,7273)) {
		NN("Security Update for Windows XP SP3 (KB4024402)");
		XX(p3+"windowsxp-kb4024402-x86-custom-enu_c2334a5c8087e107fe26570650966a8f42602b0a.exe"+a1);
	}

	if( sp==3 && (sku & XP_CLIENT) && _olecnv32_dll>zero && _olecnv32_dll<fver(5,1,2600,7285)) {
		NN("Security Update for Windows XP SP3 (KB4025218)");
		XX(p3+"windowsxp-kb4025218-x86-custom-enu_f72f48762b3c7354919c86f10d3139b65ddc8a26.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && _olecnv32_dll>zero && _olecnv32_dll<fver(5,1,2600,7285)) {
		NN("Security Update for WES09 and POSReady 2009 (KB4025218)");
		XX(p3+"windowsxp-kb4025218-x86-embedded-enu_be6f91dc331c23cbb6ce78f41a3c3976fa62f904.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _olecnv32_dll>zero && _olecnv32_dll<fver(5,1,2600,7287)) {
		NN("Security Update for Windows XP SP3 for XPe (KB4025218)");
		XX(p3+"windowsxp-kb4025218-x86-embedded-custom-enu_116dbb7db8f7d080eac7ebcaccbaf04cb9a9adaa.exe"+a1);
	}

	if( sp==3 && (sku & XP_PLUS_FLP) && _termdd_sys>zero && _termdd_sys<fver(5,1,2600,7701)) {
		NN("Security Update for Windows XP SP3 (KB4500331)");
		XX(p3+"windowsxp-kb4500331-x86-custom-enu_d7206aca53552fececf72a3dee93eb2da0421188.exe"+a1);
	}
	else if( sp==3 && (sku & XPE_2009) && _termdd_sys>zero && _termdd_sys<fver(5,1,2600,7701)) {
		NN("2019-05 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4500331)");
		XX(p3+"windowsxp-kb4500331-x86-embedded-enu_8aee1065d848661cbbf5a5b5b21d94803247741e.exe"+a1);
	}
	else if( sp==3 && (sku & XP_EMBEDDED) && _termdd_sys>zero && _termdd_sys<fver(5,1,2600,7701)) {
		NN("Security Update for Windows XP SP3 for XPe (KB4500331)");
		XX(p3+"windowsxp-kb4500331-x86-embedded-custom-enu_fb2ebfe3c0fe9d4eef23c32e53198bf4731d653d.exe"+a1);
	}

	if( sp>=0 &&  (*wga || ( (sku & XP_ALL)
		    && ( _LegitCheckControl_dll>zero && _LegitCheckControl_dll<fver(1,9,40,0))
			&& ( _WgaLogon_dll>zero          && _WgaLogon_dll         <fver(1,9,40,0))
			&& ( _WgaTray_exe>zero           && _WgaTray_exe          <fver(1,9,40,0)) ))) {
		NN("Windows Genuine Advantage Notification (KB905474)");
		XX(p3+"WindowsXP-KB905474-ENU-x86.exe /extract:%TEMP%\\KB905474 /quiet\n"
		    +"pushd %TEMP%\\KB905474\n"
		    //+"wgasetup.exe /autoauto /quiet /norestart\n"
		    +"wganotifypackageinner.exe"+a1+"\n"
		    +"REG ADD \"HKLM\\SOFTWARE\\Microsoft\\Windows Genuine Advantage\" /v InstallID /t REG_SZ /d {30F230D0-7940-499e-8A01-748682288985} /f\n"
		    +"popd\n"
		    +"rd /S /Q %TEMP%\\KB905474");
	}
	else if( /*sp>=1 &&*/ ( _LegitCheckControl_dll<fver(1,7,69,2))) {
		NN("Windows Genuine Advantage Validation Tool (KB892130)");
		XX(p3+"WindowsXP-KB892130-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _mrt_exe != fver(5,39,12900,0)) {
		NN("Windows Malicious Software Removal Tool - August 2016 (KB890830)");
		XX(p+"Windows-KB890830-v5.39.exe"+a2);
	}
	if(sp==3 && (sku & XP_PLUS_FLP) && (*xpeos || (_xp_eos_exe>zero && _xp_eos_exe<fver(5,1,2600,6526)))){
		NN("Windows XP End of Support Notification (KB2934207)");
		XX(p3+"WindowsXP-KB2934207-x86-ENU.exe"+a1);
	}
	if(*rktools) {
		NN("Windows Server 2003 Resource Kit Tools");
		if(_msiexec_exe  <fver(3,0,0,0)) {
			XX("Extras\\rktools.exe /R:N /C:\"msiexec /i rktools.msi /qb\"");
		} else {
			XX("Extras\\rktools.exe /R:N /C:\"msiexec /i rktools.msi /passive\"");
		}
	}

	// Microsoft Java Virtual Machine (MSJVM)
	if( *msjvm ) {
		NN("Microsoft Java Virtual Machine (MSJVM)");
		// MSJAVX86.EXE /c:"javatrig.exe /exe_install /l /q /qq" /q:a /r:n
		XX(sw+p+"msjavx86.exe"+a8);
	}
	if( sp>=0 && (sku & XP_ALL)
	  && ( *msjvm
	   || (_clspack_exe  >zero && _clspack_exe  <fver(5,0,3810,0))
	   || (_javacypt_dll >zero && _javacypt_dll <fver(5,0,3810,0))
	   || (_javaee_dll   >zero && _javaee_dll   <fver(5,0,3810,0))
	   || (_javaprxy_dll >zero && _javaprxy_dll <fver(5,0,3810,0))
	   || (_javart_dll   >zero && _javart_dll   <fver(5,0,3810,0))
	   || (_jdbgmgr_exe  >zero && _jdbgmgr_exe  <fver(5,0,3810,0))
	   || (_jit_dll      >zero && _jit_dll      <fver(5,0,3810,0))
	   || (_jview_exe    >zero && _jview_exe    <fver(5,0,3810,0))
	   || (_msawt_dll    >zero && _msawt_dll    <fver(5,0,3810,0))
	   || (_msjava_dll   >zero && _msjava_dll   <fver(5,0,3810,0))
	   || (_msjdbc10_dll >zero && _msjdbc10_dll <fver(5,0,3810,0))
	   || (_vmhelper_dll >zero && _vmhelper_dll <fver(5,0,3810,0))
	   || (_wjview_exe   >zero && _wjview_exe   <fver(5,0,3810,0)) )) {
		NN("Q816093 Java Virtual Machine Security Update");
		XX(sw+p+"msjavwu.exe"+a8);
	}

	// DirectX Updates
	if( sp==1 && (sku & XP_ALL) && _quartz_dll>=fver(6,4,2600,1106) && _quartz_dll<fver(6,4,2600,1738)) {
		NN("Security Update for Windows XP (KB904706)");
		XX(p1+"windowsxp-kb904706-v2-x86-enu_ec909ee2bab6b15d7d3545a1eaf07bbb066e038b.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _quartz_dll>=fver(6,5,1,900) && _quartz_dll<fver(6,5,1,907)) {
		NN("Security Update for DirectX 9 for Windows XP (KB904706)");
		XX(p1+"windowsxp-kb904706-dx9-x86-enu_2ecaf33bde917cd33eaf2c8e0459a72c4bff0ce9.exe"+a1);
	}
	if((sp==0 && (sku & XP_ALL) 
		&& regQueryValue(L"SOFTWARE\\Microsoft\\DirectX",L"Version",&status)==L"4.08.01.0810"
		&& (
	      (_dplayx_dll   >zero && _dplayx_dll   <fver(5,1,2600,148))
	   || (_dpwsockx_dll >zero && _dpwsockx_dll <fver(5,1,2600,148)) ))
	 ||(sp==1 && (sku & XP_ALL) 
		&& regQueryValue(L"SOFTWARE\\Microsoft\\DirectX",L"Version",&status)==L"4.08.01.0810"
		&& (
	      (_dplayx_dll   >zero && _dplayx_dll   <fver(5,1,2600,1517))
	   || (_dpwsockx_dll >zero && _dpwsockx_dll <fver(5,1,2600,1517)) ))) {
		NN("Security Update for Windows XP (KB839643)");
		XX(p1+"windowsxp-kb839643-x86-enu_57fb5ffbac48b02d0452e07fb843834510077477.exe"+a6);
	}
	if( sp<2 && (sku & XP_ALL) 
		&& ( DirectXVersion >= fver(4,8,2,0) && DirectXVersion <= fver(4,8,2,137) )
		&& (
	      (_dplayx_dll   >zero && _dplayx_dll   <fver(5,2,3677,144))
	   || (_dpwsockx_dll >zero && _dpwsockx_dll <fver(5,2,3677,144)) )) {
		NN("Security Update for DirectX 8.2 (KB839643)");
		XX(p1+"directx82-kb839643-x86-enu_2c6260d2b78db6079e86cd8e8d578b8140cabca1.exe"+a1);
	}
	if( sp<2 && (sku & XP_ALL) 
		&& ( DirectXVersion >= fver(4,9,0,0) && DirectXVersion <= fver(4,9,0,902) )
		&& (
	      (_dplayx_dll   >zero && _dplayx_dll   <fver(5,3,0,903))
	   || (_dpwsockx_dll >zero && _dpwsockx_dll <fver(5,3,0,903)) )) {
		NN("Security Update for DirectX 9.0 (KB839643)");
		XX(p1+"directx90-kb839643-x86-enu_ed1fc697f95aa71dfd7773edb3bebc77030e08df.exe"+a1);
	}

	// Outlook Express Updates
	/*if( sp==0 && (sku & XP_ALL) && (
	      (_inetcomm_dll >zero && _inetcomm_dll  <fver(6,0,2739,300))
	   || (_msoe_dll >zero && _msoe_dll  <fver(6,0,2720,3000)) )) {
		// Microsoft Security Bulletin MS04-013
		// Cumulative Security Update for Outlook Express (837009)
		// Issued: April 13, 2004
		// KB837009 is replaced by KB823353
		NN("Cumulative Security Update for Outlook Express 6 (KB837009)");
		XX(sw+rtm+"q837009_5a0166ddbe242c76aed9c7c4b595f9f.exe"+a8);
	}*/
	if( sp==0 && (sku & XP_ALL) && (
	      (_inetcomm_dll >zero && _inetcomm_dll  <fver(6,0,2742,200))
	   || (_msoe_dll >zero && _msoe_dll  <fver(6,0,2741,2600)) )) {
		// Microsoft Security Bulletin MS04-018
		// Cumulative Security Update for Outlook Express (823353)
		// Issued: July 13, 2004
		NN("Cumulative Security Update for Outlook Express 6 (KB823353)");
		XX(sw+rtm+"ie6.0-kb823353-windowsxp-x86-enu_6769d29ed07c313c9cd3b7d659a9b93e5feb1b8a.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0))
	  && ((_directdb_dll >zero && _directdb_dll <fver(6,0,2800,1123))
	   || (_inetcomm_dll >zero && _inetcomm_dll <fver(6,0,2800,1441))
	   || (_inetres_dll  >zero && _inetres_dll  <fver(6,0,2800,1123))
	   || (_msident_dll  >zero && _msident_dll  <fver(6,0,2800,1123))
	   || (_msimn_exe    >zero && _msimn_exe    <fver(6,0,2800,1123))
	   || (_msoe_dll     >zero && _msoe_dll     <fver(6,0,2800,1437))
	   || (_msoeacct_dll >zero && _msoeacct_dll <fver(6,0,2800,1123))
	   || (_msoeres_dll  >zero && _msoeres_dll  <fver(6,0,2800,1123))
	   || (_msoert2_dll  >zero && _msoert2_dll  <fver(6,0,2800,1123))
	   || (_oeimport_dll >zero && _oeimport_dll <fver(6,0,2800,1123))
	   || (_oemig50_exe  >zero && _oemig50_exe  <fver(6,0,2800,1123))
	   || (_oemiglib_dll >zero && _oemiglib_dll <fver(6,0,2800,1123))
//	   || (_oeuninst_exe >zero && _oeuninst_exe <fver(6,0,2800,1223))
//	   || (_oeupdate_exe >zero && _oeupdate_exe <fver(6,0,2800,1276))
	   || (_wab_exe      >zero && _wab_exe      <fver(6,0,2800,1123))
	   || (_wab32_dll    >zero && _wab32_dll    <fver(6,0,2800,1450))
	   || (_wabfind_dll  >zero && _wabfind_dll  <fver(6,0,2800,1123))
	   || (_wabimp_dll   >zero && _wabimp_dll   <fver(6,0,2800,1123))
	   || (_wabmig_exe   >zero && _wabmig_exe   <fver(6,0,2800,1123)) )) {
		NN("Cumulative Security Update for Outlook Express 6 SP1 (KB823353)");
		XX(sw+rtm+"ie6.0sp1-kb823353-x86-enu_fc5e431c90275cbf25628c46712a6aa7fe86acd9.exe"+a8);
	}
	if( sp==1 && (sku & XP_ALL) && (
	      (_directdb_dll   >zero && _directdb_dll   <fver(6,0,2800,1807))
//	   || (_iecustom_dll  >zero && _iecustom_dll  <fver(6,0,2800,1502))
	   || (_inetcomm_dll >zero && _inetcomm_dll  <fver(6,0,2800,1807))
	   || (_inetres_dll >zero && _inetres_dll  <fver(6,0,2800,1807))
	   || (_msident_dll >zero && _msident_dll  <fver(6,0,2800,1807))
	   || (_msimn_exe >zero && _msimn_exe  <fver(6,0,2800,1807))
	   || (_msoe_dll >zero && _msoe_dll  <fver(6,0,2800,1807))
	   || (_msoeacct_dll >zero && _msoeacct_dll  <fver(6,0,2800,1807))
	   || (_msoeres_dll >zero && _msoeres_dll  <fver(6,0,2800,1807))
	   || (_msoert2_dll >zero && _msoert2_dll  <fver(6,0,2800,1807))
	   || (_oeimport_dll >zero && _oeimport_dll  <fver(6,0,2800,1807))
	   || (_oemig50_exe >zero && _oemig50_exe  <fver(6,0,2800,1807))
	   || (_oemiglib_dll >zero && _oemiglib_dll  <fver(6,0,2800,1807))
	   || (_wab_exe >zero && _wab_exe  <fver(6,0,2800,1807))
	   || (_wab32_dll >zero && _wab32_dll  <fver(6,0,2800,1816))
	   || (_wabfind_dll >zero && _wabfind_dll  <fver(6,0,2800,1807))
	   || (_wabimp_dll >zero && _wabimp_dll  <fver(6,0,2800,1807))
	   || (_wabmig_exe >zero && _wabmig_exe <fver(6,0,2800,1807)) )) {
		NN("Cumulative Security Update for Outlook Express 6 Service Pack 1 (KB911567)");
		XX(p1+"oe6.0sp1-kb911567-windows-2000-xp-x86-enu_4d946bdca2160231bd638c04c83f226cda161ca1.exe"+a1);
	}
	/*if(sp==2 && (sku & XP_ALL) && _inetcomm_dll>zero && _inetcomm_dll<fver(6,0,2900,3198)) {
		// KB941202 is replaced by KB978542 on SP2
		NN("Security Update for Outlook Express for Windows XP (KB941202)");
		XX(p+"windowsxp-kb941202-x86-enu_a82783a1dd4d11145d52b0fa0aa2f8dc172eefc3.exe"+a1);
	}*/
	if((sp==2 && (sku & XP_ALL) && ( (_inetcomm_dll>zero && _inetcomm_dll<fver(6,0,2900,3664))
					  ||  (_msoe_dll>zero && _msoe_dll<fver(6,0,2900,3664))))
	|| (sp==3 && (sku & XP_ALL) && ( (_inetcomm_dll>zero && _inetcomm_dll<fver(6,0,2900,5931))
					  ||  (_msoe_dll>zero && _msoe_dll<fver(6,0,2900,5931))))) {
		NN("Security Update for Windows XP (KB978542)");
		XX(p3+"WindowsXP-KB978542-x86-ENU.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && (
		                  ( _inetcomm_dll >zero && _inetcomm_dll <fver(6,0,2900,2869))
					  ||  ( _msoe_dll     >zero && _msoe_dll     <fver(6,0,2900,2869))
					  ||  ( _wab32_dll    >zero && _wab32_dll    <fver(6,0,2900,2869))
					  ||  ( _wabimp_dll   >zero && _wabimp_dll   <fver(6,0,2900,2869)) )) {
		// KB911567 is replaced by KB929123 on Windows XP SP2
		NN("Cumulative Security Update for Outlook Express for Windows XP (KB911567)");
		XX(p2+"windowsxp-kb911567-x86-enu_3322015f70b63fd2c82af81148ed88055a6f7e5b.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && (
		                  ( _directdb_dll >zero && _directdb_dll <fver(6,0,2900,3138))
					  ||  ( _inetcomm_dll >zero && _inetcomm_dll <fver(6,0,2900,3138))
					  ||  ( _msoe_dll     >zero && _msoe_dll     <fver(6,0,2900,3138))
					  ||  ( _wab32_dll    >zero && _wab32_dll    <fver(6,0,2900,3138))
					  ||  ( _wabimp_dll   >zero && _wabimp_dll   <fver(6,0,2900,3138)) )) {
		NN("Cumulative Security Update for Outlook Express for Windows XP (KB929123)");
		XX(p2+"windowsxp-kb929123-x86-enu_e0a3cac53fbefd5629a4cc6ec79803875ff473b3.exe"+a1);
	}


	// Internet Explorer Updates
	/*if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2713,1100))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2713,1100))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2713,1500))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2712,300))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2713,1100))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2712,300)) )) {
		NN("11 February 2002 Cumulative Patch for Internet Explorer");
		XX(sw+rtm+"ie6.0-q316059.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2715,400))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2715,400))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2715,400))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2715,400)) )) {
		NN("28 March 2002 Cumulative Patch for Internet Explorer");
		XX(sw+rtm+"ie6.0-q319182.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2716,2200))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2716,1500))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2716,1500))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2715,400)) )) {
		NN("May 2002: Cumulative Patch for Internet Explorer (Q321232)");
		XX(sw+rtm+"ie6.0-q321232.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2719,2200))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2719,2200))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2719,2200))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("August 2002, Cumulative Update for Internet Explorer (Q323759)");
		XX(sw+rtm+"ie6.0-q323759.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2722,900))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2722,900))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2722,900))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("Q328970: November, 2002 Cumulative Patch for Internet Explorer 6");
		XX(sw+rtm+"ie6.0-q328970.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2722,900))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2723,100))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2722,900))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("Q324929: December, 2002 Cumulative Patch for Internet Explorer");
		XX(sw+rtm+"ie6.0-q324929.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2723,2500))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2723,100))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2722,900))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("810847: February 2003, Cumulative Patch for Internet Explorer 6");
		XX(sw+rtm+"ie6.0-q810847.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2726,2500))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2723,100))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2727,2400))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("813489: April 2003, Cumulative Patch for Internet Explorer 6");
		XX(sw+rtm+"ie6.0-q813489.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2726,2500))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2729,2200))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2728,1400))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("818529: June 2003, Cumulative Patch for Internet Explorer 6");
		XX(sw+rtm+"ie6.0-q818529.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2730,1700))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2729,2200))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2730,1200))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2731,1000))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("August 2003, Cumulative Patch for Internet Explorer 6 (822925)");
		XX(sw+rtm+"ie6.0-q822925.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2733,1800))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2729,2200))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2730,1200))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2733,1000))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("October 2003, Cumulative Patch for Internet Explorer 6 (828750)");
		XX(sw+rtm+"ie6.0-q828750.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2734,1600))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2734,1600))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2730,1200))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2734,200))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2718,400)) )) {
		NN("824145: November 2003, Cumulative Patch for Internet Explorer 6");
		XX(sw+rtm+"ie6.0-q824145.exe"+a8);
	}*/
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2600,0) && _shdocvw_dll<fver(6,0,2800,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2737,1600))
	    || (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2737,800))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2722,900))
		|| (_shdoclc_dll  >zero && _shdoclc_dll  <fver(6,0,2715,400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2737,800))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2730,1200))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2715,400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2736,2300))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2737,800)) )) {
		NN("Cumulative Security Update for Internet Explorer 6 (KB832894)");
		XX(sw+rtm+"ie6.0-Q832894.exe"+a8);
	}
	/*if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1126))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1126)) )) {
		NN("Q328970: November, 2002 Cumulative Patch for Internet Explorer 6 SP1");
		XX(sw+rtm+"ie6.0sp1-q328970.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1126))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1133))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1126)) )) {
		NN("December, 2002 Cumulative Patch for Internet Explorer 6 Service Pack 1");
		XX(sw+rtm+"ie6.0sp1-q324929.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1141))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1154))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1154)) )) {
		// Q810847 is replaced by Q824145
		NN("810847: February 2003, Cumulative Patch for Internet Explorer 6 Service Pack 1");
		XX(sw+rtm+"ie6.0sp1-q810847.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) &&
		(  _mshtml_dll  >= fver(6,0,2800,1141)
		&& _shdocvw_dll >= fver(6,0,2800,1154)
		&& _urlmon_dll  >= fver(6,0,2800,1154)) && (
		   (_urlmon_dll >=fver(6,0,2800,0) && _urlmon_dll <fver(6,0,2800,1167)) )) {
	// This update requires Internet Explorer 6.0 Service Pack 1 and Q810847 to be installed.
	// Q813951 is replaced by Q824145
		NN("813951: Update for Internet Explorer 6 SP1");
		XX(sw+rtm+"q813951_080984187358e485219cec8a3ea3fa4ab37d7833.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1170))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1170))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1170)) )) {
		NN("813489: April 2003, Cumulative Patch for Internet Explorer 6 Service Pack 1");
		XX(sw+rtm+"ie6.0sp1-q813489.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1170))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1203))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1188)) )) {
		NN("818529: June 2003, Cumulative Patch for Internet Explorer 6 Service Pack 1");
		XX(sw+rtm+"ie6.0sp1-q818529.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1226))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1203))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1226))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1226)) )) {
		NN("August 2003, Cumulative Patch for Internet Explorer 6 Service Pack 1 (822925)");
		XX(sw+rtm+"ie6.0sp1-q822925.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1264))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1203))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1226))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1259)) )) {
		NN("October 2003, Cumulative Patch for Internet Explorer 6 Service Pack 1 (828750)");
		XX(sw+rtm+"ie6.0sp1-q828750.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1276))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1276))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1276))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1282)) )) {
		NN("824145: November 2003, Cumulative Patch for Internet Explorer 6 SP1");
		XX(sw+rtm+"ie6.0sp1-q824145.exe"+a8);
	}*/
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2800,1400))
		|| (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1400))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1400))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1400))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1400))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2800,1400)) )) {
		NN("Cumulative Security Update for Internet Explorer 6 Service Pack 1 (KB832894)");
		XX(sw+rtm+"ie6.0sp1-Q832894.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_wininet_dll   >zero && _wininet_dll   <fver(6,0,2800,1405)) )) {
		NN("Update for Internet Explorer 6 Service Pack 1 (KB831167)");
		XX(sw+rtm+"Q831167.exe"+a8);
	}
	if( sp==1 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2800,1612))
	    || (_cdfview_dll  >zero && _cdfview_dll  <fver(6,0,2800,1612))
		|| (_iepeers_dll  >zero && _iepeers_dll  <fver(6,0,2800,1485))
		|| (_inseng_dll   >zero && _inseng_dll   <fver(6,0,2800,1469))
		|| (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1491))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1612))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1612))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1485))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2800,1485)) )) {
		NN("Cumulative Security Update for Internet Explorer 6 Service Pack 1 (KB867282)");
		XX(p1+"ie6.0sp1-kb867282-windows-2000-xp-x86-enu_261bd11af527fd33c1c30bb16789cfee73d0f7d9.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2800,1692))
	    || (_cdfview_dll  >zero && _cdfview_dll  <fver(6,0,2800,1612))
		|| (_danim_dll    >zero && _danim_dll    <fver(6,3,1,147))
		|| (_dxtrans_dll  >zero && _dxtrans_dll  <fver(6,3,2800,1519))
		|| (_iepeers_dll  >zero && _iepeers_dll  <fver(6,0,2800,1496))
		|| (_inseng_dll   >zero && _inseng_dll   <fver(6,0,2800,1469))
		|| (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2800,1522))
		|| (_msrating_dll >zero && _msrating_dll <fver(6,0,2800,1623))
		|| (_mstime_dll   >zero && _mstime_dll   <fver(6,0,2800,1519))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2800,1505))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2800,1692))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2800,1740))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2800,1519))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2800,1511)) )) {
		NN("Cumulative Security Update for Internet Explorer 6 Service Pack 1 (KB896688)");
		XX(p1+"ie6.0sp1-kb896688-windows-2000-xp-x86-enu_57950e4fbed19228cd32acf537946a9077d4b6d5.exe"+a1);
	}
	if((sp==1 && (sku & XP_ALL) && ( (_browseui_dll>zero && _browseui_dll<fver(6,0,2800,1892))
					  ||  (_shdocvw_dll>zero && _shdocvw_dll<fver(6,0,2800,1892)) ))
	 ||(sp==2 && (sku & XP_ALL) && _shdocvw_dll>zero && _shdocvw_dll<fver(6,0,2900,2987))) {
		NN("Security Update for Windows XP (KB924496)");
		XX(p2+"windowsxp-kb924496-x86-enu_f1e2421551a739eae947590735fb3f4abec82c22.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		   (_vgx_dll >zero && _vgx_dll <fver(6,0,2800,1580)) )) {
		NN("Security Update for Internet Explorer 6 Service Pack 1 (KB925486)");
		XX(p1+"ie6.0sp1-kb925486-windowsxp-x86-enu_08de2f06f64de1e2225032d8e49e45b6f31eb8ba.exe"+a1);
	}
	/*if( sp==2 && (sku & XP_ALL) && (_iexplore_exe>=fver(6,0,0,0) && _iexplore_exe<fver(7,0,0,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2900,3231))
		|| (_cdfview_dll  >zero && _cdfview_dll  <fver(6,0,2900,3231))
		|| (_danim_dll    >zero && _danim_dll    <fver(6,3,1,148))
		|| (_dxtmsft_dll  >zero && _dxtmsft_dll  <fver(6,3,2900,3231))
		|| (_dxtrans_dll  >zero && _dxtrans_dll  <fver(6,3,2900,3231))
		|| (_extmgr_dll   >zero && _extmgr_dll   <fver(6,0,2900,3231))
		|| (_iedw_exe     >zero && _iedw_exe     <fver(5,1,2600,3231))
		|| (_iepeers_dll  >zero && _iepeers_dll  <fver(6,0,2900,3231))
		|| (_inseng_dll   >zero && _inseng_dll   <fver(6,0,2900,3231))
		|| (_jsproxy_dll  >zero && _jsproxy_dll  <fver(6,0,2900,3231))
		|| (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2900,3243))
		|| (_mshtmled_dll >zero && _mshtmled_dll <fver(6,0,2900,3231))
		|| (_msrating_dll >zero && _msrating_dll <fver(6,0,2900,3231))
		|| (_mstime_dll   >zero && _mstime_dll   <fver(6,0,2900,3231))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2900,3231))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2900,3231))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2900,3231))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2900,3231))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2900,3231)) )) {
		// KB942615 is replaced by KB947864
		NN("Cumulative Security Update for Internet Explorer for Windows XP Service Pack 2 (KB942615)");
		XX(p2+"WindowsXP-KB942615-x86-ENU.exe"+a1);
	}*/
	if( sp==2 && (sku & XP_ALL) && (_iexplore_exe>=fver(6,0,0,0) && _iexplore_exe<fver(7,0,0,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2900,3314))
	    || (_cdfview_dll  >zero && _cdfview_dll  <fver(6,0,2900,3314))
		|| (_danim_dll    >zero && _danim_dll    <fver(6,3,1,148))
		|| (_dxtmsft_dll  >zero && _dxtmsft_dll  <fver(6,3,2900,3314))
		|| (_dxtrans_dll  >zero && _dxtrans_dll  <fver(6,3,2900,3314))
		|| (_extmgr_dll   >zero && _extmgr_dll   <fver(6,0,2900,3314))
		|| (_iedw_exe     >zero && _iedw_exe     <fver(5,1,2600,3314))
		|| (_iepeers_dll  >zero && _iepeers_dll  <fver(6,0,2900,3314))
		|| (_inseng_dll   >zero && _inseng_dll   <fver(6,0,2900,3314))
		|| (_jsproxy_dll  >zero && _jsproxy_dll  <fver(6,0,2900,3314))
		|| (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2900,3314))
		|| (_mshtmled_dll >zero && _mshtmled_dll <fver(6,0,2900,3314))
		|| (_msrating_dll >zero && _msrating_dll <fver(6,0,2900,3314))
		|| (_mstime_dll   >zero && _mstime_dll   <fver(6,0,2900,3314))
		|| (_pngfilt_dll  >zero && _pngfilt_dll  <fver(6,0,2900,3314))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2900,3314))
		|| (_shlwapi_dll  >zero && _shlwapi_dll  <fver(6,0,2900,3314))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2900,3314))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2900,3314)) )) {
		NN("Cumulative Security Update for Internet Explorer for Windows XP Service Pack 2 (KB947864)");
		XX(p2+"WindowsXP-KB947864-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (_iexplore_exe>=fver(6,0,0,0) && _iexplore_exe<fver(7,0,0,0)) && 
			regQueryDWORD(L"SOFTWARE\\Microsoft\\Internet Explorer\\Main\\FeatureControl\\FEATURE_PROTECT_DECOMPRESSION_FILTER_FROM_ABORT_KB942367",L"*",&status)!=1 ) {
		NN("Update for Internet Explorer 6 for Windows XP (KB946627)");
		XX(p2+"windowsxp-kb946627-x86-enu_f46e072004e766768205a78b6cd207f871c5376e.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (_iexplore_exe>=fver(7,0,5730,11) && _iexplore_exe<fver(7,1,0,0)) && (
		/* GDR */
		   (_advpack_dll     >zero && _advpack_dll     <fver(7,0,6000,16640))
		|| (_dxtmsft_dll     >zero && _dxtmsft_dll     <fver(7,0,6000,16640))
		|| (_dxtrans_dll     >zero && _dxtrans_dll     <fver(7,0,6000,16640))
		|| (_extmgr_dll      >zero && _extmgr_dll      <fver(7,0,6000,16640))
		|| (_icardie_dll     >zero && _icardie_dll     <fver(7,0,6000,16640))
		|| (_ie4uinit_exe    >zero && _ie4uinit_exe    <fver(7,0,6000,16640))
		|| (_ieakeng_dll     >zero && _ieakeng_dll     <fver(7,0,6000,16640))
		|| (_ieaksie_dll     >zero && _ieaksie_dll     <fver(7,0,6000,16640))
		|| (_ieakui_dll      >zero && _ieakui_dll      <fver(7,0,6000,16640))
		|| (_ieapfltr_dat    >zero && _ieapfltr_dat    <fver(7,0,6011,0))
		|| (_ieapfltr_dll    >zero && _ieapfltr_dll    <fver(7,0,6000,16461))
		|| (_iedkcs32_dll    >zero && _iedkcs32_dll    <fver(17,0,6000,16640))
		|| (_ieframe_dll     >zero && _ieframe_dll     <fver(7,0,6000,16640))
		|| (_ieframe_dll_mui >zero && _ieframe_dll_mui <fver(7,0,6000,16414))
		|| (_iernonce_dll    >zero && _iernonce_dll    <fver(7,0,6000,16640))
		|| (_iertutil_dll    >zero && _iertutil_dll    <fver(7,0,6000,16640))
		|| (_ieudinit_exe    >zero && _ieudinit_exe    <fver(7,0,6000,16640))
		|| (_iexplore_exe    >zero && _iexplore_exe    <fver(7,0,6000,16640))
		|| (_inetcpl_cpl     >zero && _inetcpl_cpl     <fver(7,0,6000,16640))
		|| (_jsproxy_dll     >zero && _jsproxy_dll     <fver(7,0,6000,16640))
		|| (_msfeeds_dll     >zero && _msfeeds_dll     <fver(7,0,6000,16640))
		|| (_msfeedsbs_dll   >zero && _msfeedsbs_dll   <fver(7,0,6000,16640))
		|| (_mshtml_dll      >zero && _mshtml_dll      <fver(7,0,6000,16640))
		|| (_mshtmled_dll    >zero && _mshtmled_dll    <fver(7,0,6000,16640))
		|| (_msrating_dll    >zero && _msrating_dll    <fver(7,0,6000,16640))
		|| (_mstime_dll      >zero && _mstime_dll      <fver(7,0,6000,16640))
		|| (_occache_dll     >zero && _occache_dll     <fver(7,0,6000,16640))
		|| (_pngfilt_dll     >zero && _pngfilt_dll     <fver(7,0,6000,16640))
		|| (_url_dll         >zero && _url_dll         <fver(7,0,6000,16640))
		|| (_urlmon_dll      >zero && _urlmon_dll      <fver(7,0,6000,16640))
		|| (_webcheck_dll    >zero && _webcheck_dll    <fver(7,0,6000,16640))
		|| (_wininet_dll     >zero && _wininet_dll     <fver(7,0,6000,16640))
		/* QFE */
		|| (_advpack_dll     >fver(7,0,6000,16640) && _advpack_dll     <fver(7,0,6000,20772))
		|| (_dxtmsft_dll     >fver(7,0,6000,16640) && _dxtmsft_dll     <fver(7,0,6000,20772))
		|| (_dxtrans_dll     >fver(7,0,6000,16640) && _dxtrans_dll     <fver(7,0,6000,20772))
		|| (_extmgr_dll      >fver(7,0,6000,16640) && _extmgr_dll      <fver(7,0,6000,20772))
		|| (_icardie_dll     >fver(7,0,6000,16640) && _icardie_dll     <fver(7,0,6000,20772))
		|| (_ie4uinit_exe    >fver(7,0,6000,16640) && _ie4uinit_exe    <fver(7,0,6000,20772))
		|| (_ieakeng_dll     >fver(7,0,6000,16640) && _ieakeng_dll     <fver(7,0,6000,20772))
		|| (_ieaksie_dll     >fver(7,0,6000,16640) && _ieaksie_dll     <fver(7,0,6000,20772))
		|| (_ieakui_dll      >fver(7,0,6000,16640) && _ieakui_dll      <fver(7,0,6000,20772))
		|| (_iedkcs32_dll    >fver(17,0,6000,16640)&& _iedkcs32_dll    <fver(17,0,6000,20772))
		|| (_ieframe_dll     >fver(7,0,6000,16640) && _ieframe_dll     <fver(7,0,6000,20772))
		|| (_iernonce_dll    >fver(7,0,6000,16640) && _iernonce_dll    <fver(7,0,6000,20772))
		|| (_iertutil_dll    >fver(7,0,6000,16640) && _iertutil_dll    <fver(7,0,6000,20772))
		|| (_ieudinit_exe    >fver(7,0,6000,16640) && _ieudinit_exe    <fver(7,0,6000,20772))
		|| (_iexplore_exe    >fver(7,0,6000,16640) && _iexplore_exe    <fver(7,0,6000,20772))
		|| (_inetcpl_cpl     >fver(7,0,6000,16640) && _inetcpl_cpl     <fver(7,0,6000,20772))
		|| (_jsproxy_dll     >fver(7,0,6000,16640) && _jsproxy_dll     <fver(7,0,6000,20772))
		|| (_msfeeds_dll     >fver(7,0,6000,16640) && _msfeeds_dll     <fver(7,0,6000,20772))
		|| (_msfeedsbs_dll   >fver(7,0,6000,16640) && _msfeedsbs_dll   <fver(7,0,6000,20772))
		|| (_mshtml_dll      >fver(7,0,6000,16640) && _mshtml_dll      <fver(7,0,6000,20772))
		|| (_mshtmled_dll    >fver(7,0,6000,16640) && _mshtmled_dll    <fver(7,0,6000,20772))
		|| (_msrating_dll    >fver(7,0,6000,16640) && _msrating_dll    <fver(7,0,6000,20772))
		|| (_mstime_dll      >fver(7,0,6000,16640) && _mstime_dll      <fver(7,0,6000,20772))
		|| (_occache_dll     >fver(7,0,6000,16640) && _occache_dll     <fver(7,0,6000,20772))
		|| (_pngfilt_dll     >fver(7,0,6000,16640) && _pngfilt_dll     <fver(7,0,6000,20772))
		|| (_url_dll         >fver(7,0,6000,16640) && _url_dll         <fver(7,0,6000,20772))
		|| (_urlmon_dll      >fver(7,0,6000,16640) && _urlmon_dll      <fver(7,0,6000,20772))
		|| (_webcheck_dll    >fver(7,0,6000,16640) && _webcheck_dll    <fver(7,0,6000,20772))
		|| (_wininet_dll     >fver(7,0,6000,16640) && _wininet_dll     <fver(7,0,6000,20772)))) {
		NN("Cumulative Security Update for Internet Explorer 7 for Windows XP (KB947864)");
		XX(p2+"IE7-WindowsXP-KB947864-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (_iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0)) && (
		/* GDR */
		   (_ie4uinit_exe  >zero && _ie4uinit_exe  <fver(8,0,6001,18876))
		|| (_iedkcs32_dll  >zero && _iedkcs32_dll  <fver(18,0,6001,18876))
		|| (_ieframe_dll   >zero && _ieframe_dll   <fver(8,0,6001,18876))
		|| (_iepeers_dll   >zero && _iepeers_dll   <fver(8,0,6001,18876))
		|| (_ieproxy_dll   >zero && _ieproxy_dll   <fver(8,0,6001,18876))
		|| (_iertutil_dll  >zero && _iertutil_dll  <fver(8,0,6001,18876))
		|| (_inetcpl_cpl   >zero && _inetcpl_cpl   <fver(8,0,6001,18876))
		|| (_jsproxy_dll   >zero && _jsproxy_dll   <fver(8,0,6001,18876))
		|| (_msfeeds_dll   >zero && _msfeeds_dll   <fver(8,0,6001,18876))
		|| (_msfeedsbs_dll >zero && _msfeedsbs_dll <fver(8,0,6001,18876))
		|| (_mshtml_dll    >zero && _mshtml_dll    <fver(8,0,6001,18876))
		|| (_occache_dll   >zero && _occache_dll   <fver(8,0,6001,18876))
		|| (_urlmon_dll    >zero && _urlmon_dll    <fver(8,0,6001,18876))
		|| (_wininet_dll   >zero && _wininet_dll   <fver(8,0,6001,18876))
		|| (_xpshims_dll   >zero && _xpshims_dll   <fver(8,0,6001,18876))
		/* QFE */
		|| (_ie4uinit_exe  >fver(8,0,6001,18876) && _ie4uinit_exe  <fver(8,0,6001,22967))
		|| (_iedkcs32_dll  >fver(18,0,6001,18876)&& _iedkcs32_dll  <fver(18,0,6001,22967))
		|| (_ieframe_dll   >fver(8,0,6001,18876) && _ieframe_dll   <fver(8,0,6001,22967))
		|| (_iepeers_dll   >fver(8,0,6001,18876) && _iepeers_dll   <fver(8,0,6001,22967))
		|| (_ieproxy_dll   >fver(8,0,6001,18876) && _ieproxy_dll   <fver(8,0,6001,22967))
		|| (_iertutil_dll  >fver(8,0,6001,18876) && _iertutil_dll  <fver(8,0,6001,22967))
		|| (_inetcpl_cpl   >fver(8,0,6001,18876) && _inetcpl_cpl   <fver(8,0,6001,22967))
		|| (_jsproxy_dll   >fver(8,0,6001,18876) && _jsproxy_dll   <fver(8,0,6001,22967))
		|| (_msfeeds_dll   >fver(8,0,6001,18876) && _msfeeds_dll   <fver(8,0,6001,22967))
		|| (_msfeedsbs_dll >fver(8,0,6001,18876) && _msfeedsbs_dll <fver(8,0,6001,22967))
		|| (_mshtml_dll    >fver(8,0,6001,18876) && _mshtml_dll    <fver(8,0,6001,22967))
		|| (_occache_dll   >fver(8,0,6001,18876) && _occache_dll   <fver(8,0,6001,22967))
		|| (_urlmon_dll    >fver(8,0,6001,18876) && _urlmon_dll    <fver(8,0,6001,22967))
		|| (_wininet_dll   >fver(8,0,6001,18876) && _wininet_dll   <fver(8,0,6001,22967))
		|| (_xpshims_dll   >fver(8,0,6001,18876) && _xpshims_dll   <fver(8,0,6001,22967)) )) {
		NN("Cumulative Security Update for Internet Explorer 8 for Windows XP (KB978207)");
		XX(p2+"IE8-WindowsXP-KB978207-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && (_iexplore_exe>=fver(6,0,0,0) && _iexplore_exe<fver(7,0,0,0)) && (
		   (_browseui_dll >zero && _browseui_dll <fver(6,0,2900,6452))
		|| (_html_iec     >zero && _html_iec     <fver(2011,1,31,10))
		|| (_ieencode_dll >zero && _ieencode_dll <fver(2011,1,31,10))
		|| (_iepeers_dll  >zero && _iepeers_dll  <fver(6,0,2900,6452))
		|| (_mshtml_dll   >zero && _mshtml_dll   <fver(6,0,2900,6452))
		|| (_mshtmled_dll >zero && _mshtmled_dll <fver(6,0,2900,6452))
		|| (_mstime_dll   >zero && _mstime_dll   <fver(6,0,2900,6452))
		|| (_shdocvw_dll  >zero && _shdocvw_dll  <fver(6,0,2900,6452))
		|| (_tdc_ocx      >zero && _tdc_ocx      <fver(1,3,0,3131))
		|| (_url_dll      >zero && _url_dll      <fver(6,0,2900,6452))
		|| (_urlmon_dll   >zero && _urlmon_dll   <fver(6,0,2900,6452))
		|| (_vgx_dll      >zero && _vgx_dll      <fver(6,0,2900,6452))
		|| (_wininet_dll  >zero && _wininet_dll  <fver(6,0,2900,6452)) )) {
		NN("Cumulative Security Update for Internet Explorer 6 for Windows XP (KB2879017)");
		XX(p3+"windowsxp-kb2879017-x86-enu_e6b354acd732c385254babb1a6a61568bf185b65.exe"+a1);
	}
	if( sp==3 && (sku & XP_PLUS_FLP) && (_iexplore_exe>=fver(7,0,5730,11) && _iexplore_exe<fver(7,1,0,0)) && (
		   (_advpack_dll     >zero && _advpack_dll     <fver(7,0,6000,21357))
	    || (_corpol_dll      >zero && _corpol_dll      <fver(2007,0,0,21306))
		|| (_dxtmsft_dll     >zero && _dxtmsft_dll     <fver(7,0,6000,21357))
		|| (_dxtrans_dll     >zero && _dxtrans_dll     <fver(7,0,6000,21357))
		|| (_extmgr_dll      >zero && _extmgr_dll      <fver(7,0,6000,21357))
		|| (_html_iec        >zero && _html_iec        <fver(2017,0,0,21306))
		|| (_icardie_dll     >zero && _icardie_dll     <fver(7,0,6000,21357))
		|| (_ie4uinit_exe    >zero && _ie4uinit_exe    <fver(7,0,6000,21357))
		|| (_ieakeng_dll     >zero && _ieakeng_dll     <fver(7,0,6000,21357))
		|| (_ieaksie_dll     >zero && _ieaksie_dll     <fver(7,0,6000,21357))
		|| (_ieakui_dll      >zero && _ieakui_dll      <fver(7,0,6000,21357))
		|| (_ieapfltr_dat    >zero && _ieapfltr_dat    <fver(7,0,6011,1))
		|| (_ieapfltr_dll    >zero && _ieapfltr_dll    <fver(7,0,6000,16730))
		|| (_iedkcs32_dll    >zero && _iedkcs32_dll    <fver(17,0,6000,21357))
		|| (_ieencode_dll    >zero && _ieencode_dll    <fver(2017,0,0,21306))
		|| (_ieframe_dll     >zero && _ieframe_dll     <fver(7,0,6000,21357))
		|| (_ieframe_dll_mui >zero && _ieframe_dll_mui <fver(7,0,6000,16414))
		|| (_iepeers_dll     >zero && _iepeers_dll     <fver(7,0,6000,21357))
		|| (_iernonce_dll    >zero && _iernonce_dll    <fver(7,0,6000,21357))
		|| (_iertutil_dll    >zero && _iertutil_dll    <fver(7,0,6000,21357))
		|| (_ieudinit_exe    >zero && _ieudinit_exe    <fver(7,0,6000,21357))
		|| (_iexplore_exe    >zero && _iexplore_exe    <fver(7,0,6000,21357))
		|| (_inetcpl_cpl     >zero && _inetcpl_cpl     <fver(7,0,6000,21357))
		|| (_jsproxy_dll     >zero && _jsproxy_dll     <fver(7,0,6000,21357))
		|| (_msfeeds_dll     >zero && _msfeeds_dll     <fver(7,0,6000,21357))
		|| (_msfeedsbs_dll   >zero && _msfeedsbs_dll   <fver(7,0,6000,21357))
		|| (_mshtml_dll      >zero && _mshtml_dll      <fver(7,0,6000,21357))
		|| (_mshtmled_dll    >zero && _mshtmled_dll    <fver(7,0,6000,21357))
		|| (_msrating_dll    >zero && _msrating_dll    <fver(7,0,6000,21357))
		|| (_mstime_dll      >zero && _mstime_dll      <fver(7,0,6000,21357))
		|| (_occache_dll     >zero && _occache_dll     <fver(7,0,6000,21357))
		|| (_pngfilt_dll     >zero && _pngfilt_dll     <fver(7,0,6000,21357))
		|| (_url_dll         >zero && _url_dll         <fver(7,0,6000,21357))
		|| (_urlmon_dll      >zero && _urlmon_dll      <fver(7,0,6000,21357))
		|| (_vgx_dll         >zero && _vgx_dll         <fver(7,0,6000,21357))
		|| (_webcheck_dll    >zero && _webcheck_dll    <fver(7,0,6000,21357))
		|| (_wininet_dll     >zero && _wininet_dll     <fver(7,0,6000,21357))
		)) {
		NN("Cumulative Security Update for Internet Explorer 7 for Windows XP (KB2879017)");
		XX(p3+"IE7-WindowsXP-KB2879017-x86-ENU.exe"+a1);
	}
	/*if( sp==3 && (sku & XPE_2009) && (_iexplore_exe>=fver(7,0,5730,11) && _iexplore_exe<fver(7,1,0,0)) && (
		// KB3148198 is replaced by KB4018271
		   (_advpack_dll     >zero && _advpack_dll     <fver(7,0,6000,21582))
	    || (_corpol_dll      >zero && _corpol_dll      <fver(2007,0,0,21306))
		|| (_dxtmsft_dll     >zero && _dxtmsft_dll     <fver(7,0,6000,21582))
		|| (_dxtrans_dll     >zero && _dxtrans_dll     <fver(7,0,6000,21582))
		|| (_extmgr_dll      >zero && _extmgr_dll      <fver(7,0,6000,21582))
		|| (_html_iec        >zero && _html_iec        <fver(2017,0,0,21306))
		|| (_icardie_dll     >zero && _icardie_dll     <fver(7,0,6000,21582))
		|| (_ie4uinit_exe    >zero && _ie4uinit_exe    <fver(7,0,6000,21582))
		|| (_ieakeng_dll     >zero && _ieakeng_dll     <fver(7,0,6000,21582))
		|| (_ieaksie_dll     >zero && _ieaksie_dll     <fver(7,0,6000,21582))
		|| (_ieakui_dll      >zero && _ieakui_dll      <fver(7,0,6000,21582))
		|| (_ieapfltr_dat    >zero && _ieapfltr_dat    <fver(7,0,6011,1))
		|| (_ieapfltr_dll    >zero && _ieapfltr_dll    <fver(7,0,6000,16730))
		|| (_iedkcs32_dll    >zero && _iedkcs32_dll    <fver(17,0,6000,21582))
		|| (_ieencode_dll    >zero && _ieencode_dll    <fver(2017,0,0,21306))
		|| (_ieframe_dll     >zero && _ieframe_dll     <fver(7,0,6000,21582))
		|| (_ieframe_dll_mui >zero && _ieframe_dll_mui <fver(7,0,6000,16414))
		|| (_iepeers_dll     >zero && _iepeers_dll     <fver(7,0,6000,21582))
		|| (_iernonce_dll    >zero && _iernonce_dll    <fver(7,0,6000,21582))
		|| (_iertutil_dll    >zero && _iertutil_dll    <fver(7,0,6000,21582))
		|| (_ieudinit_exe    >zero && _ieudinit_exe    <fver(7,0,6000,21582))
		|| (_iexplore_exe    >zero && _iexplore_exe    <fver(7,0,6000,21582))
		|| (_inetcpl_cpl     >zero && _inetcpl_cpl     <fver(7,0,6000,21582))
		|| (_jsproxy_dll     >zero && _jsproxy_dll     <fver(7,0,6000,21582))
		|| (_msfeeds_dll     >zero && _msfeeds_dll     <fver(7,0,6000,21582))
		|| (_msfeedsbs_dll   >zero && _msfeedsbs_dll   <fver(7,0,6000,21582))
		|| (_mshtml_dll      >zero && _mshtml_dll      <fver(7,0,6000,21582))
		|| (_mshtmled_dll    >zero && _mshtmled_dll    <fver(7,0,6000,21582))
		|| (_msrating_dll    >zero && _msrating_dll    <fver(7,0,6000,21582))
		|| (_mstime_dll      >zero && _mstime_dll      <fver(7,0,6000,21582))
		|| (_occache_dll     >zero && _occache_dll     <fver(7,0,6000,21582))
		|| (_pngfilt_dll     >zero && _pngfilt_dll     <fver(7,0,6000,21582))
		|| (_url_dll         >zero && _url_dll         <fver(7,0,6000,21582))
		|| (_urlmon_dll      >zero && _urlmon_dll      <fver(7,0,6000,21582))
		|| (_vgx_dll         >zero && _vgx_dll         <fver(7,0,6000,21582))
		|| (_webcheck_dll    >zero && _webcheck_dll    <fver(7,0,6000,21582))
		|| (_wininet_dll     >zero && _wininet_dll     <fver(7,0,6000,21582)) )) {
		NN("Cumulative Security Update for Internet Explorer 7 for WEPOS and POSReady 2009 (KB3148198)");
		XX(p3+"ie7-windowsxp-kb3148198-x86-embedded-enu_254750ce40d33a8a4985de8339d8e66df6861f19.exe"+a1);
	}*/
	if( sp==3 && (sku & XP_EMBEDDED) && (_iexplore_exe>=fver(7,0,5730,11) && _iexplore_exe<fver(7,1,0,0)) && (
		   (_advpack_dll     >zero && _advpack_dll     <fver(7,0,6000,24071))
	    || (_corpol_dll      >zero && _corpol_dll      <fver(2007,0,0,21306))
		|| (_dxtmsft_dll     >zero && _dxtmsft_dll     <fver(7,0,6000,24071))
		|| (_dxtrans_dll     >zero && _dxtrans_dll     <fver(7,0,6000,24071))
		|| (_extmgr_dll      >zero && _extmgr_dll      <fver(7,0,6000,24071))
		|| (_html_iec        >zero && _html_iec        <fver(2017,0,0,21306))
		|| (_icardie_dll     >zero && _icardie_dll     <fver(7,0,6000,24071))
		|| (_ie4uinit_exe    >zero && _ie4uinit_exe    <fver(7,0,6000,24071))
		|| (_ieakeng_dll     >zero && _ieakeng_dll     <fver(7,0,6000,24071))
		|| (_ieaksie_dll     >zero && _ieaksie_dll     <fver(7,0,6000,24071))
		|| (_ieakui_dll      >zero && _ieakui_dll      <fver(7,0,6000,24071))
		|| (_ieapfltr_dat    >zero && _ieapfltr_dat    <fver(7,0,6011,1))
		|| (_ieapfltr_dll    >zero && _ieapfltr_dll    <fver(7,0,6000,16730))
		|| (_iedkcs32_dll    >zero && _iedkcs32_dll    <fver(17,0,6000,24071))
		|| (_ieencode_dll    >zero && _ieencode_dll    <fver(2017,0,0,21306))
		|| (_ieframe_dll     >zero && _ieframe_dll     <fver(7,0,6000,24071))
		|| (_ieframe_dll_mui >zero && _ieframe_dll_mui <fver(7,0,6000,16414))
		|| (_iepeers_dll     >zero && _iepeers_dll     <fver(7,0,6000,24071))
		|| (_iernonce_dll    >zero && _iernonce_dll    <fver(7,0,6000,24071))
		|| (_iertutil_dll    >zero && _iertutil_dll    <fver(7,0,6000,24071))
		|| (_ieudinit_exe    >zero && _ieudinit_exe    <fver(7,0,6000,24071))
		|| (_iexplore_exe    >zero && _iexplore_exe    <fver(7,0,6000,24071))
		|| (_inetcomm_dll    >zero && _inetcomm_dll    <fver(6,0,2900,7246))
		|| (_inetcpl_cpl     >zero && _inetcpl_cpl     <fver(7,0,6000,24071))
		|| (_jscript_dll     >zero && _jscript_dll     <fver(5,7,6002,24071))
		|| (_jsproxy_dll     >zero && _jsproxy_dll     <fver(7,0,6000,24071))
		|| (_msfeeds_dll     >zero && _msfeeds_dll     <fver(7,0,6000,24071))
		|| (_msfeedsbs_dll   >zero && _msfeedsbs_dll   <fver(7,0,6000,24071))
		|| (_mshtml_dll      >zero && _mshtml_dll      <fver(7,0,6000,24071))
		|| (_mshtmled_dll    >zero && _mshtmled_dll    <fver(7,0,6000,24071))
		|| (_msrating_dll    >zero && _msrating_dll    <fver(7,0,6000,24071))
		|| (_mstime_dll      >zero && _mstime_dll      <fver(7,0,6000,24071))
		|| (_occache_dll     >zero && _occache_dll     <fver(7,0,6000,24071))
		|| (_pngfilt_dll     >zero && _pngfilt_dll     <fver(7,0,6000,24071))
		|| (_tdc_ocx         >zero && _tdc_ocx         <fver(7,0,6000,24071))
		|| (_url_dll         >zero && _url_dll         <fver(7,0,6000,24071))
		|| (_urlmon_dll      >zero && _urlmon_dll      <fver(7,0,6000,24071))
		|| (_vbscript_dll    >zero && _vbscript_dll    <fver(5,7,6002,24071))
		|| (_vgx_dll         >zero && _vgx_dll         <fver(7,0,6000,24071))
		|| (_webcheck_dll    >zero && _webcheck_dll    <fver(7,0,6000,24071))
		|| (_wininet_dll     >zero && _wininet_dll     <fver(7,0,6000,24071)) )) {
		NN("Cumulative Security Update for Internet Explorer 7 for Windows XP SP3 for XPe (KB4018271)");
		XX(p3+"ie7-windowsxp-kb4018271-x86-embedded-custom-enu_8a89028acf7c751031a5748720deff45a054045e.exe"+a1);
	}
	if( sp==3 && (sku & XPE_FLP) && (_iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0)) && (
		/* QFE */
		   (_corpol_dll       >zero && _corpol_dll       <fver(2008,0,0,23532))
		|| (_html_iec         >zero && _html_iec         <fver(2018,0,0,23532))
		|| (_ie4uinit_exe     >zero && _ie4uinit_exe     <fver(8,0,6001,23532))
		|| (_iedkcs32_dll     >zero && _iedkcs32_dll     <fver(18,0,6001,23532))
		|| (_iedvtool_dll     >zero && _iedvtool_dll     <fver(8,0,6001,23532))
		|| (_ieframe_dll      >zero && _ieframe_dll      <fver(8,0,6001,23532))
		|| (_iepeers_dll      >zero && _iepeers_dll      <fver(8,0,6001,23532))
		|| (_ieproxy_dll      >zero && _ieproxy_dll      <fver(8,0,6001,23532))
		|| (_iertutil_dll     >zero && _iertutil_dll     <fver(8,0,6001,23532))
		|| (_inetcpl_cpl      >zero && _inetcpl_cpl      <fver(8,0,6001,23532))
		|| (_jsdbgui_dll      >zero && _jsdbgui_dll      <fver(8,0,6001,23532))
		|| (_jsproxy_dll      >zero && _jsproxy_dll      <fver(8,0,6001,23532))
		|| (_licmgr10_dll     >zero && _licmgr10_dll     <fver(8,0,6001,23532))
		|| (_msfeeds_dll      >zero && _msfeeds_dll      <fver(8,0,6001,23532))
		|| (_msfeedsbs_dll    >zero && _msfeedsbs_dll    <fver(8,0,6001,23532))
		|| (_mshtml_dll       >zero && _mshtml_dll       <fver(8,0,6001,23532))
		|| (_mshtmled_dll     >zero && _mshtmled_dll     <fver(8,0,6001,23532))
		|| (_mstime_dll       >zero && _mstime_dll       <fver(8,0,6001,23532))
		|| (_occache_dll      >zero && _occache_dll      <fver(8,0,6001,23532))
		|| (_url_dll          >zero && _url_dll          <fver(8,0,6001,23532))
		|| (_urlmon_dll       >zero && _urlmon_dll       <fver(8,0,6001,23532))
		|| (_vgx_dll          >zero && _vgx_dll          <fver(8,0,6001,23532))
		|| (_wininet_dll      >zero && _wininet_dll      <fver(8,0,6001,23532))
		|| (_xpshims_dll      >zero && _xpshims_dll      <fver(8,0,6001,23532)) )) {
		NN("Cumulative Security Update for Internet Explorer 8 for Windows XP (KB2879017)");
		XX(p3+"IE8-WindowsXP-KB2879017-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_CLIENT) && (_iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0)) && (
		   (_corpol_dll       >zero && _corpol_dll       <fver(2008,0,0,23942))
		|| (_dxtmsft_dll      >zero && _dxtmsft_dll      <fver(8,0,6001,23942))
		|| (_dxtrans_dll      >zero && _dxtrans_dll      <fver(8,0,6001,23942))
		|| (_hlink_dll        >zero && _hlink_dll        <fver(6,0,6001,23942))
		|| (_html_iec         >zero && _html_iec         <fver(2018,0,0,23942))
		|| (_ie4uinit_exe     >zero && _ie4uinit_exe     <fver(8,0,6001,23942))
		|| (_iedkcs32_dll     >zero && _iedkcs32_dll     <fver(18,0,6001,23942))
		|| (_iedvtool_dll     >zero && _iedvtool_dll     <fver(8,0,6001,23942))
		|| (_ieframe_dll      >zero && _ieframe_dll      <fver(8,0,6001,23942))
		|| (_iepeers_dll      >zero && _iepeers_dll      <fver(8,0,6001,23942))
		|| (_ieproxy_dll      >zero && _ieproxy_dll      <fver(8,0,6001,23942))
		|| (_iertutil_dll     >zero && _iertutil_dll     <fver(8,0,6001,23942))
		|| (_iexplore_exe     >zero && _iexplore_exe     <fver(8,0,6001,23942))
		|| (_inetcomm_dll     >zero && _inetcomm_dll     <fver(6,0,2900,7246))
		|| (_inetcpl_cpl      >zero && _inetcpl_cpl      <fver(8,0,6001,23942))
		|| (_jscript_dll      >zero && _jscript_dll      <fver(5,8,6001,23942))
		|| (_jsdbgui_dll      >zero && _jsdbgui_dll      <fver(8,0,6001,23942))
		|| (_jsprofilerui_dll >zero && _jsprofilerui_dll <fver(8,0,6001,23942))
		|| (_jsproxy_dll      >zero && _jsproxy_dll      <fver(8,0,6001,23942))
		|| (_licmgr10_dll     >zero && _licmgr10_dll     <fver(8,0,6001,23942))
		|| (_msfeeds_dll      >zero && _msfeeds_dll      <fver(8,0,6001,23942))
		|| (_msfeedsbs_dll    >zero && _msfeedsbs_dll    <fver(8,0,6001,23942))
		|| (_mshtml_dll       >zero && _mshtml_dll       <fver(8,0,6001,23942))
		|| (_mshtmled_dll     >zero && _mshtmled_dll     <fver(8,0,6001,23942))
		|| (_msrating_dll     >zero && _msrating_dll     <fver(8,0,6001,23942))
		|| (_mstime_dll       >zero && _mstime_dll       <fver(8,0,6001,23942))
		|| (_occache_dll      >zero && _occache_dll      <fver(8,0,6001,23942))
		|| (_tdc_ocx          >zero && _tdc_ocx          <fver(8,0,6001,23942))
		|| (_url_dll          >zero && _url_dll          <fver(8,0,6001,23942))
		|| (_urlmon_dll       >zero && _urlmon_dll       <fver(8,0,6001,23942))
		|| (_vbscript_dll     >zero && _vbscript_dll     <fver(5,8,6001,23942))
		|| (_vgx_dll          >zero && _vgx_dll          <fver(8,0,6001,23942))
		|| (_webcheck_dll     >zero && _webcheck_dll     <fver(8,0,6001,23942))
		|| (_wininet_dll      >zero && _wininet_dll      <fver(8,0,6001,23942))
		|| (_xpshims_dll      >zero && _xpshims_dll      <fver(8,0,6001,23942))
		)) {
		NN("Cumulative Security Update for Internet Explorer 8 for Windows XP SP3 (KB4018271)");
		XX(p3+"ie8-windowsxp-kb4018271-x86-custom-enu_122a1122404b9862e6998bcec0917de02c378de6.exe"+a1);
	}
	if( sp==3 && (sku & XPE_WES2009) && (_iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0)) && (
		   (_corpol_dll       >zero && _corpol_dll       <fver(2008,0,0,24159))
		|| (_dxtmsft_dll      >zero && _dxtmsft_dll      <fver(8,0,6001,24159))
		|| (_dxtrans_dll      >zero && _dxtrans_dll      <fver(8,0,6001,24159))
		|| (_hlink_dll        >zero && _hlink_dll        <fver(6,0,6001,24159))
		|| (_html_iec         >zero && _html_iec         <fver(2018,0,0,24159))
		|| (_ie4uinit_exe     >zero && _ie4uinit_exe     <fver(8,0,6001,24159))
		|| (_iedkcs32_dll     >zero && _iedkcs32_dll     <fver(18,0,6001,24159))
		|| (_iedvtool_dll     >zero && _iedvtool_dll     <fver(8,0,6001,24159))
		|| (_ieframe_dll      >zero && _ieframe_dll      <fver(8,0,6001,24159))
		|| (_iepeers_dll      >zero && _iepeers_dll      <fver(8,0,6001,24159))
		|| (_ieproxy_dll      >zero && _ieproxy_dll      <fver(8,0,6001,24159))
		|| (_iertutil_dll     >zero && _iertutil_dll     <fver(8,0,6001,24159))
		|| (_iexplore_exe     >zero && _iexplore_exe     <fver(8,0,6001,24159))
		|| (_inetcomm_dll     >zero && _inetcomm_dll     <fver(6,0,2900,7641))
		|| (_inetcpl_cpl      >zero && _inetcpl_cpl      <fver(8,0,6001,24159))
		|| (_jscript_dll      >zero && _jscript_dll      <fver(5,8,6001,24159))
		|| (_jsdbgui_dll      >zero && _jsdbgui_dll      <fver(8,0,6001,24159))
		|| (_jsprofilerui_dll >zero && _jsprofilerui_dll <fver(8,0,6001,24159))
		|| (_jsproxy_dll      >zero && _jsproxy_dll      <fver(8,0,6001,24159))
		|| (_licmgr10_dll     >zero && _licmgr10_dll     <fver(8,0,6001,24159))
		|| (_msfeeds_dll      >zero && _msfeeds_dll      <fver(8,0,6001,24159))
		|| (_msfeedsbs_dll    >zero && _msfeedsbs_dll    <fver(8,0,6001,24159))
		|| (_mshtml_dll       >zero && _mshtml_dll       <fver(8,0,6001,24159))
		|| (_mshtmled_dll     >zero && _mshtmled_dll     <fver(8,0,6001,24159))
		|| (_msrating_dll     >zero && _msrating_dll     <fver(8,0,6001,24159))
		|| (_mstime_dll       >zero && _mstime_dll       <fver(8,0,6001,24159))
		|| (_occache_dll      >zero && _occache_dll      <fver(8,0,6001,24159))
		|| (_tdc_ocx          >zero && _tdc_ocx          <fver(8,0,6001,24159))
		|| (_url_dll          >zero && _url_dll          <fver(8,0,6001,24159))
		|| (_urlmon_dll       >zero && _urlmon_dll       <fver(8,0,6001,24159))
		|| (_vbscript_dll     >zero && _vbscript_dll     <fver(5,8,6001,24159))
		|| (_vgx_dll          >zero && _vgx_dll          <fver(8,0,6001,24159))
		|| (_webcheck_dll     >zero && _webcheck_dll     <fver(8,0,6001,24159))
		|| (_wininet_dll      >zero && _wininet_dll      <fver(8,0,6001,24159))
		|| (_xpshims_dll      >zero && _xpshims_dll      <fver(8,0,6001,24159))
		)) {
		NN("2019-02 Cumulative Security Update for Internet Explorer 8 for WES09 and POSReady 2009 for x86-based systems (KB4486474)");
		XX(p3+"ie8-windowsxp-kb4486474-x86-embedded-enu_c48cdecf2affafccc0668791ac03df6f110922c4.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && (_iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0)) && (
		   (_corpol_dll       >zero && _corpol_dll       <fver(2008,0,0,24177))
		|| (_dxtmsft_dll      >zero && _dxtmsft_dll      <fver(8,0,6001,24177))
		|| (_dxtrans_dll      >zero && _dxtrans_dll      <fver(8,0,6001,24177))
		|| (_hlink_dll        >zero && _hlink_dll        <fver(6,0,6001,24177))
		|| (_html_iec         >zero && _html_iec         <fver(2018,0,0,24177))
		|| (_ie4uinit_exe     >zero && _ie4uinit_exe     <fver(8,0,6001,24177))
		|| (_iedkcs32_dll     >zero && _iedkcs32_dll     <fver(18,0,6001,24177))
		|| (_iedvtool_dll     >zero && _iedvtool_dll     <fver(8,0,6001,24177))
		|| (_ieframe_dll      >zero && _ieframe_dll      <fver(8,0,6001,24177))
		|| (_iepeers_dll      >zero && _iepeers_dll      <fver(8,0,6001,24177))
		|| (_ieproxy_dll      >zero && _ieproxy_dll      <fver(8,0,6001,24177))
		|| (_iertutil_dll     >zero && _iertutil_dll     <fver(8,0,6001,24177))
		|| (_iexplore_exe     >zero && _iexplore_exe     <fver(8,0,6001,24177))
		|| (_inetcomm_dll     >zero && _inetcomm_dll     <fver(6,0,2900,7681))
		|| (_inetcpl_cpl      >zero && _inetcpl_cpl      <fver(8,0,6001,24177))
		|| (_jscript_dll      >zero && _jscript_dll      <fver(5,8,6001,24177))
		|| (_jsdbgui_dll      >zero && _jsdbgui_dll      <fver(8,0,6001,24177))
		|| (_jsprofilerui_dll >zero && _jsprofilerui_dll <fver(8,0,6001,24177))
		|| (_jsproxy_dll      >zero && _jsproxy_dll      <fver(8,0,6001,24177))
		|| (_licmgr10_dll     >zero && _licmgr10_dll     <fver(8,0,6001,24177))
		|| (_msfeeds_dll      >zero && _msfeeds_dll      <fver(8,0,6001,24177))
		|| (_msfeedsbs_dll    >zero && _msfeedsbs_dll    <fver(8,0,6001,24177))
		|| (_mshtml_dll       >zero && _mshtml_dll       <fver(8,0,6001,24177))
		|| (_mshtmled_dll     >zero && _mshtmled_dll     <fver(8,0,6001,24177))
		|| (_msrating_dll     >zero && _msrating_dll     <fver(8,0,6001,24177))
		|| (_mstime_dll       >zero && _mstime_dll       <fver(8,0,6001,24177))
		|| (_occache_dll      >zero && _occache_dll      <fver(8,0,6001,24177))
		|| (_tdc_ocx          >zero && _tdc_ocx          <fver(8,0,6001,24177))
		|| (_url_dll          >zero && _url_dll          <fver(8,0,6001,24177))
		|| (_urlmon_dll       >zero && _urlmon_dll       <fver(8,0,6001,24177))
		|| (_vbscript_dll     >zero && _vbscript_dll     <fver(5,8,6001,24177))
		|| (_vgx_dll          >zero && _vgx_dll          <fver(8,0,6001,24177))
		|| (_webcheck_dll     >zero && _webcheck_dll     <fver(8,0,6001,24177))
		|| (_wininet_dll      >zero && _wininet_dll      <fver(8,0,6001,24177))
		|| (_xpshims_dll      >zero && _xpshims_dll      <fver(8,0,6001,24177))
		)) {
		NN("2019-04 Cumulative Security Update for Internet Explorer 8 for POSReady 2009 for x86-based systems (KB4493435)");
		XX(p3+"ie8-windowsxp-kb4493435-x86-embedded-enu_1029e7193ca4b683d1d0c1996bc7c66f1a2ae37f.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && (_iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0)) && (
		/* GDR */
		   (_iecompat_dll     <fver(8,0,6001,19130))
		/* QFE */
		|| (_iecompat_dll      >fver(8,0,6001,19130) && _iecompat_dll      <fver(8,0,6001,23226)) )) {
		NN("Update for Internet Explorer 8 Compatibility View List for Windows XP (KB2598845)");
		XX(p3+"IE8-WindowsXP-KB2598845-x86-ENU.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && (_shdocvw_dll>=fver(6,0,2800,1106) && _shdocvw_dll<fver(6,0,2900,0)) && (
		                  ( _jgdw400_dll >zero && _jgdw400_dll <fver(106,0,0,0))
					  ||  ( _jgpl400_dll >zero && _jgpl400_dll <fver( 54,0,0,0)) )) {
		NN("Security Update for Internet Explorer 6 Service Pack 1 (KB918439)");
		XX(p1+"ie6.0sp1-kb918439-windows-2000-xp-x86-enu_a6973947012196eb5a2be06bffdc5c19a14209ea.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (
		                  ( _jgdw400_dll >zero && _jgdw400_dll <fver(106,0,0,0))
					  ||  ( _jgpl400_dll >zero && _jgpl400_dll <fver( 54,0,0,0)) )) {
		NN("Security Update for Windows XP (KB918439)");
		XX(p2+"windowsxp-kb918439-x86-enu_056bf5d0c049e0e8e799593b3508627ee8557dc1.exe"+a1);
	}

	// Jscript and Vbscript 5.6 (IE6)
	if( sp==0 && (sku & XP_ALL) && _jscript_dll>=fver(5,6,0,0) && _jscript_dll<fver(5,6,0,8513)) {
		NN("814078: Security Update (Microsoft Jscript version 5.6, Windows 2000, Windows XP)");
		XX(rtm+"js56_db18c6ea0f4e8522715beea284f6843ece71d944.exe"+a2);
	}
	if( sp==1 && (sku & XP_ALL) && _jscript_dll>=fver(5,6,0,0) && _jscript_dll<fver(5,6,0,8831)) {
		NN("Security Update for Windows XP (KB917344)");
		XX(p1+"windowsxp-kb917344-x86-enu_9c0c688c3e5c11a1b2ce0666117dc193823367b0.exe"+a1);
	}
	if( sp<2 && (sku & XP_ALL) && _vbscript_dll>=fver(5,6,0,0) && _vbscript_dll<fver(5,6,0,7426)) {
		NN("Security Update, February 14, 2002 (Internet Explorer 6)");
		XX(sw+rtm+"VBS56NEN.EXE"+a8);
	}
	if( sp==2 && (sku & XP_ALL) && 
		( _jscript_dll <= fver(5,6,0,8835) && _vbscript_dll <= fver(5,6,0,8835)) &&
		(                 (_jscript_dll >=fver(5,6,0,0) && _jscript_dll <fver(5,6,0,8835))
		              ||  (_vbscript_dll>=fver(5,6,0,0) && _vbscript_dll<fver(5,6,0,8835)) )) {
		NN("Security Update for Windows XP (KB944338)");
		XX(p2+"windowsxp-kb944338-v2-x86-enu_d8cade8456591867f22cdae4c42db7f473afbd67.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _jscript_dll>=fver(5,6,0,0) && _jscript_dll<fver(5,6,0,8837)) {
		NN("Security Update for Jscript 5.6 for Windows XP (KB971961)");
		XX(p2+"windowsxp-kb971961-js56-x86-enu_9ce6ccd0ed6a7b00212e2a90afe2b4747017b3a5.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _vbscript_dll>=fver(5,6,0,0) && _vbscript_dll<fver(5,6,0,8838)) {
		NN("Security Update for Windows XP (KB981350)");
		XX(p2+"windowsxp-kb981350-x86-enu_9fc0909c468c17870ab868d0d7f4eb815b309045.exe"+a1);
	}

	// Jscript and Vbscript 5.7 (IE7)
	if( sp==2 && (sku & XP_ALL) && _jscript_dll>=fver(5,7,0,0) && _jscript_dll<fver(5,7,6002,22145)) {
		NN("Security Update for Jscript 5.7 for Windows XP (KB971961)");
		XX(p2+"windowsxp-kb971961-js57-x86-enu_9722544230b316cbd21740ed56dc7a9e7af9603b.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _vbscript_dll>=fver(5,7,0,0) && _vbscript_dll<fver(5,7,6002,22354)) {
		NN("Security Update for Windows XP (KB981349)");
		XX(p2+"windowsxp-kb981349-x86-enu_e5fec41b4a9647bc5050dbbd1a08b469d72c69f4.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && (
		                  (_cscript_exe  >zero && _cscript_exe  <fver(5,7,0,18066))
		              ||  (_jscript_dll  >zero && _jscript_dll  <fver(5,7,0,18066))
					  ||  (_scrobj_dll   >zero && _scrobj_dll   <fver(5,7,0,18066))
					  ||  (_scrrun_dll   >zero && _scrrun_dll   <fver(5,7,0,18066))
					  ||  (_vbscript_dll >zero && _vbscript_dll <fver(5,7,0,18066))
					  ||  (_wscript_exe  >zero && _wscript_exe  <fver(5,7,0,18066))
					  ||  (_wshext_dll   >zero && _wshext_dll   <fver(5,7,0,18066))
					  ||  (_wshom_ocx    >zero && _wshom_ocx    <fver(5,7,0,18066)) )) {
		NN("Update for Windows XP (KB951978)");
		XX(p3+"WindowsXP-KB951978-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( (_jscript_dll>=fver(5,7,0,0) && _jscript_dll<fver(5,7,6002,22589))
					  ||  (_vbscript_dll>=fver(5,7,0,0) && _vbscript_dll<fver(5,7,6002,22589)) )) {
		NN("Security Update for Windows XP (KB2510581)");
		XX(p3+"WindowsXP-KB2510581-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _vbscript_dll>=fver(5,7,0,0) && _vbscript_dll<fver(5,7,6002,23292)) {
		NN("Security Update for Windows XP (KB2909212)");
		XX(p3+"WindowsXP-KB2909212-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XPE_2009) && ( (_jscript_dll>=fver(5,7,0,0) && _jscript_dll<fver(5,7,6002,23877))
					  ||  (_vbscript_dll>=fver(5,7,0,0) && _vbscript_dll<fver(5,7,6002,23877)) )) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3124624)");
		XX(p3+"windowsxp-kb3124624-x86-embedded-enu_31d357f9fffe58a498ac46282610be31263ab3de.exe"+a1);
	}

	// Jscript and Vbscript 5.8 (IE8)
//	if( sp==2 && (sku & XP_ALL) && ( _iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0))
//		/*&& _jscript_dll>=fver(5,8,0,0)*/ && _jscript_dll<fver(5,8,6001,22886)) {
//		NN("Security Update for Jscript 5.8 for Windows XP (KB971961)");
//		XX(p+"ie8-windowsxp-kb971961-x86-enu_49f7f3fb604da50265f558ff5db3542dcd947d87.exe"+a1);
//	}
	if( sp==2 && (sku & XP_ALL) && ( _iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0))
		/*&& _jscript_dll>=fver(5,8,0,0)*/ && _jscript_dll<fver(5,8,6001,22960)) {
		NN("Update for Windows XP (KB976662)");
		XX(p2+"ie8-windowsxp-kb976662-x86-enu_6060b6dd1afd7d41be7f1ae4632bd31657d5e0c4.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ( _iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0))
		/*&& _vbscript_dll>=fver(5,8,0,0)*/ && _vbscript_dll<fver(5,8,6001,23000)) {
		NN("Security Update for Windows XP (KB981332)");
		XX(p2+"ie8-windowsxp-kb981332-x86-enu_d1ba1076cb5d94b06333011c6eb29dedb5c2c48d.exe"+a1);
	}
	if( sp==3 && (sku & XP_PLUS_FLP) && ( _iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0))
		                && ( (_jscript_dll>zero && _jscript_dll<fver(5,8,6001,23141))
					  ||  (_vbscript_dll>zero && _vbscript_dll<fver(5,8,6001,23141)) )) {
		NN("Security Update for Windows XP (KB2510531)");
		XX(p3+"IE8-WindowsXP-KB2510531-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( _iexplore_exe>=fver(8,0,6001,18702) && _iexplore_exe<fver(8,1,0,0))
		/*&& _jscript_dll>=fver(5,8,0,0)*/ && _jscript_dll<fver(5,8,6001,23259)) {
		NN("Update for Windows XP (KB2632503)");
		XX(p3+"IE8-WindowsXP-KB2632503-x86-ENU.exe"+a1);
	}
	

	// Windows Media Player Updates

	// This key can also be used to determine WMP version:
	// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Active Setup\Installed Components\{6BF52A52-394A-11d3-B153-00C04F79FAA6}

	if( sp==0 && (sku & XP_ALL) && _wmpcore_dll >=fver(8,0,0,0) && _wmpcore_dll < fver(9,0,0,0) && (
		   (_dxmasf_dll   >=fver(6,4,9,0) && _dxmasf_dll   <fver(6,4,9,1121))
		|| (_msdxm_ocx    >=fver(6,4,9,0) && _msdxm_ocx    <fver(6,4,9,1124))
		|| (_unregmp2_exe >=fver(8,0,0,0) && _unregmp2_exe <fver(8,0,0,4482))
		|| (_wmpcore_dll  >=fver(8,0,0,0) && _wmpcore_dll  <fver(8,0,0,4482))
		|| (_wmplayer_exe >=fver(8,0,0,0) && _wmplayer_exe <fver(8,0,0,4482)) )) {
		NN("Q320920: Security Update (Windows Media Player for Windows XP)");
		XX(rtm+"wm320920_8_2d2bb2749d7aebe8efee1df06da63b146dd1176a.exe"+a8);
	}
	else if( sp==0 && (sku & XP_ALL) && (
		   (_dxmasf_dll   >=fver(6,4,9,0) && _dxmasf_dll   <fver(6,4,9,1121))
		|| (_msdxm_ocx    >=fver(6,4,9,0) && _msdxm_ocx    <fver(6,4,9,1124)) )) {
		NN("Q320920: Security Update (Windows Media Player 6.4)");
		XX(rtm+"wm320920_64.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (
		   (_wmp_dll      >=fver(9,0,0,2980) && _wmp_dll   <fver(9,0,0,3008)) )) {
		NN("Flaw In Windows Media Player May Allow Media Library Access (819639)");
		XX(sw+rtm+"windowsmedia9-kb819639-x86-enu_bfd620da8e1529c3e4ffadfb93f33fa.exe"+a8);
	}
	if( sp==0 && (sku & XP_ALL) && (
		   (_wmp_dll      >=fver(9,0,0,2980) && _wmp_dll   <fver(9,0,0,3128)) )) {
		NN("Update for Windows Media Player 9 Series (KB837272)");
		XX(rtm+"windowsmedia9-kb837272-enu_481af581c0365fc3fc7bfc482760a68.exe"+a6);
	}
	if( sp<2 && (sku & XP_ALL) && (
		   (_msdxm_ocx    >=fver(6,4,9,0) && _msdxm_ocx    <fver(6,4,9,1128))
		|| (_wmp_dll      >=fver(9,0,0,0) && _wmp_dll      <fver(9,0,0,3075))
		|| (_wmpcore_dll  >=fver(7,10,0,0)&& _wmpcore_dll  <fver(7,10,0,3075))
		|| (_wmpcore_dll  >=fver(8,0,0,0) && _wmpcore_dll  <fver(8,0,0,4491)) )) {
		NN("Critical Update for Windows Media Player Script Commands (KB828026)");
		XX(p1+"windowsmedia-q828026-x86-enu_461ed9f2ff022332af70d41d8fdaec4.exe"+a6);
	}
	if( sp==1 && (sku & XP_ALL) && _wmplayer_exe>=fver(8,0,0,0) && _wmplayer_exe<fver(8,0,0,4490)) {
		NN("817787: Security Update Windows Media Player for XP");
		XX(p1+"windowsmedia8-kb817787-x86-enu_3dd1c7dba24e432e5df843b4bd01d81.exe"+a2);
	}
	if( sp==1 && (sku & XP_ALL) && _wmpui_dll>=fver(8,0,0,2) && _wmpui_dll<fver(8,0,0,4496)) {
		NN("Security Update for Windows Media Player 8 for Windows XP (KB917734)");
		XX(p1+"windowsmedia8-kb917734-x86-enu_4733e6d1a9dc7f044197a7be6dc83223c797b3cf.exe"+a1);
	}
	if( sp==1 && (sku & XP_ALL) && _wmp_dll>=fver(9,0,0,2980) && _wmp_dll<fver(9,0,0,3349)) {
		NN("Security Update for Windows Media Player 9 (KB917734)");
		XX(p1+"windowsmedia9-kb917734-x86-enu_6e46e8d786614f9306b1307c2991e5acaa4dceac.exe"+a1);
	}
	/*if( sp==1 && (sku & XP_ALL) && _wmp_dll>=fver(10,0,0,3646) && _wmp_dll<fver(10,0,0,3909)) {
		// KB895572 is replaced by KB917734
		NN("Update for Windows XP (KB895572)");
		XX(p1+"windowsmedia10-kb895572-x86-enu.exe"+a1);
	}*/
	if( sp==1 && (sku & XP_ALL) && _wmp_dll>=fver(10,0,0,3646) && _wmp_dll<fver(10,0,0,4036)) {
		NN("Security Update for Windows Media Player 10 for Windows XP (KB917734)");
		XX(p1+"windowsmedia10-kb917734-x86-enu_499fe88d62843835153a4225712e1b2f19120527.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && ( (_dxmasf_dll>=fver(6,4,9,0) && _dxmasf_dll<fver(6,4,9,1133))
					  ||  (_strmdll_dll>=fver(4,1,0,0) && _strmdll_dll<fver(4,1,0,3936)) )) {
		NN("Security Update for Windows Media Player 6.4 (KB925398)");
		XX(p3+"windowsmedia6-kb925398-v2-x86-enu_c8b7d9a38ecbd2bd8925345d69c53697a285b917.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _wmpeffects_dll>=fver(11,0,5721,5145) && _wmpeffects_dll<fver(11,0,5721,5252)) {
		NN("Security Update for Windows XP (KB954154)");
		XX(p3+"windowsmedia11-kb954154-x86-enu_9b75815f8c5d57dc8e2e22e221fb1a106f9ce6a0.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ((_wmp_dll  >=fver(9,0,0,2980) /* WM9L Section */
		                 && _wmpdxm_dll >=fver(9,0,0,2980)
					     && (_wmp_dll<fver(9,0,0,3364) || _wmpdxm_dll<fver(9,0,0,3364)))

					  || (_wmp_dll  >=fver(10,0,0,3646) /* WM10L Section */
		                 && _wmpdxm_dll >=fver(10,0,0,3646)
					     && (_wmp_dll<fver(10,0,0,4074) || _wmpdxm_dll<fver(10,0,0,4074)))

					  || (_wmp_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmpdxm_dll >=fver(11,0,5721,5145)
					     && (_wmp_dll<fver(11,0,5721,5268) || _wmpdxm_dll<fver(11,0,5721,5268)))					   
					   )) {
		NN("Security Update for Windows XP Service Pack 2 (KB973540)");
		XX(p2+"windowsxp-sp2-windowsmedia-kb973540-x86-enu_dff9fd1cafd1b740784f00e43a3aff588d0c810d.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && _wmp_dll>=fver(9,0,0,2980) && _wmp_dll<fver(9,0,0,3367)) {
		NN("Security Update for Windows Media Player 9 for Windows XP SP2 (KB979402)");
		XX(p2+"windowsxp-sp2-windowsmedia-kb979402-x86-enu_09cff283401228cd946081159b796d5d1abdfbd3.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ((_wmp_dll  >=fver(9,0,0,2980) /* WM9 Section */
		                 && _wmpdxm_dll >=fver(9,0,0,2980)
					     && (_wmp_dll<fver(9,0,0,4507) || _wmpdxm_dll<fver(9,0,0,4507)))

					  || (_wmp_dll  >=fver(10,0,0,3646) /* WM10L Section */
		                 && _wmpdxm_dll >=fver(10,0,0,3646)
					     && (_wmp_dll<fver(10,0,0,4074) || _wmpdxm_dll<fver(10,0,0,4074)))

					  || (_wmp_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmpdxm_dll >=fver(11,0,5721,5145)
					     && (_wmp_dll<fver(11,0,5721,5268) || _wmpdxm_dll<fver(11,0,5721,5268)))					   
					   )) {
		NN("Security Update for Windows XP Service Pack 3 (KB973540)");
		XX(p3+"WindowsXP-WindowsMedia-KB973540-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && ( (_l3codeca_acm>zero && _l3codeca_acm<fver(1,9,0,306))
					  ||  (_l3codecx_ax>zero && _l3codecx_ax<fver(1,6,0,51)) )) {
		NN("Security Update for Windows XP (KB977816)");
		XX(p3+"WindowsXP-KB977816-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && ((_wmasf_dll     >=fver(9,0,0,2980) /* WMP9NL Section */
					     && (_wmasf_dll<fver(9,0,0,3267) || (_wmvcore_dll>zero && _wmvcore_dll<fver(9,0,0,3267))))
						 
					  || (_wmasf_dll     >=fver(9,0,0,3287) /* WMP9L Section */
					     && (_wmasf_dll<fver(9,0,0,3355) || (_wmvcore_dll>zero && _wmvcore_dll<fver(9,0,0,3355))))

					  || (_wmasf_dll  >=fver(10,0,0,3646) /* WMP10 Section */
		                 && _wmasf_dll < fver(10,0,0,4060))

					  || (_wmasf_dll  >=fver(10,0,0,4332) /* EMERALD Section */
		                 && _wmasf_dll < fver(10,0,0,4359))

					  || (_wmasf_dll  >=fver(11,0,5721,5145) /* WMP11 Section */
		                 && _wmasf_dll < fver(11,0,5721,5238))					   
					   )) {
		if(_wmasf_dll<fver(10,0,0,3646)) {
			NN("Security Update for Windows XP with Windows Media Format Runtime 9 (KB941569)");
		} else {
			NN("Security Update for Windows XP with Windows Media Format Runtime 9.5 and 11 (KB941569)");
		}
		XX(p3+"windowsxp-kb941569-x86-enu_55e8b619dd436e8be22c381fa8643a58b5f7c2a8.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ((_logagent_exe  >=fver(9,0,0,2980) /* WM9 Section */
		                 && _wmnetmgr_dll >=fver(9,0,0,2980)
						 && _wmvcore_dll  >=fver(9,0,0,2980)
					     && (_logagent_exe<fver(9,0,0,3268) || _wmnetmgr_dll<fver(9,0,0,3268) || _wmvcore_dll<fver(9,0,0,3268)))

					  || (_logagent_exe   >=fver(9,0,0,3287) /* WM9L Section */
		                 && _wmnetmgr_dll >=fver(9,0,0,3287)
						 && _wmvcore_dll  >=fver(9,0,0,3287)
					     && (_logagent_exe<fver(9,0,0,3358) || _wmnetmgr_dll<fver(9,0,0,3358) || _wmvcore_dll<fver(9,0,0,3358)))
					  
					  || (_logagent_exe   >=fver(10,0,0,3646) /* WM10 Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,3646)
						 && _wmvcore_dll  >=fver(10,0,0,3646)
					     && (_logagent_exe<fver(10,0,0,3703) || _wmnetmgr_dll<fver(10,0,0,3703) || _wmvcore_dll<fver(10,0,0,3703)))

					  || (_logagent_exe   >=fver(10,0,0,3802) /* WM10L Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,3802)
						 && _wmvcore_dll  >=fver(10,0,0,3802)
					     && (_logagent_exe<fver(10,0,0,4066) || _wmnetmgr_dll<fver(10,0,0,4066) || _wmvcore_dll<fver(10,0,0,4066)))

					  || (_logagent_exe   >=fver(10,0,0,3646) /* WM10MIX Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,3802)
						 && _wmvcore_dll  >=fver(10,0,0,3646)
					     && (_logagent_exe<fver(10,0,0,3703) || _wmnetmgr_dll<fver(10,0,0,4066) || _wmvcore_dll<fver(10,0,0,3703)))

					  || (_logagent_exe   >=fver(10,0,0,4332) /* MCEUR2 Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,4332)
						 && _wmvcore_dll  >=fver(10,0,0,4332)
					     && (_logagent_exe<fver(10,0,0,4362) || _wmnetmgr_dll<fver(10,0,0,4362) || _wmvcore_dll<fver(10,0,0,4362)))

					  || (_logagent_exe   >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmnetmgr_dll >=fver(11,0,5721,5145)
						 && _wmvcore_dll  >=fver(11,0,5721,5145)
					     && (_logagent_exe<fver(11,0,5721,5251) || _wmnetmgr_dll<fver(11,0,5721,5251) || _wmvcore_dll<fver(11,0,5721,5251)))
					   
					   )) {
		NN("Security Update for Windows XP Service Pack 2 (KB952069)");
		XX(p2+"windowsxp-sp2-windowsmedia-kb952069-v2-x86-enu_97de22fb2fed50fb03e1107b579975f2295feb8a.exe"+a1);
	}
	else if( sp==3 && (sku & XP_ALL) && ((_logagent_exe  >=fver(9,0,0,4500) /* WM9 Section */
		                 && _wmnetmgr_dll >=fver(9,0,0,4500)
						 && _wmvcore_dll  >=fver(9,0,0,4500)
					     && (_logagent_exe<fver(9,0,0,4504) || _wmnetmgr_dll<fver(9,0,0,4504) || _wmvcore_dll<fver(9,0,0,4504)))

					  || (_logagent_exe   >=fver(10,0,0,3646) /* WM10 Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,3646)
						 && _wmvcore_dll  >=fver(10,0,0,3646)
					     && (_logagent_exe<fver(10,0,0,3703) || _wmnetmgr_dll<fver(10,0,0,3703) || _wmvcore_dll<fver(10,0,0,3703)))

					  || (_logagent_exe   >=fver(10,0,0,3802) /* WM10L Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,3802)
						 && _wmvcore_dll  >=fver(10,0,0,3802)
					     && (_logagent_exe<fver(10,0,0,4066) || _wmnetmgr_dll<fver(10,0,0,4066) || _wmvcore_dll<fver(10,0,0,4066)))

					  || (_logagent_exe   >=fver(10,0,0,3646) /* WM10MIX Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,3802)
						 && _wmvcore_dll  >=fver(10,0,0,3646)
					     && (_logagent_exe<fver(10,0,0,3703) || _wmnetmgr_dll<fver(10,0,0,4066) || _wmvcore_dll<fver(10,0,0,3703)))

					  || (_logagent_exe   >=fver(10,0,0,4332) /* MCEUR2 Section */
		                 && _wmnetmgr_dll >=fver(10,0,0,4332)
						 && _wmvcore_dll  >=fver(10,0,0,4332)
					     && (_logagent_exe<fver(10,0,0,4362) || _wmnetmgr_dll<fver(10,0,0,4362) || _wmvcore_dll<fver(10,0,0,4362)))

					  || (_logagent_exe   >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmnetmgr_dll >=fver(11,0,5721,5145)
						 && _wmvcore_dll  >=fver(11,0,5721,5145)
					     && (_logagent_exe<fver(11,0,5721,5251) || _wmnetmgr_dll<fver(11,0,5721,5251) || _wmvcore_dll<fver(11,0,5721,5251)))
					   
					   )) {
		NN("Security Update for Windows XP Service Pack 3 (KB952069)");
		XX(p3+"WindowsXP-WindowsMedia-KB952069-v2-x86-ENU.exe"+a1);
	}
	else if( sp==3 && (sku & XP_ALL) && (
		/* WM9 Section */
		   (_logagent_exe >= fver(9,0,0,2980)  && _logagent_exe <= fver(9,0,0,9999) && _logagent_exe < fver(9,0,0,4504) )
		/* WM10 Section */
		|| (_logagent_exe >= fver(10,0,0,3646) && _logagent_exe <= fver(10,0,0,3801) && _logagent_exe < fver(10,0,0,3703) )
		/* WM10L Section */
		|| (_logagent_exe >= fver(10,0,0,3802) && _logagent_exe <= fver(10,0,0,4299) && _logagent_exe < fver(10,0,0,4066) )
		/* MCEUR2 Section */
		|| (_logagent_exe >= fver(10,0,0,4332) && _logagent_exe <= fver(10,0,0,4999) && _logagent_exe < fver(10,0,0,4362) )
		/* WM11 Section */
		|| (_logagent_exe >= fver(11,0,5721,5145) && _logagent_exe <= fver(11,0,5721,9999) && _logagent_exe < fver(11,0,5721,5251) ) )) {
		NN("Security Update for Windows XP Service Pack 3 (KB972187)");
		XX(p3+"windowsxp-windowsmedia-kb972187-x86-enu_d07aa4b7065e73bb2e9db2e7ce3289e4d91848ce.exe"+a1);
	}
	//	if( sp==2 && (sku & XP_ALL) && ((_wmvcore_dll  >=fver(9,0,0,2980) /* WMP9NL Section */
//		                 && _wmvcore_dll < fver(9,0,0,3265))
//
//					  || (_wmvcore_dll  >=fver(9,0,0,3287) /* WMP9L Section */
//		                 && _wmvcore_dll < fver(9,0,0,3353))
//
//					  || (_wmvcore_dll  >=fver(10,0,0,3646) /* WMP10NL Section */
//		                 && _wmvcore_dll < fver(10,0,0,3702))
//
//					  || (_wmvcore_dll  >=fver(10,0,0,3802) /* WMP10L Section */
//		                 && _wmvcore_dll < fver(10,0,0,4054))
//
//					  || (_wmvcore_dll  >=fver(10,0,0,4332) /* Emerald Section */
//		                 && _wmvcore_dll < fver(10,0,0,4357))					   
//					   )) {
//		// KB923689 is replaced by KB978695 on SP2
//		NN("Security Update for Windows XP with Windows Media Format Runtime 9 (KB923689)");
//		XX(p+"windowsxp-kb923689-v2-x86-enu_2cd41423c99bd3bd35d64fd2afcbe1005fd92a62.exe"+a1);
//	}
	if( sp==2 && (sku & XP_ALL) && ((_wmvcore_dll  >=fver(9,0,0,2980) /* WM9 Section */
		                 && _wmvcore_dll < fver(9,0,0,3272))

					  || (_wmvcore_dll  >=fver(9,0,0,3287) /* WM9L Section */
		                 && _wmvcore_dll < fver(9,0,0,3369))

					  || (_wmvcore_dll  >=fver(10,0,0,3646) /* WM10 Section */
		                 && _wmvcore_dll < fver(10,0,0,3706))

					  || (_wmvcore_dll  >=fver(10,0,0,3802) /* WM10L Section */
		                 && _wmvcore_dll < fver(10,0,0,4078))

					  || (_wmvcore_dll  >=fver(10,0,0,4332) /* MCEUR2 Section */
		                 && _wmvcore_dll < fver(10,0,0,4374))

					  || (_wmvcore_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmvcore_dll < fver(11,0,5721,5275))					   
					   )) {
		NN("Security Update for Windows Media Format Runtime 9, 9.5 & 11 for Windows XP SP2 (KB978695)");
		XX(p2+"windowsxp-sp2-windowsmedia-kb978695-x86-enu_0903920ab44d6cd39bcb6c8d192547e30d2c5809.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ((_wmvcore_dll  >=fver(9,0,0,2980) /* WM9 Section */
		                 && _wmvcore_dll < fver(9,0,0,4509))

					  || (_wmvcore_dll  >=fver(10,0,0,3646) /* WM10 Section */
		                 && _wmvcore_dll < fver(10,0,0,3706))

					  || (_wmvcore_dll  >=fver(10,0,0,3802) /* WM10L Section */
		                 && _wmvcore_dll < fver(10,0,0,4078))

					  || (_wmvcore_dll  >=fver(10,0,0,4332) /* MCEUR2 Section */
		                 && _wmvcore_dll < fver(10,0,0,4374))

					  || (_wmvcore_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmvcore_dll < fver(11,0,5721,5275))					   
					   )) {
		NN("Security Update for Windows Media Format Runtime 9, 9.5 & 11 for Windows XP SP3 (KB978695)");
		XX(p3+"WindowsXP-WindowsMedia-KB978695-x86-ENU.exe"+a1);
	}
	if( sp>=0 && !(sku & XP_MCE2005) && !kb891122 && !kb913800 && wmp >= fver(8,0,0,0) && (
		   (_mp4sdmod_dll <fver(9,0,0,2980))
		|| (_MP43DMOD_dll <fver(9,0,0,2980))
		|| (_MPG4DMOD_dll <fver(9,0,0,2980))
		|| (_WMADMOD_dll  <fver(9,0,0,2980))
		|| (_wmsdmod_dll  <fver(9,0,0,2980))
		|| (_wmspdmod_dll <fver(9,0,0,2980))
		|| (_wmvdmod_dll  <fver(9,0,0,2980)) ) && (

		   (_mp4sdmod_dll <=fver(9,0,0,2980))
		&& (_MP43DMOD_dll <=fver(9,0,0,2980))
		&& (_MPG4DMOD_dll <=fver(9,0,0,2980))
		&& (_WMADMOD_dll  <=fver(9,0,0,2980))
		&& (_wmsdmod_dll  <=fver(9,0,0,2980))
		&& (_wmspdmod_dll <=fver(9,0,0,2980))
		&& (_wmvdmod_dll  <=fver(9,0,0,2980)) )) {
		wm9codecs_v1_install=true;
		NN("Windows Media 9 Series Codec Install Package");
		XX(sw+p3+"wm9codecs_6dbfa989738fa4c4fd9934afbda30cef570582d6.exe /Q:U /R:N /C:\"setup_wm.exe /Q:U /R:N /DisallowSystemRestore\"");
	}
	if( wm9codecs_v1_install || (sp>=0 && !(sku & XP_MCE2005) && !kb891122 && !kb913800 && wmp >= fver(8,0,0,0) && (
		   (_WMADMOD_dll  <fver(10,0,0,3646))
		|| (_wmsdmod_dll  <fver(10,0,0,3646))
		|| (_wmspdmod_dll <fver(10,0,0,3646))
		|| (_WMVADVD_dll  <fver(10,0,0,3646))
		|| (_wmvdmod_dll  <fver(10,0,0,3646)) ) && (

		   (_WMADMOD_dll  <=fver(10,0,0,3646))
		&& (_wmsdmod_dll  <=fver(10,0,0,3646))
		&& (_wmspdmod_dll <=fver(10,0,0,3646))
		&& (_WMVADVD_dll  <=fver(10,0,0,3646))
		&& (_wmvdmod_dll  <=fver(10,0,0,3646)) ))) {
		wm9codecs_v2_install=true;
		NN("Windows Media Player 9 Codecs");
		XX(sw+p3+"WM9Codecs.exe /Q:U /R:N /C:\"setup_wm.exe /Q:U /R:N /DisallowSystemRestore\"");
	}
	if( sp==3 && !kb891122 && !kb913800 
		      && (sku & XP_ALL) && (
			  (_wmvdmod_dll>=fver(10,0,0,3646) && _wmvdmod_dll<fver(10,0,0,3708))
			  || wm9codecs_v2_install )) {
		NN("Security Update for Windows Media Format Runtime 9.5 for Windows XP (KB2834902)");
		XX(p3+"windowsxp-windowsmedia-kb2834902-v2-x86-enu_ecc8652da2b85688917a4f7aa48ac1efe84975fd.exe"+a1);
	}
	if( sp==3 && !kb913800 && (sku & XP_ALL) && (kb891122 || _wmvdmod_dll>=fver(10,0,0,3802)) && _wmvdmod_dll<fver(10,0,0,4084)) {
		NN("Security Update for Windows Media Format Runtime 9.5 for Windows XP (KB2834903)");
		XX(p3+"WindowsXP-WindowsMedia-KB2834903-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _wmvdecod_dll>=fver(11,0,5721,5145) && _wmvdecod_dll<fver(11,0,5721,5289)) {
		NN("Security Update for Windows Media Format Runtime 11 for Windows XP (KB2834904)");
		XX(p3+"WindowsXP-WindowsMedia-KB2834904-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && (kb913800 || _wmvdmod_dll>=fver(10,0,0,4332)) && _wmvdmod_dll<fver(10,0,0,4377)) {
		NN("Security Update for Windows Media Format Runtime 9.5 for Windows XP (KB2834905)");
		XX(p3+"windowsxp-windowsmedia-kb2834905-v2-x86-enu_c34829167838fddcc6d679649a1f79d39280a2f0.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && ((_wmspdmod_dll >=fver(9,0,0,2980) /* WM9 Section */
					     && _wmspdmod_dll<fver(9,0,0,3269))

					  || (_wmspdmod_dll  >=fver(10,0,0,3646) /* WM10 Section */
		                 && _wmspdmod_dll < fver(10,0,0,3704))

					  || (_wmspdmod_dll  >=fver(10,0,0,3802) /* WM10L Section */
		                 && _wmspdmod_dll < fver(10,0,0,4070))

					  || (_wmspdmod_dll  >=fver(10,0,0,4332) /* MCEUR2 Section */
		                 && _wmspdmod_dll < fver(10,0,0,4365))

					  || (_wmspdmod_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmspdmod_dll < fver(11,0,5721,5263))					   
					   )) {
		NN("Security Update for Windows Media Format Runtime 9, 9.5 & 11 for Windows XP SP 2 (KB954155)");
		XX(p2+"windowsxp-sp2-windowsmedia-kb954155-x86-enu_77db0c2aa79d2a5049fb9fb0fa4fbc5e98d4f30d.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( wm9codecs_v2_install
		              || (_wmspdmod_dll >=fver(9,0,0,2980) /* WM9 Section */
					     && _wmspdmod_dll<fver(9,0,0,4505))

					  || (_wmspdmod_dll  >=fver(10,0,0,3646) /* WM10 Section */
		                 && _wmspdmod_dll < fver(10,0,0,3704))

					  || (_wmspdmod_dll  >=fver(10,0,0,3802) /* WM10L Section */
		                 && _wmspdmod_dll < fver(10,0,0,4070))

					  || (_wmspdmod_dll  >=fver(10,0,0,4332) /* MCEUR2 Section */
		                 && _wmspdmod_dll < fver(10,0,0,4365))

					  || (_wmspdmod_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmspdmod_dll < fver(11,0,5721,5263))					   
					   )) {
		NN("Security Update for Windows Media Format Runtime 9, 9.5 & 11 for Windows XP SP 3 (KB954155)");
		XX(p3+"WindowsXP-WindowsMedia-KB954155-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ((_wmp_dll >=fver(9,0,0,2980) /* WM9 Section */
					     && _wmp_dll<fver(9,0,0,4510))

					  || (_wmp_dll  >=fver(10,0,0,3646) /* WM10L Section */
		                 && _wmp_dll < fver(10,0,0,4081))

					  || (_wmp_dll  >=fver(11,0,5721,5145) /* WM11 Section */
		                 && _wmp_dll < fver(11,0,5721,5280))					   
					   )) {
		NN("Security Update for Windows XP (KB2378111)");
		XX(p3+"WindowsXP-WindowsMedia-KB2378111-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _wmp_dll>=fver(9,0,0,0) && _wmp_dll<fver(9,0,0,4513)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3033890)");
		XX(p3+"windowsmedia9-kb3033890-x86-enu_cd51ca12cb3275c117784ab38eced274b61012bd.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _wmp_dll>=fver(10,0,0,0) && _wmp_dll<fver(10,0,0,4092)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3033890)");
		XX(p3+"windowsmedia10-kb3033890-x86-enu_ef4e82e7007462fb64ed10c4e127d828e5126efc.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _wmp_dll>=fver(11,0,5721,0) && _wmp_dll<fver(11,0,5721,5293)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3033890)");
		XX(p3+"windowsmedia11-kb3033890-x86-enu_3da047c27ca7b35c2a15e2bdf4a87fd7243e2b49.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _l3codecx_ax>zero && _l3codecx_ax<fver(1,6,0,52)) {
		NN("Security Update for Windows XP (KB2115168)");
		XX(p3+"WindowsXP-KB2115168-x86-ENU.exe"+a1);
	}
	if( sp>=0 && *wmp6cdcs) {
		NN("Codecs Package for Windows Media Player 6.4");
		XX(p3+"wmp6cdcs.EXE /Q:U /R:N /C:\"setup_wm.exe /Q:U /R:N\"");
	}
	if( sp>=0 && wmp >= fver(8,0,0,0) && (
		   (_wmv9vcm_dll  <fver(9,0,1,369))
		|| strncmp(_film_320x240_vbr_wv9_md5,"0c60d0a17cdb324e7ab0f66170a1c775",32) != 0) ) {
		wmv9vcm_install=true;
		NN("Windows Media Video 9 VCM");
		XX(sw+p3+"wmv9VCMsetup.exe"+a8);
	}
	if( sp>=2 && (sku & XP_ALL) && ( *wmp6cdcs || (_wmavds32_ax>=fver(9,0,0,3224) && _wmavds32_ax<fver(9,0,0,3360)))) {
		NN("Security Update for Windows 2000, Windows XP and Windows 2003 (KB969878)");
		XX(p3+"windowsmedia9-kb969878-x86-enu_c512a3bf6fb3be974534bbaf4dacb78808741128.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && ( *wmp6cdcs || (_msaud32_acm>zero && _msaud32_acm<fver(8,0,0,4502)))) {
		NN("Security Update for Windows XP (KB975025)");
		XX(p3+"WindowsXP-KB975025-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ( *wmp6cdcs || wmv9vcm_install
		      || (_wmv9vcm_dll>=fver(9,0,1,369) && _wmv9vcm_dll<fver(9,0,1,3073)))) {
		NN("Security Update for Windows XP (KB2845142)");
		XX(p3+"windowsxp-windowsmedia-kb2845142-x86-enu_560190cc172d1009450366fa48a274f2ae0672a7.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && ((_mpg4ds32_ax >=fver(8,0,0,0) /* WM8 Section */
					     && _mpg4ds32_ax<fver(8,0,0,4504))

					  || (_mp4sds32_ax  >=fver(8,0,0,0) /* V8DME Section */
		                 && _mp4sds32_ax < fver(8,0,0,406))

					  || (_mp4sdmod_dll  >=fver(9,0,0,0) /* WM9 Section */
		                 && _mp4sdmod_dll < fver(9,0,0,4509))

					  || (_mp4sdmod_dll >=fver(10,0,0,0) /* WM10 Section */
		                 && _mp4sdmod_dll < fver(10,0,0,3706))

					  || (_mp4sdecd_dll  >=fver(11,0,5721,0) /* WM11 Section */
		                 && _mp4sdecd_dll < fver(11,0,5721,5274))

					  || *wmp6cdcs || wm9codecs_v1_install  )) {
		NN("Security Update for Windows XP (KB975558)");
		XX(p3+"WindowsXP-WindowsMedia-KB975558-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && ( _unregmp2_exe >= fver(11,0,5721,5145) && _unregmp2_exe < fver(11,0,5721,5235))) {
		NN("Update for Windows Media Player 11 for Windows XP (KB939683)");
		XX(p3+"windowsmedia11-kb939683-x86-enu_17d48a1cb35ad4f1cac39bd758158efe690730b5.exe"+a1);
	}
	if( sp==3 && !kb913800 && (sku & XP_ALL) && _cewmdm_dll>=fver(10,0,3790,3646) && _cewmdm_dll<fver(10,0,3790,3747)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3067903)");   /* WM10 Version */
		XX(p3+"windowsmedia-kb3067903-x86-enu_65c7ef0682d0b3b8a23e579145728ac9d27ea242.exe"+a1);
	}
	if( sp==3 && !kb913800 && (sku & XP_ALL) && _cewmdm_dll>=fver(10,0,3790,3802) && _cewmdm_dll<fver(10,0,3790,4093)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3067903)");   /* WM10L Version */
		XX(p3+"windowsmedia-kb3067903-x86-enu_17cadd59515063d00d9f0d92d023ae1aa2349e4e.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _cewmdm_dll>=fver(11,0,5721,5145) && _cewmdm_dll<fver(11,0,5721,5295)) {
		NN("Security Update for WEPOS and POSReady 2009 (KB3067903)");   /* WM11 Version */
		XX(p3+"windowsmedia-kb3067903-x86-enu_510155e48bfc1c08e7f2023fde514be960fa8fed.exe"+a1);
	}
	if( sp>=1 && (sku & XP_ALL) && _npdsplay_dll>zero && _npdsplay_dll<fver(3,0,2,629)) {
		NN("Security Update for Windows Media Player Plug-in (KB911564)");
		XX(p3+"windowsmedia-kb911564-x86-enu_8f5e0c3d5c50c32200a78dab3ccadb175649858c.exe"+a1);
	}
	if( sp>=2 && !kb913800 && (sku & XP_ALL)
			&& ( kb891122 || _wmvcore_dll >= fver(10,0,0,3802))
		    && ((kb891122 || _msscp_dll   >= fver(10,0,0,3646)) && _msscp_dll<fver(10,0,0,3934)) ) {
		NN("Update for WMDRM-enabled Media Players (KB902344)");
		XX(p3+"WindowsMedia10-KB902344-x86-INTL.exe"+a1);
	}
	if( sp>=2 && (sku & XP_ALL) && _msscp_dll>=fver(11,0,5721,5145) && _msscp_dll<fver(11,0,5721,5201)) {
		NN("Update for Windows Media Format 11 SDK for Windows XP (KB929399)");
		XX(p3+"windowsmedia11-kb929399-v2-x86-intl_50e2bc6b5e79909a0061043e6211600548368431.exe"+a1);
	}

	if( sp==3 && (sku & XP_ALL) && _infocomm_dll>zero && _infocomm_dll<fver(6,0,2600,6018)) {
		NN("Security Update for Windows XP (KB2290570)");
		XX(p3+"WindowsXP-KB2290570-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && ( (_msw3prt_dll >zero && _msw3prt_dll <fver(5,1,2600,3435))
	                             ||  (_win32spl_dll>zero && _win32spl_dll<fver(5,1,2600,3435)) ))
	 ||(sp==3 && (sku & XP_ALL) && ( (_msw3prt_dll> zero && _msw3prt_dll <fver(5,1,2600,5664))
	                             ||  (_win32spl_dll>zero && _win32spl_dll<fver(5,1,2600,5664)) )) ) {
		NN("Security Update for Windows XP (KB953155)");
		XX(p3+"WindowsXP-KB953155-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _httpext_dll>zero && _httpext_dll<fver(6,0,2600,3574))
	 ||(sp==3 && (sku & XP_ALL) && _httpext_dll>zero && _httpext_dll<fver(6,0,2600,5817))) {
		NN("Security Update for Windows XP (KB970483)");
		XX(p3+"WindowsXP-KB970483-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _asp_dll>zero && _asp_dll<fver(5,1,2600,6007)) {
		NN("Security Update for Windows XP (KB2124261)");
		XX(p3+"WindowsXP-KB2124261-x86-ENU.exe"+a1);
	}
	if((sp==2 && (sku & XP_ALL) && _ftpsvc2_dll>zero && _ftpsvc2_dll<fver(6,0,2600,3624))
	 ||(sp==3 && (sku & XP_ALL) && _ftpsvc2_dll>zero && _ftpsvc2_dll<fver(6,0,2600,5875))) {
		NN("Security Update for Windows XP (KB975254)");
		XX(p3+"WindowsXP-KB975254-x86-ENU.exe"+a1);
	}
	if( sp==3 && (sku & XP_ALL) && _fxscover_exe>zero && _fxscover_exe<fver(5,2,2600,6078)) {
		NN("Security Update for Windows XP (KB2491683)");
		XX(p3+"WindowsXP-KB2491683-x86-ENU.exe"+a1);
	}
	if( sp==0 && (sku & XP_ALL) && _smtpsvc_dll>zero && _smtpsvc_dll<fver(6,0,2600,28)) {
		NN("Q313450: Security Update");
		XX(rtm+"Q313450_WXP.exe"+a7);
	}
	if((sp==2 && (sku & XP_ALL) && _smtpsvc_dll>zero && _smtpsvc_dll<fver(6,0,2600,3680))
	 ||(sp==3 && (sku & XP_ALL) && _smtpsvc_dll>zero && _smtpsvc_dll<fver(6,0,2600,5949))) {
		NN("Security Update for Windows XP (KB976323)");
		XX(p3+"WindowsXP-KB976323-x86-ENU.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (
		                  (_magnify_exe  >zero && _magnify_exe  <fver(5,1,2600,3008))
		              ||  (_narrator_exe >zero && _narrator_exe <fver(5,1,2600,3008))
					  ||  (_osk_exe      >zero && _osk_exe      <fver(5,1,2600,3008))
					  ||  (_umandlg_dll  >zero && _umandlg_dll  <fver(5,1,2600,3008))
					  ||  (_utilman_exe  >zero && _utilman_exe  <fver(5,1,2600,3008)) )) {
		NN("February 2007 CardSpace Update for Windows XP (KB925720)");
		XX(p2+"windowsxp-kb925720-x86-enu_b9966c90242454bce1f2edb066141ad45851008f.exe"+a1);
	}
	
	if( sp==2 && *rdp60) {
		NN("Remote Desktop Connection (Terminal Services Client 6.0) for Windows XP (KB925876)");
		XX(mstsc+"windowsxp-kb925876-x86-enu_402612fb3d6636c721ff8d8edc0c4241aa68e477.exe"+a1);
	}
	if( sp>=2 && *rdp61) {
		NN("Remote Desktop Connection (Terminal Services Client 6.1) for Windows XP (KB952155)");
		XX(mstsc+"WindowsXP-KB952155-x86-ENU.exe"+a1);
	}
	if( sp==2 && !(*rdp60 || *rdp61) && ((sku & XP_ALL) && (_mstscax_dll >= fver(5,1,0,0) && _mstscax_dll<=fver(5,2,0,0)))
		              && (
					  /* GDR and QFE */
					      (_mstscax_dll     >zero && _mstscax_dll  <fver(5,1,2600,3581))
					  /*||  (_msrdp_ocx       >zero && _msrdp_ocx    <fver(5,2,3790,4522))*/ )) {
		NN("Security Update for Windows XP (KB958470)");
		XX(p2+"windowsxp-kb958470-x86-enu_887a259c39636eaf1ab9bfec71172203224415cc.exe"+a1);
	}
	if( sp==2 && (sku & XP_ALL) && (
		// Update for RDP 6.0
		(*rdp60 || ( _mstscax_dll >= fver(6,0,6000,0) && _mstscax_dll<fver(6,0,6000,16865)))

		// Update for RDP 6.1
	 || (*rdp61 || ( _mstscax_dll >= fver(6,0,6001,0) && _mstscax_dll<fver(6,0,6002,0))
			&& ( (_aaclient_dll     >zero && _aaclient_dll     <fver(6,0,6001,22443))
			  || (_aaclient_dll_mui >zero && _aaclient_dll_mui <fver(6,0,6001,22403))
			  || (_mstsc_exe        >zero && _mstsc_exe        <fver(6,0,6001,22443))
			  || (_mstsc_exe_mui    >zero && _mstsc_exe_mui    <fver(6,0,6001,22403))
			  || (_mstscax_dll      >zero && _mstscax_dll      <fver(6,0,6001,22443))
			  || (_mstscax_dll_mui  >zero && _mstscax_dll_mui  <fver(6,0,6001,22403))
			  || (_rhttpaa_dll      >zero && _rhttpaa_dll      <fver(6,0,6001,22443))
			  || (_tsgqec_dll       >zero && _tsgqec_dll       <fver(6,0,6001,22443))
			)))) { // Also installs on SP3, but superceded by KB2813345 on that platform
		NN("Security Update for Windows XP (KB956744)");
		XX(p2+"windowsxp-kb956744-x86-enu_3d8a4f0990d13778983b5f2c4dcb982d4d7bfd20.exe"+a1);
	}
	if( sp==3 && *rdp70){
		NN("Remote Desktop Connection 7.0 client update for Windows XP (KB969084)");
		XX(mstsc+"windowsxp-kb969084-x86-enu_153587172bf060ddc9d2c966f1eaaf4f47926f28.exe"+a1);
	}
//	if( sp==3 && (*rdp70 || ((sku & XP_ALL) && (_mstscax_dll >= fver(6,1,7600,16385) && _mstscax_dll<fver(6,1,7601,0))
//		              && (
//					  /* GDR */
//					      (_aaclient_dll    >zero && _aaclient_dll <fver(6,1,7600,16385))
//		              ||  (_mstsc_exe       >zero && _mstsc_exe    <fver(6,1,7600,16722))
//					  ||  (_mstscax_dll     >zero && _mstscax_dll  <fver(6,1,7600,16722))
//					  ||  (_tsgqec_dll      >zero && _tsgqec_dll   <fver(6,1,7600,16385))
//					  /* QFE */
//					  ||  (_aaclient_dll    >fver(6,1,7600,17233) && _aaclient_dll    <fver(6,1,7600,20861))
//		              ||  (_mstsc_exe       >fver(6,1,7600,16722) && _mstsc_exe       <fver(6,1,7600,20861))
//					  ||  (_mstscax_dll     >fver(6,1,7600,17233) && _mstscax_dll     <fver(6,1,7600,20861))
//					  ||  (_tsgqec_dll      >fver(6,1,7600,17233) && _tsgqec_dll      <fver(6,1,7600,20861)) )))) {
//		// KB2483614 is replaced by KB2813347
//		NN("Security Update for Windows XP (KB2483614)");
//		XX(p3+"windowsxp-kb2483614-x86-enu_c7386b821d55b92e56a33280bccc5b406a6e8797.exe"+a1);
//	}
	if( sp==3 && (*rdp70 || ((sku & XP_ALL) && (_mstscax_dll >= fver(6,1,7600,16385) && _mstscax_dll<fver(6,1,7601,0))
		              && (
					  /* GDR */
					      (_aaclient_dll    >zero && _aaclient_dll <fver(6,1,7600,17233))
		              ||  (_mstsc_exe       >zero && _mstsc_exe    <fver(6,1,7600,16722))
					  ||  (_mstscax_dll     >zero && _mstscax_dll  <fver(6,1,7600,17233))
					  ||  (_tsgqec_dll      >zero && _tsgqec_dll   <fver(6,1,7600,17233))
					  ||  (_wksprt_exe      >zero && _wksprt_exe   <fver(6,1,7600,16385))
					  /* QFE */
					  ||  (_aaclient_dll    >fver(6,1,7600,17233) && _aaclient_dll    <fver(6,1,7600,21448))
		              ||  (_mstsc_exe       >fver(6,1,7600,16722) && _mstsc_exe       <fver(6,1,7600,21448))
					  ||  (_mstscax_dll     >fver(6,1,7600,17233) && _mstscax_dll     <fver(6,1,7600,21448))
					  ||  (_mstscax_dll_mui >fver(6,1,7600,16385) && _mstscax_dll_mui <fver(6,1,7600,21448))
					  ||  (_tsgqec_dll      >fver(6,1,7600,17233) && _tsgqec_dll      <fver(6,1,7600,21448))
					  ||  (_wksprt_exe      >fver(6,1,7600,16385) && _wksprt_exe      <fver(6,1,7600,21448)) )))) {
		NN("Security Update for Windows XP (KB2813347)");
		XX(p3+"windowsxp-kb2813347-v2-x86-enu_7f5b07e5464702cdfa24a869d21f55335223069a.exe"+a1);
	}
	if( sp==3 && (!*rdp70 && ((sku & XP_ALL) && (_mstscax_dll >= fver(6,0,6001,0) && _mstscax_dll<=fver(6,0,6002,0))
		              && ( *rdp61
					  /* GDR */
					  ||  (_mstsc_exe       >zero && _mstsc_exe    <fver(6,0,6001,18589))
					  ||  (_mstscax_dll     >zero && _mstscax_dll  <fver(6,0,6001,18589))
					  /* QFE */
					  ||  (_aaclient_dll    >fver(6,0,6001,18000) && _aaclient_dll    <fver(6,0,6001,22840))
		              ||  (_mstsc_exe       >fver(6,0,6001,18589) && _mstsc_exe       <fver(6,0,6001,22840))
					  ||  (_mstscax_dll     >fver(6,0,6001,18926) && _mstscax_dll     <fver(6,0,6001,22840))
					  ||  (_tsgqec_dll      >fver(6,0,6001,18000) && _tsgqec_dll      <fver(6,0,6001,22840)) )))) {
		NN("Security Update for Windows XP (KB2481109)");
		XX(p3+"WindowsXP-KB2481109-x86-ENU.exe"+a1);
	}
	if( sp==3 && (!*rdp70 && ((sku & XP_ALL) && (_mstscax_dll >= fver(6,0,6001,0) && _mstscax_dll<=fver(6,0,6002,0))
		              && ( *rdp61
					  /* GDR */
					  ||  (_mstscax_dll >zero && _mstscax_dll <fver(6,0,6001,18926))
					  /* QFE */
					  ||  (_aaclient_dll    >fver(6,0,6001,18000) && _aaclient_dll    <fver(6,0,6001,23191))
					  ||  (_mstscax_dll     >fver(6,0,6001,18926) && _mstscax_dll     <fver(6,0,6001,23191))
					  ||  (_tsgqec_dll      >fver(6,0,6001,18000) && _tsgqec_dll      <fver(6,0,6001,23191)) )))) {
		NN("Security Update for Windows XP (KB2813345)");
		XX(p3+"WindowsXP-KB2813345-x86-ENU.exe"+a1);
	}

	if( sp>=2 && (*imapi2 || 
		( sp==2 && (sku & XP_ALL) && (_imapi2_dll >zero && _imapi2fs_dll >zero)
		&& ((_cdrom_sys   <fver(5,1,2600,3364))
		||  (_imapi2_dll  <fver(5,1,2600,3364))
		||  (_imapi2fs_dll<fver(5,1,2600,3364)))) ||
		( sp==3 && (sku & XP_ALL) && (_imapi2_dll >zero && _imapi2fs_dll >zero)
		&& ((_cdrom_sys   <fver(5,1,2600,5593))
		||  (_imapi2_dll  <fver(5,1,2600,5593))
		||  (_imapi2fs_dll<fver(5,1,2600,5593)))) )) {
		NN("Image Mastering API v2.0 (IMAPIv2.0) for Windows XP (KB932716)");
		XX(p3+"WindowsXP-KB932716-v2-x86-ENU.exe"+a1);
	}
	if( sp>=2 && (*imapi2 || (   (sku & XP_ALL)
		&& (_imapi2_dll >zero && _imapi2fs_dll >zero)
		&& (
		   (_cdrom_sys   <fver(5,1,2600,3126))
		|| (_imapi2_dll  <fver(6,0,6001,18164))
		|| (_imapi2fs_dll<fver(6,0,6001,18164)) )))) {
		NN("Windows Feature Pack for Storage (32-bit) - IMAPI update for Blu-Ray (KB952011)");
		XX(p3+"IMAPI_XP_SRV2003_x86.exe"+a1);
	}
	if( sp>=2 && (*smartcard || (   (sku & XP_ALL)
		            && (_winusbcoinstaller_dll >zero && _wudfupdate_01007_dll >zero
					     && _wudfusbcciddriver_dll >zero)
		            && (
						(_WdfCoInstaller01007_dll <fver(1,7,6001,18000))
					||  (_winusbcoinstaller_dll <fver(6,0,5841,16388))
		            ||  (_wudfupdate_01007_dll  <fver(6,0,6001,18000))
					||  (_wudfusbcciddriver_dll <fver(6,0,6001,18158)) )))) {
		NN("Windows Feature Pack for Storage (32-bit) - Smart Card driver (KB952013)");
		XX(p3+"SmartCard_XP_x86.exe"+a1);
	}
	if( sp>=2 && (*wsearch4 || (   (sku & XP_ALL)
		            && (_searchfilterhost_exe >=fver(7,0,6001,16503) && _searchindexer_exe >=fver(7,0,6001,16503)
					     && _searchprotocolhost_exe >=fver(7,0,6001,16503))
		            && (
						(_dbres_dll                <fver(7,0,6001,16503))
		            ||  (_dbres_dll_mui            <fver(7,0,6001,16503))
					||  (_dbsetup_dll              <fver(7,0,6001,16503))
					||  (_deskbar_dll              <fver(7,0,6001,16503))
					||  (_mapine_dll               <fver(7,0,6001,16503))
					||  (_mapineres_dll            <fver(7,0,6001,16503))
					||  (_mapineres_dll_mui        <fver(7,0,6001,16503))
					||  (_msnldl_dll               <fver(7,0,6001,16503))
					||  (_msnldlps_dll             <fver(7,0,6001,16503))
					||  (_msnlext_dll              <fver(7,0,6001,16503))
					||  (_msnlextres_dll           <fver(7,0,6001,16503))
					||  (_msnlextres_dll_mui       <fver(7,0,6001,16503))
					||  (_msnlnamespacemgr_dll     <fver(7,0,6001,16503))
					||  (_msnlred_dll              <fver(7,0,6001,16503))
					||  (_msscb_dll                <fver(7,0,6001,16503))
					||  (_msscntrs_dll             <fver(7,0,6001,16503))
					||  (_msshooks_dll             <fver(7,0,6001,16503))
					||  (_msshsq_dll               <fver(7,0,6001,16503))
					||  (_mssitlb_dll              <fver(7,0,6001,16503))
					||  (_mssph_dll                <fver(7,0,6001,16503))
					||  (_mssph_dll_mui            <fver(7,0,6001,16503))
					||  (_mssphtb_dll              <fver(7,0,6001,16503))
					||  (_mssphtb_dll_mui          <fver(7,0,6001,16503))
					||  (_mssprxy_dll              <fver(7,0,6001,16503))
					||  (_mssrch_dll               <fver(7,0,6001,16503))
					||  (_mssrch_dll_mui           <fver(7,0,6001,16503))
					||  (_msstrc_dll               <fver(7,0,6001,16503))
					||  (_oeph_dll                 <fver(7,0,6001,16503))
					||  (_oephres_dll              <fver(7,0,6001,16503))
					||  (_oephres_dll_mui          <fver(7,0,6001,16503))
					||  (_propdefs_dll             <fver(7,0,6001,16503))
					||  (_propsys_dll              <fver(7,0,6001,16503))
					||  (_propsys_dll_mui          <fver(7,0,6001,16503))
					||  (_searchfilterhost_exe     <fver(7,0,6001,16503))
					||  (_searchindexer_exe        <fver(7,0,6001,16503))
					||  (_searchindexer_exe_mui    <fver(7,0,6001,16503))
					||  (_searchprotocolhost_exe   <fver(7,0,6001,16503))
					||  (_srchadmin_dll            <fver(7,0,6001,16503))
					||  (_srchadmin_dll_mui        <fver(7,0,6001,16503))
					||  (_tquery_dll               <fver(7,0,6001,16503))
					||  (_tquery_dll_mui           <fver(7,0,6001,16503))
					||  (_unccplext_dll            <fver(7,0,6001,16503))
					||  (_uncdms_dll               <fver(7,0,6001,16503))
					||  (_uncne_dll                <fver(7,0,6001,16503))
					||  (_uncph_dll                <fver(7,0,6001,16503))
					||  (_uncres_dll               <fver(7,0,6001,16503))
					||  (_uncres_dll_mui           <fver(7,0,6001,16503))
					||  (_wdsmkttools_dll          <fver(7,0,6001,16503))
					||  (_wdsshell_dll             <fver(7,0,6001,16503))
					||  (_wdsview_dll              <fver(7,0,6001,16503))
					||  (_windowssearch_exe        <fver(7,0,6001,16503))
					||  (_windowssearchres_dll     <fver(7,0,6001,16503))
					||  (_windowssearchres_dll_mui <fver(7,0,6001,16503))
					||  (_wordwheel_dll            <fver(7,0,6001,16503))
					||  (_wwres_dll                <fver(7,0,6001,16503))
					||  (_wwres_dll_mui            <fver(7,0,6001,16503))
					||  (_xppreviewproxy_dll       <fver(7,0,6001,16503))
					||  (_xmlfilter_dll            <fver(2006,0,6001,16503))
					||  (_rtffilt_dll              <fver(2006,0,6001,16503)) )))) {
		NN("Windows Search 4.0 for Windows XP (KB940157)");
		XX(p3+"WindowsSearch-KB940157-XP-x86-enu.exe"+a4);
	}
	if( sp>=2 && (*wsearch4 || ((sku & XP_ALL) && ((   _searchfilterhost_exe >=fver(7,0,6001,16503)
		                                 && _searchindexer_exe >=fver(7,0,6001,16503)
					                     && _searchprotocolhost_exe >=fver(7,0,6001,16503))
							|| (regQueryValue(L"SOFTWARE\\Microsoft\\Windows Search",L"CurrentVersion",&status)==L"04.00.6001.503"))
		              && ((_mapine_dll           <fver(7,0,6001,18260))
		              ||  (_msnlext_dll          <fver(7,0,6001,18260))
					  ||  (_msnlnamespacemgr_dll <fver(7,0,6001,18260))
					  ||  (_mssph_dll            <fver(7,0,6001,18260)))))) {
		// KB963093: This security update resolves a privately reported vulnerability in Windows
		// Search. The vulnerability could allow information disclosure if a user performs a
		// search that returns a specially crafted file as the first result or if the user
		// previews a specially crafted file from the search results.
		NN("Security Update for Windows XP (KB963093)");
		XX(p3+"WindowsServer2003.WindowsXP-KB963093-x86-ENU.exe"+a1);
	}

	if( sp>=0 && !(sku & XP_TABLET) && (*jview || (_jntview_exe>zero && _jntview_exe<fver(1,5,2315,3)))) {
		NN("Microsoft Windows Journal Viewer 1.5");
		if(_msiexec_exe  <fver(3,0,0,0)) {
			XX("\"Extras\\Journal Viewer 1.5\\JournalViewer1.5.exe\" /Q /R:N /C:\"msiexec /i \"\"Microsoft Windows Journal Viewer.msi\"\" /qb\"");
		} else {
			XX("\"Extras\\Journal Viewer 1.5\\JournalViewer1.5.exe\" /Q /R:N /C:\"msiexec /i \"\"Microsoft Windows Journal Viewer.msi\"\" /passive\"");
		}
	}
	if( sp==2 && (sku & XP_TABLET) && (
		                  (_ContextTagger_dll >zero && _ContextTagger_dll <fver(1,7,2600,2663))
		              ||  (_TabTip_exe        >zero && _TabTip_exe        <fver(1,7,2600,2663))
					  ||  (_TCServer_exe      >zero && _TCServer_exe      <fver(1,7,2600,2663))
					  ||  (_TipLibrary_dll    >zero && _TipLibrary_dll    <fver(1,7,2600,2657))
					  ||  (_tiptsf_dll        >zero && _tiptsf_dll        <fver(1,7,2600,2663)) )) {
		NN("Update for Windows XP Tablet PC Edition 2005 (KB895953)");
		XX(p2+"windowsxp-kb895953-v4-x86-enu_9e8006f0a1303efca8503b73f9a4243cfba236ff.exe"+a1);
	}
	/*if( sp==3 && (sku & XP_TABLET) && ( // KB2660649 is replaced by KB2835364
		                  (_jntfiltr_dll >zero && _jntfiltr_dll <fver(1,7,2600,6189))
		              ||  (_jnwdrv_dll   >zero && _jnwdrv_dll   <fver(0,3,7,0))
					  ||  (_nbdoc_dll    >zero && _nbdoc_dll    <fver(1,7,2600,6189)) )) {
		NN("Security Update for Windows XP (KB2660649)");
		XX(p+"windowsxp-kb2660649-x86-enu_689f93b51ce8325d1947ae0d40a3d41ceedcb4b0.exe"+a1);
	}*/
	if( sp==3 && (sku & XP_TABLET) && (
		                  (_jntfiltr_dll >zero && _jntfiltr_dll <fver(1,7,2600,6379))
		              ||  (_jnwdrv_dll   >zero && _jnwdrv_dll   <fver(0,3,7,0))
					  ||  (_nbdoc_dll    >zero && _nbdoc_dll    <fver(1,7,2600,6379)) )) {
		NN("Security Update for Windows XP (KB2835364)");
		XX(p3+"windowsxp-kb2835364-x86-enu_b055260207a810f44e7d68c2b93a151d6f8d9691.exe"+a1);
	}
	if( sp>=2 && *pshell1) {
		NN("Windows PowerShell 1.0 English-Language Installation Package for Windows XP (KB926139)");
		XX(p2+"WindowsXP-KB926139-v2-x86-ENU.exe"+a1);
	}
	if( sp==3 && *pshell2) {
		NN("Windows Powershell 2.0 and WinRM 2.0 for Windows XP and Windows Embedded (KB968930)");
		XX(p3+"WindowsXP-KB968930-x86-ENG.exe"+a1);
	}
	if( sp>=2 && (*bitlocker || (_BitLockerToGo_exe>zero && _BitLockerToGo_exe<fver(6,1,7600,16385)))) {
		NN("BitLocker To Go Reader (KB970401)");
		XX(p3+"KB970401-x86.exe"+a1);
	}
	if( sp>=0 && (*winrms ||   (     (_msdrm_dll              >zero && _msdrm_dll              <fver(5,2,3790,433))
		              ||  (_RmActivate_exe         >zero && _RmActivate_exe         <fver(6,0,6406,0))
					  ||  (_RmActivate_isv_exe     >zero && _RmActivate_isv_exe     <fver(6,0,6406,0))
					  ||  (_RmActivate_ssp_exe     >zero && _RmActivate_ssp_exe     <fver(6,0,6406,0))
					  ||  (_RmActivate_ssp_isv_exe >zero && _RmActivate_ssp_isv_exe <fver(6,0,6406,0))
					  ||  (_SecProc_dll            >zero && _SecProc_dll            <fver(6,0,6406,0))
					  ||  (_SecProc_isv_dll        >zero && _SecProc_isv_dll        <fver(6,0,6406,0))
					  ||  (_SecProc_ssp_dll        >zero && _SecProc_ssp_dll        <fver(6,0,6406,0))
					  ||  (_SecProc_ssp_isv_dll    >zero && _SecProc_ssp_isv_dll    <fver(6,0,6406,0))))) {
		NN("Rights Management Services Client with Service Pack 2 for Windows XP (KB979099)");
		if(_msiexec_exe  <fver(3,0,0,0)) {
			XX(sw+p3+"windowsrightsmanagementservicessp2-kb979099-client-x86-enu_c57a952c1c55300114000b769e6f97e8a44322e3.exe -override 1 /I MsDrmClient.msi REBOOT=ReallySuppress /qb -override 2 /I RmClientBackCompat.msi REBOOT=ReallySuppress /qb");
		} else {
			XX(sw+p3+"windowsrightsmanagementservicessp2-kb979099-client-x86-enu_c57a952c1c55300114000b769e6f97e8a44322e3.exe -override 1 /I MsDrmClient.msi REBOOT=ReallySuppress /passive -override 2 /I RmClientBackCompat.msi REBOOT=ReallySuppress /passive");
		}
	}
	if( sp>=0 && (*msxml4 || ((sku & XP_ALL) && (
		                  (_msxml4_dll  >zero && _msxml4_dll  <fver(4,30,2117,0))
					  ||  (_msxml4r_dll >zero && _msxml4r_dll <fver(4,30,2100,0)))))) {
		NN("Security Update for Microsoft XML Core Services 4.0 Service Pack 3 (KB2758694)");
		if(_msiexec_exe  <fver(3,0,0,0)) {
			XX(p3+"msxml4-kb2758694-enu_24abccbcceaf5bea9c3e34ff1f64c2aa3d57e308.exe REBOOT=ReallySuppress /qb");
		} else {
			XX(p3+"msxml4-kb2758694-enu_24abccbcceaf5bea9c3e34ff1f64c2aa3d57e308.exe"+a3);
		}
	}
	if( sp==0 && (sku & XP_ALL) && (
	    (_msxml4_dll  >zero && _msxml4_dll  <fver(4,0,9406,0))
	 || (_msxml4r_dll >zero && _msxml4r_dll <fver(4,0,9406,0))
	 || ((_msxml4_dll>zero || _msxml4r_dll>zero || *msxml4) && _WINHTTP5_DLL<fver(5,0,2613,0)) )){
		NN("Security Update, February 13, 2002 (MSXML 4.0)");
		XX(sw+rtm+"q317244_21ecede68f96753e7cb328ef7473711f9f3b4115.exe"+a2);
	}
	/*if( sp>=2 && (*msxml6 || ((sku & XP_ALL) && ( 
		                  ( _msxml6_dll  >zero && _msxml6_dll  <fver(6,10,1129,0))
					  ||  ( _msxml6r_dll >zero && _msxml6r_dll <fver(6,0,3883,0)))))) {
		NN("Update for Microsoft Core XML Services (MSXML) 6.0 Service Pack 1 (KB934268)");
		XX(p+"msxml6sp1-kb934268-enu-x86_dac3db6bec59d839c713a6a2d1e169fd7dec1e25.exe"+a3);
	}
	if( sp>=2 && (*msxml6 || ((sku & XP_ALL) && ( 
		                  ( _msxml6_dll  >zero && _msxml6_dll  <fver(6,20,1099,0))))) {
		NN("Security Update for Microsoft XML Core Services 6.0 Service Pack 2 (KB954459)");
		NN(p+"windowsxp-kb954459-x86-enu_a409813d4541734043c419d399d20a463b52f8e1.exe"+a1);
	}*/
	if( sp>=0 && (_msiexec_exe >=fver(3,1,4000,1823)) && (*msxml6 || ((sku & XP_ALL) && ( 
		                  ( _msxml6_dll  >zero && _msxml6_dll  <fver(6,20,2003,0))
					  ||  ( _msxml6r_dll >zero && _msxml6r_dll <fver(6,0,3883,0)))))) {
	// To install MSXML 6.0 you must have Microsoft Windows(R) Installer 3.1 (MSI 3.1) or higher on your computer.
		NN("Update for Microsoft XML Core Services 6.0 Service Pack 2 (KB973686)");
		XX(p3+"msxml6-kb973686-enu-x86_e139664a78bc2806cf0c5bcf0bedec7ea073c3b1.exe REBOOT=ReallySuppress /passive /norestart");
	}

	if( sp>=2 && ((sku & XP_ALL) && _agcore_debug_dll>zero
		              && ( _slup_exe>zero || _sllauncher_exe>zero || _agcore_dll>zero )
		              &&   _sllauncher_exe<fver(5,1,50918,0) )) {
		NN("Update for Microsoft Silverlight (KB4481252)");
		XX(sw+p+"silverlight_developer_16882b33f82a87224e6b6dd1150ca5665e6f309d.exe"+a2l);
	}
	else if( sp>=2 && (*silverlight || ((sku & XP_ALL) && ( _slup_exe>zero || _sllauncher_exe>zero || _agcore_dll>zero )
					  &&   _sllauncher_exe<fver(5,1,50918,0)))) {
		NN("Update for Microsoft Silverlight (KB4481252)");
		XX(sw+p+"silverlight_a6ea175976edbdeeddfbfb65e64d9d1cfded84a1.exe"+a2l);
	}
	

	if( sp>=2 && *msi45 ) {
		NN("Microsoft Windows Installer 4.5 (KB942288)");
		XX(p3+"WindowsXP-KB942288-v3-x86.exe"+a1);
	}
	if( !*msi45 && (sku & XP_ALL) && (  _msi_dll      <fver(3,1,4000,2435)
					  ||    _msiexec_exe  <fver(3,1,4000,1823)
					  ||    _msihnd_dll   <fver(3,1,4000,1823)
					  ||    _msimsg_dll   <fver(3,1,4000,1823)
					  ||    _msisip_dll   <fver(3,1,4000,1823) )) {
		NN("Microsoft Windows Installer 3.1");
		XX(p+"WindowsInstaller-KB893803-v2-x86.exe"+a1);
	}
	if( sp==2 && !*msi45 && (sku & XP_ALL) && (  _msi_dll <fver(3,1,4000,4039) )) {
		NN("Update for Windows XP (KB927891)");
		XX(p2+"windowsxp-kb927891-v3-x86-enu_831fc2b9075b0a490adf15d2c5452e01e6feaa17.exe"+a1);
	}
	if( sp==3 && (sku & XPE_WES2009) && ((_msi_dll>=fver(4,5,0,0) && _msi_dll<fver(4,5,6002,24556))
		              ||  (_msiexec_exe>=fver(4,5,0,0) && _msiexec_exe<fver(4,5,6002,24433))
					  ||  (_msihnd_dll>=fver(4,5,0,0) && _msihnd_dll<fver(4,5,6002,23415))
					  ||  *msi45 )) {
		NN("2019-02 Security Update for WES09 and POSReady 2009 for x86-based Systems (KB4486464)");
		XX(p3+"windowsxp-kb4486464-x86-embedded-enu_ba779c434ec819e2932b7f70422b74c526ae244d.exe"+a1);
	}
	if( sp==3 && (sku & XPE_POSREADY2009) && ((_msi_dll>=fver(4,5,0,0) && _msi_dll<fver(4,5,6003,20488))
		              ||  (_msiexec_exe>=fver(4,5,0,0) && _msiexec_exe<fver(4,5,6002,24433))
					  ||  (_msihnd_dll>=fver(4,5,0,0) && _msihnd_dll<fver(4,5,6002,23415))
					  ||  *msi45 )) {
		// Out of the box, update KB4494528 messes up Windows Installer.
		// Fixing it requires manually re-registering a few MSI DLLs.
		NN("2019-04 Security Update for POSReady 2009 for x86-based Systems (KB4494528)");
		XX(p3+"windowsxp-kb4494528-x86-embedded-enu_77ead5923f0753b90d935f2d6546886b063a6fe1.exe"+a1
			+"\n"
			+"regsvr32.exe /s %SystemRoot%\\system32\\MSI.DLL\n"
			+"regsvr32.exe /s %SystemRoot%\\system32\\MSIHND.DLL\n"
			+"regsvr32.exe /s %SystemRoot%\\system32\\MSISIP.DLL");
	}
	if( sp<2 && (sku & XP_ALL) && name->size() > 1 ) {
		NN("QChain.exe command-line tool (Q815062)");
		XX("Extras\\QChain\\qchain.exe qchain.log");
	}
}
