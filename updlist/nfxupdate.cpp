#include "stdafx.h"
#include "nfxupdate.h"
#include "fver.h"
#include "winsku.h"
#include "registry.h"
#include "certificates.h"
#include <vector>
#include <string>

#define NN name->push_back
#define XX exe->push_back

using namespace std;

void nfxInstallation(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
					 int* nfxdata, bool* nfxinstall, std::vector<std::string> *notifications) {
	int status=0;

	// Index the nfxdata array;
	int* v_nfx10=nfxdata+0;
	int* v_nfx10mce=nfxdata+1;
	int* v_nfx11=nfxdata+2;
	int* v_nfx20=nfxdata+3;
	int* v_nfx30=nfxdata+4;
	int* v_nfx35=nfxdata+5;
	int* v_nfx40c=nfxdata+6;
	int* v_nfx40f=nfxdata+7;

	// Index the nfxinstall array;
	bool* i_nfx10=nfxinstall+0;
	bool* i_nfx10mce=nfxinstall+1;
	bool* i_nfx11=nfxinstall+2;
	bool* i_nfx20=nfxinstall+3;
	bool* i_nfx30=nfxinstall+4;
	bool* i_nfx35=nfxinstall+5;
	bool* i_nfx40c=nfxinstall+6;
	bool* i_nfx40f=nfxinstall+7;

	bool NFX40=(*v_nfx40c>=0 || *v_nfx40f>=0);
	bool NFX40_EXT=(*v_nfx40c>=0 && *v_nfx40f>=0);

	const std::string sw="start /wait ";
	const std::string p="Packages\\";
	const std::string p1=p+"SP1\\";
	const std::string p2=p+"SP2\\";
	const std::string p3=p+"SP3\\";
	const std::string a1=" /passive /norestart /overwriteoem /nobackup";
	const std::string np="NetFx\\";
	const std::string n3=" /Q";
	const std::string n6=" /passive /norestart";

	// System paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);

	std::wstring System32 = SystemRoot+L"\\System32";
	std::wstring Framework = SystemRoot+L"\\Microsoft.NET\\Framework\\";
	std::wstring p_nfx10 = Framework+L"v1.0.3705";

	fver zero = fver();

	fver _10_aspnet_isapi_dll = getFileVer(p_nfx10+L"\\aspnet_isapi.dll",&status);
	fver _10_aspnet_wp_exe    = getFileVer(p_nfx10+L"\\aspnet_wp.exe",&status);
	fver _10_IEExec_exe       = getFileVer(p_nfx10+L"\\IEExec.exe",&status);
	fver _10_mscorie_dll      = getFileVer(p_nfx10+L"\\mscorie.dll",&status);
	fver _10_mscorld_dll      = getFileVer(p_nfx10+L"\\mscorld.dll",&status);
	fver _10_mscorlib_dll     = getFileVer(p_nfx10+L"\\mscorlib.dll",&status);
	fver _10_mscorsvr_dll     = getFileVer(p_nfx10+L"\\mscorsvr.dll",&status);
	fver _10_mscorwks_dll     = getFileVer(p_nfx10+L"\\mscorwks.dll",&status);
	fver _10_System_Web_dll   = getFileVer(p_nfx10+L"\\System.Web.dll",&status);

	fver _photometadatahandler_dll = getFileVer(System32+L"\\photometadatahandler.dll",&status);
	fver _windowscodecs_dll = getFileVer(System32+L"\\windowscodecs.dll",&status);
	fver _windowscodecsext_dll = getFileVer(System32+L"\\windowscodecsext.dll",&status);
	fver _wmphoto_dll  = getFileVer(System32+L"\\wmphoto.dll",&status);

	bool install_nfx10sp3=false;
	bool wic_error=false;

	// .NET Framework 1.0 Install
	if( sp>=0 && *v_nfx10mce<0 && !NFX40 && *i_nfx10 && *v_nfx10<0) {
		NN("Microsoft .NET Framework 1.0");
		XX(np+"dotnetf10.exe /Q /C:\"install.exe /q /l\"");
	}
	if( sp>=0 && *v_nfx10mce<0 && (*i_nfx10 || (*v_nfx10>=0 && *v_nfx10<3))){
		install_nfx10sp3=true;
		if(!NFX40) {
			NN("Microsoft .NET Framework 1.0 Service Pack 3, English Version (KB867461)");
			XX(p3+"NDP1.0sp3-KB867461-X86-Enu.exe"+n3);
		} else {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Update KB867461 for .NET Framework 1.0 is")
										   +"|missing, but cannot be installed because"
										   +"|.NET Framework 4.0 is installed."
										   +"| "
										   +"|Please uninstall .NET Framework 4.0 and try"
										   +"|again.");
		}
	}
	if( sp==1 && *v_nfx10mce<0 && ((*i_nfx10 || install_nfx10sp3 || *v_nfx10>=3)) && (
		// According to online docs, KB886906 is intended for use on SP1 or SP2
		   ( _10_System_Web_dll  <fver(1,0,3705,6021)) )) {
		NN("Security Update for Microsoft .NET Framework, Version 1.0 SP3, English (KB886906)");
		XX(p1+"ndp1.0sp3-kb886906-x86-enu_704fc2f79b62d7116b5bdbcaeb74e2039e2c7d4f.exe"+n3);
	}
	if( sp>=2 && *v_nfx10mce<0 && ((*i_nfx10 || install_nfx10sp3 || *v_nfx10>=3)) && (
		// According to online docs, KB928367 is officially supported on SP2+
		   ( _10_aspnet_isapi_dll<fver(1,0,3705,6060))
		|| ( _10_aspnet_wp_exe   <fver(1,0,3705,6060))
		|| ( _10_IEExec_exe      <fver(1,0,3705,6060))
		|| ( _10_mscorie_dll     <fver(1,0,3705,6060))
		|| ( _10_mscorld_dll     <fver(1,0,3705,6060))
		|| ( _10_mscorlib_dll    <fver(1,0,3705,6060))
		|| ( _10_mscorsvr_dll    <fver(1,0,3705,6060))
		|| ( _10_mscorwks_dll    <fver(1,0,3705,6060))
		|| ( _10_System_Web_dll  <fver(1,0,3705,6060)) )) {
		if(!NFX40) {
			NN("Security Update for Microsoft .NET Framework, Version 1.0 Service Pack 3 (KB928367)");
			XX(p3+"NDP1.0sp3-KB928367-X86-Enu.exe"+n3);
		} else {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Update KB928367 for .NET Framework 1.0 is")
										   +"|missing, but cannot be installed because"
										   +"|.NET Framework 4.0 is installed."
										   +"| "
										   +"|Please uninstall .NET Framework 4.0 and try"
										   +"|again.");
		}
	}
	if( sp>=0 && *i_nfx11 && *v_nfx11<0) {
		NN("Microsoft .NET Framework 1.1");
		XX(np+"dotnetf11.exe /q:a /c:\"install.exe /qb /l\"");
	}
	if( sp>=0 && *i_nfx11 && *v_nfx11<1){
		// i_nfx11 means NFX11 is flagged to be upgraded (e.g. install SP1)
		// v_nfx11 is the current NFX11 SP level
		NN("Microsoft .NET Framework 1.1 Service Pack 1 (KB867460)");
		XX(p3+"NDP1.1sp1-KB867460-X86.exe"+n3);
	}
	if( sp>=2 && (*i_nfx20 || *i_nfx30 || *i_nfx35)) {
		NN("Microsoft .NET Framework 3.5 Service Pack 1 and .NET Framework 3.5 Family Update for .NET versions 2.0 through 3.5 (KB951847) x86");
		XX(np+"dotnetfx35_x86.exe"+n6);
	}

	if( ( sp==3 && (*i_nfx40c || *i_nfx40f)) && ( 
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
		notifications->push_back(std::string(".NET Framework 4.0 cannot be installed because")
			                               +"|the Windows Imaging Component is missing."
										   +"| "
							               +"|Windows Imaging Component cannot be installed"
										   +"|because some newer versions of parts of it are"
										   +"|already present on the system."
										   +"| "
										   +"|Maybe it should've been installed before SP3?");
			wic_error=true;
		}
	}

	if( sp==3 && (*i_nfx40c || *i_nfx40f) && !wic_error) {
		//NN("Microsoft .NET Framework 4 Client Profile for Windows XP x86 (KB982670)");
		NN("Microsoft .NET Framework 4 Full Profile for Windows XP x86");
		XX(np+"dotNetFx40_Full_x86.exe /passive /showfinalerror /promptrestart");
	}

}

void nfxUpdates(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
				int* nfxdata, bool* nfxinstall, std::vector<std::string> *notifications) {

	int status=0;

	// Index the nfxdata array;
	int* v_nfx10=nfxdata+0;
	int* v_nfx10mce=nfxdata+1;
	int* v_nfx11=nfxdata+2;
	int* v_nfx20=nfxdata+3;
	int* v_nfx30=nfxdata+4;
	int* v_nfx35=nfxdata+5;
	int* v_nfx40c=nfxdata+6;
	int* v_nfx40f=nfxdata+7;

	// Index the nfxinstall array;
	bool* i_nfx10=nfxinstall+0;
	bool* i_nfx10mce=nfxinstall+1;
	bool* i_nfx11=nfxinstall+2;
	bool* i_nfx20=nfxinstall+3;
	bool* i_nfx30=nfxinstall+4;
	bool* i_nfx35=nfxinstall+5;
	bool* i_nfx40c=nfxinstall+6;
	bool* i_nfx40f=nfxinstall+7;

	bool NFX40=(*v_nfx40c>=0 || *v_nfx40f>=0);
	bool NFX40_EXT=(*v_nfx40c>=0 && *v_nfx40f>=0);

	const std::string sw="start /wait ";
	const std::string p="Packages\\";
	const std::string p1=p+"SP1\\";
	const std::string p2=p+"SP2\\";
	const std::string p3=p+"SP3\\";
	const std::string np="NetFx\\";
	const std::string n1=" /passive /norestart /overwriteoem /nobackup";
	const std::string n3=" /Q";
	const std::string n6=" /passive /norestart";

	// System paths
	int CannotFindSystemRoot=0;
	int CannotFindProgramFiles=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&CannotFindProgramFiles);

	std::wstring System32 = SystemRoot+L"\\System32";
	std::wstring Framework = SystemRoot+L"\\Microsoft.NET\\Framework\\";
	std::wstring p_nfx10 = Framework+L"v1.0.3705";
	std::wstring p_nfx11 = Framework+L"v1.1.4322";
	std::wstring p_nfx20 = Framework+L"v2.0.50727";
	std::wstring p_nfx30ra = ProgramFiles+L"\\Reference Assemblies\\Microsoft\\Framework\\v3.0";
	std::wstring p_nfx30wpf = Framework+L"v3.0\\WPF";
	std::wstring p_nfx30xps = System32+L"\\XPSViewer";
	std::wstring p_nfx30wcf = Framework+L"v3.0\\Windows Communication Foundation";
	std::wstring p_nfx35ra = ProgramFiles+L"\\Reference Assemblies\\Microsoft\\Framework\\v3.5";
	std::wstring p_nfx35 = Framework+L"v3.5";
	std::wstring p_nfx40 = Framework+L"v4.0.30319";
	std::wstring p_nfx40wpf = p_nfx40 + L"\\WPF";

	fver zero = fver();

	int _ntprint_cat = (int)getFileSize(System32+L"\\CatRoot\\{F750E6C3-38EE-11D1-85E5-00C04FC295EE}\\ntprint.cat");

	fver _10_aspnet_isapi_dll = getFileVer(p_nfx10+L"\\aspnet_isapi.dll",&status);
	fver _10_aspnet_state_exe = getFileVer(p_nfx10+L"\\aspnet_state.exe",&status);
	fver _10_aspnet_wp_exe    = getFileVer(p_nfx10+L"\\aspnet_wp.exe",&status);
	fver _10_gacutil_exe      = getFileVer(p_nfx10+L"\\gacutil.exe",&status);
	fver _10_IEExec_exe       = getFileVer(p_nfx10+L"\\IEExec.exe",&status);
	fver _10_mscorie_dll      = getFileVer(p_nfx10+L"\\mscorie.dll",&status);
	fver _10_mscorjit_dll     = getFileVer(p_nfx10+L"\\mscorjit.dll",&status);
	fver _10_mscorld_dll      = getFileVer(p_nfx10+L"\\mscorld.dll",&status);
	fver _10_mscorlib_dll     = getFileVer(p_nfx10+L"\\mscorlib.dll",&status);
	fver _10_mscorsvr_dll     = getFileVer(p_nfx10+L"\\mscorsvr.dll",&status);
	fver _10_mscorwks_dll     = getFileVer(p_nfx10+L"\\mscorwks.dll",&status);
	fver _10_netfxupdate_exe  = getFileVer(p_nfx10+L"\\netfxupdate.exe",&status);
	fver _10_SetRegNI_exe     = getFileVer(p_nfx10+L"\\SetRegNI.exe",&status);
	fver _10_system_dll       = getFileVer(p_nfx10+L"\\system.dll",&status);
	fver _10_System_Drawing_dll = getFileVer(p_nfx10+L"\\System.Drawing.dll",&status);
	fver _10_System_Security_dll = getFileVer(p_nfx10+L"\\System.Security.dll",&status);
	fver _10_System_Web_dll   = getFileVer(p_nfx10+L"\\System.Web.dll",&status);
	fver _10_System_Windows_Forms_dll = getFileVer(p_nfx10+L"\\System.Windows.Forms.dll",&status);
	fver _10_ToGac_exe        = getFileVer(p_nfx10+L"\\ToGac.exe",&status);

	fver _11_aspnet_filter_dll        = getFileVer(p_nfx11+L"\\aspnet_filter.dll",&status);
	fver _11_aspnet_isapi_dll         = getFileVer(p_nfx11+L"\\aspnet_isapi.dll",&status);
	fver _11_aspnet_wp_exe            = getFileVer(p_nfx11+L"\\aspnet_wp.exe",&status);
	fver _11_CORPerfMonExt_dll        = getFileVer(p_nfx11+L"\\CORPerfMonExt.dll",&status);
	fver _11_mscorie_dll              = getFileVer(p_nfx11+L"\\mscorie.dll",&status);
	fver _11_mscorjit_dll             = getFileVer(p_nfx11+L"\\mscorjit.dll",&status);
	fver _11_mscorld_dll              = getFileVer(p_nfx11+L"\\mscorld.dll",&status);
	fver _11_mscorlib_dll             = getFileVer(p_nfx11+L"\\mscorlib.dll",&status);
	fver _11_mscorsn_dll              = getFileVer(p_nfx11+L"\\mscorsn.dll",&status);
	fver _11_mscorsvr_dll             = getFileVer(p_nfx11+L"\\mscorsvr.dll",&status);
	fver _11_System_dll               = getFileVer(p_nfx11+L"\\System.dll",&status);
	fver _11_System_Drawing_dll       = getFileVer(p_nfx11+L"\\System.Drawing.dll",&status);
	fver _11_System_Security_dll      = getFileVer(p_nfx11+L"\\System.Security.dll",&status);
	fver _11_System_Windows_Forms_dll = getFileVer(p_nfx11+L"\\System.Windows.Forms.dll",&status);
	fver _11_mscorwks_dll             = getFileVer(p_nfx11+L"\\mscorwks.dll",&status);
	fver _11_System_Web_dll           = getFileVer(p_nfx11+L"\\System.Web.dll",&status);
	fver _mscoree_dll                 = getFileVer(System32+L"\\mscoree.dll",&status);

	fver _20_AppLaunch_exe               = getFileVer(p_nfx20+L"\\AppLaunch.exe",&status);
	fver _20_aspnet_filter_dll           = getFileVer(p_nfx20+L"\\aspnet_filter.dll",&status);
	fver _20_aspnet_isapi_dll            = getFileVer(p_nfx20+L"\\aspnet_isapi.dll",&status);
	fver _20_Aspnet_perf_dll             = getFileVer(p_nfx20+L"\\Aspnet_perf.dll",&status);
	fver _20_aspnet_rc_dll               = getFileVer(p_nfx20+L"\\aspnet_rc.dll",&status);
	fver _20_aspnet_regiis_exe           = getFileVer(p_nfx20+L"\\aspnet_regiis.exe",&status);
	fver _20_aspnet_state_exe            = getFileVer(p_nfx20+L"\\aspnet_state.exe",&status);
	fver _20_aspnet_wp_exe               = getFileVer(p_nfx20+L"\\aspnet_wp.exe",&status);
	fver _20_AspNetMMCExt_dll            = getFileVer(p_nfx20+L"\\AspNetMMCExt.dll",&status);
	fver _20_CORPerfMonExt_dll           = getFileVer(p_nfx20+L"\\CORPerfMonExt.dll",&status);
	fver _20_dfdll_dll                   = getFileVer(p_nfx20+L"\\dfdll.dll",&status);
	fver _20_IEExec_exe                  = getFileVer(p_nfx20+L"\\IEExec.exe",&status);
	fver _20_ilasm_exe                   = getFileVer(p_nfx20+L"\\ilasm.exe",&status);
	fver _20_InstallUtil_exe             = getFileVer(p_nfx20+L"\\InstallUtil.exe",&status);
	fver _20_Microsoft_Build_Engine_dll  = getFileVer(p_nfx20+L"\\Microsoft.Build.Engine.dll",&status);
	fver _20_Microsoft_Build_Tasks_dll   = getFileVer(p_nfx20+L"\\Microsoft.Build.Tasks.dll",&status);
	fver _20_Microsoft_JScript_dll       = getFileVer(p_nfx20+L"\\Microsoft.JScript.dll",&status);
	fver _20_MmcAspExt_dll               = getFileVer(p_nfx20+L"\\MmcAspExt.dll",&status);
	fver _20_mscordacwks_dll             = getFileVer(p_nfx20+L"\\mscordacwks.dll",&status);
	fver _20_mscorie_dll                 = getFileVer(p_nfx20+L"\\mscorie.dll",&status);
	fver _20_mscorjit_dll                = getFileVer(p_nfx20+L"\\mscorjit.dll",&status);
	fver _20_mscorlib_dll                = getFileVer(p_nfx20+L"\\mscorlib.dll",&status);
	fver _20_mscorpe_dll                 = getFileVer(p_nfx20+L"\\mscorpe.dll",&status);
	fver _20_mscorsvc_dll                = getFileVer(p_nfx20+L"\\mscorsvc.dll",&status);
	fver _20_mscorsvw_exe                = getFileVer(p_nfx20+L"\\mscorsvw.exe",&status);
	fver _20_mscorwks_dll                = getFileVer(p_nfx20+L"\\mscorwks.dll",&status);
	fver _20_ngen_exe                    = getFileVer(p_nfx20+L"\\ngen.exe",&status);
	fver _20_normalization_dll           = getFileVer(p_nfx20+L"\\normalization.dll",&status);
	fver _20_peverify_dll                = getFileVer(p_nfx20+L"\\peverify.dll",&status);
	fver _20_SOS_dll                     = getFileVer(p_nfx20+L"\\SOS.dll",&status);
	fver _20_sysglobl_dll                = getFileVer(p_nfx20+L"\\sysglobl.dll",&status);
	fver _20_System_dll                  = getFileVer(p_nfx20+L"\\System.dll",&status);
	fver _20_System_configuration_dll    = getFileVer(p_nfx20+L"\\System.configuration.dll",&status);
	fver _20_System_Data_dll             = getFileVer(p_nfx20+L"\\System.Data.dll",&status);
	fver _20_System_Data_OracleClient_dll= getFileVer(p_nfx20+L"\\System.Data.OracleClient.dll",&status);
	fver _20_System_Data_SqlXml_dll      = getFileVer(p_nfx20+L"\\System.Data.SqlXml.dll",&status);
	fver _20_System_DeploymentFramework_dll = getFileVer(p_nfx20+L"\\System.DeploymentFramework.dll",&status);
	fver _20_System_Design_dll           = getFileVer(p_nfx20+L"\\System.Design.dll",&status);
	fver _20_System_DirectoryServices_dll = getFileVer(p_nfx20+L"\\System.DirectoryServices.dll",&status);
	fver _20_System_DirectoryServices_Protocols_dll = getFileVer(p_nfx20+L"\\System.DirectoryServices.Protocols.dll",&status);
	fver _20_System_Drawing_dll          = getFileVer(p_nfx20+L"\\System.Drawing.dll",&status);
	fver _20_System_EnterpriseServices_dll = getFileVer(p_nfx20+L"\\System.EnterpriseServices.dll",&status);
	fver _20_System_EnterpriseServices_Thunk_dll = getFileVer(p_nfx20+L"\\System.EnterpriseServices.Thunk.dll",&status);
	fver _20_System_EnterpriseServices_tlb = getFileVer(p_nfx20+L"\\System.EnterpriseServices.tlb",&status);
	fver _20_System_EnterpriseServices_Wrapper_dll = getFileVer(p_nfx20+L"\\System.EnterpriseServices.Wrapper.dll",&status);
	fver _20_System_Management_dll       = getFileVer(p_nfx20+L"\\System.Management.dll",&status);
	fver _20_System_Runtime_Remoting_dll = getFileVer(p_nfx20+L"\\System.Runtime.Remoting.dll",&status);
	fver _20_System_Security_dll         = getFileVer(p_nfx20+L"\\System.Security.dll",&status);
	fver _20_System_Transactions_dll         = getFileVer(p_nfx20+L"\\System.Transactions.dll",&status);
	fver _20_System_Web_dll              = getFileVer(p_nfx20+L"\\System.Web.dll",&status);
	fver _20_System_Web_Mobile_dll       = getFileVer(p_nfx20+L"\\System.Web.Mobile.dll",&status);
	fver _20_System_Web_RegularExpressions_dll = getFileVer(p_nfx20+L"\\System.Web.RegularExpressions.dll",&status);
	fver _20_System_Windows_Forms_dll    = getFileVer(p_nfx20+L"\\System.Windows.Forms.dll",&status);
	fver _20_System_XML_dll              = getFileVer(p_nfx20+L"\\System.XML.dll",&status);
	fver _20_vbc_exe                     = getFileVer(p_nfx20+L"\\vbc.exe",&status);
	fver _20_VsaVb7rt_dll                = getFileVer(p_nfx20+L"\\VsaVb7rt.dll",&status);
	fver _20_webengine_dll               = getFileVer(p_nfx20+L"\\webengine.dll",&status);
	fver _20_WMINet_Utils_dll            = getFileVer(p_nfx20+L"\\WMINet_Utils.dll",&status);

	fver _30_PenIMC_dll                = getFileVer(p_nfx30wpf+L"\\PenIMC.dll",&status);
	fver _30_PresentationHostDLL_dll   = getFileVer(p_nfx30wpf+L"\\PresentationHostDLL.dll",&status);
	fver _30_wpfgfx_v0300_dll          = getFileVer(p_nfx30wpf+L"\\wpfgfx_v0300.dll",&status);
	fver _30_XPSViewer_exe             = getFileVer(p_nfx30xps+L"\\XPSViewer.exe",&status);
	fver _30_PresentationCore_dll      = getFileVer(p_nfx30ra+L"\\PresentationCore.dll",&status);
	fver _30_PresentationFramework_dll = getFileVer(p_nfx30ra+L"\\PresentationFramework.dll",&status);
	fver _30_ReachFramework_dll        = getFileVer(p_nfx30ra+L"\\ReachFramework.dll",&status);
	fver _30_System_Printing_dll       = getFileVer(p_nfx30ra+L"\\System.Printing.dll",&status);
	fver _30_System_Workflow_Activities_dll = getFileVer(p_nfx30ra+L"\\System.Workflow.Activities.dll",&status);
	fver _30_System_Workflow_ComponentModel_dll = getFileVer(p_nfx30ra+L"\\System.Workflow.ComponentModel.dll",&status);
	fver _30_System_Workflow_Runtime_dll = getFileVer(p_nfx30ra+L"\\System.Workflow.Runtime.dll",&status);
	fver _30_WindowsBase_dll           = getFileVer(p_nfx30ra+L"\\WindowsBase.dll",&status);

	fver _30_ServiceMonikerSupport_dll = getFileVer(p_nfx30wcf+L"\\ServiceMonikerSupport.dll",&status);
	fver _30_SMdiagnostics_dll         = getFileVer(p_nfx30wcf+L"\\SMdiagnostics.dll",&status);
	fver _30_SMSvcHost_exe             = getFileVer(p_nfx30wcf+L"\\SMSvcHost.exe",&status);
	fver _30_System_IdentityModel_dll  = getFileVer(p_nfx30ra+L"\\System.IdentityModel.dll",&status);
	fver _30_System_Runtime_Serialization_dll = getFileVer(p_nfx30ra+L"\\System.Runtime.Serialization.dll",&status);
	fver _30_System_ServiceModel_dll   = getFileVer(p_nfx30ra+L"\\System.ServiceModel.dll",&status);
	fver _30_System_ServiceModel_WasHosting_dll = getFileVer(p_nfx30wcf+L"\\System.ServiceModel.WasHosting.dll",&status);
	fver _30_PresentationCFFRasterizerNative_v0300_dll = getFileVer(System32+L"\\PresentationCFFRasterizerNative_v0300.dll",&status);

	fver _35_DataSvcUtil_exe                 = getFileVer(p_nfx35+L"\\DataSvcUtil.exe",&status);
	fver _35_System_AddIn_dll                = getFileVer(p_nfx35ra+L"\\System.AddIn.dll",&status);
	fver _35_System_Data_Linq_dll            = getFileVer(p_nfx35ra+L"\\System.Data.Linq.dll",&status);
	fver _35_System_Data_Services_dll        = getFileVer(p_nfx35ra+L"\\System.Data.Services.dll",&status);
	fver _35_System_Data_Services_Client_dll = getFileVer(p_nfx35ra+L"\\System.Data.Services.Client.dll",&status);
	fver _35_System_Data_Services_Design_dll = getFileVer(p_nfx35ra+L"\\System.Data.Services.Design.dll",&status);
	fver _35_System_ServiceModel_Web_dll     = getFileVer(p_nfx35ra+L"\\System.ServiceModel.Web.dll",&status);
	fver _35_System_Web_DynamicData_dll      = getFileVer(p_nfx35ra+L"\\System.Web.DynamicData.dll",&status);
	fver _35_System_Web_Entity_dll           = getFileVer(p_nfx35ra+L"\\System.Web.Entity.dll",&status);
	fver _35_System_Web_Extensions_dll       = getFileVer(p_nfx35ra+L"\\System.Web.Extensions.dll",&status);

	int _35_chrome_jar      = (int)getFileSize(p_nfx35+L"\\Windows Presentation Foundation\\DotNetAssistantExtension\\chrome\\chrome.jar");
	int _35_chrome_manifest = (int)getFileSize(p_nfx35+L"\\Windows Presentation Foundation\\DotNetAssistantExtension\\chrome.manifest");
	int _35_defaults_js     = (int)getFileSize(p_nfx35+L"\\Windows Presentation Foundation\\DotNetAssistantExtension\\defaults\\preferences\\defaults.js");
	int _35_install_rdf     = (int)getFileSize(p_nfx35+L"\\Windows Presentation Foundation\\DotNetAssistantExtension\\install.rdf");

	fver _40_aspnet_compiler_exe             = getFileVer(p_nfx40+L"\\aspnet_compiler.exe",&status);
	fver _40_clr_dll                         = getFileVer(p_nfx40+L"\\clr.dll",&status);
	fver _40_clrjit_dll                      = getFileVer(p_nfx40+L"\\clrjit.dll",&status);
	fver _40_Culture_dll                     = getFileVer(p_nfx40+L"\\Culture.dll",&status);
	fver _40_Microsoft_Build_Tasks_v4_0_dll  = getFileVer(p_nfx40+L"\\Microsoft.Build.Tasks.v4.0.dll",&status);
	fver _40_Microsoft_Data_Entity_Build_Tasks_dll = getFileVer(p_nfx40+L"\\Microsoft.Data.Entity.Build.Tasks.dll",&status);
	fver _40_Microsoft_JScript_dll           = getFileVer(p_nfx40+L"\\Microsoft.JScript.dll",&status);
	fver _40_mscordacwks_dll                 = getFileVer(p_nfx40+L"\\mscordacwks.dll",&status);
	fver _40_mscordbi_dll                    = getFileVer(p_nfx40+L"\\mscordbi.dll",&status);
	fver _40_mscoreei_dll                    = getFileVer(p_nfx40+L"\\mscoreei.dll",&status);
	fver _40_mscorlib_dll                    = getFileVer(p_nfx40+L"\\mscorlib.dll",&status);
	fver _40_nlssorting_dll                  = getFileVer(p_nfx40+L"\\nlssorting.dll",&status);
	fver _40_PenIMC_dll                      = getFileVer(p_nfx40wpf+L"\\PenIMC.dll",&status);
	fver _40_PresentationCore_dll            = getFileVer(p_nfx40wpf+L"\\PresentationCore.dll",&status);
	fver _40_PresentationFramework_dll       = getFileVer(p_nfx40wpf+L"\\PresentationFramework.dll",&status);
	fver _40_PresentationHost_v0400_dll      = getFileVer(p_nfx40wpf+L"\\PresentationHost_v0400.dll",&status);
	fver _40_ServiceModelReg_exe             = getFileVer(p_nfx40+L"\\ServiceModelReg.exe",&status);
	fver _40_SOS_dll                         = getFileVer(p_nfx40+L"\\SOS.dll",&status);
	fver _40_System_dll                      = getFileVer(p_nfx40+L"\\System.dll",&status);
	fver _40_System_AddIn_dll                = getFileVer(p_nfx40+L"\\System.AddIn.dll",&status);
	fver _40_System_configuration_dll        = getFileVer(p_nfx40+L"\\System.configuration.dll",&status);
	fver _40_System_Core_dll                 = getFileVer(p_nfx40+L"\\System.Core.dll",&status);
	fver _40_System_Data_dll                 = getFileVer(p_nfx40+L"\\System.Data.dll",&status);
	fver _40_System_Data_Entity_Design_dll   = getFileVer(p_nfx40+L"\\System.Data.Entity.Design.dll",&status);
	fver _40_System_Data_Linq_dll            = getFileVer(p_nfx40+L"\\System.Data.Linq.dll",&status);
	fver _40_System_Data_SqlXml_dll          = getFileVer(p_nfx40+L"\\System.Data.SqlXml.dll",&status);
	fver _40_System_Design_dll               = getFileVer(p_nfx40+L"\\System.Design.dll",&status);
	fver _40_System_DirectoryServices_dll    = getFileVer(p_nfx40+L"\\System.DirectoryServices.dll",&status);
	fver _40_System_DirectoryServices_Protocols_dll = getFileVer(p_nfx40+L"\\System.DirectoryServices.Protocols.dll",&status);
	fver _40_System_Drawing_dll              = getFileVer(p_nfx40+L"\\System.Drawing.dll",&status);
	fver _40_System_IdentityModel_dll        = getFileVer(p_nfx40+L"\\System.IdentityModel.dll",&status);
	fver _40_System_Management_dll           = getFileVer(p_nfx40+L"\\System.Management.dll",&status);
	fver _40_System_Net_dll                  = getFileVer(p_nfx40+L"\\System.Net.dll",&status);
	fver _40_System_Runtime_Remoting_dll     = getFileVer(p_nfx40+L"\\System.Runtime.Remoting.dll",&status);
	fver _40_System_Runtime_Serialization_dll= getFileVer(p_nfx40+L"\\System.Runtime.Serialization.dll",&status);
	fver _40_System_Security_dll             = getFileVer(p_nfx40+L"\\System.Security.dll",&status);
	fver _40_System_ServiceModel_dll         = getFileVer(p_nfx40+L"\\System.ServiceModel.dll",&status);
	fver _40_System_ServiceModel_Activation_dll = getFileVer(p_nfx40+L"\\System.ServiceModel.Activation.dll",&status);
	fver _40_System_ServiceModel_Web_dll     = getFileVer(p_nfx40+L"\\System.ServiceModel.Web.dll",&status);
	fver _40_System_Windows_Forms_dll        = getFileVer(p_nfx40+L"\\System.Windows.Forms.dll",&status);
	fver _40_System_Xaml_dll                 = getFileVer(p_nfx40+L"\\System.Xaml.dll",&status);
	fver _40_System_XML_dll                  = getFileVer(p_nfx40+L"\\System.XML.dll",&status);
	fver _40_System_Xml_Serialization_dll    = getFileVer(p_nfx40+L"\\System.Xml.Serialization.dll",&status);
	fver _40_vbc_exe                         = getFileVer(p_nfx40+L"\\vbc.exe",&status);
	fver _40_WindowsBase_dll                 = getFileVer(p_nfx40wpf+L"\\WindowsBase.dll",&status);
	fver _40_WMINet_Utils_dll                = getFileVer(p_nfx40+L"\\WMINet_Utils.dll",&status);
	fver _40_WPFFontCache_v0400_exe          = getFileVer(p_nfx40wpf+L"\\WPFFontCache_v0400.exe",&status);
	fver _40_wpfgfx_v0400_dll                = getFileVer(p_nfx40wpf+L"\\wpfgfx_v0400.dll",&status);
	fver _40_wpftxt_v0400_dll                = getFileVer(p_nfx40wpf+L"\\wpftxt_v0400.dll",&status);

	// These files are only installed with the .NET Framework 4.0 FULL PROFILE.
	// If your installation has only the CLIENT PROFILE, you will be missing these.
	fver _40_aspnet_wp_exe                   = getFileVer(p_nfx40+L"\\aspnet_wp.exe",&status);
	fver _40_DataSvcUtil_exe                 = getFileVer(p_nfx40+L"\\DataSvcUtil.exe",&status);
	fver _40_peverify_dll                    = getFileVer(p_nfx40+L"\\peverify.dll",&status);
	fver _40_System_Data_Services_Design_dll = getFileVer(p_nfx40+L"\\System.Data.Services.Design.dll",&status);
	fver _40_System_Data_Services_dll        = getFileVer(p_nfx40+L"\\System.Data.Services.dll",&status);
	fver _40_System_Web_DataVisualization_dll = getFileVer(p_nfx40+L"\\System.Web.DataVisualization.dll",&status);
	fver _40_System_Web_Extensions_dll       = getFileVer(p_nfx40+L"\\System.Web.Extensions.dll",&status);
	fver _40_System_Web_dll                  = getFileVer(p_nfx40+L"\\System.Web.dll",&status);
	fver _40_System_Web_Mobile_dll           = getFileVer(p_nfx40+L"\\System.Web.Mobile.dll",&status);
	fver _40_System_Workflow_ComponentModel_dll = getFileVer(p_nfx40+L"\\System.Workflow.ComponentModel.dll",&status);
	fver _40_System_Workflow_Runtime_dll     = getFileVer(p_nfx40+L"\\System.Workflow.Runtime.dll",&status);
	fver _40_webengine4_dll                  = getFileVer(p_nfx40+L"\\webengine4.dll",&status);
	fver _40_webengine_dll                   = getFileVer(p_nfx40+L"\\webengine.dll",&status);

	fver _dfshim_dll                           = getFileVer(System32+L"\\dfshim.dll",&status);
	//fver _mscoree_dll                          = getFileVer(System32+L"\\mscoree.dll",&status);
	fver _mscorees_dll                         = getFileVer(System32+L"\\mui\\0409\\mscorees.dll",&status);
	fver _netfxperf_dll                        = getFileVer(System32+L"\\netfxperf.dll",&status);
	fver _NETFXSBS10_exe                       = getFileVer(Framework+L"\\NETFXSBS10.exe",&status);
	fver _PresentationHostProxy_dll            = getFileVer(System32+L"\\PresentationHostProxy.dll",&status);
	fver _PresentationHost_exe                 = getFileVer(System32+L"\\PresentationHost.exe",&status);
	fver _sbs_diasymreader_dll                 = getFileVer(Framework+L"\\sbs_diasymreader.dll",&status);
	fver _sbs_microsoft_jscript_dll            = getFileVer(Framework+L"\\sbs_microsoft.jscript.dll",&status);
	fver _sbs_mscordbi_dll                     = getFileVer(Framework+L"\\sbs_mscordbi.dll",&status);
	fver _sbs_mscorrc_dll                      = getFileVer(Framework+L"\\sbs_mscorrc.dll",&status);
	fver _sbs_mscorsec_dll                     = getFileVer(Framework+L"\\sbs_mscorsec.dll",&status);
	fver _sbs_system_configuration_install_dll = getFileVer(Framework+L"\\sbs_system.configuration.install.dll",&status);
	fver _sbs_system_data_dll                  = getFileVer(Framework+L"\\sbs_system.data.dll",&status);
	fver _sbs_system_enterpriseservices_dll    = getFileVer(Framework+L"\\sbs_system.enterpriseservices.dll",&status);
	fver _sbs_wminet_utils_dll                 = getFileVer(Framework+L"\\sbs_wminet_utils.dll",&status);
	fver _sbscmp10_dll                         = getFileVer(Framework+L"\\sbscmp10.dll",&status);
	fver _sbscmp20_mscorwks_dll                = getFileVer(Framework+L"\\sbscmp20_mscorwks.dll",&status);
	fver _sbscmp20_mscorlib_dll                = getFileVer(p_nfx20+L"\\sbscmp20_mscorlib.dll",&status);
	fver _sbscmp20_perfcounter_dll             = getFileVer(Framework+L"\\sbscmp20_perfcounter.dll",&status);
	fver _SharedReg12_dll                      = getFileVer(Framework+L"\\SharedReg12.dll",&status);

	int _40_blackberry_browser = (int)getFileSize(p_nfx40+L"\\Config\\Browsers\\blackberry.browser");
	int _40_firefox_browser    = (int)getFileSize(p_nfx40+L"\\Config\\Browsers\\firefox.browser");
	int _40_gateway_browser    = (int)getFileSize(p_nfx40+L"\\Config\\Browsers\\gateway.browser");
	int _40_ie_browser         = (int)getFileSize(p_nfx40+L"\\Config\\Browsers\\ie.browser");
	int _40_locale_nlp         = (int)getFileSize(p_nfx40+L"\\locale.nlp");
/*
	printf("echo blackberry:\t%d\n",_40_blackberry_browser);
	printf("echo    firefox:\t%d\n",_40_firefox_browser);
	printf("echo    gateway:\t%d\n",_40_gateway_browser);
	printf("echo         ie:\t%d\n",_40_ie_browser);
*/	
	// Check for Microsoft Root Certificate Authority 2011
	// This root certificate is needed for the newer NDP4.0 patches.
	// Without this, they will fail due to "Generic Trust Failure".
	if( sp>=2 &&  !checkCertificate("8f43288ad272f3103b6fb1428485ea3014c0bcfe Microsoft Root Certificate Authority 2011","Root")) {
		//NN("Microsoft Root Certificate Authority 2011");
		//XX("bin\\certutil.exe -addstore ROOT "+p+"MicrosoftRootCertificateAuthority2011.cer");
		NN("Update for Root Certificates for Windows XP [March 2014] (KB931125)");
		XX(sw+p3+"rootsupd_01a26e5c75ff5b3e34fb6b763ace486fe6836aac.exe");
	}

	// Patches for .NET Framework 1.0, Regular Edition
	if( sp>=0 && *i_nfx10 && !NFX40){
		NN("Microsoft .NET Framework 1.0 Service Pack 3, English Version (KB867461)");
		XX(p3+"NDP1.0sp3-KB867461-X86-Enu.exe"+n3);
	}
	if( sp==1 && ((*i_nfx10 || *v_nfx10>=3)) && (
		// According to online docs, KB886906 is intended for use on SP1 or SP2
		   ( _10_System_Web_dll  <fver(1,0,3705,6021)) )) {
		NN("Security Update for Microsoft .NET Framework, Version 1.0 SP3, English (KB886906)");
		XX(p1+"ndp1.0sp3-kb886906-x86-enu_704fc2f79b62d7116b5bdbcaeb74e2039e2c7d4f.exe"+n3);
	}
	if( sp>=2 /*&& !NFX40*/ && ((*i_nfx10 || *v_nfx10>=3)) && (
		// According to online docs, KB928367 is officially supported on SP2+
		   ( _10_aspnet_isapi_dll<fver(1,0,3705,6060))
		|| ( _10_aspnet_wp_exe   <fver(1,0,3705,6060))
		|| ( _10_IEExec_exe      <fver(1,0,3705,6060))
		|| ( _10_mscorie_dll     <fver(1,0,3705,6060))
		|| ( _10_mscorld_dll     <fver(1,0,3705,6060))
		|| ( _10_mscorlib_dll    <fver(1,0,3705,6060))
		|| ( _10_mscorsvr_dll    <fver(1,0,3705,6060))
		|| ( _10_mscorwks_dll    <fver(1,0,3705,6060))
		|| ( _10_System_Web_dll  <fver(1,0,3705,6060)) )) {
		if(!NFX40) {
			NN("Security Update for Microsoft .NET Framework, Version 1.0 Service Pack 3 (KB928367)");
			XX(p3+"NDP1.0sp3-KB928367-X86-Enu.exe"+n3);
		} else {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Update KB928367 for .NET Framework 1.0 is")
			                               +"|missing, but cannot be installed because"
							               +"|.NET Framework 4.0 is installed."
							               +"| "
							               +"|Please uninstall .NET Framework 4.0 and try"
							               +"|again.");
		}
	}

	// .NET Framework 1.0 for Tablet PC and Media Center Edition
	if( sp==2 && ((*i_nfx10mce || *v_nfx10mce>=3)) && (
		   ( _10_aspnet_isapi_dll<fver(1,0,3705,6073))
		|| ( _10_aspnet_state_exe<fver(1,0,3705,6073))
		|| ( _10_aspnet_wp_exe   <fver(1,0,3705,6073))
		|| ( _10_gacutil_exe     <fver(1,0,3705,0))
		|| ( _10_IEExec_exe      <fver(1,0,3705,6073))
		|| ( _10_mscorie_dll     <fver(1,0,3705,6073))
		|| ( _10_mscorjit_dll    <fver(1,0,3705,6073))
		|| ( _10_mscorld_dll     <fver(1,0,3705,6073))
		|| ( _10_mscorlib_dll    <fver(1,0,3705,6073))
		|| ( _10_mscorsvr_dll    <fver(1,0,3705,6073))
		|| ( _10_mscorwks_dll    <fver(1,0,3705,6073))
		|| ( _10_netfxupdate_exe <fver(1,0,3705,1))
		|| ( _10_SetRegNI_exe    <fver(1,1,4322,2035))
		|| ( _10_System_Web_dll  <fver(1,0,3705,6073))
		|| ( _10_ToGac_exe       <fver(1,1,4322,2251)) )) {
		NN("Microsoft .NET Framework 1.0 Service Pack 3 Security Update for Windows XP Tablet PC and Media Center (KB953295)");
		XX(p2+"ndp1.0sp3-kb953295-x86-ocm-enu_6bcabcb5c10ed16416a6325743cca321b42d64c4.exe"+n1);
	}
	if( sp>=2 && ((*i_nfx10mce || *v_nfx10mce>=3)) && (
		   ( _10_gacutil_exe         <fver(1,0,3705,0))
		|| ( _10_netfxupdate_exe     <fver(1,0,3705,1))
		|| ( _10_SetRegNI_exe        <fver(1,1,4322,2035))
		|| ( _10_System_Security_dll <fver(1,0,3705,6074))
		|| ( _10_ToGac_exe           <fver(1,1,4322,2251)) )) {
		NN("Microsoft .NET Framework 1.0 SP3 Security Update for Windows XP Tablet PC and Media Center (KB979904)");
		XX(p3+"ndp1.0sp3-kb979904-x86-ocm-enu_a617462b19d63e11f602c18ada0070ebc99f0a0f.exe"+n1);
	}
	if( sp==3 && ((*i_nfx10mce || *v_nfx10mce>=3)) && (
		   ( _10_aspnet_isapi_dll <fver(1,0,3705,6115))
		|| ( _10_aspnet_state_exe <fver(1,0,3705,6115))
		|| ( _10_aspnet_wp_exe    <fver(1,0,3705,6115))
		|| ( _10_gacutil_exe      <fver(1,0,3705,0))
		|| ( _10_IEExec_exe       <fver(1,0,3705,6115))
		|| ( _10_mscorie_dll      <fver(1,0,3705,6115))
		|| ( _10_mscorjit_dll     <fver(1,0,3705,6115))
		|| ( _10_mscorld_dll      <fver(1,0,3705,6115))
		|| ( _10_mscorlib_dll     <fver(1,0,3705,6115))
		|| ( _10_mscorsvr_dll     <fver(1,0,3705,6115))
		|| ( _10_mscorwks_dll     <fver(1,0,3705,6115))
		|| ( _10_netfxupdate_exe  <fver(1,0,3705,1))
		|| ( _10_SetRegNI_exe     <fver(1,1,4322,2035))
		|| ( _10_System_Web_dll   <fver(1,0,3705,6115))
		|| ( _10_ToGac_exe        <fver(1,1,4322,2251)) )) {
		NN("Security Update for Microsoft .NET Framework 1.0 SP3 on Windows XP Tablet PC and Media Center (KB2904878)");
		XX(p3+"NDP1.0sp3-KB2904878-X86-Ocm-Enu.exe"+n1);
	}
	if( sp==3 && ((*i_nfx10mce || *v_nfx10mce>=3)) && (
		   ( _10_aspnet_isapi_dll         <fver(1,0,3705,6110))
		|| ( _10_aspnet_state_exe         <fver(1,0,3705,6110))
		|| ( _10_aspnet_wp_exe            <fver(1,0,3705,6110))
		|| ( _10_gacutil_exe              <fver(1,0,3705,0))
		|| ( _10_IEExec_exe               <fver(1,0,3705,6110))
		|| ( _10_mscorie_dll              <fver(1,0,3705,6110))
		|| ( _10_mscorjit_dll             <fver(1,0,3705,6110))
		|| ( _10_mscorld_dll              <fver(1,0,3705,6110))
		|| ( _10_mscorlib_dll             <fver(1,0,3705,6110))
		|| ( _10_mscorsvr_dll             <fver(1,0,3705,6110))
		|| ( _10_mscorwks_dll             <fver(1,0,3705,6110))
		|| ( _10_netfxupdate_exe          <fver(1,0,3705,1))
		|| ( _10_SetRegNI_exe             <fver(1,1,4322,2035))
		|| ( _10_system_dll               <fver(1,0,3705,6110))
		|| ( _10_System_Drawing_dll       <fver(1,0,3705,6110))
		|| ( _10_System_Web_dll           <fver(1,0,3705,6110))
		|| ( _10_System_Windows_Forms_dll <fver(1,0,3705,6110))
		|| ( _10_ToGac_exe                <fver(1,1,4322,2251)) )) {
		NN("Security Update for Microsoft .NET Framework 1.0 SP3 on Windows XP Tablet PC and Media Center (KB2742607)");
		XX(p3+"NDP1.0sp3-KB2742607-X86-Ocm-Enu.exe"+n1);
	}

	// .NET Framework 1.1
	if( sp>=0 && *i_nfx11 && *v_nfx11==0){
		// i_nfx11 means NFX11 is flagged to be upgraded (e.g. install SP1)
		// v_nfx11 means the current NFX11 SP level is at least 0 (e.g. not -1 (absent))
		NN("Microsoft .NET Framework 1.1 Service Pack 1 (KB867460)");
		XX(p3+"NDP1.1sp1-KB867460-X86.exe"+n3);
	}
	if( sp==1 &&  ((*i_nfx11 && *v_nfx11>=0) || *v_nfx11>=1) && (
		// According to online docs, KB886903 is officially supported on SP1 or SP2
		   ( _11_System_Web_dll           < fver(1,1,4322,2037)) )) {
		NN("Security Update for Microsoft .NET Framework, Version 1.1 Service Pack 1 (KB886903)");
		XX(p1+"ndp1.1sp1-kb886903-x86_535f57e5a8eceab18533c50c0b0e1469ed45331f.exe"+n3);
	}
	/*if( sp==1 &&  ((*i_nfx11 && *v_nfx11>=0) || *v_nfx11>=1) && (
		// According to online docs, KB928366 is officially supported only on SP2
		// On SP2, newer patch KB979906 is available.
		   ( _11_aspnet_isapi_dll         < fver(1,1,4322,2407))
		|| ( _11_aspnet_wp_exe            < fver(1,1,4322,2407))
		|| ( _11_CORPerfMonExt_dll        < fver(1,1,4322,2407))
		|| ( _mscorees_dll                < fver(2,0,50727,253))
		|| ( _11_mscorie_dll              < fver(1,1,4322,2407))
		|| ( _11_mscorjit_dll             < fver(1,1,4322,2407))
		|| ( _11_mscorld_dll              < fver(1,1,4322,2407))
		|| ( _11_mscorlib_dll             < fver(1,1,4322,2407))
		|| ( _11_mscorsn_dll              < fver(1,1,4322,2407))
		|| ( _11_mscorsvr_dll             < fver(1,1,4322,2407))
		|| ( _11_System_dll               < fver(1,1,4322,2407))
		|| ( _11_mscorwks_dll             < fver(1,1,4322,2407))
		|| ( _11_System_Web_dll           < fver(1,1,4322,2407)) )) {
		NN("Security Update for Microsoft .NET Framework, Version 1.1 Service Pack 1 (KB928366)");
		XX(p1+"ndp1.1sp1-kb928366-x86_20112ef50011e0de2c0e3378139245d81a178b15.exe"+n3);
	}*/
	if( sp==2 &&  ((*i_nfx11 && *v_nfx11>=0) || *v_nfx11>=1) && (
		   ( _11_aspnet_filter_dll        < fver(1,1,4322,2463))
		|| ( _11_aspnet_isapi_dll         < fver(1,1,4322,2463))
		|| ( _11_aspnet_wp_exe            < fver(1,1,4322,2463))
		|| ( _11_CORPerfMonExt_dll        < fver(1,1,4322,2463))
		|| ( _mscorees_dll                < fver(4,0,31106,0))
		|| ( _11_mscorie_dll              < fver(1,1,4322,2463))
		|| ( _11_mscorjit_dll             < fver(1,1,4322,2463))
		|| ( _11_mscorld_dll              < fver(1,1,4322,2463))
		|| ( _11_mscorlib_dll             < fver(1,1,4322,2463))
		|| ( _11_mscorsn_dll              < fver(1,1,4322,2463))
		|| ( _11_mscorsvr_dll             < fver(1,1,4322,2463))
		|| ( _11_System_dll               < fver(1,1,4322,2463))
		|| ( _11_System_Security_dll      < fver(1,1,4322,2463))
		|| ( _11_mscorwks_dll             < fver(1,1,4322,2463))
		|| ( _11_System_Web_dll           < fver(1,1,4322,2463)) )) {
		NN("Microsoft .NET Framework 1.1 SP1 Security Update for Windows 2000 and Windows XP (KB979906)");
//		XX(p2+"ndp1.1sp1-kb979906-x86_642ace45800ffefd77231150a07a2bdfe170732c.exe"+n6); // Shows no UI, but fails on Windows Embedded.
		XX(p2+"ndp1.1sp1-kb979906-x86_642ace45800ffefd77231150a07a2bdfe170732c.exe /extract %TEMP%\\KB979906\n"
				+"pushd %TEMP%\\KB979906\n"
				+"msiexec /update NDP1.1sp1-KB979906-X86.msp /passive /norestart\n"
				+"popd\n"
				+"rd /S /Q %TEMP%\\KB979906");
	}
	if( sp==3 &&  ((*i_nfx11 && *v_nfx11>=0) || *v_nfx11>=1) && (
		   ( _11_aspnet_filter_dll        < fver(1,1,4322,2503))
		|| ( _11_aspnet_isapi_dll         < fver(1,1,4322,2503))
		|| ( _11_aspnet_wp_exe            < fver(1,1,4322,2503))
		|| ( _11_CORPerfMonExt_dll        < fver(1,1,4322,2503))
		|| ( _mscoree_dll                 < fver(4,0,31106,0))
		|| ( _11_mscorie_dll              < fver(1,1,4322,2503))
		|| ( _11_mscorjit_dll             < fver(1,1,4322,2503))
		|| ( _11_mscorld_dll              < fver(1,1,4322,2503))
		|| ( _11_mscorlib_dll             < fver(1,1,4322,2503))
		|| ( _11_mscorsn_dll              < fver(1,1,4322,2503))
		|| ( _11_mscorsvr_dll             < fver(1,1,4322,2503))
		|| ( _11_System_dll               < fver(1,1,4322,2503))
		|| ( _11_System_Drawing_dll       < fver(1,1,4322,2503))
		|| ( _11_System_Security_dll      < fver(1,1,4322,2503))
		|| ( _11_System_Windows_Forms_dll < fver(1,1,4322,2503))
		|| ( _11_mscorwks_dll             < fver(1,1,4322,2503))
		|| ( _11_System_Web_dll           < fver(1,1,4322,2503)) )) {
		NN("Security Update for Microsoft .NET Framework 1.1 SP1 on Windows XP, Windows Vista, and Windows Server 2008 x86 (KB2833941)");
		//XX(p+"NDP1.1SP1-KB2833941-x86.exe"+n6); // Shows no UI, but fails on Windows Embedded.
		XX(p3+"NDP1.1SP1-KB2833941-x86.exe /extract %TEMP%\\KB2833941\n"
				+"pushd %TEMP%\\KB2833941\n"
				+"msiexec /update NDP1.1sp1-KB2833941-X86.msp /passive /norestart\n"
				+"popd\n"
				+"rd /S /Q %TEMP%\\KB2833941");
	}
	

	// .NET Framework 2.0

	// Base file versions of various SP levels:
	// RTM 2,0,50727,42
	// SP1 2,0,50727,1433
	// SP2 2,0,50727,3053

	if( sp>=2 &&  *i_nfx30 && *i_nfx35) {
		NN("Microsoft .NET Framework 3.5 Service Pack 1 and .NET Framework 3.5 Family Update for .NET versions 2.0 through 3.5 (KB951847) x86");
		XX(np+"dotnetfx35_x86.exe"+n6);
	}
	else if( sp>=2 && *i_nfx20){
		NN("Microsoft .NET Framework 2.0 Service Pack 2");
		XX(np+"NetFx20SP2_x86.exe"+n6);
	}

//	if( sp==2 && !*i_nfx20 && *v_nfx20==0 && (
//		// This patch (KB928365) is only for .NET 2.0 RTM.  Does not apply to post-SP1 configurations.
//		/* No GDR */
//		/* QFE */
//		    ( _20_AppLaunch_exe                          < fver(2,0,50727,832))
//		 || ( _20_aspnet_filter_dll                      < fver(2,0,50727,832))
//		 || ( _20_aspnet_isapi_dll                       < fver(2,0,50727,832))
//		 || ( _20_AspNetMMCExt_dll                       < fver(2,0,50727,832))
//		 || ( _20_Aspnet_perf_dll                        < fver(2,0,50727,832))
//		 || ( _20_aspnet_rc_dll                          < fver(2,0,50727,832))
//		 || ( _20_aspnet_regiis_exe                      < fver(2,0,50727,832))
//		 || ( _20_aspnet_state_exe                       < fver(2,0,50727,832))
//		 || ( _20_aspnet_wp_exe                          < fver(2,0,50727,832))
//		 || ( _20_CORPerfMonExt_dll                      < fver(2,0,50727,832))
//		 || ( _20_IEExec_exe                             < fver(2,0,50727,832))
//		 || ( _20_ilasm_exe                              < fver(2,0,50727,832))
//		 || ( _20_InstallUtil_exe                        < fver(2,0,50727,832))
//		 || ( _20_MmcAspExt_dll                          < fver(2,0,50727,832))
//		 || ( _20_Microsoft_Build_Engine_dll             < fver(2,0,50727,832))
//		 || ( _20_Microsoft_Build_Tasks_dll              < fver(2,0,50727,832))
//		 || ( _20_mscordacwks_dll                        < fver(2,0,50727,832))
////		 || ( _20_mscoree_dll                            < fver(2,0,50727,832))
//		 || ( _20_mscorie_dll                            < fver(2,0,50727,832))
//		 || ( _20_mscorjit_dll                           < fver(2,0,50727,832))
//		 || ( _20_mscorlib_dll                           < fver(2,0,50727,832))
//		 || ( _20_mscorpe_dll                            < fver(2,0,50727,832))
//		 || ( _20_mscorsvc_dll                           < fver(2,0,50727,832))
//		 || ( _20_mscorsvw_exe                           < fver(2,0,50727,832))
//		 || ( _20_ngen_exe                               < fver(2,0,50727,832))
//		 || ( _20_normalization_dll                      < fver(2,0,50727,832))
//		 || ( _20_peverify_dll                           < fver(2,0,50727,832))
//		 || ( _20_SOS_dll                                < fver(2,0,50727,832))
//		 || ( _20_sysglobl_dll                           < fver(2,0,50727,832))
//		 || ( _20_System_configuration_dll               < fver(2,0,50727,832))
//		 || ( _20_System_Data_dll                        < fver(2,0,50727,832))
//		 || ( _20_System_Data_OracleClient_dll           < fver(2,0,50727,832))
//		 || ( _20_System_Data_SqlXml_dll                 < fver(2,0,50727,832))
//		 || ( _20_System_DeploymentFramework_dll         < fver(2,0,50727,832))
////		 || ( _20_System_DeploymentFramework_Service_exe < fver(2,0,50727,832))
//		 || ( _20_System_Design_dll                      < fver(2,0,50727,832))
//		 || ( _20_System_DirectoryServices_Protocols_dll < fver(2,0,50727,832))
//		 || ( _20_System_dll                             < fver(2,0,50727,832))
//		 || ( _20_System_Drawing_dll                     < fver(2,0,50727,832))
//		 || ( _20_System_EnterpriseServices_dll          < fver(2,0,50727,832))
//		 || ( _20_System_EnterpriseServices_Thunk_dll    < fver(2,0,50727,832))
//		 || ( _20_System_EnterpriseServices_tlb          < fver(2,0,50727,832))
//		 || ( _20_System_EnterpriseServices_Wrapper_dll  < fver(2,0,50727,832))
//		 || ( _20_System_Management_dll                  < fver(2,0,50727,832))
//		 || ( _20_System_Runtime_Remoting_dll            < fver(2,0,50727,832))
//		 || ( _20_System_Transactions_dll                < fver(2,0,50727,832))
//		 || ( _20_System_Windows_Forms_dll               < fver(2,0,50727,832))
//		 || ( _20_System_XML_dll                         < fver(2,0,50727,832))
//		 || ( _20_vbc_exe                                < fver(8,0,50727,832))
//		 || ( _20_webengine_dll                          < fver(2,0,50727,832))
//		 || ( _20_Microsoft_JScript_dll                  < fver(8,0,50727,832))
//		 || ( _20_mscorwks_dll                           < fver(2,0,50727,832))
//		 || ( _20_System_Web_dll                         < fver(2,0,50727,832))
//		 || ( _20_VsaVb7rt_dll                           < fver(8,0,50727,832)) )) {
//		NN("Security Update for Microsoft .NET Framework, Version 2.0 (KB928365)");
//		XX(p2+"NDP20-KB928365-X86.exe"+n6);
//	}
	//	if( sp==2 && !*i_nfx20 && *v_nfx20==1 && (
	// Patch KB953300 is only for SP1 configurations.
//		/* No GDR */
//		/* QFE */
//			( _20_mscordacwks_dll       < fver(2,0,50727,1873))
//		 || ( _20_mscorlib_dll          < fver(2,0,50727,1873))
//		 || ( _20_mscorwks_dll          < fver(2,0,50727,1873)) )) {
//		NN("Microsoft .NET Framework 2.0 Service Pack 1 Security Update for Windows 2000, Windows Server 2003, and Windows XP (KB953300)");
//		XX(p2+"NDP20SP1-KB953300-v2-x86.exe"+n6);
//	}
	//	if( sp==2 && !*i_nfx20 && *v_nfx20==1 && (
	// Patch KB2416468 is only for SP1 configurations.
//		/* No GDR */
//		/* QFE */
//			( _20_Aspnet_perf_dll       < fver(2,0,50727,1887))
//		 || ( _20_aspnet_wp_exe         < fver(2,0,50727,1887))
//		 || ( _20_webengine_dll         < fver(2,0,50727,1887))
//		 || ( _20_System_Web_dll        < fver(2,0,50727,1887)) )) {
//		NN("Security Update for Microsoft .NET Framework 3.5 on Windows Server 2003 and Windows XP x86 (KB2416468)");
//		XX(p2+"NDP20SP1-KB2416468-x86.exe"+n6);
//	}
		//	if( sp==2 && !*i_nfx20 && *v_nfx20==1 && (
	// Patch KB982865 is only for SP1 configurations.
//		/* No GDR */
//		/* QFE */
//			( _20_Aspnet_perf_dll       < fver(2,0,50727,1879))
//		 || ( _20_aspnet_wp_exe         < fver(2,0,50727,1879))
//		 || ( _20_webengine_dll         < fver(2,0,50727,1879))
//		 || ( _20_System_Security_dll   < fver(2,0,50727,1879))
//		 || ( _20_System_Web_dll        < fver(2,0,50727,1879)) )) {
//		NN("Microsoft .NET Framework 3.5 Security Update for Windows Server 2003 and Windows XP x86 (KB982865)");
//		XX(p2+"NDP20SP1-KB982865-x86.exe"+n6);
//	}
		//	if( sp==2 && !*i_nfx20 && *v_nfx20==1 && (
	// Patch KB2478656 is only for SP1 configurations.
//		/* No GDR */
//		/* QFE */
//			( _20_System_dll        < fver(2,0,50727,1889)) )) {
//		NN("Security Update for .NET Framework 3.5 on Windows Server 2003 and Windows XP x86 (KB2478656)");
//		XX(p2+"NDP20SP1-KB2478656-x86.exe"+n6);
//	}
	//	if( sp==2 && !*i_nfx20 && *v_nfx20==1 && (
	// Patch KB2530095 is only for SP1 configurations.
//		/* No GDR */
//		/* QFE */
//			( _20_mscordacwks_dll       < fver(2,0,50727,1891))
//		 || ( _20_mscorlib_dll          < fver(2,0,50727,1891))
//		 || ( _20_SOS_dll               < fver(2,0,50727,1891))
//		 || ( _20_System_Windows_Forms_dll < fver(2,0,50727,1891))
//		 || ( _20_mscorwks_dll          < fver(2,0,50727,1891)) )) {
//		NN("Security Update for .NET Framework 3.5 on Windows Server 2003 and Windows XP x86 (KB2530095)");
//		XX(p2+"NDP20SP1-KB2530095-x86.exe"+n6);
//	}
	if( sp==2 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_Security_dll < fver(2,0,50727,3613))
		/* QFE */
		 || ( _20_System_Security_dll > fver(2,0,50727,3675) && _20_System_Security_dll < fver(2,0,50727,4434)) )) {
		NN("Microsoft .NET Framework 3.5 SP1 and .NET Framework 2.0 SP2 Security Update for Windows 2000, Windows Server 2003, and Windows XP x86 (KB979909)");
		XX(p2+"ndp20sp2-kb979909-x86_dc7f4b15ff426d9e413b87909c35929660d8a33c.exe"+n6);
	}
//	if( sp==2 &&  (*i_nfx20 || *v_nfx20==2) && (
//		/* GDR */
//			( _20_mscordacwks_dll       < fver(2,0,50727,3603))
//		 || ( _20_mscorlib_dll          < fver(2,0,50727,3603))
//		 || ( _20_mscorwks_dll          < fver(2,0,50727,3603))
//		/* QFE */
//		 || ( _20_mscordacwks_dll       > fver(2,0,50727,3655) && _20_mscordacwks_dll       < fver(2,0,50727,4062))
//		 || ( _20_mscorlib_dll          > fver(2,0,50727,3655) && _20_mscorlib_dll          < fver(2,0,50727,4062))
//		 || ( _20_SOS_dll               > fver(2,0,50727,3655) && _20_SOS_dll               < fver(2,0,50727,4062))
//		 || ( _20_mscorwks_dll          > fver(2,0,50727,3655) && _20_mscorwks_dll          < fver(2,0,50727,4062)) )) {
//		NN("Microsoft .NET Framework 2.0 Service Pack 2 Security Update for Windows 2000, Windows Server 2003, and Windows XP (KB974417)");
//		XX(p+"ndp20sp2-kb974417-x86_c8d08aa227c88b7ed915b839eff3e8c0d2f67e06.exe"+n6);
//	}
	if( sp==2 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_mscordacwks_dll       < fver(2,0,50727,3607))
		 || ( _20_mscorlib_dll          < fver(2,0,50727,3607))
		 || ( _20_mscorwks_dll          < fver(2,0,50727,3607))
		/* QFE */
		 || ( _20_mscordacwks_dll       > fver(2,0,50727,3655) && _20_mscordacwks_dll       < fver(2,0,50727,4413))
		 || ( _20_mscorlib_dll          > fver(2,0,50727,3655) && _20_mscorlib_dll          < fver(2,0,50727,4413))
		 || ( _20_SOS_dll               > fver(2,0,50727,3655) && _20_SOS_dll               < fver(2,0,50727,4413))
		 || ( _20_mscorwks_dll          > fver(2,0,50727,3655) && _20_mscorwks_dll          < fver(2,0,50727,4413)) )) {
		NN("Microsoft .NET Framework 2.0 Service Pack 2 Update for Windows Server 2003 and Windows XP x86 (KB976569)");
		XX(p2+"ndp20sp2-kb976569-x86_dd5f498a00cff2addd29b7f9e5ea8b1054ac2d73.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_Design_dll        < fver(2,0,50727,3645))
		 || ( _20_System_Windows_Forms_dll < fver(2,0,50727,3645))
		/* QFE */
		 || ( _20_System_Design_dll        > fver(2,0,50727,3645) && _20_System_Design_dll        < fver(2,0,50727,7015))
		 || ( _20_System_Windows_Forms_dll > fver(2,0,50727,3645) && _20_System_Windows_Forms_dll < fver(2,0,50727,7015)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2789643)");
		XX(p3+"NDP20SP2-KB2789643-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_Runtime_Remoting_dll < fver(2,0,50727,3659))
		/* QFE */
		 || ( _20_System_Runtime_Remoting_dll > fver(2,0,50727,3659) && _20_System_Runtime_Remoting_dll < fver(2,0,50727,7055)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 (KB2932079)");
		XX(p3+"ndp20sp2-kb2932079-x86_62d2a448718d1a1343055d53eb722e8c72aafaeb.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
		    ( _20_mscordacwks_dll     < fver(2,0,50727,8788))
		 || ( _20_mscorlib_dll        < fver(2,0,50727,8788))
		 || ( _20_mscorwks_dll        < fver(2,0,50727,8788))
		 || ( _20_SOS_dll             < fver(2,0,50727,8788))
		 || ( _20_System_Security_dll < fver(2,0,50727,8788)) )) {
		NN("2018-05 Security Only Update for .NET Framework 2.0 on WES09 and POSReady 2009 (KB4095516)");
		XX(p3+"ndp20sp2-kb4095516-x86_c5111ba0c2003141ee07c8cd75932e98f0d80948.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_mscordacwks_dll       < fver(2,0,50727,3643))
		 || ( _20_mscorlib_dll          < fver(2,0,50727,3643))
		 || ( _20_SOS_dll               < fver(2,0,50727,3643))
		 || ( _20_System_dll            < fver(2,0,50727,3643))
		 || ( _20_Microsoft_JScript_dll < fver(8,0,50727,3643))
		 || ( _20_mscorwks_dll          < fver(2,0,50727,3643))
		/* QFE */
		 || ( _20_mscordacwks_dll       > fver(2,0,50727,3655) && _20_mscordacwks_dll       < fver(2,0,50727,5737))
		 || ( _20_mscorlib_dll          > fver(2,0,50727,3655) && _20_mscorlib_dll          < fver(2,0,50727,5737))
		 || ( _20_SOS_dll               > fver(2,0,50727,3655) && _20_SOS_dll               < fver(2,0,50727,5737))
		 || ( _20_System_dll            > fver(2,0,50727,3677) && _20_System_dll            < fver(2,0,50727,5737))
		 || ( _20_Microsoft_JScript_dll > fver(8,0,50727,3643) && _20_Microsoft_JScript_dll < fver(8,0,50727,5737))
		 || ( _20_mscorwks_dll          > fver(2,0,50727,3655) && _20_mscorwks_dll          < fver(2,0,50727,5737)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2729450)");
		XX(p3+"NDP20SP2-KB2729450-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_mscordacwks_dll < fver(2,0,50727,3655))
		 || ( _20_mscorlib_dll    < fver(2,0,50727,3655))
		 || ( _20_SOS_dll         < fver(2,0,50727,3655))
		 || ( _20_mscorwks_dll    < fver(2,0,50727,3655))
		/* QFE */
		 || ( _20_mscordacwks_dll > fver(2,0,50727,3655) && _20_mscordacwks_dll < fver(2,0,50727,7041))
		 || ( _20_mscorlib_dll    > fver(2,0,50727,3655) && _20_mscorlib_dll    < fver(2,0,50727,7041))
		 || ( _20_SOS_dll         > fver(2,0,50727,3655) && _20_SOS_dll         < fver(2,0,50727,7041))
		 || ( _20_mscorwks_dll    > fver(2,0,50727,3655) && _20_mscorwks_dll    < fver(2,0,50727,7041)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2898856)");
		XX(p3+"NDP20SP2-KB2898856-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_configuration_dll < fver(2,0,50727,3654))
		 || ( _20_System_Data_SqlXml_dll   < fver(2,0,50727,3654))
		 || ( _20_System_XML_dll           < fver(2,0,50727,3654))
		/* QFE */
		 || ( _20_System_configuration_dll > fver(2,0,50727,3654) && _20_System_configuration_dll < fver(2,0,50727,7037))
		 || ( _20_System_Data_SqlXml_dll   > fver(2,0,50727,3654) && _20_System_Data_SqlXml_dll   < fver(2,0,50727,7037))
		 || ( _20_System_XML_dll           > fver(2,0,50727,3654) && _20_System_XML_dll           < fver(2,0,50727,7037)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2844285)");
		XX(p3+"NDP20SP2-KB2844285-v2-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
		    ( _20_System_Runtime_Remoting_dll < fver(2,0,50727,8772)) )) {
		NN("2017-10 Security and Quality Rollup for .NET Framework 2.0 SP2 on WES09 and POSReady 2009 (KB4040968)");
		XX(p3+"ndp20sp2-kb4040968-x86_627a7d41e164c26280b1db64fefbba23c754ebc4.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_dll < fver(2,0,50727,3677))
		/* QFE */
		 || ( _20_System_dll > fver(2,0,50727,3677) && _20_System_dll < fver(2,0,50727,8688)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 for WES09 and POSReady 2009 (KB3142027)");
		XX(p3+"ndp20sp2-kb3142027-x86_dfb224d16b27f203dedc393678a150e887b98e84.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
			( _20_mscordacwks_dll < fver(2,0,50727,8797))
		 || ( _20_mscorlib_dll    < fver(2,0,50727,8797))
		 || ( _20_SOS_dll         < fver(2,0,50727,8797))
		 || ( _20_System_dll      < fver(2,0,50727,8797))
		 || ( _20_mscorwks_dll    < fver(2,0,50727,8797)) )) {
		NN("2018-08 Security Only Update for .NET Framework 2.0 on WES09 and POSReady 2009 (KB4344180)");
		XX(p3+"ndp20sp2-kb4344180-x86_d0f73b32fe800b37b11cb66c2f0cc07f047a7b0f.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_aspnet_wp_exe                     < fver(2,0,50727,3658))
		 || ( _20_System_Web_RegularExpressions_dll < fver(2,0,50727,3658))
		 || ( _20_webengine_dll                     < fver(2,0,50727,3658))
		 || ( _20_System_Web_dll                    < fver(2,0,50727,3658))
		 || ( _20_System_dll                        < fver(2,0,50727,3614))
		/* QFE */
		 || ( _20_aspnet_wp_exe                     > fver(2,0,50727,3658) && _20_aspnet_wp_exe                     < fver(2,0,50727,7046))
		 || ( _20_System_Web_RegularExpressions_dll > fver(2,0,50727,3658) && _20_System_Web_RegularExpressions_dll < fver(2,0,50727,7046))
		 || ( _20_webengine_dll                     > fver(2,0,50727,3658) && _20_webengine_dll                     < fver(2,0,50727,7046))
		 || ( _20_System_Web_dll                    > fver(2,0,50727,3658) && _20_System_Web_dll                    < fver(2,0,50727,7046))
		 || ( _20_System_dll                        > fver(2,0,50727,3677) && _20_System_dll                        < fver(2,0,50727,4443)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2901111)");
		XX(p3+"NDP20SP2-KB2901111-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_Data_dll < fver(2,0,50727,3678))
		/* QFE */
		 || ( _20_System_Data_dll > fver(2,0,50727,3678) && _20_System_Data_dll < fver(2,0,50727,8692)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 for WES09 and POSReady 2009 (KB3163248)");
		XX(p3+"ndp20sp2-kb3163248-x86_3144185c68b59a85109a7d03ecc3849f4423006a.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
			( _20_System_dll < fver(2,0,50727,8804)) )) {
		NN("2019-02 Security Only Update for .NET Framework 2.0 SP2 on WES09 and POSReady 2009 (KB4483485)");
		XX(p3+"ndp20sp2-kb4483485-x86_4b0a0ebe86b450617b48584326a5a6e74352f027.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_Drawing_dll < fver(2,0,50727,3674))
		/* QFE */
		 || ( _20_System_Drawing_dll > fver(2,0,50727,3674) && _20_System_Drawing_dll < fver(2,0,50727,8682)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 for WEPOS and POSReady 2009 (KB3127223)");
		XX(p3+"ndp20sp2-kb3127223-x86_297ce3410a8ae6d50522d3da7d2f926af3329439.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
			( _20_mscordacwks_dll     < fver(2,0,50727,8761))
		 || ( _20_mscorlib_dll        < fver(2,0,50727,8761))
		 || ( _20_SOS_dll             < fver(2,0,50727,8761))
		 || ( _20_System_dll          < fver(2,0,50727,8761))
		 || ( _20_System_Security_dll < fver(2,0,50727,8761))
		 || ( _20_mscorwks_dll        < fver(2,0,50727,8761)) )) {
		NN("May, 2017 Security and Quality Rollup for .NET Framework 2.0 SP2 on WES09 and POSReady 2009 (KB4014585)");
		XX(p3+"ndp20sp2-kb4014585-x86_4e8fac1a909fd3443b5a1ddd4ae2af5601f1d407.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_mscordacwks_dll          < fver(2,0,50727,3634))
		 || ( _20_mscorjit_dll             < fver(2,0,50727,3634))
		 || ( _20_mscorlib_dll             < fver(2,0,50727,3634))
		 || ( _20_SOS_dll                  < fver(2,0,50727,3634))
		 || ( _20_System_dll               < fver(2,0,50727,3634))
		 || ( _20_System_Windows_Forms_dll < fver(2,0,50727,3634))
		 || ( _20_mscorwks_dll             < fver(2,0,50727,3634))
		/* QFE */
		 || ( _20_mscordacwks_dll          > fver(2,0,50727,3655) && _20_mscordacwks_dll          < fver(2,0,50727,5710))
		 || ( _20_mscorjit_dll             > fver(2,0,50727,3644) && _20_mscorjit_dll             < fver(2,0,50727,5710))
		 || ( _20_mscorlib_dll             > fver(2,0,50727,3655) && _20_mscorlib_dll             < fver(2,0,50727,5710))
		 || ( _20_SOS_dll                  > fver(2,0,50727,3655) && _20_SOS_dll                  < fver(2,0,50727,5710))
		 || ( _20_System_dll               > fver(2,0,50727,3677) && _20_System_dll               < fver(2,0,50727,5710))
		 || ( _20_System_Windows_Forms_dll > fver(2,0,50727,3645) && _20_System_Windows_Forms_dll < fver(2,0,50727,5710))
		 || ( _20_mscorwks_dll             > fver(2,0,50727,3655) && _20_mscorwks_dll             < fver(2,0,50727,5710)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2604092)");
		XX(p3+"NDP20SP2-KB2604092-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_aspnet_wp_exe                     < fver(2,0,50727,3657))
		 || ( _20_System_Web_dll                    < fver(2,0,50727,3657))
		 || ( _20_System_Web_Mobile_dll             < fver(2,0,50727,3657))
		 || ( _20_System_Web_RegularExpressions_dll < fver(2,0,50727,3657))
		 || ( _20_webengine_dll                     < fver(2,0,50727,3657))
		 || ( _20_System_dll                        < fver(2,0,50727,3614))
		/* QFE */
		 || ( _20_aspnet_wp_exe                     > fver(2,0,50727,3658) && _20_aspnet_wp_exe                     < fver(2,0,50727,7043))
		 || ( _20_System_Web_dll                    > fver(2,0,50727,3658) && _20_System_Web_dll                    < fver(2,0,50727,7043))
		 || ( _20_System_Web_Mobile_dll             > fver(2,0,50727,3657) && _20_System_Web_Mobile_dll             < fver(2,0,50727,7043))
		 || ( _20_System_Web_RegularExpressions_dll > fver(2,0,50727,3658) && _20_System_Web_RegularExpressions_dll < fver(2,0,50727,7043))
		 || ( _20_webengine_dll                     > fver(2,0,50727,3658) && _20_webengine_dll                     < fver(2,0,50727,7043))
		 || ( _20_System_dll                        > fver(2,0,50727,3677) && _20_System_dll                        < fver(2,0,50727,4443)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 (KB2894843)");
		XX(p3+"ndp20sp2-kb2894843-x86_36228a0825744e86a31a0d0d020ada3b2d0da9fa.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
			( _20_mscordacwks_dll < fver(2,0,50727,8790))
		 || ( _20_mscorlib_dll    < fver(2,0,50727,8790))
		 || ( _20_SOS_dll         < fver(2,0,50727,8790))
		 || ( _20_mscorwks_dll    < fver(2,0,50727,8790)) )) {
		NN("2018-07 Security Only Update for .NET Framework 2.0 on WES09 and POSReady 2009 (KB4338615)");
		XX(p3+"ndp20sp2-kb4338615-x86_2efbd12454fd3710958d88de466e1902eefca318.exe"+n6);
	}
//	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && ( // KB2863239 is superceded by KB3135986
//		/* GDR */
//			( _20_System_Security_dll < fver(2,0,50727,3652))
//		/* QFE */
//		 || ( _20_System_Security_dll > fver(2,0,50727,3675) && _20_System_Security_dll < fver(2,0,50727,7032)) )) {
//		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2863239)");
//		XX(p+"NDP20SP2-KB2863239-x86.exe"+n6);
//	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_System_Security_dll < fver(2,0,50727,3675))
		/* QFE */
		 || ( _20_System_Security_dll > fver(2,0,50727,3675) && _20_System_Security_dll < fver(2,0,50727,8683)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 for WEPOS and POSReady 2009 (KB3135986)");
		XX(p3+"ndp20sp2-kb3135986-x86_3c143b65f9e2cc3ab1f140fbd71a50f34af88a49.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_dfdll_dll                      < fver(2,0,50727,3663))
		 || ( _20_System_DeploymentFramework_dll > zero && _20_System_DeploymentFramework_dll < fver(2,0,50727,3663))
		/* QFE */
		 || ( _20_dfdll_dll                      > fver(2,0,50727,3663) && _20_dfdll_dll                      < fver(2,0,50727,8641))
		 || ( _20_System_DeploymentFramework_dll > fver(2,0,50727,3663) && _20_System_DeploymentFramework_dll < fver(2,0,50727,8641)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 (KB2979574)");
		XX(p3+"ndp20sp2-kb2979574-v2-x86_e9d7e292cff96f768a99c2f2dbeba9ca14784c70.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
			( _20_System_Management_dll < fver(2,0,50727,8758))
		 || ( _20_WMINet_Utils_dll      < fver(2,0,50727,8758)) )) {
		NN("April, 2017 Security and Quality Rollup for .NET Framework 2.0 SP2 on WES09 and POSReady 2009 (KB4014580)");
		XX(p3+"ndp20sp2-kb4014580-x86_00bc481a903b582272cae81e76334295a1209f73.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* No GDR */
		/* QFE */
			( _20_System_Data_SqlXml_dll < fver(2,0,50727,8776))
		 || ( _20_System_XML_dll         < fver(2,0,50727,8776)) )) {
		NN("2018-01 Security Only Update for .NET Framework 2.0 on WES09 and POSReady 2009 (KB4054178)");
		XX(p3+"ndp20sp2-kb4054178-x86_c0d933409d0128ad7df07c0eaf71ae01f49c48c3.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx20 || *v_nfx20==2) && (
		/* GDR */
			( _20_mscorjit_dll                           < fver(2,0,50727,3644))
		 || ( _20_peverify_dll                           < fver(2,0,50727,3644))
		 || ( _20_System_Design_dll                      < fver(2,0,50727,3644))
		 || ( _20_System_DirectoryServices_dll           < fver(2,0,50727,3644))
		 || ( _20_System_DirectoryServices_Protocols_dll < fver(2,0,50727,3644))
		 || ( _20_System_dll                             < fver(2,0,50727,3644))
		 || ( _20_System_Drawing_dll                     < fver(2,0,50727,3644))
		 || ( _20_System_Security_dll                    < fver(2,0,50727,3644))
		 || ( _20_System_Windows_Forms_dll               < fver(2,0,50727,3644))
		/* QFE */
		 || ( _20_mscorjit_dll                           > fver(2,0,50727,3644) && _20_mscorjit_dll                           < fver(2,0,50727,5740))
		 || ( _20_peverify_dll                           > fver(2,0,50727,3644) && _20_peverify_dll                           < fver(2,0,50727,5740))
		 || ( _20_System_Design_dll                      > fver(2,0,50727,3645) && _20_System_Design_dll                      < fver(2,0,50727,5740))
		 || ( _20_System_DirectoryServices_dll           > fver(2,0,50727,3644) && _20_System_DirectoryServices_dll           < fver(2,0,50727,5740))
		 || ( _20_System_DirectoryServices_Protocols_dll > fver(2,0,50727,3644) && _20_System_DirectoryServices_Protocols_dll < fver(2,0,50727,5740))
		 || ( _20_System_dll                             > fver(2,0,50727,3677) && _20_System_dll                             < fver(2,0,50727,5740))
		 || ( _20_System_Drawing_dll                     > fver(2,0,50727,3674) && _20_System_Drawing_dll                     < fver(2,0,50727,5740))
		 || ( _20_System_Security_dll                    > fver(2,0,50727,3675) && _20_System_Security_dll                    < fver(2,0,50727,5740))
		 || ( _20_System_Windows_Forms_dll               > fver(2,0,50727,3645) && _20_System_Windows_Forms_dll               < fver(2,0,50727,5740)) )) {
		NN("Security Update for Microsoft .NET Framework 2.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2742596)");
		XX(p3+"NDP20SP2-KB2742596-x86.exe"+n6);
	}

	// .NET Framework 3.0
//	if( sp==2 &&  (*i_nfx30 || *v_nfx30==2) && (
//		/* GDR */
//			( _30_System_Runtime_Serialization_dll   < fver(3,0,4506,3613))
//		 || ( _30_System_ServiceModel_dll            < fver(3,0,4506,3613))
//		/* QFE */
//		 || ( _30_ServiceMonikerSupport_dll          > fver(3,0,4506,4037) && _30_ServiceMonikerSupport_dll          < fver(3,0,4506,4426))
//		 || ( _30_SMdiagnostics_dll                  > fver(3,0,4506,4037) && _30_SMdiagnostics_dll                  < fver(3,0,4506,4426))
//		 || ( _30_System_IdentityModel_dll           > fver(3,0,4506,4037) && _30_System_IdentityModel_dll           < fver(3,0,4506,4426))
//		 || ( _30_System_Runtime_Serialization_dll   > fver(3,0,4506,4037) && _30_System_Runtime_Serialization_dll   < fver(3,0,4506,4426))
//		 || ( _30_System_ServiceModel_dll            > fver(3,0,4506,4037) && _30_System_ServiceModel_dll            < fver(3,0,4506,4426)) )) {
//		NN("Microsoft .NET Framework 3.0 Service Pack 2 Update for Windows Server 2003 and Windows XP x86 (KB976570)");
//		XX(p+"ndp30sp2-kb976570-x86_dcff59f3947e2c3d4f88e72959e6b5bb40c5b1d7.exe"+n6);
//	}
	if( sp==2 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* GDR */
			( _30_SMdiagnostics_dll                  < fver(3,0,4506,3636))
		 || ( _30_System_IdentityModel_dll           < fver(3,0,4506,3636))
		 || ( _30_System_Runtime_Serialization_dll   < fver(3,0,4506,3636))
		 || ( _30_System_ServiceModel_dll            < fver(3,0,4506,3636))
		 || ( _30_System_ServiceModel_WasHosting_dll < fver(3,0,4506,3636))
		/* QFE */
		 || ( _30_ServiceMonikerSupport_dll          > fver(3,0,4506,4037) && _30_ServiceMonikerSupport_dll          < fver(3,0,4506,4532))
		 || ( _30_SMdiagnostics_dll                  > fver(3,0,4506,4037) && _30_SMdiagnostics_dll                  < fver(3,0,4506,4532))
		 || ( _30_System_IdentityModel_dll           > fver(3,0,4506,4037) && _30_System_IdentityModel_dll           < fver(3,0,4506,4532))
		 || ( _30_System_Runtime_Serialization_dll   > fver(3,0,4506,4037) && _30_System_Runtime_Serialization_dll   < fver(3,0,4506,4532))
		 || ( _30_System_ServiceModel_dll            > fver(3,0,4506,4037) && _30_System_ServiceModel_dll            < fver(3,0,4506,4532))
		 || ( _30_System_ServiceModel_WasHosting_dll > fver(3,0,4506,4037) && _30_System_ServiceModel_WasHosting_dll < fver(3,0,4506,4532)) )) {
		NN("Microsoft .NET Framework 3.5 SP1 Update for Windows Server 2003 and Windows XP x86 (KB982168)");
		XX(p2+"ndp30sp2-kb982168-x86_55b498e96d1ea7db2d25302f30fc7f8fb5441158.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* GDR */
			( _30_PenIMC_dll                < fver(3,0,6920,4050))
		 || ( _30_PresentationHostDLL_dll   < fver(3,0,6920,4050))
		 || ( _30_XPSViewer_exe             < fver(3,0,6920,4050))
		 || ( _30_PresentationCore_dll      < fver(3,0,6920,4050))
		 || ( _30_PresentationFramework_dll < fver(3,0,6920,4050))
		 || ( _30_ReachFramework_dll        < fver(3,0,6920,4050))
		 || ( _30_System_Printing_dll       < fver(3,0,6920,4050))
		 || ( _30_WindowsBase_dll           < fver(3,0,6920,4050))
		 || ( _30_wpfgfx_v0300_dll          < fver(3,0,6920,4050))
		/* QFE */
		 || ( _30_PenIMC_dll                > fver(3,0,6920,4050) && _30_PenIMC_dll                < fver(3,0,6920,7045))
		 || ( _30_PresentationHostDLL_dll   > fver(3,0,6920,4050) && _30_PresentationHostDLL_dll   < fver(3,0,6920,7045))
		 || ( _30_XPSViewer_exe             > fver(3,0,6920,4050) && _30_XPSViewer_exe             < fver(3,0,6920,7045))
		 || ( _30_PresentationCore_dll      > fver(3,0,6920,4050) && _30_PresentationCore_dll      < fver(3,0,6920,7045))
		 || ( _30_PresentationFramework_dll > fver(3,0,6920,4050) && _30_PresentationFramework_dll < fver(3,0,6920,7045))
		 || ( _30_ReachFramework_dll        > fver(3,0,6920,4050) && _30_ReachFramework_dll        < fver(3,0,6920,7045))
		 || ( _30_System_Printing_dll       > fver(3,0,6920,4050) && _30_System_Printing_dll       < fver(3,0,6920,7045))
		 || ( _30_WindowsBase_dll           > fver(3,0,6920,4050) && _30_WindowsBase_dll           < fver(3,0,6920,7045))
		 || ( _30_wpfgfx_v0300_dll          > fver(3,0,6920,4050) && _30_wpfgfx_v0300_dll          < fver(3,0,6920,7045)) )) {
		NN("Security Update for Microsoft .NET Framework 3.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2832411)");
		XX(p3+"NDP30SP2-KB2832411-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* GDR */
			( _30_PresentationCFFRasterizerNative_v0300_dll < fver(3,0,6920,4058))
		/* QFE */
		 || ( _30_PresentationCFFRasterizerNative_v0300_dll > fver(3,0,6920,4058)
		     && _30_PresentationCFFRasterizerNative_v0300_dll < fver(3,0,6920,7061)) )) {
		NN("Security Update for Microsoft .NET Framework 3.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2861189)");
		XX(p3+"NDP30SP2-KB2861189-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* No GDR */
		/* QFE */
		    ( _30_PenIMC_dll                < fver(3,0,6920,8721))
		 || ( _30_PresentationHostDLL_dll   < fver(3,0,6920,8721))
		 || ( _30_XPSViewer_exe             < fver(3,0,6920,8721))
		 || ( _30_PresentationCore_dll      < fver(3,0,6920,8721))
		 || ( _30_PresentationFramework_dll < fver(3,0,6920,8721))
		 || ( _30_ReachFramework_dll        < fver(3,0,6920,8721))
		 || ( _30_System_Printing_dll       < fver(3,0,6920,8721))
		 || ( _30_WindowsBase_dll           < fver(3,0,6920,8721))
		 || ( _30_wpfgfx_v0300_dll          < fver(3,0,6920,8721)) )) {
		NN("November, 2016 Security and Quality Rollup for .NET Framework 3.0, 4 on WES09 and POSReady (KB3189598)");
		XX(p3+"ndp30sp2-kb3188734-x86_1a1e2f42b2aaf963a2d79a8072f08e29243a3330.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* No GDR */
		/* QFE */
		    ( _30_ServiceMonikerSupport_dll          < fver(3,0,4506,8796))
		 || ( _30_SMdiagnostics_dll                  < fver(3,0,4506,8796))
		 || ( _30_SMSvcHost_exe                      < fver(3,0,4506,8796))
		 || ( _30_System_IdentityModel_dll           < fver(3,0,4506,8796))
		 || ( _30_System_Runtime_Serialization_dll   < fver(3,0,4506,8796))
		 || ( _30_System_ServiceModel_dll            < fver(3,0,4506,8796))
		 || ( _30_System_ServiceModel_WasHosting_dll < fver(3,0,4506,8796)) )) {
		NN("2018-01 Security Only Update for .NET Framework 3.0 on WES09 and POSReady 2009 (KB4055229)");
		XX(p3+"ndp30sp2-kb4055229-x86_45c6c6c58de4c40a6ab5c31cca4c685e289493ad.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* No GDR */
		/* QFE */
		    ( _30_PenIMC_dll                < fver(3,0,6920,8774))
		 || ( _30_PresentationHostDLL_dll   < fver(3,0,6920,8774))
		 || ( _30_XPSViewer_exe             < fver(3,0,6920,8774))
		 || ( _30_PresentationCore_dll      < fver(3,0,6920,8774))
		 || ( _30_PresentationFramework_dll < fver(3,0,6920,8774))
		 || ( _30_ReachFramework_dll        < fver(3,0,6920,8774))
		 || ( _30_System_Printing_dll       < fver(3,0,6920,8774))
		 || ( _30_WindowsBase_dll           < fver(3,0,6920,8774))
		 || ( _30_wpfgfx_v0300_dll          < fver(3,0,6920,8774)) )) {
		NN("May, 2017 Security and Quality Rollup for .NET Framework 3.0 SP2 on WES09 and POSReady 2009 (KB4017101)");
		XX(p3+"ndp30sp2-kb4017101-x86_d96aaad1352c554a2740fe30d371144cdb5a5ffa.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* GDR */
			( _30_ServiceMonikerSupport_dll          < fver(3,0,4506,4037))
		 || ( _30_SMdiagnostics_dll                  < fver(3,0,4506,4037))
		 || ( _30_System_IdentityModel_dll           < fver(3,0,4506,4037))
		 || ( _30_System_Runtime_Serialization_dll   < fver(3,0,4506,4037))
		 || ( _30_System_ServiceModel_dll            < fver(3,0,4506,4037))
		 || ( _30_System_ServiceModel_WasHosting_dll < fver(3,0,4506,4037))
		/* QFE */
		 || ( _30_ServiceMonikerSupport_dll          > fver(3,0,4506,4037) && _30_ServiceMonikerSupport_dll          < fver(3,0,4506,5845))
		 || ( _30_SMdiagnostics_dll                  > fver(3,0,4506,4037) && _30_SMdiagnostics_dll                  < fver(3,0,4506,5845))
		 || ( _30_System_IdentityModel_dll           > fver(3,0,4506,4037) && _30_System_IdentityModel_dll           < fver(3,0,4506,5845))
		 || ( _30_System_Runtime_Serialization_dll   > fver(3,0,4506,4037) && _30_System_Runtime_Serialization_dll   < fver(3,0,4506,5845))
		 || ( _30_System_ServiceModel_dll            > fver(3,0,4506,4037) && _30_System_ServiceModel_dll            < fver(3,0,4506,5845))
		 || ( _30_System_ServiceModel_WasHosting_dll > fver(3,0,4506,4037) && _30_System_ServiceModel_WasHosting_dll < fver(3,0,4506,5845)) )) {
		NN("Security Update for Microsoft .NET Framework 3.0 SP2 on Windows Server 2003 and Windows XP x86 (KB2756918)");
		XX(p3+"NDP30SP2-KB2756918-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* No GDR */
		/* QFE */
		    ( _30_ServiceMonikerSupport_dll          < fver(3,0,4506,8801))
		 || ( _30_SMSvcHost_exe                      < fver(3,0,4506,8801))
		 || ( _30_System_IdentityModel_dll           < fver(3,0,4506,8801))
		 || ( _30_System_Runtime_Serialization_dll   < fver(3,0,4506,8801))
		 || ( _30_System_ServiceModel_dll            < fver(3,0,4506,8801))
		 || ( _30_System_ServiceModel_WasHosting_dll < fver(3,0,4506,8801))
		 || ( _30_System_Workflow_Activities_dll     < fver(3,0,4203,8801))
		 || ( _30_System_Workflow_ComponentModel_dll < fver(3,0,4203,8801))
		 || ( _30_System_Workflow_Runtime_dll        < fver(3,0,4203,8801)))) {
		NN("2018-07 Security Only Update for .NET Framework 3.0 on WES09 and POSReady 2009 (KB4338597)");
		XX(p3+"ndp30sp2-kb4338597-x86_7eefe88d95fcdb3b2e2941fdc555d4baeeab5864.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx30 || *v_nfx30==2) && (
		/* No GDR */
		/* QFE */
		    ( _30_System_Workflow_Activities_dll     < fver(3,0,4203,8820))
		 || ( _30_System_Workflow_ComponentModel_dll < fver(3,0,4203,8820))
		 || ( _30_System_Workflow_Runtime_dll        < fver(3,0,4203,8820)))) {
		NN("2019-02 Security Only Update for .NET Framework 3.0 SP2 on WES09 and POSReady 2009 (KB4483495)");
		XX(p3+"ndp30sp2-kb4483495-x86_08c0291774b8f619507a581cfebb8f522d65cb75.exe"+n6);
	}

	// .NET Framework 3.5
//	if( sp>=2 && !*i_nfx35 && *v_nfx35==0 && (
//		/* No GDR */
//		/* QFE */
//		    ( _35_System_Web_Extensions_dll   < fver(3,5,21022,239)) )) {
//		NN("Security Update for Microsoft .NET Framework 3.5 on Windows XP, Windows Server 2003, Windows Vista, Windows Server 2008 x86 (KB2418240)");
//		XX(p2+"NDP35-KB2418240-x86.exe"+n6);
//	}
//	if( sp==3 &&  (*i_nfx35 || *v_nfx35==1) && (
//		/* GDR */
//			( _35_System_Web_Extensions_dll < fver(3,5,30729,4051))
//		/* QFE */
//		 || ( _35_System_Web_Extensions_dll > fver(3,5,30729,4056) && _35_System_Web_Extensions_dll < fver(3,5,30729,7047)) )) {
//	  // KB2836940 is replaced by KB2861697
//		NN("Update for Microsoft .NET Framework 3.5 SP1 on Windows XP, Server 2003, Vista and Server 2008 x86 (KB2836940)");
//		XX(p3+"ndp35sp1-kb2836940-x86_43c873339b256d55a01b8e0303d53153949fdec3.exe"+n6);
//	}
	if( sp==3 &&  (*i_nfx35 || *v_nfx35==1) && (
		/* GDR */
			( _35_System_ServiceModel_Web_dll < fver(3,5,594,4056))
		 || ( _35_System_Web_Extensions_dll   < fver(3,5,30729,4056))
		/* QFE */
		 || ( _35_System_ServiceModel_Web_dll > fver(3,5,594,4056)   && _35_System_ServiceModel_Web_dll < fver(3,5,594,7056))
		 || ( _35_System_Web_Extensions_dll   > fver(3,5,30729,4056) && _35_System_Web_Extensions_dll   < fver(3,5,30729,7056)) )) {
		NN("Security Update for Microsoft .NET Framework 3.5 SP1 on Windows XP, Server 2003, Vista, Server 2008 x86 (KB2861697)");
		XX(p3+"NDP35SP1-KB2861697-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx35 || *v_nfx35==1) && (
		/* No GDR */
		/* QFE */
		    ( _35_System_Web_Extensions_dll   < fver(3,5,30729,8815)) )) {
		NN("2018-12 Security Only Update for .NET Framework 3.5 SP1 on WES09 and POSReady 2009 (KB4470633)");
		XX(p3+"ndp35sp1-kb4470633-x86_dc94f13d3a8c1eaaf30ca31c03a6364758c3b5ab.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx35 || *v_nfx35==1) && (
		/* GDR */
			( _35_DataSvcUtil_exe                 < fver(3,5,30729,4039))
		 || ( _35_System_Data_Services_dll        < fver(3,5,30729,4039))
		 || ( _35_System_Data_Services_Client_dll < fver(3,5,30729,4039))
		 || ( _35_System_Data_Services_Design_dll < fver(3,5,30729,4039))
		/* QFE */
		 || ( _35_DataSvcUtil_exe                 > fver(3,5,30729,4039) && _35_DataSvcUtil_exe                 < fver(3,5,30729,5851))
		 || ( _35_System_Data_Services_dll        > fver(3,5,30729,4039) && _35_System_Data_Services_dll        < fver(3,5,30729,5851))
		 || ( _35_System_Data_Services_Client_dll > fver(3,5,30729,4039) && _35_System_Data_Services_Client_dll < fver(3,5,30729,5851))
		 || ( _35_System_Data_Services_Design_dll > fver(3,5,30729,4039) && _35_System_Data_Services_Design_dll < fver(3,5,30729,5851)) )) {
		NN("Security Update for Microsoft .NET Framework 3.5 SP1 on Windows XP, Server 2003, Vista, Server 2008 x86 (KB2736416)");
		XX(p3+"NDP35SP1-KB2736416-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx35 || *v_nfx35==1) && (
		/* GDR */
			( _35_System_Data_Linq_dll < fver(3,5,30729,4052))
		/* QFE */
		 || ( _35_System_Data_Linq_dll > fver(3,5,30729,4052) && _35_System_Data_Linq_dll < fver(3,5,30729,7049)) )) {
		NN("Security Update for Microsoft .NET Framework 3.5 SP1 on Windows XP, Server 2003, Vista, Server 2008 x86 (KB2840629)");
		XX(p3+"NDP35SP1-KB2840629-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx35 || *v_nfx35==1) && (
		/* GDR */
			( _35_System_AddIn_dll < fver(3,5,30729,3676))
		/* QFE */
		 || ( _35_System_AddIn_dll > fver(3,5,30729,3676) && _35_System_AddIn_dll < fver(3,5,30729,5766)) )) {
		NN("Security Update for Microsoft .NET Framework 3.5 SP1 on Windows XP, Server 2003, Vista, Server 2008 x86 (KB2604111)");
		XX(p3+"NDP35SP1-KB2604111-x86.exe"+n6);
	}
	if( sp>=2 && ( *i_nfx35 || ( *v_nfx35==1  && (
			( _35_chrome_jar      > -1 && _35_chrome_jar      != 10010 )
		 || ( _35_chrome_manifest > -1 && _35_chrome_manifest != 1762  )
		 || ( _35_defaults_js     > -1 && _35_defaults_js     != 10351 )
		 || ( _35_install_rdf     > -1 && _35_install_rdf     != 1071  ))))) {
		NN("Update to .NET Framework 3.5 Service Pack 1 for the .NET Framework Assistant 1.0 x86 (KB963707)");
		XX(p3+"NDP35SP1-KB963707-x86.exe"+n6);
	}
	if( sp>=2 &&  (*i_nfx20 || *i_nfx35 || *v_nfx20==2 || *v_nfx35==1 ) && (
		/* GDR */
			( _dfshim_dll                           < fver(4,0,31106,0))
		 || ( _mscoree_dll                          < fver(4,0,31106,0))
		 || ( _mscorees_dll                         < fver(4,0,31106,0))
		 || ( _netfxperf_dll                        < fver(4,0,31106,0))
		 || ( _NETFXSBS10_exe                       < fver(4,0,31106,0))
		 || ( _sbs_diasymreader_dll                 < fver(4,0,30902,0))
		 || ( _sbs_microsoft_jscript_dll            < fver(4,0,30902,0))
		 || ( _sbs_mscordbi_dll                     < fver(4,0,30902,0))
		 || ( _sbs_mscorrc_dll                      < fver(4,0,30902,0))
		 || ( _sbs_mscorsec_dll                     < fver(4,0,30902,0))
		 || ( _sbs_system_configuration_install_dll < fver(4,0,30902,0))
		 || ( _sbs_system_data_dll                  < fver(4,0,30902,0))
		 || ( _sbs_system_enterpriseservices_dll    < fver(4,0,30902,0))
		 || ( _sbs_wminet_utils_dll                 < fver(4,0,30902,0))
		 || ( _sbscmp10_dll                         < fver(4,0,31106,0))
		 || ( _sbscmp20_mscorwks_dll                < fver(4,0,31106,0))
		 || ( _sbscmp20_mscorlib_dll                < fver(4,0,31106,0))
		 || ( _sbscmp20_perfcounter_dll             < fver(4,0,31106,0))
		 || ( _SharedReg12_dll                      < fver(4,0,31106,0))
		/* QFE */
		 || ( _sbs_diasymreader_dll                 > fver(4,0,30902,0) && _sbs_diasymreader_dll                 < fver(4,0,31106,0))
		 || ( _sbs_microsoft_jscript_dll            > fver(4,0,30902,0) && _sbs_microsoft_jscript_dll            < fver(4,0,31106,0))
		 || ( _sbs_mscordbi_dll                     > fver(4,0,30902,0) && _sbs_mscordbi_dll                     < fver(4,0,31106,0))
		 || ( _sbs_mscorrc_dll                      > fver(4,0,30902,0) && _sbs_mscorrc_dll                      < fver(4,0,31106,0))
		 || ( _sbs_mscorsec_dll                     > fver(4,0,30902,0) && _sbs_mscorsec_dll                     < fver(4,0,31106,0))
		 || ( _sbs_system_configuration_install_dll > fver(4,0,30902,0) && _sbs_system_configuration_install_dll < fver(4,0,31106,0))
		 || ( _sbs_system_data_dll                  > fver(4,0,30902,0) && _sbs_system_data_dll                  < fver(4,0,31106,0))
		 || ( _sbs_system_enterpriseservices_dll    > fver(4,0,30902,0) && _sbs_system_enterpriseservices_dll    < fver(4,0,31106,0))
		 || ( _sbs_wminet_utils_dll                 > fver(4,0,30902,0) && _sbs_wminet_utils_dll                 < fver(4,0,31106,0)) )) {
		NN("Microsoft .NET Framework 3.5 SP1 and .NET Framework 2.0 SP2 Update for Windows Server 2003 and Windows XP x86 (KB982524)");
		XX(p3+"NDP20SP2-KB976576-x86.exe"+n6);
		// Article http://support.microsoft.com/kb/982524/
		// Rolls up KB976576 and KB977354
	}
	if( sp>=2 &&  (*i_nfx30 || *v_nfx30==2 ) && (
		/* GDR */
			( _30_PresentationCore_dll      < fver(3,0,6920,4016))
		 || ( _30_PresentationFramework_dll < fver(3,0,6920,4016))
		 || ( _30_PresentationHostDLL_dll   < fver(3,0,6920,4016))
		 || ( _30_WindowsBase_dll           < fver(3,0,6920,4016))
		 || ( _PresentationHostProxy_dll    < fver(4,0,31106,0))
		 || ( _PresentationHost_exe         < fver(4,0,40305,0))		 
		/* QFE */
		 || ( _30_PresentationCore_dll      > fver(3,0,6920,4016) && _30_PresentationCore_dll      < fver(3,0,6920,4030))
		 || ( _30_PresentationFramework_dll > fver(3,0,6920,4016) && _30_PresentationFramework_dll < fver(3,0,6920,4030))
		 || ( _30_PresentationHostDLL_dll   > fver(3,0,6920,4016) && _30_PresentationHostDLL_dll   < fver(3,0,6920,4030))
		 || ( _30_WindowsBase_dll           > fver(3,0,6920,4016) && _30_WindowsBase_dll           < fver(3,0,6920,4030)) )) {
		NN("Microsoft .NET Framework 3.5 SP1 and .NET Framework 2.0 SP2 Update for Windows Server 2003 and Windows XP x86 (KB982524)");
		XX(p3+"ndp30sp2-kb977354-v2-x86_36d5f7b86e33e09d4e1934c43bbfab04c64a280e.exe"+n6);
		// Article http://support.microsoft.com/kb/982524/
		// Rolls up KB976576 and KB977354
	}
	if( sp>=2 &&  (*i_nfx35 || *v_nfx35==1) && (
		    ( _ntprint_cat != 1089593 && _ntprint_cat != 1089601 )) ) {
		NN("Update for Windows XP (KB961118)");
		XX(p3+"WindowsXP-KB961118-x86-ENU.exe"+n1);
	}

	if( sp>=2 &&  (*i_nfx20 || *v_nfx20==2) && (
			( _20_aspnet_wp_exe                < fver(2,0,50727,3082))
		 || ( _20_mscordacwks_dll              < fver(2,0,50727,3082))
		 || ( _20_mscorjit_dll                 < fver(2,0,50727,3082))
		 || ( _20_mscorlib_dll                 < fver(2,0,50727,3082))
		 || ( _20_System_Data_OracleClient_dll < fver(2,0,50727,3082))
		 || ( _20_System_XML_dll               < fver(2,0,50727,3082))
		 || ( _20_mscorwks_dll                 < fver(2,0,50727,3082))
		 || ( _20_webengine_dll                < fver(2,0,50727,3082))
		 || ( _20_System_Web_dll               < fver(2,0,50727,3082)) )) {
		NN("Application Compatibility Update for the .NET Framework 2.0 SP2 (KB958481)");
		XX(p3+"NDP20SP2-KB958481-x86.exe"+n6);
	}
	if( sp>=2 &&  (*i_nfx30 || *v_nfx30==2) && (
			( _30_System_ServiceModel_dll   < fver(3,0,4506,2254))
		 || ( _30_wpfgfx_v0300_dll          < fver(3,0,6920,1500))
		 || ( _30_PresentationFramework_dll < fver(3,0,6920,1500)) )) {
		NN("Application Compatibility Update for the .NET Framework 3.0 SP2 (KB958483)");
		XX(p3+"NDP30SP2-KB958483-x86.exe"+n6);
	}
	if( sp>=2 &&  (*i_nfx35 || *v_nfx35==1) && (
			( _35_System_Data_Services_Client_dll < fver(3,5,30729,196))
		 || ( _35_System_Data_Services_dll        < fver(3,5,30729,196))
		 || ( _35_System_Web_Entity_dll           < fver(3,5,30729,196))
		 || ( _35_System_Web_DynamicData_dll      < fver(3,5,30729,196))
		 || ( _35_System_Web_Extensions_dll       < fver(3,5,30729,196)) )) {
		NN("Application Compatibility Update for the .NET Framework 3.5 SP1 (KB958484)");
		XX(p3+"NDP35SP1-KB958484-x86.exe"+n6);
	}

	// .NET Framework 4.0
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_System_dll < fver(4,0,30319,258))
		/* QFE */
		 || ( _40_System_dll > fver(4,0,30319,258) && _40_System_dll < fver(4,0,30319,523))
		 || regQueryValue(L"SOFTWARE\\Microsoft\\Updates\\Microsoft .NET Framework 4 Client Profile\\KB2633870",
							L"ThisVersionInstalled",&status)!=L"Y" )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2633870)");
		XX(p3+"NDP40-KB2633870-x86.exe"+n6);
	}
	if( sp==3 &&  (NFX40) && (
		/* GDR */
			( _40_ServiceModelReg_exe              < fver(4,0,30319,272))
		 || ( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,272)) // Full Profile Only
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,272)) // Full Profile Only
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,272)) // Full Profile Only
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,272)) // Full Profile Only
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,272)) // Full Profile Only
		/* QFE */
		 || ( _40_ServiceModelReg_exe              > fver(4,0,30319,272) && _40_ServiceModelReg_exe              < fver(4,0,30319,547))
		 || ( _40_aspnet_wp_exe                    > fver(4,0,30319,272) && _40_aspnet_wp_exe                    < fver(4,0,30319,547))
		 || ( _40_System_Web_Extensions_dll        > fver(4,0,30319,272) && _40_System_Web_Extensions_dll        < fver(4,0,30319,547))
		 || ( _40_System_Web_dll                   > fver(4,0,30319,272) && _40_System_Web_dll                   < fver(4,0,30319,547))
		 || ( _40_webengine4_dll                   > fver(4,0,30319,272) && _40_webengine4_dll                   < fver(4,0,30319,547))
		 || ( _40_webengine_dll                    > fver(4,0,30319,272) && _40_webengine_dll                    < fver(4,0,30319,547))
	     || regQueryValue(L"SOFTWARE\\Microsoft\\Updates\\Microsoft .NET Framework 4 Client Profile\\KB2656351",
							L"ThisVersionInstalled",&status)!=L"Y" )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2656351)");
		XX(p3+"NDP40-KB2656351-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_clr_dll	                       < fver(4,0,30319,239))
		 || ( _40_mscordacwks_dll                  < fver(4,0,30319,239))
		 || ( _40_mscordbi_dll                     < fver(4,0,30319,239))
		 || ( _40_mscorlib_dll                     < fver(4,0,30319,239))
		 || ( _40_nlssorting_dll                   < fver(4,0,30319,239))
		 || ( _40_SOS_dll                          < fver(4,0,30319,239))
		/* QFE */
		 || ( _40_clr_dll                          > fver(4,0,30319,239) && _40_clr_dll                          < fver(4,0,30319,488))
		 || ( _40_mscordacwks_dll                  > fver(4,0,30319,239) && _40_mscordacwks_dll                  < fver(4,0,30319,488))
		 || ( _40_mscordbi_dll                     > fver(4,0,30319,239) && _40_mscordbi_dll                     < fver(4,0,30319,488))
		 || ( _40_mscorlib_dll                     > fver(4,0,30319,239) && _40_mscorlib_dll                     < fver(4,0,30319,488))
		 || ( _40_nlssorting_dll                   > fver(4,0,30319,239) && _40_nlssorting_dll                   < fver(4,0,30319,488))
		 || ( _40_SOS_dll                          > fver(4,0,30319,239) && _40_SOS_dll                          < fver(4,0,30319,488))
	     || regQueryValue(L"SOFTWARE\\Microsoft\\Updates\\Microsoft .NET Framework 4 Client Profile\\KB2572078",
							L"ThisVersionInstalled",&status)!=L"Y" )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2572078)");
		XX(p3+"NDP40-KB2572078-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_clr_dll	                       < fver(4,0,30319,235))
		 || ( _40_clrjit_dll                       < fver(4,0,30319,235))
		 || ( _40_mscordacwks_dll                  < fver(4,0,30319,235))
		 || ( _40_mscordbi_dll                     < fver(4,0,30319,235))
		 || ( _40_mscorlib_dll                     < fver(4,0,30319,235))
		 || ( _40_nlssorting_dll                   < fver(4,0,30319,235))
		 || ( _40_SOS_dll                          < fver(4,0,30319,235))
		 || ( _40_System_Windows_Forms_dll         < fver(4,0,30319,235))
		/* QFE */
		 || ( _40_clr_dll                          > fver(4,0,30319,235) && _40_clr_dll                          < fver(4,0,30319,454))
		 || ( _40_clrjit_dll                       > fver(4,0,30319,235) && _40_clrjit_dll                       < fver(4,0,30319,454))
		 || ( _40_mscordacwks_dll                  > fver(4,0,30319,235) && _40_mscordacwks_dll                  < fver(4,0,30319,454))
		 || ( _40_mscordbi_dll                     > fver(4,0,30319,235) && _40_mscordbi_dll                     < fver(4,0,30319,454))
		 || ( _40_mscorlib_dll                     > fver(4,0,30319,235) && _40_mscorlib_dll                     < fver(4,0,30319,454))
		 || ( _40_nlssorting_dll                   > fver(4,0,30319,235) && _40_nlssorting_dll                   < fver(4,0,30319,454))
		 || ( _40_SOS_dll                          > fver(4,0,30319,235) && _40_SOS_dll                          < fver(4,0,30319,454))
		 || ( _40_System_Windows_Forms_dll         > fver(4,0,30319,235) && _40_System_Windows_Forms_dll         < fver(4,0,30319,454))
	     || regQueryValue(L"SOFTWARE\\Microsoft\\Updates\\Microsoft .NET Framework 4 Client Profile\\KB2518870",
							L"ThisVersionInstalled",&status)!=L"Y" )) {
		NN("Security Update for Microsoft .NET Framework 4 on Windows XP, Windows Server 2003, Windows Vista, Windows 7, Windows Server 2008 x86 (KB2518870)");
		XX(p3+"NDP40-KB2518870-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_System_Windows_Forms_dll < fver(4,0,30319,1002))
		/* QFE */
		 || ( _40_System_Windows_Forms_dll > fver(4,0,30319,1002) && _40_System_Windows_Forms_dll < fver(4,0,30319,2003)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2789642)");
		XX(p3+"NDP40-KB2789642-x86.exe"+n6);
	}
//	if( sp==3 &&  (NFX40) && (
//		/* GDR */
//			( _40_ServiceModelReg_exe              < fver(4,0,30319,1016))
//		 || ( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,1016)) // Full Profile Only
//		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,1016)) // Full Profile Only
//		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,1016)) // Full Profile Only
//		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,1016)) // Full Profile Only
//		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,1016)) // Full Profile Only
//		/* QFE */
//		 || ( _40_ServiceModelReg_exe              > fver(4,0,30319,1025) && _40_ServiceModelReg_exe              < fver(4,0,30319,2026))
//		 || ( _40_aspnet_wp_exe                    > fver(4,0,30319,1025) && _40_aspnet_wp_exe                    < fver(4,0,30319,2026))
//		 || ( _40_System_Web_Extensions_dll        > fver(4,0,30319,1025) && _40_System_Web_Extensions_dll        < fver(4,0,30319,2026))
//		 || ( _40_System_Web_dll                   > fver(4,0,30319,1025) && _40_System_Web_dll                   < fver(4,0,30319,2026))
//		 || ( _40_webengine4_dll                   > fver(4,0,30319,1025) && _40_webengine4_dll                   < fver(4,0,30319,2026))
//		 || ( _40_webengine_dll                    > fver(4,0,30319,1025) && _40_webengine_dll                    < fver(4,0,30319,2026)) )) {
//		 // KB2836939 is replaced by KB2858302
//		NN("Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2836939)");
//		XX(p3+"ndp40-kb2836939-v3-x86_5f7e7f9811136bd3dfeb994d75a96aece646ea3d.exe"+n6);
//	}
	if( sp==3 &&  (NFX40) && (
		/* GDR */
			( _40_ServiceModelReg_exe              < fver(4,0,30319,1016))
		 || ( _40_System_Runtime_Serialization_dll < fver(4,0,30319,1016))
		 || ( _40_System_Security_dll              < fver(4,0,30319,1016))
		 || ( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,1016)) // Full Profile Only
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,1016)) // Full Profile Only
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,1016)) // Full Profile Only
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,1016)) // Full Profile Only
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,1016)) // Full Profile Only
		/* QFE */
		 || ( _40_ServiceModelReg_exe              > fver(4,0,30319,1025) && _40_ServiceModelReg_exe              < fver(4,0,30319,2026))
		 || ( _40_System_Runtime_Serialization_dll > fver(4,0,30319,1026) && _40_System_Runtime_Serialization_dll < fver(4,0,30319,2026))
		 || ( _40_System_Security_dll              > fver(4,0,30319,1051) && _40_System_Security_dll              < fver(4,0,30319,2026))
		 || ( _40_aspnet_wp_exe                    > fver(4,0,30319,1025) && _40_aspnet_wp_exe                    < fver(4,0,30319,2026))
		 || ( _40_System_Web_Extensions_dll        > fver(4,0,30319,1025) && _40_System_Web_Extensions_dll        < fver(4,0,30319,2026))
		 || ( _40_System_Web_dll                   > fver(4,0,30319,1025) && _40_System_Web_dll                   < fver(4,0,30319,2026))
		 || ( _40_webengine4_dll                   > fver(4,0,30319,1025) && _40_webengine4_dll                   < fver(4,0,30319,2026))
		 || ( _40_webengine_dll                    > fver(4,0,30319,1025) && _40_webengine_dll                    < fver(4,0,30319,2026)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2858302)");
		XX(p3+"NDP40-KB2858302-v2-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_PenIMC_dll                 < fver(4,0,30319,2091))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,2091))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,2091))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,2091))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,2091))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,2091))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,2091)) )) {
		NN("November, 2016 Security and Quality Rollup for .NET Framework 3.0, 4 on WES09 and POSReady (KB3189598)");
		XX(p3+"ndp40-kb3189017-x86_3100ea6231780b202c1bbc764cd460a8d84bc542.exe"+n6);
	}
//	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
//		/* GDR */
//			( _40_clr_dll	                       < fver(4,0,30319,1022))
//		 || ( _40_mscordacwks_dll                  < fver(4,0,30319,1022))
//		 || ( _40_mscordbi_dll                     < fver(4,0,30319,1022))
//		 || ( _40_mscorlib_dll                     < fver(4,0,30319,1022))
//		 || ( _40_nlssorting_dll                   < fver(4,0,30319,1022))
//		 || ( _40_SOS_dll                          < fver(4,0,30319,1022))
//		/* QFE */
//		 || ( _40_clr_dll                          > fver(4,0,30319,1026) && _40_clr_dll                          < fver(4,0,30319,2034))
//		 || ( _40_mscordacwks_dll                  > fver(4,0,30319,1026) && _40_mscordacwks_dll                  < fver(4,0,30319,2034))
//		 || ( _40_mscordbi_dll                     > fver(4,0,30319,1026) && _40_mscordbi_dll                     < fver(4,0,30319,2034))
//		 || ( _40_mscorlib_dll                     > fver(4,0,30319,1026) && _40_mscorlib_dll                     < fver(4,0,30319,2034))
//		 || ( _40_nlssorting_dll                   > fver(4,0,30319,1026) && _40_nlssorting_dll                   < fver(4,0,30319,2034))
//		 || ( _40_SOS_dll                          > fver(4,0,30319,1026) && _40_SOS_dll                          < fver(4,0,30319,2034)) )) {
//	 // KB2898855 is replaced by KB2972215
//		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2898855)");
//		XX(p3+"NDP40-KB2898855-v2-x86.exe"+n6);
//	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_clr_dll	                       < fver(4,0,30319,1026))
		 || ( _40_mscordacwks_dll                  < fver(4,0,30319,1026))
		 || ( _40_mscordbi_dll                     < fver(4,0,30319,1026))
		 || ( _40_mscorlib_dll                     < fver(4,0,30319,1026))
		 || ( _40_nlssorting_dll                   < fver(4,0,30319,1026))
		 || ( _40_SOS_dll                          < fver(4,0,30319,1026))
		 || ( _40_System_Data_SqlXml_dll           < fver(4,0,30319,1026))
		 || ( _40_System_Runtime_Serialization_dll < fver(4,0,30319,1026))
		 || ( _40_System_XML_dll                   < fver(4,0,30319,1026))
		/* QFE */
		 || ( _40_clr_dll                          > fver(4,0,30319,1026) && _40_clr_dll                          < fver(4,0,30319,2045))
		 || ( _40_mscordacwks_dll                  > fver(4,0,30319,1026) && _40_mscordacwks_dll                  < fver(4,0,30319,2045))
		 || ( _40_mscordbi_dll                     > fver(4,0,30319,1026) && _40_mscordbi_dll                     < fver(4,0,30319,2045))
		 || ( _40_mscorlib_dll                     > fver(4,0,30319,1026) && _40_mscorlib_dll                     < fver(4,0,30319,2045))
		 || ( _40_nlssorting_dll                   > fver(4,0,30319,1026) && _40_nlssorting_dll                   < fver(4,0,30319,2045))
		 || ( _40_SOS_dll                          > fver(4,0,30319,1026) && _40_SOS_dll                          < fver(4,0,30319,2045))
		 || ( _40_System_Data_SqlXml_dll           > fver(4,0,30319,1026) && _40_System_Data_SqlXml_dll           < fver(4,0,30319,2045))
		 || ( _40_System_Runtime_Serialization_dll > fver(4,0,30319,1026) && _40_System_Runtime_Serialization_dll < fver(4,0,30319,2045))
		 || ( _40_System_XML_dll                   > fver(4,0,30319,1026) && _40_System_XML_dll                   < fver(4,0,30319,2045)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on Windows Server 2003, Vista, Windows 7, Server 2008 x86 (KB2972215)");
		XX(p3+"ndp40-kb2972215-x86_e29cbc6c4dd603c8965aca1ce75528fe1b345a30.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_System_IdentityModel_dll < fver(4,0,30319,2110))
		 || ( _40_System_ServiceModel_dll  < fver(4,0,30319,2110))
		 || ( _40_System_Data_SqlXml_dll   < fver(4,0,30319,2110))
		 || ( _40_System_XML_dll           < fver(4,0,30319,2110)) )) {
		NN("2018-01 Security Only Update for .NET Framework 4 on WES09 and POSReady 2009 (KB4054173)");
		XX(p3+"ndp40-kb4054173-x86_a223c7d77b86086e3fd412e816edc47e31458f96.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_System_dll < fver(4,0,30319,1053))
		/* QFE */
		 || ( _40_System_dll > fver(4,0,30319,1053) && _40_System_dll < fver(4,0,30319,2088)) )) {
		NN("Security Update for Microsoft .NET Framework 4 for WES09 and POSReady 2009 (KB3142029)");
		XX(p3+"ndp40-kb3142029-x86_78ec90fdf9239c367f154a347cf026a1c27f3e9a.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_System_Drawing_dll < fver(4,0,30319,1047))
		/* QFE */
		 || ( _40_System_Drawing_dll > fver(4,0,30319,1047) && _40_System_Drawing_dll < fver(4,0,30319,2082)) )) {
		NN("Security Update for Microsoft .NET Framework 4 for WEPOS and POSReady 2009 (KB3127225)");
		XX(p3+"ndp40-kb3127225-x86_6086071d07278955e22366990a3f8ffbfdccda85.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_ServiceModelReg_exe < fver(4,0,30319,1022))
		 || ( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,1022)) // Full Profile Only
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,1022)) // Full Profile Only
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,1022)) // Full Profile Only
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,1022)) // Full Profile Only
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,1022)) // Full Profile Only
		/* QFE */
		 || ( _40_ServiceModelReg_exe > fver(4,0,30319,1025) && _40_ServiceModelReg_exe < fver(4,0,30319,2034))
		 || ( _40_aspnet_wp_exe                    > fver(4,0,30319,1025) && _40_aspnet_wp_exe                    < fver(4,0,30319,2034))
		 || ( _40_System_Web_Extensions_dll        > fver(4,0,30319,1025) && _40_System_Web_Extensions_dll        < fver(4,0,30319,2034))
		 || ( _40_System_Web_dll                   > fver(4,0,30319,1025) && _40_System_Web_dll                   < fver(4,0,30319,2034))
		 || ( _40_webengine4_dll                   > fver(4,0,30319,1025) && _40_webengine4_dll                   < fver(4,0,30319,2034))
		 || ( _40_webengine_dll                    > fver(4,0,30319,1025) && _40_webengine_dll                    < fver(4,0,30319,2034)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2901110)");
		XX(p3+"NDP40-KB2901110-v2-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_clr_dll                    < fver(4,0,30319,269))
		 || ( _40_clrjit_dll                 < fver(4,0,30319,269))
		 || ( _40_mscordacwks_dll            < fver(4,0,30319,269))
		 || ( _40_mscordbi_dll               < fver(4,0,30319,269))
		 || ( _40_mscorlib_dll               < fver(4,0,30319,269))
		 || ( _40_nlssorting_dll             < fver(4,0,30319,269))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,269))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,269))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,269))
		 || ( _40_ServiceModelReg_exe        < fver(4,0,30319,269))
		 || ( _40_SOS_dll                    < fver(4,0,30319,269))
		 || ( _40_System_dll                 < fver(4,0,30319,269))
		 || ( _40_System_AddIn_dll           < fver(4,0,30319,269))
		 || ( _40_System_Windows_Forms_dll   < fver(4,0,30319,269))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,269))
		/* QFE */
		 || ( _40_clr_dll                    > fver(4,0,30319,269) && _40_clr_dll                    < fver(4,0,30319,544))
		 || ( _40_clrjit_dll                 > fver(4,0,30319,269) && _40_clrjit_dll                 < fver(4,0,30319,544))
		 || ( _40_mscordacwks_dll            > fver(4,0,30319,269) && _40_mscordacwks_dll            < fver(4,0,30319,544))
		 || ( _40_mscordbi_dll               > fver(4,0,30319,269) && _40_mscordbi_dll               < fver(4,0,30319,544))
		 || ( _40_mscorlib_dll               > fver(4,0,30319,269) && _40_mscorlib_dll               < fver(4,0,30319,544))
		 || ( _40_nlssorting_dll             > fver(4,0,30319,269) && _40_nlssorting_dll             < fver(4,0,30319,544))
		 || ( _40_PresentationCore_dll       > fver(4,0,30319,269) && _40_PresentationCore_dll       < fver(4,0,30319,544))
		 || ( _40_PresentationFramework_dll  > fver(4,0,30319,269) && _40_PresentationFramework_dll  < fver(4,0,30319,544))
		 || ( _40_PresentationHost_v0400_dll > fver(4,0,30319,269) && _40_PresentationHost_v0400_dll < fver(4,0,30319,544))
		 || ( _40_ServiceModelReg_exe        > fver(4,0,30319,269) && _40_ServiceModelReg_exe        < fver(4,0,30319,544))
		 || ( _40_SOS_dll                    > fver(4,0,30319,269) && _40_SOS_dll                    < fver(4,0,30319,544))
		 || ( _40_System_dll                 > fver(4,0,30319,269) && _40_System_dll                 < fver(4,0,30319,544))
		 || ( _40_System_AddIn_dll           > fver(4,0,30319,269) && _40_System_AddIn_dll           < fver(4,0,30319,544))
		 || ( _40_System_Windows_Forms_dll   > fver(4,0,30319,269) && _40_System_Windows_Forms_dll   < fver(4,0,30319,544))
		 || ( _40_WindowsBase_dll            > fver(4,0,30319,269) && _40_WindowsBase_dll            < fver(4,0,30319,544)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2604121)");
		XX(p3+"NDP40-KB2604121-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* No GDR */
		/* QFE */
			( _40_clr_dll                            < fver(4,0,30319,2113))
		 || ( _40_mscordacwks_dll                    < fver(4,0,30319,2113))
		 || ( _40_mscordbi_dll                       < fver(4,0,30319,2113))
		 || ( _40_mscorlib_dll                       < fver(4,0,30319,2113))
		 || ( _40_nlssorting_dll                     < fver(4,0,30319,2113))
		 || ( _40_SOS_dll                            < fver(4,0,30319,2113))
		 || ( _40_System_IdentityModel_dll           < fver(4,0,30319,2113))
		 || ( _40_System_ServiceModel_dll            < fver(4,0,30319,2113))
		 || ( _40_System_Workflow_ComponentModel_dll > zero && _40_System_Workflow_ComponentModel_dll < fver(4,0,30319,2113))      // Full Profile Only
		 || ( _40_System_Workflow_Runtime_dll        > zero && _40_System_Workflow_Runtime_dll        < fver(4,0,30319,2113)) )) { // Full Profile Only
		NN("2018-07 Security Only Update for .NET Framework 4.0 on WES09 and POSReady 2009 (KB4338598)");
		XX(p3+"ndp40-kb4338598-x86_0c01ff2ae51080f9ba371861795bba423b957b1b.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_System_Security_dll < fver(4,0,30319,2111)) )) {
		NN("2018-05 Security Only Update for .NET Framework 4 on WES09 and POSReady 2009 (KB4095528)");
		XX(p3+"ndp40-kb4095528-x86_169b199b7738982df1df89dc8951cf087695165a.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_System_Management_dll < fver(4,0,30319,2097))
		 || ( _40_WMINet_Utils_dll      < fver(4,0,30319,2097)) )) {
		NN("April, 2017 Security and Quality Rollup for .NET Framework 4 on WES09 and POSReady 2009 (KB4014570)");
		XX(p3+"ndp40-kb4014570-x86_da912e6659c218306c1850642966ae13ac114051.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_PenIMC_dll                 < fver(4,0,30319,2102))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,2102))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,2102))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,2102))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,2102))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,2102))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,2102)) )) {
		NN("June, 2017 Security Only Update for .NET Framework 4 on WES09 and POSReady 2009 (KB4021915)");
		XX(p3+"ndp40-kb4021915-x86_e520b6b35c1e6461d73ace5c130ab7ba077296a6.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_PenIMC_dll	             < fver(4,0,30319,1015))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,1015))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,1015))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,1015))
		 || ( _40_System_Data_Linq_dll       < fver(4,0,30319,1015))
		 || ( _40_System_configuration_dll   < fver(4,0,30319,1015))
		 || ( _40_System_Data_SqlXml_dll     < fver(4,0,30319,1015))
		 || ( _40_System_XML_dll             < fver(4,0,30319,1015))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,1015))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,1015))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,1015))
		/* QFE */
		 || ( _40_PenIMC_dll                 > fver(4,0,30319,1051) && _40_PenIMC_dll                 < fver(4,0,30319,2022))
		 || ( _40_PresentationCore_dll       > fver(4,0,30319,1051) && _40_PresentationCore_dll       < fver(4,0,30319,2022))
		 || ( _40_PresentationFramework_dll  > fver(4,0,30319,1051) && _40_PresentationFramework_dll  < fver(4,0,30319,2022))
		 || ( _40_PresentationHost_v0400_dll > fver(4,0,30319,1051) && _40_PresentationHost_v0400_dll < fver(4,0,30319,2022))
		 || ( _40_System_Data_Linq_dll       > fver(4,0,30319,1015) && _40_System_Data_Linq_dll       < fver(4,0,30319,2022))
		 || ( _40_System_configuration_dll   > fver(4,0,30319,1015) && _40_System_configuration_dll   < fver(4,0,30319,2022))
		 || ( _40_System_Data_SqlXml_dll     > fver(4,0,30319,1026) && _40_System_Data_SqlXml_dll     < fver(4,0,30319,2022))
		 || ( _40_System_XML_dll             > fver(4,0,30319,1026) && _40_System_XML_dll             < fver(4,0,30319,2022))
		 || ( _40_WindowsBase_dll            > fver(4,0,30319,1051) && _40_WindowsBase_dll            < fver(4,0,30319,2022))
		 || ( _40_wpfgfx_v0400_dll           > fver(4,0,30319,1051) && _40_wpfgfx_v0400_dll           < fver(4,0,30319,2022))
		 || ( _40_wpftxt_v0400_dll           > fver(4,0,30319,1051) && _40_wpftxt_v0400_dll           < fver(4,0,30319,2022)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2840628)");
		XX(p3+"NDP40-KB2840628-v2-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* No GDR */
		/* QFE */
			( _40_ServiceModelReg_exe       < fver(4,0,30319,2125))
		 || ( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,2125)) // Full Profile Only
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,2125)) // Full Profile Only
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,2125)) // Full Profile Only
		 || ( _40_System_Web_Mobile_dll     > zero && _40_System_Web_Mobile_dll     < fver(4,0,30319,2125)) // Full Profile Only
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,2125)) // Full Profile Only
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,2125)) )) {  // Full Profile Only
		NN("2018-12 Security Only Update for .NET Framework 4.0 on WES09 and POSReady 2009 (KB4470490)");
		XX(p3+"ndp40-kb4470490-x86_ff434d474cd210560f89cef4fda1cbb87224b0db.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_PenIMC_dll	             < fver(4,0,30319,1015))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,1015))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,1015))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,1015))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,1015))
		 || ( _40_WPFFontCache_v0400_exe     < fver(4,0,30319,1015))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,1015))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,1015))
		/* QFE */
		 || ( _40_PenIMC_dll                 > fver(4,0,30319,1051) && _40_PenIMC_dll                 < fver(4,0,30319,2021))
		 || ( _40_PresentationCore_dll       > fver(4,0,30319,1051) && _40_PresentationCore_dll       < fver(4,0,30319,2021))
		 || ( _40_PresentationFramework_dll  > fver(4,0,30319,1051) && _40_PresentationFramework_dll  < fver(4,0,30319,2021))
		 || ( _40_PresentationHost_v0400_dll > fver(4,0,30319,1051) && _40_PresentationHost_v0400_dll < fver(4,0,30319,2021))
		 || ( _40_WindowsBase_dll            > fver(4,0,30319,1051) && _40_WindowsBase_dll            < fver(4,0,30319,2021))
		 || ( _40_WPFFontCache_v0400_exe     > fver(4,0,30319,1015) && _40_WPFFontCache_v0400_exe     < fver(4,0,30319,2021))
		 || ( _40_wpfgfx_v0400_dll           > fver(4,0,30319,1051) && _40_wpfgfx_v0400_dll           < fver(4,0,30319,2021))
		 || ( _40_wpftxt_v0400_dll           > fver(4,0,30319,1051) && _40_wpftxt_v0400_dll           < fver(4,0,30319,2021)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Server 2008 x86 (KB2861188)");
		XX(p3+"NDP40-KB2861188-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_ServiceModelReg_exe       < fver(4,0,30319,1025))
		 || ( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,1025)) // Full Profile Only
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,1025)) // Full Profile Only
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,1025)) // Full Profile Only
		 || ( _40_System_Web_Mobile_dll     > zero && _40_System_Web_Mobile_dll     < fver(4,0,30319,1025)) // Full Profile Only
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,1025)) // Full Profile Only
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,1025)) // Full Profile Only
		/* QFE */
		 || ( _40_ServiceModelReg_exe       > fver(4,0,30319,1025) && _40_ServiceModelReg_exe       < fver(4,0,30319,2042))
		 || ( _40_aspnet_wp_exe             > fver(4,0,30319,1025) && _40_aspnet_wp_exe             < fver(4,0,30319,2042))
		 || ( _40_System_Web_dll            > fver(4,0,30319,1025) && _40_System_Web_dll            < fver(4,0,30319,2042))
		 || ( _40_System_Web_Extensions_dll > fver(4,0,30319,1025) && _40_System_Web_Extensions_dll < fver(4,0,30319,2042))
		 || ( _40_System_Web_Mobile_dll     > fver(4,0,30319,1025) && _40_System_Web_Mobile_dll     < fver(4,0,30319,2042))
		 || ( _40_webengine_dll             > fver(4,0,30319,1025) && _40_webengine_dll             < fver(4,0,30319,2042))
		 || ( _40_webengine4_dll            > fver(4,0,30319,1025) && _40_webengine4_dll            < fver(4,0,30319,2042)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on Windows Server 2003, Vista, Windows 7, Server 2008 x86 (KB2894842)");
		XX(p3+"ndp40-kb2894842-v2-x86_5ce3160ba23ace5b2137c87a1ded990e4503b3f1.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_System_Runtime_Remoting_dll < fver(4,0,30319,2108)) )) {
		NN("2017-10 Security and Quality Rollup for .NET Framework 4 on WES09 and POSReady 2009 (KB4040962)");
		XX(p3+"ndp40-kb4040962-x86_976f8849c17de7a0cab333c0081e9858a6407a85.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_clrjit_dll                             < fver(4,0,30319,1001))
		 || ( _40_peverify_dll > zero && _40_peverify_dll < fver(4,0,30319,1001)) // Full Profile Only
		 || ( _40_ServiceModelReg_exe                    < fver(4,0,30319,1001))
		 || ( _40_System_IdentityModel_dll               < fver(4,0,30319,1001))
		 || ( _40_System_ServiceModel_dll                < fver(4,0,30319,1001))
		 || ( _40_System_DirectoryServices_dll           < fver(4,0,30319,1001))
		 || ( _40_System_DirectoryServices_Protocols_dll < fver(4,0,30319,1001))
		 || ( _40_System_dll                             < fver(4,0,30319,1001))
		 || ( _40_System_Drawing_dll                     < fver(4,0,30319,1001))
		 || ( _40_System_Security_dll                    < fver(4,0,30319,1001))
		 || ( _40_System_Windows_Forms_dll               < fver(4,0,30319,1001))
		/* QFE */
		 || ( _40_clrjit_dll                             > fver(4,0,30319,1001) && _40_clrjit_dll                             < fver(4,0,30319,2001))
		 || ( _40_peverify_dll                           > fver(4,0,30319,1001) && _40_peverify_dll                           < fver(4,0,30319,2001))
		 || ( _40_ServiceModelReg_exe                    > fver(4,0,30319,1025) && _40_ServiceModelReg_exe                    < fver(4,0,30319,2001))
		 || ( _40_System_IdentityModel_dll               > fver(4,0,30319,1001) && _40_System_IdentityModel_dll               < fver(4,0,30319,2001))
		 || ( _40_System_ServiceModel_dll                > fver(4,0,30319,1001) && _40_System_ServiceModel_dll                < fver(4,0,30319,2001))
		 || ( _40_System_DirectoryServices_dll           > fver(4,0,30319,1001) && _40_System_DirectoryServices_dll           < fver(4,0,30319,2001))
		 || ( _40_System_DirectoryServices_Protocols_dll > fver(4,0,30319,1001) && _40_System_DirectoryServices_Protocols_dll < fver(4,0,30319,2001))
		 || ( _40_System_dll                             > fver(4,0,30319,1053) && _40_System_dll                             < fver(4,0,30319,2001))
		 || ( _40_System_Drawing_dll                     > fver(4,0,30319,1047) && _40_System_Drawing_dll                     < fver(4,0,30319,2001))
		 || ( _40_System_Security_dll                    > fver(4,0,30319,1051) && _40_System_Security_dll                    < fver(4,0,30319,2001))
		 || ( _40_System_Windows_Forms_dll               > fver(4,0,30319,1002) && _40_System_Windows_Forms_dll               < fver(4,0,30319,2001)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2742595)");
		XX(p3+"NDP40-KB2742595-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_clr_dll               < fver(4,0,30319,296))
		 || ( _40_mscordacwks_dll       < fver(4,0,30319,296))
		 || ( _40_mscordbi_dll          < fver(4,0,30319,296))
		 || ( _40_mscorlib_dll          < fver(4,0,30319,296))
		 || ( _40_nlssorting_dll        < fver(4,0,30319,296))
		 || ( _40_SOS_dll               < fver(4,0,30319,296))
		 || ( _40_System_dll            < fver(4,0,30319,296))
		 || ( _40_Microsoft_JScript_dll < fver(10,0,30319,296))
		/* QFE */
		 || ( _40_clr_dll               > fver(4,0,30319,296) && _40_clr_dll               < fver(4,0,30319,586))
		 || ( _40_mscordacwks_dll       > fver(4,0,30319,296) && _40_mscordacwks_dll       < fver(4,0,30319,586))
		 || ( _40_mscordbi_dll          > fver(4,0,30319,296) && _40_mscordbi_dll          < fver(4,0,30319,586))
		 || ( _40_mscorlib_dll          > fver(4,0,30319,296) && _40_mscorlib_dll          < fver(4,0,30319,586))
		 || ( _40_nlssorting_dll        > fver(4,0,30319,296) && _40_nlssorting_dll        < fver(4,0,30319,586))
		 || ( _40_SOS_dll               > fver(4,0,30319,296) && _40_SOS_dll               < fver(4,0,30319,586))
		 || ( _40_System_dll            > fver(4,0,30319,296) && _40_System_dll            < fver(4,0,30319,586))
		 || ( _40_Microsoft_JScript_dll > fver(10,0,30319,296)&& _40_Microsoft_JScript_dll < fver(10,0,30319,586)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2729449)");
		XX(p3+"NDP40-KB2729449-x86.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_PenIMC_dll                 < fver(4,0,30319,2098))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,2098))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,2098))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,2098))
		 || ( _40_System_dll                 < fver(4,0,30319,2098))
		 || ( _40_System_Security_dll        < fver(4,0,30319,2098))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,2098))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,2098))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,2098)) )) {
		NN("May, 2017 Security and Quality Rollup for .NET Framework 4 on WES09 and POSReady 2009 (KB4014605)");
		XX(p3+"ndp40-kb4014605-x86_e9b8f1c169c032585d0a74e16aabbbf7d227417d.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* No GDR */
		/* QFE */
			( _40_clr_dll         < fver(4,0,30319,2121))
		 || ( _40_mscordacwks_dll < fver(4,0,30319,2121))
		 || ( _40_mscordbi_dll    < fver(4,0,30319,2121))
		 || ( _40_mscorlib_dll    < fver(4,0,30319,2121))
		 || ( _40_nlssorting_dll  < fver(4,0,30319,2121))
		 || ( _40_SOS_dll         < fver(4,0,30319,2121))
		 || ( _40_System_dll      < fver(4,0,30319,2121)) )) {
		NN("2018-08 Security Only Update for .NET Framework 4.0 on WES09 and POSReady 2009 (KB4462774)");
		XX(p3+"ndp40-kb4462774-x86_3d9d6cc17c34a22b9f1421bf8a8cdbbe9ede2b5c.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_PenIMC_dll                 < fver(4,0,30319,1051))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,1051))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,1051))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,1051))
		 || ( _40_System_Security_dll        < fver(4,0,30319,1051))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,1051))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,1051))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,1051))
		/* QFE */
		 || ( _40_PenIMC_dll                 > fver(4,0,30319,1051) && _40_PenIMC_dll                 < fver(4,0,30319,2086))
		 || ( _40_PresentationCore_dll       > fver(4,0,30319,1051) && _40_PresentationCore_dll       < fver(4,0,30319,2086))
		 || ( _40_PresentationFramework_dll  > fver(4,0,30319,1051) && _40_PresentationFramework_dll  < fver(4,0,30319,2086))
		 || ( _40_PresentationHost_v0400_dll > fver(4,0,30319,1051) && _40_PresentationHost_v0400_dll < fver(4,0,30319,2086))
		 || ( _40_System_Security_dll        > fver(4,0,30319,1051) && _40_System_Security_dll        < fver(4,0,30319,2086))
		 || ( _40_WindowsBase_dll            > fver(4,0,30319,1051) && _40_WindowsBase_dll            < fver(4,0,30319,2086))
		 || ( _40_wpfgfx_v0400_dll           > fver(4,0,30319,1051) && _40_wpfgfx_v0400_dll           < fver(4,0,30319,2086))
		 || ( _40_wpftxt_v0400_dll           > fver(4,0,30319,1051) && _40_wpftxt_v0400_dll           < fver(4,0,30319,2086)) )) {
		NN("Security Update for Microsoft .NET Framework 4 for WEPOS and POSReady 2009 (KB3135993)");
		XX(p3+"ndp40-kb3135993-x86_d7f3f1d40b58935f52381123d60f19b22cfcc872.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_PenIMC_dll                 < fver(4,0,30319,298))
		 || ( _40_PresentationCore_dll       < fver(4,0,30319,298))
		 || ( _40_PresentationFramework_dll  < fver(4,0,30319,298))
		 || ( _40_PresentationHost_v0400_dll < fver(4,0,30319,298))
		 || ( _40_System_Xaml_dll            < fver(4,0,30319,298))
		 || ( _40_WindowsBase_dll            < fver(4,0,30319,298))
		 || ( _40_wpfgfx_v0400_dll           < fver(4,0,30319,298))
		 || ( _40_wpftxt_v0400_dll           < fver(4,0,30319,298))
		/* QFE */
		 || ( _40_PenIMC_dll                 > fver(4,0,30319,298) && _40_PenIMC_dll                 < fver(4,0,30319,588))
		 || ( _40_PresentationCore_dll       > fver(4,0,30319,298) && _40_PresentationCore_dll       < fver(4,0,30319,588))
		 || ( _40_PresentationFramework_dll  > fver(4,0,30319,298) && _40_PresentationFramework_dll  < fver(4,0,30319,588))
		 || ( _40_PresentationHost_v0400_dll > fver(4,0,30319,298) && _40_PresentationHost_v0400_dll < fver(4,0,30319,588))
		 || ( _40_System_Xaml_dll            > fver(4,0,30319,298) && _40_System_Xaml_dll            < fver(4,0,30319,588))
		 || ( _40_WindowsBase_dll            > fver(4,0,30319,298) && _40_WindowsBase_dll            < fver(4,0,30319,588))
		 || ( _40_wpfgfx_v0400_dll           > fver(4,0,30319,298) && _40_wpfgfx_v0400_dll           < fver(4,0,30319,588))
		 || ( _40_wpftxt_v0400_dll           > fver(4,0,30319,298) && _40_wpftxt_v0400_dll           < fver(4,0,30319,588)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2737019)");
		XX(p3+"NDP40-KB2737019-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* No GDR */
		/* QFE */
			( _40_ServiceModelReg_exe                < fver(4,0,30319,2128))
		 || ( _40_System_Workflow_ComponentModel_dll > zero && _40_System_Workflow_ComponentModel_dll < fver(4,0,30319,2128)) // Full Profile Only
		 || ( _40_System_Workflow_Runtime_dll        > zero && _40_System_Workflow_Runtime_dll        < fver(4,0,30319,2128)) // Full Profile Only
		 || ( _40_System_dll                         < fver(4,0,30319,2128)) )) {
		NN("2019-02 Security Only Update for .NET Framework 4.0 on WES09 and POSReady 2009 (KB4483475)");
		XX(p3+"ndp40-kb4483475-x86_25db1d55a1c940328a6f3de2105f9454a778ff2f.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_System_Data_dll < fver(4,0,30319,1054))
		/* QFE */
		 || ( _40_System_Data_dll > fver(4,0,30319,1054) && _40_System_Data_dll < fver(4,0,30319,2089)) )) {
		NN("Security Update for Microsoft .NET Framework 4 for WES09 and POSReady 2009 (KB3163249)");
		XX(p3+"ndp40-kb3163249-x86_c30a31e9c3f8b8c3d0a43887da18486f1f7cc3de.exe"+n6);
	}
	if( sp==3 &&  (*i_nfx40c || *v_nfx40c>=0) && (
		/* GDR */
			( _40_System_Runtime_Remoting_dll < fver(4,0,30319,1023))
		/* QFE */
		 || ( _40_System_Runtime_Remoting_dll > fver(4,0,30319,1023) && _40_System_Runtime_Remoting_dll < fver(4,0,30319,2036)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on Windows Server 2003, Vista, Windows 7, Server 2008 x86 (KB2931365)");
		XX(p3+"ndp40-kb2931365-x86_99d59cfc6e59f2c5d406b61f06172dca0a89286d.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* No GDR */
		/* QFE */
			// Both of these files are Full Profile Only
			( _40_System_Workflow_ComponentModel_dll > zero && _40_System_Workflow_ComponentModel_dll < fver(4,0,30319,2119))
		 || ( _40_System_Workflow_Runtime_dll        > zero && _40_System_Workflow_Runtime_dll        < fver(4,0,30319,2119)) )) {
		NN("2018-09 Security Only Update for .NET Framework 4.0 on WES09 and POSReady 2009 (KB4457046)");
		XX(p3+"ndp40-kb4457046-x86_407a3efb88f4bdf4974375f288761502e1683155.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			// Full Profile Only
			( _40_System_Web_DataVisualization_dll > zero && _40_System_Web_DataVisualization_dll < fver(4,0,30319,236))
		/* QFE */
		 || ( _40_System_Web_DataVisualization_dll > fver(4,0,30319,236) && _40_System_Web_DataVisualization_dll < fver(4,0,30319,461)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on Windows XP, Windows Server 2003, Windows Vista, Windows 7, Windows Server 2008 x86 (KB2487367)");
		XX(p3+"NDP40-KB2487367-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_ServiceModelReg_exe < fver(4,0,30319,297))
		 || ( _40_DataSvcUtil_exe                 > zero && _40_DataSvcUtil_exe                 < fver(4,0,30319,297)) // Full Profile Only
		 || ( _40_System_Data_Services_Design_dll > zero && _40_System_Data_Services_Design_dll < fver(4,0,30319,297)) // Full Profile Only
		 || ( _40_System_Data_Services_dll        > zero && _40_System_Data_Services_dll        < fver(4,0,30319,297)) // Full Profile Only
		/* QFE */
		 || ( _40_ServiceModelReg_exe > fver(4,0,30319,1025) && _40_ServiceModelReg_exe < fver(4,0,30319,2034))
		 || ( _40_DataSvcUtil_exe                 > fver(4,0,30319,297) && _40_DataSvcUtil_exe                 < fver(4,0,30319,587))
		 || ( _40_System_Data_Services_Design_dll > fver(4,0,30319,297) && _40_System_Data_Services_Design_dll < fver(4,0,30319,587))
		 || ( _40_System_Data_Services_dll        > fver(4,0,30319,297) && _40_System_Data_Services_dll        < fver(4,0,30319,587)) )) {
		NN("Security Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2736428)");
		XX(p3+"NDP40-KB2736428-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,261))
		 || ( _40_clr_dll                   > zero && _40_clr_dll                   < fver(4,0,30319,261))
		 || ( _40_mscordacwks_dll           > zero && _40_mscordacwks_dll           < fver(4,0,30319,261))
		 || ( _40_mscordbi_dll              > zero && _40_mscordbi_dll              < fver(4,0,30319,261))
		 || ( _40_mscorlib_dll              > zero && _40_mscorlib_dll              < fver(4,0,30319,261))
		 || ( _40_nlssorting_dll            > zero && _40_nlssorting_dll            < fver(4,0,30319,261))
		 || ( _40_ServiceModelReg_exe       > zero && _40_ServiceModelReg_exe       < fver(4,0,30319,261))
		 || ( _40_SOS_dll                   > zero && _40_SOS_dll                   < fver(4,0,30319,261))
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,261))
		 || ( _40_System_dll                > zero && _40_System_dll                < fver(4,0,30319,261))
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,261))
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,261))
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,261))
		/* BROWSER CONFIG (GDR & QFE) */
		 || ( _40_blackberry_browser > -1 && _40_blackberry_browser != 2391 )
		 || ( _40_firefox_browser    > -1 && _40_firefox_browser    != 3082 )
		 || ( _40_gateway_browser    > -1 && _40_gateway_browser    != 5634 )
		 || ( _40_ie_browser         > -1 && _40_ie_browser         != 4283 )
		/* QFE */
		 || ( _40_aspnet_wp_exe             > fver(4,0,30319,261) && _40_aspnet_wp_exe             < fver(4,0,30319,530))
		 || ( _40_clr_dll                   > fver(4,0,30319,261) && _40_clr_dll                   < fver(4,0,30319,530))
		 || ( _40_mscordacwks_dll           > fver(4,0,30319,261) && _40_mscordacwks_dll           < fver(4,0,30319,530))
		 || ( _40_mscordbi_dll              > fver(4,0,30319,261) && _40_mscordbi_dll              < fver(4,0,30319,530))
		 || ( _40_mscorlib_dll              > fver(4,0,30319,261) && _40_mscorlib_dll              < fver(4,0,30319,530))
		 || ( _40_nlssorting_dll            > fver(4,0,30319,261) && _40_nlssorting_dll            < fver(4,0,30319,530))
		 || ( _40_ServiceModelReg_exe       > fver(4,0,30319,261) && _40_ServiceModelReg_exe       < fver(4,0,30319,530))
		 || ( _40_SOS_dll                   > fver(4,0,30319,261) && _40_SOS_dll                   < fver(4,0,30319,530))
		 || ( _40_System_Web_Extensions_dll > fver(4,0,30319,261) && _40_System_Web_Extensions_dll < fver(4,0,30319,530))
		 || ( _40_System_dll                > fver(4,0,30319,261) && _40_System_dll                < fver(4,0,30319,530))
		 || ( _40_System_Web_dll            > fver(4,0,30319,261) && _40_System_Web_dll            < fver(4,0,30319,530))
		 || ( _40_webengine_dll             > fver(4,0,30319,261) && _40_webengine_dll             < fver(4,0,30319,530))
		 || ( _40_webengine4_dll            > fver(4,0,30319,261) && _40_webengine4_dll            < fver(4,0,30319,530)) )) {
		NN("Update for Microsoft .NET Framework 4 on XP, Server 2003, Vista, Windows 7, Server 2008 x86 (KB2600217)");
		XX(p3+"NDP40-KB2600217-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_aspnet_wp_exe             > zero && _40_aspnet_wp_exe             < fver(4,0,30319,237))
		 || ( _40_clr_dll                   > zero && _40_clr_dll                   < fver(4,0,30319,237))
		 || ( _40_clrjit_dll                > zero && _40_clrjit_dll                < fver(4,0,30319,237))
		 || ( _40_Culture_dll               > zero && _40_Culture_dll               < fver(4,0,30319,237))
		 || ( _40_mscordacwks_dll           > zero && _40_mscordacwks_dll           < fver(4,0,30319,237))
		 || ( _40_mscordbi_dll              > zero && _40_mscordbi_dll              < fver(4,0,30319,237))
		 || ( _40_mscoreei_dll              > zero && _40_mscoreei_dll              < fver(4,0,30319,237))
		 || ( _40_mscorlib_dll              > zero && _40_mscorlib_dll              < fver(4,0,30319,237))
		 || ( _40_nlssorting_dll            > zero && _40_nlssorting_dll            < fver(4,0,30319,237))
		 || ( _40_peverify_dll              > zero && _40_peverify_dll              < fver(4,0,30319,237))
		 || ( _40_SOS_dll                   > zero && _40_SOS_dll                   < fver(4,0,30319,237))
		 || ( _40_System_Web_Extensions_dll > zero && _40_System_Web_Extensions_dll < fver(4,0,30319,237))
		 //|| ( _40_System_Caching_dll        > zero && _40_System_Caching_dll        < fver(4,0,30319,237))
		 || ( _40_System_Data_dll           > zero && _40_System_Data_dll           < fver(4,0,30319,237))
		 || ( _40_System_Design_dll         > zero && _40_System_Design_dll         < fver(4,0,30319,237))
		 || ( _40_System_Web_dll            > zero && _40_System_Web_dll            < fver(4,0,30319,237))
		 || ( _40_webengine_dll             > zero && _40_webengine_dll             < fver(4,0,30319,237))
		 || ( _40_webengine4_dll            > zero && _40_webengine4_dll            < fver(4,0,30319,237))
		/* BROWSER CONFIG (GDR & QFE) */
		 || ( _40_blackberry_browser > -1 && _40_blackberry_browser != 2391 && _40_blackberry_browser != 2390   )
		 || ( _40_gateway_browser    > -1 && _40_gateway_browser    != 5634   )
		 || ( _40_locale_nlp         > -1 && _40_locale_nlp         != 419696 )
		/* QFE */
		 || ( _40_aspnet_wp_exe             > fver(4,0,30319,237) && _40_aspnet_wp_exe             < fver(4,0,30319,468))
		 || ( _40_clr_dll                   > fver(4,0,30319,237) && _40_clr_dll                   < fver(4,0,30319,468))
		 || ( _40_clrjit_dll                > fver(4,0,30319,237) && _40_clrjit_dll                < fver(4,0,30319,468))
		 || ( _40_Culture_dll               > fver(4,0,30319,237) && _40_Culture_dll               < fver(4,0,30319,468))
		 || ( _40_mscordacwks_dll           > fver(4,0,30319,237) && _40_mscordacwks_dll           < fver(4,0,30319,468))
		 || ( _40_mscordbi_dll              > fver(4,0,30319,237) && _40_mscordbi_dll              < fver(4,0,30319,468))
		 || ( _40_mscoreei_dll              > fver(4,0,30319,237) && _40_mscoreei_dll              < fver(4,0,30319,468))
		 || ( _40_mscorlib_dll              > fver(4,0,30319,237) && _40_mscorlib_dll              < fver(4,0,30319,468))
		 || ( _40_nlssorting_dll            > fver(4,0,30319,237) && _40_nlssorting_dll            < fver(4,0,30319,468))
		 || ( _40_peverify_dll              > fver(4,0,30319,237) && _40_peverify_dll              < fver(4,0,30319,468))
		 || ( _40_SOS_dll                   > fver(4,0,30319,237) && _40_SOS_dll                   < fver(4,0,30319,468))
		 || ( _40_System_Web_Extensions_dll > fver(4,0,30319,237) && _40_System_Web_Extensions_dll < fver(4,0,30319,468))
		 //|| ( _40_System_Caching_dll        > fver(4,0,30319,237) && _40_System_Caching_dll        < fver(4,0,30319,468))
		 || ( _40_System_Data_dll           > fver(4,0,30319,237) && _40_System_Data_dll           < fver(4,0,30319,468))
		 || ( _40_System_Design_dll         > fver(4,0,30319,237) && _40_System_Design_dll         < fver(4,0,30319,468))
		 || ( _40_System_Web_dll            > fver(4,0,30319,237) && _40_System_Web_dll            < fver(4,0,30319,468))
		 || ( _40_webengine_dll             > fver(4,0,30319,237) && _40_webengine_dll             < fver(4,0,30319,468))
		 || ( _40_webengine4_dll            > fver(4,0,30319,237) && _40_webengine4_dll            < fver(4,0,30319,468)) )) {
		NN("Update for Microsoft .NET Framework 4 on Windows XP, Windows Server 2003, Windows Vista, Windows 7, Windows Server 2008 x86 (KB2533523)");
		XX(p3+"NDP40-KB2533523-x86.exe"+n6);
	}
	if( sp==3 &&  ( NFX40 ) && (
		/* GDR */
			( _40_vbc_exe                               > zero && _40_vbc_exe                               < fver(10,0,30319,233))
		 || ( _40_aspnet_compiler_exe                   > zero && _40_aspnet_compiler_exe                   < fver(4,0,30319,233))
		 || ( _40_aspnet_wp_exe                         > zero && _40_aspnet_wp_exe                         < fver(4,0,30319,233))
		 || ( _40_clr_dll                               > zero && _40_clr_dll                               < fver(4,0,30319,233))
		 || ( _40_clrjit_dll                            > zero && _40_clrjit_dll                            < fver(4,0,30319,233))
		 || ( _40_Microsoft_Build_Tasks_v4_0_dll        > zero && _40_Microsoft_Build_Tasks_v4_0_dll        < fver(4,0,30319,233))
		 || ( _40_Microsoft_Data_Entity_Build_Tasks_dll > zero && _40_Microsoft_Data_Entity_Build_Tasks_dll < fver(4,0,30319,233))
		 || ( _40_mscordacwks_dll                       > zero && _40_mscordacwks_dll                       < fver(4,0,30319,233))
		 || ( _40_mscordbi_dll                          > zero && _40_mscordbi_dll                          < fver(4,0,30319,233))
		 || ( _40_mscorlib_dll                          > zero && _40_mscorlib_dll                          < fver(4,0,30319,233))
		 || ( _40_nlssorting_dll                        > zero && _40_nlssorting_dll                        < fver(4,0,30319,233))
		 || ( _40_PresentationCore_dll                  > zero && _40_PresentationCore_dll                  < fver(4,0,30319,233))
		 || ( _40_PresentationFramework_dll             > zero && _40_PresentationFramework_dll             < fver(4,0,30319,233))
		 || ( _40_ServiceModelReg_exe                   > zero && _40_ServiceModelReg_exe                   < fver(4,0,30319,233))
		 || ( _40_SOS_dll                               > zero && _40_SOS_dll                               < fver(4,0,30319,233))
		 || ( _40_System_Core_dll                       > zero && _40_System_Core_dll                       < fver(4,0,30319,233))
		 || ( _40_System_Data_Entity_Design_dll         > zero && _40_System_Data_Entity_Design_dll         < fver(4,0,30319,233))
		 || ( _40_System_Net_dll                        > zero && _40_System_Net_dll                        < fver(4,0,30319,233))
		 || ( _40_System_ServiceModel_Activation_dll    > zero && _40_System_ServiceModel_Activation_dll    < fver(4,0,30319,233))
		 || ( _40_System_ServiceModel_dll               > zero && _40_System_ServiceModel_dll               < fver(4,0,30319,233))
		 || ( _40_System_ServiceModel_Web_dll           > zero && _40_System_ServiceModel_Web_dll           < fver(4,0,30319,233))
		 || ( _40_System_Web_Extensions_dll             > zero && _40_System_Web_Extensions_dll             < fver(4,0,30319,233))
		 || ( _40_System_Xml_Serialization_dll          > zero && _40_System_Xml_Serialization_dll          < fver(4,0,30319,233))
		 || ( _40_System_Design_dll                     > zero && _40_System_Design_dll                     < fver(4,0,30319,233))
		 || ( _40_System_dll                            > zero && _40_System_dll                            < fver(4,0,30319,233))
		 || ( _40_System_Web_dll                        > zero && _40_System_Web_dll                        < fver(4,0,30319,233))
		 || ( _40_System_XML_dll                        > zero && _40_System_XML_dll                        < fver(4,0,30319,233))
		 || ( _40_webengine4_dll                        > zero && _40_webengine4_dll                        < fver(4,0,30319,233))
		 || ( _40_webengine_dll                         > zero && _40_webengine_dll                         < fver(4,0,30319,233))
		 || ( _40_WindowsBase_dll                       > zero && _40_WindowsBase_dll                       < fver(4,0,30319,233))
		 || ( _40_wpfgfx_v0400_dll                      > zero && _40_wpfgfx_v0400_dll                      < fver(4,0,30319,233))
		 || ( _40_wpftxt_v0400_dll                      > zero && _40_wpftxt_v0400_dll                      < fver(4,0,30319,233))
		/* QFE */
		 || ( _40_vbc_exe                               > fver(10,0,30319,233)&& _40_vbc_exe                               < fver(10,0,30319,450))
		 || ( _40_aspnet_compiler_exe                   > fver(4,0,30319,233) && _40_aspnet_compiler_exe                   < fver(4,0,30319,450))
		 || ( _40_aspnet_wp_exe                         > fver(4,0,30319,233) && _40_aspnet_wp_exe                         < fver(4,0,30319,450))
		 || ( _40_clr_dll                               > fver(4,0,30319,233) && _40_clr_dll                               < fver(4,0,30319,450))
		 || ( _40_clrjit_dll                            > fver(4,0,30319,233) && _40_clrjit_dll                            < fver(4,0,30319,450))
		 || ( _40_Microsoft_Build_Tasks_v4_0_dll        > fver(4,0,30319,233) && _40_Microsoft_Build_Tasks_v4_0_dll        < fver(4,0,30319,450))
		 || ( _40_Microsoft_Data_Entity_Build_Tasks_dll > fver(4,0,30319,233) && _40_Microsoft_Data_Entity_Build_Tasks_dll < fver(4,0,30319,450))
		 || ( _40_mscordacwks_dll                       > fver(4,0,30319,233) && _40_mscordacwks_dll                       < fver(4,0,30319,450))
		 || ( _40_mscordbi_dll                          > fver(4,0,30319,233) && _40_mscordbi_dll                          < fver(4,0,30319,450))
		 || ( _40_mscorlib_dll                          > fver(4,0,30319,233) && _40_mscorlib_dll                          < fver(4,0,30319,450))
		 || ( _40_nlssorting_dll                        > fver(4,0,30319,233) && _40_nlssorting_dll                        < fver(4,0,30319,450))
		 || ( _40_PresentationCore_dll                  > fver(4,0,30319,233) && _40_PresentationCore_dll                  < fver(4,0,30319,450))
		 || ( _40_PresentationFramework_dll             > fver(4,0,30319,233) && _40_PresentationFramework_dll             < fver(4,0,30319,450))
		 || ( _40_ServiceModelReg_exe                   > fver(4,0,30319,233) && _40_ServiceModelReg_exe                   < fver(4,0,30319,450))
		 || ( _40_SOS_dll                               > fver(4,0,30319,233) && _40_SOS_dll                               < fver(4,0,30319,450))
		 || ( _40_System_Core_dll                       > fver(4,0,30319,233) && _40_System_Core_dll                       < fver(4,0,30319,450))
		 || ( _40_System_Data_Entity_Design_dll         > fver(4,0,30319,233) && _40_System_Data_Entity_Design_dll         < fver(4,0,30319,450))
		 || ( _40_System_Net_dll                        > fver(4,0,30319,233) && _40_System_Net_dll                        < fver(4,0,30319,450))
		 || ( _40_System_ServiceModel_Activation_dll    > fver(4,0,30319,233) && _40_System_ServiceModel_Activation_dll    < fver(4,0,30319,450))
		 || ( _40_System_ServiceModel_dll               > fver(4,0,30319,233) && _40_System_ServiceModel_dll               < fver(4,0,30319,450))
		 || ( _40_System_ServiceModel_Web_dll           > fver(4,0,30319,233) && _40_System_ServiceModel_Web_dll           < fver(4,0,30319,450))
		 || ( _40_System_Web_Extensions_dll             > fver(4,0,30319,233) && _40_System_Web_Extensions_dll             < fver(4,0,30319,450))
		 || ( _40_System_Xml_Serialization_dll          > fver(4,0,30319,233) && _40_System_Xml_Serialization_dll          < fver(4,0,30319,450))
		 || ( _40_System_Design_dll                     > fver(4,0,30319,233) && _40_System_Design_dll                     < fver(4,0,30319,450))
		 || ( _40_System_dll                            > fver(4,0,30319,233) && _40_System_dll                            < fver(4,0,30319,450))
		 || ( _40_System_Web_dll                        > fver(4,0,30319,233) && _40_System_Web_dll                        < fver(4,0,30319,450))
		 || ( _40_System_XML_dll                        > fver(4,0,30319,233) && _40_System_XML_dll                        < fver(4,0,30319,450))
		 || ( _40_webengine4_dll                        > fver(4,0,30319,233) && _40_webengine4_dll                        < fver(4,0,30319,450))
		 || ( _40_webengine_dll                         > fver(4,0,30319,233) && _40_webengine_dll                         < fver(4,0,30319,450))
		 || ( _40_WindowsBase_dll                       > fver(4,0,30319,233) && _40_WindowsBase_dll                       < fver(4,0,30319,450))
		 || ( _40_wpfgfx_v0400_dll                      > fver(4,0,30319,233) && _40_wpfgfx_v0400_dll                      < fver(4,0,30319,450))
		 || ( _40_wpftxt_v0400_dll                      > fver(4,0,30319,237) && _40_wpftxt_v0400_dll                      < fver(4,0,30319,468)) )) {
		NN("Update for Microsoft .NET Framework 4 on Windows XP, Windows Server 2003, Windows Vista, Windows 7, Windows Server 2008 x86 (KB2468871)");
		XX(p3+"NDP40-KB2468871-v2-x86.exe"+n6);
	}
/*

*/
}
