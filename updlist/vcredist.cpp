#include "stdafx.h"
#include "vcredist.h"
#include "fver.h"
#include "winsku.h"
#include "registry.h"
#include <string>

#define NN name->push_back
#define XX exe->push_back

using namespace std;

void vcsummary() {

	fver vc2005 = vcr2005(false);
	fver vc2008 = vcr2008(false);
	fver vc2010 = vcr2010(false);
	fver vc2012 = vcr2012(false);
	fver vc2013 = vcr2013(false);
	fver vc2015 = vcr2015(false);

	int vcsp2008 = 0;
	int vcsp2010 = 0;

	bool latest2005 = false;
	bool latest2008 = false;
	bool latest2010 = false;
	bool latest2012 = false;
	bool latest2013 = false;
	bool latest2015 = false;

	if(vc2008>=fver( 9,0,30729,0)) { vcsp2008=1; }
	if(vc2010>=fver(10,0,40219,0)) { vcsp2010=1; }

	if(vc2005==fver( 8,0,50727,6195)) { latest2005 = true; }
	if(vc2008==fver( 9,0,30729,6161)) { latest2008 = true; }
	if(vc2010==fver(10,0,40219,325 )) { latest2010 = true; }
	if(vc2012==fver(11,0,61030,0   )) { latest2012 = true; }
	if(vc2013==fver(12,0,40664,0   )) { latest2013 = true; }
	if(vc2015==fver(14,0,24215,1   )) { latest2015 = true; }

	printf("VCREDIST_2005_VERSION = %s (%s)\n",vcr2005(false).format().c_str(),
		latest2005?"Latest":"Out of Date");
	printf("VCREDIST_2008_VERSION = %s (SP%d, %s)\n",vcr2008(false).format().c_str(),
		vcsp2008,latest2008?"Latest":"Out of Date");
	printf("VCREDIST_2010_VERSION = %s (SP%d, %s)\n",vcr2010(false).format().c_str(),
		vcsp2010,latest2010?"Latest":"Out of Date");
	printf("VCREDIST_2012_VERSION = %s (%s)\n",vcr2012(false).format().c_str(),
		latest2012?"Latest":"Out of Date");
	printf("VCREDIST_2013_VERSION = %s (%s)\n",vcr2013(false).format().c_str(),
		latest2013?"Latest":"Out of Date");
	printf("VCREDIST_2015_VERSION = %s (%s)\n",vcr2015(false).format().c_str(),
		latest2015?"Latest":"Out of Date");
}


void installVC(std::vector<std::string>* name, std::vector<std::string>* exe, int sp) {

	const std::string p="Packages\\";
	const std::string a2=" /Q";
	const std::string a3=" /passive /norestart";

	// Install Microsoft Visual C++ Runtime Versions
	if( sp>=2 && vcr2005(false)<fver(8,0,50727,6195) ) {
		NN("Security Update for Microsoft Visual C++ 2005 Service Pack 1 Redistributable Package (KB2538242)");
		XX(p+"vcredist_x86_b8fab0bb7f62a24ddfe77b19cd9a1451abd7b847.exe"+a2);
	}
	if( sp>=2 && vcr2008(false)<fver(9,0,30729,6161) ) {
		NN("Security Update for Microsoft Visual C++ 2008 Service Pack 1 Redistributable Package (KB2538243)");
		XX(p+"vcredist_x86_470640aa4bb7db8e69196b5edb0010933569e98d.exe /qb");
	}
	if( sp==3 && vcr2010(false)<fver(10,0,40219,325) ) {
		NN("Security Update for Microsoft Visual C++ 2010 Service Pack 1 Redistributable Package (KB2565063)");
		XX(p+"vcredist_x86_28c54491be70c38c97849c3d8cfbfdd0d3c515cb.exe"+a3);
	}
	if( sp==3 && vcr2012(false)<fver(11,0,61030,0) ) {
		NN("Update for Microsoft Visual C++ 2012 Update 4 Redistributable Package (KB3119142)");
		XX(p+"vcredist_x86_96b377a27ac5445328cbaae210fc4f0aaa750d3f.exe"+a3);
	}
	if( sp==3 && vcr2013(false)<fver(12,0,40664,0) ) {
		NN("Update for Microsoft Visual C++ 2013 Update 5 Redistributable Package (KB4032938)");
		XX(p+"vcredist_x86_0f5d66bcaf120f2d3f340e448a268fe4bbf7709d.exe"+a3);
	}
	if( sp==3 && vcr2015(false)<fver(14,0,24215,1) ) {
		NN("Update for Microsoft Visual C++ 2015 Update 3 Redistributable Package");
		XX(p+"vcredist_x86_72211bd2e7dfc91ea7c8fac549c49c0543ba791b.exe"+a3);
	}
}


fver vcr2005(bool debug) {

	int status=0;
	fver f;

	// Identify system paths
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&status);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";
	std::wstring SharedVC = ProgramFiles+L"\\Common Files\\Microsoft Shared\\VC";

	fver _msdia80_dll = getFileVer(SharedVC+L"\\msdia80.dll",&status);

	std::wstring      ATL8_42 = WinSxS+L"\\x86_Microsoft.VC80.ATL_1fc8b3b9a1e18e3b_8.0.50727.42_x-ww_6e805841";
	std::wstring      CRT8_42 = WinSxS+L"\\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.42_x-ww_0de06acd";
	std::wstring      MFC8_42 = WinSxS+L"\\x86_Microsoft.VC80.MFC_1fc8b3b9a1e18e3b_8.0.50727.42_x-ww_dec6ddd2";
	std::wstring   MFCLOC8_42 = WinSxS+L"\\x86_Microsoft.VC80.MFCLOC_1fc8b3b9a1e18e3b_8.0.50727.42_x-ww_3415f6d0";
	std::wstring   OpenMP8_42 = WinSxS+L"\\x86_Microsoft.VC80.OpenMP_1fc8b3b9a1e18e3b_8.0.50727.42_x-ww_0ee63867";
	fver _ATL80_dll_42 = getFileVer(ATL8_42+L"\\ATL80.dll",&status);
	fver _mfc80_dll_42 = getFileVer(MFC8_42+L"\\mfc80.dll",&status);
	fver _mfc80u_dll_42 = getFileVer(MFC8_42+L"\\mfc80u.dll",&status);
	fver _mfcm80_dll_42   = getFileVer(MFC8_42+L"\\mfcm80.dll",&status);
	fver _mfcm80u_dll_42  = getFileVer(MFC8_42+L"\\mfcm80u.dll",&status);
	fver _mfc80CHS_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80CHS.dll",&status);
	fver _mfc80CHT_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80CHT.dll",&status);
	fver _mfc80DEU_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80DEU.dll",&status);
	fver _mfc80ENU_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80ENU.dll",&status);
	fver _mfc80ESP_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80ESP.dll",&status);
	fver _mfc80FRA_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80FRA.dll",&status);
	fver _mfc80ITA_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80ITA.dll",&status);
	fver _mfc80JPN_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80JPN.dll",&status);
	fver _mfc80KOR_dll_42 = getFileVer(MFCLOC8_42+L"\\mfc80KOR.dll",&status);
	fver _msvcm80_dll_42 = getFileVer(CRT8_42+L"\\msvcm80.dll",&status);
	fver _msvcp80_dll_42 = getFileVer(CRT8_42+L"\\msvcp80.dll",&status);
	fver _msvcr80_dll_42 = getFileVer(CRT8_42+L"\\msvcr80.dll",&status);
	fver _vcomp_dll_42 = getFileVer(OpenMP8_42+L"\\vcomp.dll",&status);
	fver _ATL80_dll = _ATL80_dll_42;
	fver _mfc80_dll = _mfc80_dll_42;
	fver _mfc80u_dll = _mfc80u_dll_42;
	fver _mfcm80_dll = _mfcm80_dll_42;
	fver _mfcm80u_dll = _mfcm80u_dll_42;
	fver _mfc80CHS_dll = _mfc80CHS_dll_42;
	fver _mfc80CHT_dll = _mfc80CHT_dll_42;
	fver _mfc80DEU_dll = _mfc80DEU_dll_42;
	fver _mfc80ENU_dll = _mfc80ENU_dll_42;
	fver _mfc80ESP_dll = _mfc80ESP_dll_42;
	fver _mfc80FRA_dll = _mfc80FRA_dll_42;
	fver _mfc80ITA_dll = _mfc80ITA_dll_42;
	fver _mfc80JPN_dll = _mfc80JPN_dll_42;
	fver _mfc80KOR_dll = _mfc80KOR_dll_42;
	fver _msvcm80_dll = _msvcm80_dll_42;
	fver _msvcp80_dll = _msvcp80_dll_42;
	fver _msvcr80_dll = _msvcr80_dll_42;
	fver _vcomp_dll = _vcomp_dll_42;


	std::wstring     ATL8_762 = WinSxS+L"\\x86_Microsoft.VC80.ATL_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_cbb27474";
	std::wstring     CRT8_762 = WinSxS+L"\\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_6b128700";
	std::wstring     MFC8_762 = WinSxS+L"\\x86_Microsoft.VC80.MFC_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_3bf8fa05";
	std::wstring  MFCLOC8_762 = WinSxS+L"\\x86_Microsoft.VC80.MFCLOC_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_91481303";
	std::wstring  OpenMP8_762 = WinSxS+L"\\x86_Microsoft.VC80.OpenMP_1fc8b3b9a1e18e3b_8.0.50727.762_x-ww_6c18549a";
	fver _ATL80_dll_762 = getFileVer(ATL8_762+L"\\ATL80.dll",&status);
	fver _mfc80_dll_762 = getFileVer(MFC8_762+L"\\mfc80.dll",&status);
	fver _mfc80u_dll_762 = getFileVer(MFC8_762+L"\\mfc80u.dll",&status);
	fver _mfcm80_dll_762   = getFileVer(MFC8_762+L"\\mfcm80.dll",&status);
	fver _mfcm80u_dll_762  = getFileVer(MFC8_762+L"\\mfcm80u.dll",&status);
	fver _mfc80CHS_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80CHS.dll",&status);
	fver _mfc80CHT_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80CHT.dll",&status);
	fver _mfc80DEU_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80DEU.dll",&status);
	fver _mfc80ENU_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80ENU.dll",&status);
	fver _mfc80ESP_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80ESP.dll",&status);
	fver _mfc80FRA_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80FRA.dll",&status);
	fver _mfc80ITA_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80ITA.dll",&status);
	fver _mfc80JPN_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80JPN.dll",&status);
	fver _mfc80KOR_dll_762 = getFileVer(MFCLOC8_762+L"\\mfc80KOR.dll",&status);
	fver _msvcm80_dll_762 = getFileVer(CRT8_762+L"\\msvcm80.dll",&status);
	fver _msvcp80_dll_762 = getFileVer(CRT8_762+L"\\msvcp80.dll",&status);
	fver _msvcr80_dll_762 = getFileVer(CRT8_762+L"\\msvcr80.dll",&status);
	fver _vcomp_dll_762 = getFileVer(OpenMP8_762+L"\\vcomp.dll",&status);
		if (_ATL80_dll_762   >_ATL80_dll   ) { _ATL80_dll    = _ATL80_dll_762;    }
		if (_mfc80_dll_762   >_mfc80_dll   ) { _mfc80_dll    = _mfc80_dll_762;    }
		if (_mfc80u_dll_762  >_mfc80u_dll  ) { _mfc80u_dll   = _mfc80u_dll_762;   }
		if (_mfcm80_dll_762  >_mfcm80_dll  ) { _mfcm80_dll   = _mfcm80_dll_762;   }
		if (_mfcm80u_dll_762 >_mfcm80u_dll ) { _mfcm80u_dll  = _mfcm80u_dll_762;  }
		if (_mfc80CHS_dll_762>_mfc80CHS_dll) { _mfc80CHS_dll = _mfc80CHS_dll_762; }
		if (_mfc80CHT_dll_762>_mfc80CHT_dll) { _mfc80CHT_dll = _mfc80CHT_dll_762; }
		if (_mfc80DEU_dll_762>_mfc80DEU_dll) { _mfc80DEU_dll = _mfc80DEU_dll_762; }
		if (_mfc80ENU_dll_762>_mfc80ENU_dll) { _mfc80ENU_dll = _mfc80ENU_dll_762; }
		if (_mfc80ESP_dll_762>_mfc80ESP_dll) { _mfc80ESP_dll = _mfc80ESP_dll_762; }
		if (_mfc80FRA_dll_762>_mfc80FRA_dll) { _mfc80FRA_dll = _mfc80FRA_dll_762; }
		if (_mfc80ITA_dll_762>_mfc80ITA_dll) { _mfc80ITA_dll = _mfc80ITA_dll_762; }
		if (_mfc80JPN_dll_762>_mfc80JPN_dll) { _mfc80JPN_dll = _mfc80JPN_dll_762; }
		if (_mfc80KOR_dll_762>_mfc80KOR_dll) { _mfc80KOR_dll = _mfc80KOR_dll_762; }
		if (_msvcm80_dll_762 >_msvcm80_dll ) { _msvcm80_dll  = _msvcm80_dll_762;  }
		if (_msvcp80_dll_762 >_msvcp80_dll ) { _msvcp80_dll  = _msvcp80_dll_762;  }
		if (_msvcr80_dll_762 >_msvcr80_dll ) { _msvcr80_dll  = _msvcr80_dll_762;  }
		if (_vcomp_dll_762   >_vcomp_dll   ) { _vcomp_dll    = _vcomp_dll_762;    }

	// These files are installed with .NET Framework, do not use to search for updates
	std::wstring    CRT8_3053 = WinSxS+L"\\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.3053_x-ww_b80fa8ca";
	fver _msvcm80_dll_3053 = getFileVer(CRT8_3053+L"\\msvcm80.dll",&status);
	fver _msvcp80_dll_3053 = getFileVer(CRT8_3053+L"\\msvcp80.dll",&status);
	fver _msvcr80_dll_3053 = getFileVer(CRT8_3053+L"\\msvcr80.dll",&status);
	if (_msvcm80_dll_3053>_msvcm80_dll) { _msvcm80_dll = _msvcm80_dll_3053; }
	if (_msvcp80_dll_3053>_msvcp80_dll) { _msvcp80_dll = _msvcp80_dll_3053; }
	if (_msvcr80_dll_3053>_msvcr80_dll) { _msvcr80_dll = _msvcr80_dll_3053; }

	std::wstring    ATL8_4053 = WinSxS+L"\\x86_Microsoft.VC80.ATL_1fc8b3b9a1e18e3b_8.0.50727.4053_x-ww_473666fd";
	std::wstring    CRT8_4053 = WinSxS+L"\\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.4053_x-ww_e6967989";
	std::wstring    MFC8_4053 = WinSxS+L"\\x86_Microsoft.VC80.MFC_1fc8b3b9a1e18e3b_8.0.50727.4053_x-ww_b77cec8e";
	std::wstring MFCLOC8_4053 = WinSxS+L"\\x86_Microsoft.VC80.MFCLOC_1fc8b3b9a1e18e3b_8.0.50727.4053_x-ww_0ccc058c";
	std::wstring OpenMP8_4053 = WinSxS+L"\\x86_Microsoft.VC80.OpenMP_1fc8b3b9a1e18e3b_8.0.50727.4053_x-ww_e79c4723";
	fver _ATL80_dll_4053 = getFileVer(ATL8_4053+L"\\ATL80.dll",&status);
	fver _mfc80_dll_4053 = getFileVer(MFC8_4053+L"\\mfc80.dll",&status);
	fver _mfc80u_dll_4053 = getFileVer(MFC8_4053+L"\\mfc80u.dll",&status);
	fver _mfcm80_dll_4053   = getFileVer(MFC8_4053+L"\\mfcm80.dll",&status);
	fver _mfcm80u_dll_4053  = getFileVer(MFC8_4053+L"\\mfcm80u.dll",&status);
	fver _mfc80CHS_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80CHS.dll",&status);
	fver _mfc80CHT_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80CHT.dll",&status);
	fver _mfc80DEU_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80DEU.dll",&status);
	fver _mfc80ENU_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80ENU.dll",&status);
	fver _mfc80ESP_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80ESP.dll",&status);
	fver _mfc80FRA_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80FRA.dll",&status);
	fver _mfc80ITA_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80ITA.dll",&status);
	fver _mfc80JPN_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80JPN.dll",&status);
	fver _mfc80KOR_dll_4053 = getFileVer(MFCLOC8_4053+L"\\mfc80KOR.dll",&status);
	fver _msvcm80_dll_4053 = getFileVer(CRT8_4053+L"\\msvcm80.dll",&status);
	fver _msvcp80_dll_4053 = getFileVer(CRT8_4053+L"\\msvcp80.dll",&status);
	fver _msvcr80_dll_4053 = getFileVer(CRT8_4053+L"\\msvcr80.dll",&status);
	fver _vcomp_dll_4053 = getFileVer(OpenMP8_4053+L"\\vcomp.dll",&status);

		if (_ATL80_dll_4053>_ATL80_dll) { _ATL80_dll = _ATL80_dll_4053; }
		if (_mfc80_dll_4053>_mfc80_dll) { _mfc80_dll = _mfc80_dll_4053; }
		if (_mfc80u_dll_4053>_mfc80u_dll) { _mfc80u_dll = _mfc80u_dll_4053; }
		if (_mfcm80_dll_4053>_mfcm80_dll) { _mfcm80_dll = _mfcm80_dll_4053; }
		if (_mfcm80u_dll_4053>_mfcm80u_dll) { _mfcm80u_dll = _mfcm80u_dll_4053; }
		if (_mfc80CHS_dll_4053>_mfc80CHS_dll) { _mfc80CHS_dll = _mfc80CHS_dll_4053; }
		if (_mfc80CHT_dll_4053>_mfc80CHT_dll) { _mfc80CHT_dll = _mfc80CHT_dll_4053; }
		if (_mfc80DEU_dll_4053>_mfc80DEU_dll) { _mfc80DEU_dll = _mfc80DEU_dll_4053; }
		if (_mfc80ENU_dll_4053>_mfc80ENU_dll) { _mfc80ENU_dll = _mfc80ENU_dll_4053; }
		if (_mfc80ESP_dll_4053>_mfc80ESP_dll) { _mfc80ESP_dll = _mfc80ESP_dll_4053; }
		if (_mfc80FRA_dll_4053>_mfc80FRA_dll) { _mfc80FRA_dll = _mfc80FRA_dll_4053; }
		if (_mfc80ITA_dll_4053>_mfc80ITA_dll) { _mfc80ITA_dll = _mfc80ITA_dll_4053; }
		if (_mfc80JPN_dll_4053>_mfc80JPN_dll) { _mfc80JPN_dll = _mfc80JPN_dll_4053; }
		if (_mfc80KOR_dll_4053>_mfc80KOR_dll) { _mfc80KOR_dll = _mfc80KOR_dll_4053; }
		if (_msvcm80_dll_4053>_msvcm80_dll) { _msvcm80_dll = _msvcm80_dll_4053; }
		if (_msvcp80_dll_4053>_msvcp80_dll) { _msvcp80_dll = _msvcp80_dll_4053; }
		if (_msvcr80_dll_4053>_msvcr80_dll) { _msvcr80_dll = _msvcr80_dll_4053; }
		if (_vcomp_dll_4053>_vcomp_dll) { _vcomp_dll = _vcomp_dll_4053; }


	std::wstring    ATL8_5592 = WinSxS+L"\\x86_Microsoft.VC80.ATL_1fc8b3b9a1e18e3b_8.0.50727.5592_x-ww_7837863c";
	std::wstring    CRT8_5592 = WinSxS+L"\\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.5592_x-ww_179798c8";
	std::wstring    MFC8_5592 = WinSxS+L"\\x86_Microsoft.VC80.MFC_1fc8b3b9a1e18e3b_8.0.50727.5592_x-ww_e87e0bcd";
	std::wstring MFCLOC8_5592 = WinSxS+L"\\x86_Microsoft.VC80.MFCLOC_1fc8b3b9a1e18e3b_8.0.50727.5592_x-ww_3dcd24cb";
	std::wstring OpenMP8_5592 = WinSxS+L"\\x86_Microsoft.VC80.OpenMP_1fc8b3b9a1e18e3b_8.0.50727.5592_x-ww_189d6662";
	fver _ATL80_dll_5592 = getFileVer(ATL8_5592+L"\\ATL80.dll",&status);
	fver _mfc80_dll_5592 = getFileVer(MFC8_5592+L"\\mfc80.dll",&status);
	fver _mfc80u_dll_5592 = getFileVer(MFC8_5592+L"\\mfc80u.dll",&status);
	fver _mfcm80_dll_5592   = getFileVer(MFC8_5592+L"\\mfcm80.dll",&status);
	fver _mfcm80u_dll_5592  = getFileVer(MFC8_5592+L"\\mfcm80u.dll",&status);
	fver _mfc80CHS_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80CHS.dll",&status);
	fver _mfc80CHT_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80CHT.dll",&status);
	fver _mfc80DEU_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80DEU.dll",&status);
	fver _mfc80ENU_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80ENU.dll",&status);
	fver _mfc80ESP_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80ESP.dll",&status);
	fver _mfc80FRA_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80FRA.dll",&status);
	fver _mfc80ITA_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80ITA.dll",&status);
	fver _mfc80JPN_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80JPN.dll",&status);
	fver _mfc80KOR_dll_5592 = getFileVer(MFCLOC8_5592+L"\\mfc80KOR.dll",&status);
	fver _msvcm80_dll_5592 = getFileVer(CRT8_5592+L"\\msvcm80.dll",&status);
	fver _msvcp80_dll_5592 = getFileVer(CRT8_5592+L"\\msvcp80.dll",&status);
	fver _msvcr80_dll_5592 = getFileVer(CRT8_5592+L"\\msvcr80.dll",&status);
	fver _vcomp_dll_5592 = getFileVer(OpenMP8_5592+L"\\vcomp.dll",&status);

		if (_ATL80_dll_5592>_ATL80_dll) { _ATL80_dll = _ATL80_dll_5592; }
		if (_mfc80_dll_5592>_mfc80_dll) { _mfc80_dll = _mfc80_dll_5592; }
		if (_mfc80u_dll_5592>_mfc80u_dll) { _mfc80u_dll = _mfc80u_dll_5592; }
		if (_mfcm80_dll_5592>_mfcm80_dll) { _mfcm80_dll = _mfcm80_dll_5592; }
		if (_mfcm80u_dll_5592>_mfcm80u_dll) { _mfcm80u_dll = _mfcm80u_dll_5592; }
		if (_mfc80CHS_dll_5592>_mfc80CHS_dll) { _mfc80CHS_dll = _mfc80CHS_dll_5592; }
		if (_mfc80CHT_dll_5592>_mfc80CHT_dll) { _mfc80CHT_dll = _mfc80CHT_dll_5592; }
		if (_mfc80DEU_dll_5592>_mfc80DEU_dll) { _mfc80DEU_dll = _mfc80DEU_dll_5592; }
		if (_mfc80ENU_dll_5592>_mfc80ENU_dll) { _mfc80ENU_dll = _mfc80ENU_dll_5592; }
		if (_mfc80ESP_dll_5592>_mfc80ESP_dll) { _mfc80ESP_dll = _mfc80ESP_dll_5592; }
		if (_mfc80FRA_dll_5592>_mfc80FRA_dll) { _mfc80FRA_dll = _mfc80FRA_dll_5592; }
		if (_mfc80ITA_dll_5592>_mfc80ITA_dll) { _mfc80ITA_dll = _mfc80ITA_dll_5592; }
		if (_mfc80JPN_dll_5592>_mfc80JPN_dll) { _mfc80JPN_dll = _mfc80JPN_dll_5592; }
		if (_mfc80KOR_dll_5592>_mfc80KOR_dll) { _mfc80KOR_dll = _mfc80KOR_dll_5592; }
		if (_msvcm80_dll_5592>_msvcm80_dll) { _msvcm80_dll = _msvcm80_dll_5592; }
		if (_msvcp80_dll_5592>_msvcp80_dll) { _msvcp80_dll = _msvcp80_dll_5592; }
		if (_msvcr80_dll_5592>_msvcr80_dll) { _msvcr80_dll = _msvcr80_dll_5592; }
		if (_vcomp_dll_5592>_vcomp_dll) { _vcomp_dll = _vcomp_dll_5592; }


	std::wstring    ATL8_6195 = WinSxS+L"\\x86_Microsoft.VC80.ATL_1fc8b3b9a1e18e3b_8.0.50727.6195_x-ww_a4c618fa";
	std::wstring    CRT8_6195 = WinSxS+L"\\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.6195_x-ww_44262b86";
	std::wstring    MFC8_6195 = WinSxS+L"\\x86_Microsoft.VC80.MFC_1fc8b3b9a1e18e3b_8.0.50727.6195_x-ww_150c9e8b";
	std::wstring MFCLOC8_6195 = WinSxS+L"\\x86_Microsoft.VC80.MFCLOC_1fc8b3b9a1e18e3b_8.0.50727.6195_x-ww_6a5bb789";
	std::wstring OpenMP8_6195 = WinSxS+L"\\x86_Microsoft.VC80.OpenMP_1fc8b3b9a1e18e3b_8.0.50727.6195_x-ww_452bf920";
	fver _ATL80_dll_6195 = getFileVer(ATL8_6195+L"\\ATL80.dll",&status);
	fver _mfc80_dll_6195 = getFileVer(MFC8_6195+L"\\mfc80.dll",&status);
	fver _mfc80u_dll_6195 = getFileVer(MFC8_6195+L"\\mfc80u.dll",&status);
	fver _mfcm80_dll_6195   = getFileVer(MFC8_6195+L"\\mfcm80.dll",&status);
	fver _mfcm80u_dll_6195  = getFileVer(MFC8_6195+L"\\mfcm80u.dll",&status);
	fver _mfc80CHS_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80CHS.dll",&status);
	fver _mfc80CHT_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80CHT.dll",&status);
	fver _mfc80DEU_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80DEU.dll",&status);
	fver _mfc80ENU_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80ENU.dll",&status);
	fver _mfc80ESP_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80ESP.dll",&status);
	fver _mfc80FRA_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80FRA.dll",&status);
	fver _mfc80ITA_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80ITA.dll",&status);
	fver _mfc80JPN_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80JPN.dll",&status);
	fver _mfc80KOR_dll_6195 = getFileVer(MFCLOC8_6195+L"\\mfc80KOR.dll",&status);
	fver _msvcm80_dll_6195 = getFileVer(CRT8_6195+L"\\msvcm80.dll",&status);
	fver _msvcp80_dll_6195 = getFileVer(CRT8_6195+L"\\msvcp80.dll",&status);
	fver _msvcr80_dll_6195 = getFileVer(CRT8_6195+L"\\msvcr80.dll",&status);
	fver _vcomp_dll_6195 = getFileVer(OpenMP8_6195+L"\\vcomp.dll",&status);

		if (_ATL80_dll_6195>_ATL80_dll) { _ATL80_dll = _ATL80_dll_6195; }
		if (_mfc80_dll_6195>_mfc80_dll) { _mfc80_dll = _mfc80_dll_6195; }
		if (_mfc80u_dll_6195>_mfc80u_dll) { _mfc80u_dll = _mfc80u_dll_6195; }
		if (_mfcm80_dll_6195>_mfcm80_dll) { _mfcm80_dll = _mfcm80_dll_6195; }
		if (_mfcm80u_dll_6195>_mfcm80u_dll) { _mfcm80u_dll = _mfcm80u_dll_6195; }
		if (_mfc80CHS_dll_6195>_mfc80CHS_dll) { _mfc80CHS_dll = _mfc80CHS_dll_6195; }
		if (_mfc80CHT_dll_6195>_mfc80CHT_dll) { _mfc80CHT_dll = _mfc80CHT_dll_6195; }
		if (_mfc80DEU_dll_6195>_mfc80DEU_dll) { _mfc80DEU_dll = _mfc80DEU_dll_6195; }
		if (_mfc80ENU_dll_6195>_mfc80ENU_dll) { _mfc80ENU_dll = _mfc80ENU_dll_6195; }
		if (_mfc80ESP_dll_6195>_mfc80ESP_dll) { _mfc80ESP_dll = _mfc80ESP_dll_6195; }
		if (_mfc80FRA_dll_6195>_mfc80FRA_dll) { _mfc80FRA_dll = _mfc80FRA_dll_6195; }
		if (_mfc80ITA_dll_6195>_mfc80ITA_dll) { _mfc80ITA_dll = _mfc80ITA_dll_6195; }
		if (_mfc80JPN_dll_6195>_mfc80JPN_dll) { _mfc80JPN_dll = _mfc80JPN_dll_6195; }
		if (_mfc80KOR_dll_6195>_mfc80KOR_dll) { _mfc80KOR_dll = _mfc80KOR_dll_6195; }
		if (_msvcm80_dll_6195>_msvcm80_dll) { _msvcm80_dll = _msvcm80_dll_6195; }
		if (_msvcp80_dll_6195>_msvcp80_dll) { _msvcp80_dll = _msvcp80_dll_6195; }
		if (_msvcr80_dll_6195>_msvcr80_dll) { _msvcr80_dll = _msvcr80_dll_6195; }
		if (_vcomp_dll_6195>_vcomp_dll) { _vcomp_dll = _vcomp_dll_6195; }
	
		fver t = fver(8,0,50727,42);
		if( _ATL80_dll    >= t
		 && _msdia80_dll  >= t
		 && _mfc80_dll    >= t
		 && _mfc80u_dll   >= t
		 && _mfcm80_dll   >= t
		 && _mfcm80u_dll  >= t
		 && _mfc80CHS_dll >= t
		 && _mfc80CHT_dll >= t
		 && _mfc80DEU_dll >= t
		 && _mfc80ENU_dll >= t
		 && _mfc80ESP_dll >= t
		 && _mfc80FRA_dll >= t
		 && _mfc80ITA_dll >= t
		 && _mfc80JPN_dll >= t
		 && _mfc80KOR_dll >= t
		 && _msvcm80_dll  >= t
		 && _msvcp80_dll  >= t
		 && _msvcr80_dll  >= t
		 && _vcomp_dll    >= t ) { f = t; }

		t = fver(8,0,50727,762);
		if( _ATL80_dll    >= t
		 && _msdia80_dll  >= t
		 && _mfc80_dll    >= t
		 && _mfc80u_dll   >= t
		 && _mfcm80_dll   >= t
		 && _mfcm80u_dll  >= t
		 && _mfc80CHS_dll >= t
		 && _mfc80CHT_dll >= t
		 && _mfc80DEU_dll >= t
		 && _mfc80ENU_dll >= t
		 && _mfc80ESP_dll >= t
		 && _mfc80FRA_dll >= t
		 && _mfc80ITA_dll >= t
		 && _mfc80JPN_dll >= t
		 && _mfc80KOR_dll >= t
		 && _msvcm80_dll  >= t
		 && _msvcp80_dll  >= t
		 && _msvcr80_dll  >= t
		 && _vcomp_dll    >= t ) { f = t; }

		t = fver(8,0,50727,4053);
		if( _ATL80_dll    >= t
		 && _msdia80_dll  >= t
		 && _mfc80_dll    >= t
		 && _mfc80u_dll   >= t
		 && _mfcm80_dll   >= t
		 && _mfcm80u_dll  >= t
		 && _mfc80CHS_dll >= t
		 && _mfc80CHT_dll >= t
		 && _mfc80DEU_dll >= t
		 && _mfc80ENU_dll >= t
		 && _mfc80ESP_dll >= t
		 && _mfc80FRA_dll >= t
		 && _mfc80ITA_dll >= t
		 && _mfc80JPN_dll >= t
		 && _mfc80KOR_dll >= t
		 && _msvcm80_dll  >= t
		 && _msvcp80_dll  >= t
		 && _msvcr80_dll  >= t
		 && _vcomp_dll    >= t ) { f = t; }

		t = fver(8,0,50727,5592);
		if( _ATL80_dll    >= t
		 && _msdia80_dll  >= t
		 && _mfc80_dll    >= t
		 && _mfc80u_dll   >= t
		 && _mfcm80_dll   >= t
		 && _mfcm80u_dll  >= t
		 && _mfc80CHS_dll >= t
		 && _mfc80CHT_dll >= t
		 && _mfc80DEU_dll >= t
		 && _mfc80ENU_dll >= t
		 && _mfc80ESP_dll >= t
		 && _mfc80FRA_dll >= t
		 && _mfc80ITA_dll >= t
		 && _mfc80JPN_dll >= t
		 && _mfc80KOR_dll >= t
		 && _msvcm80_dll  >= t
		 && _msvcp80_dll  >= t
		 && _msvcr80_dll  >= t
		 && _vcomp_dll    >= t ) { f = t; }

		t = fver(8,0,50727,6195);
		if( _ATL80_dll    >= t
		 && _msdia80_dll  >= t
		 && _mfc80_dll    >= t
		 && _mfc80u_dll   >= t
		 && _mfcm80_dll   >= t
		 && _mfcm80u_dll  >= t
		 && _mfc80CHS_dll >= t
		 && _mfc80CHT_dll >= t
		 && _mfc80DEU_dll >= t
		 && _mfc80ENU_dll >= t
		 && _mfc80ESP_dll >= t
		 && _mfc80FRA_dll >= t
		 && _mfc80ITA_dll >= t
		 && _mfc80JPN_dll >= t
		 && _mfc80KOR_dll >= t
		 && _msvcm80_dll  >= t
		 && _msvcp80_dll  >= t
		 && _msvcr80_dll  >= t
		 && _vcomp_dll    >= t ) { f = t; }

		if(debug) {
			printf("Microsoft Visual C++ 2005 DEBUG:\n");
			printf("%s\t%s\n","ATL80.dll",_ATL80_dll.format().c_str());
			printf("%s\t%s\n","msdia80.dll",_msdia80_dll.format().c_str());
			printf("%s\t%s\n","mfc80.dll",_mfc80_dll.format().c_str());
			printf("%s\t%s\n","mfc80u.dll",_mfc80u_dll.format().c_str());
			printf("%s\t%s\n","mfcm80.dll",_mfcm80_dll.format().c_str());
			printf("%s\t%s\n","mfcm80u.dll",_mfcm80u_dll.format().c_str());
			printf("%s\t%s\n","mfc80CHS.dll",_mfc80CHS_dll.format().c_str());
			printf("%s\t%s\n","mfc80CHT.dll",_mfc80CHT_dll.format().c_str());
			printf("%s\t%s\n","mfc80DEU.dll",_mfc80DEU_dll.format().c_str());
			printf("%s\t%s\n","mfc80ENU.dll",_mfc80ENU_dll.format().c_str());
			printf("%s\t%s\n","mfc80ESP.dll",_mfc80ESP_dll.format().c_str());
			printf("%s\t%s\n","mfc80FRA.dll",_mfc80FRA_dll.format().c_str());
			printf("%s\t%s\n","mfc80ITA.dll",_mfc80ITA_dll.format().c_str());
			printf("%s\t%s\n","mfc80JPN.dll",_mfc80JPN_dll.format().c_str());
			printf("%s\t%s\n","mfc80KOR.dll",_mfc80KOR_dll.format().c_str());
			printf("%s\t%s\n","msvcm80.dll",_msvcm80_dll.format().c_str());
			printf("%s\t%s\n","msvcp80.dll",_msvcp80_dll.format().c_str());
			printf("%s\t%s\n","msvcr80.dll",_msvcr80_dll.format().c_str());
			printf("%s\t%s\n","vcomp.dll",_vcomp_dll.format().c_str());
			printf("\n");
			printf("VCREDIST_2005_VERSION = %s\n\n",f.format().c_str());
		}

		return f;
}

fver vcr2008(bool debug) {

	int status=0;
	fver f; int vcsp=0;

	// Identify system paths
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&status);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";

	std::wstring      ATL9_8 = WinSxS+L"\\x86_Microsoft.VC90.ATL_1fc8b3b9a1e18e3b_9.0.21022.8_x-ww_312cf0e9";
	std::wstring      CRT9_8 = WinSxS+L"\\x86_Microsoft.VC90.CRT_1fc8b3b9a1e18e3b_9.0.21022.8_x-ww_d08d0375";
	std::wstring      MFC9_8 = WinSxS+L"\\x86_Microsoft.VC90.MFC_1fc8b3b9a1e18e3b_9.0.21022.8_x-ww_a173767a";
	std::wstring   MFCLOC9_8 = WinSxS+L"\\x86_Microsoft.VC90.MFCLOC_1fc8b3b9a1e18e3b_9.0.21022.8_x-ww_11f3ea3a";
	std::wstring   OpenMP9_8 = WinSxS+L"\\x86_Microsoft.VC90.OpenMP_1fc8b3b9a1e18e3b_9.0.21022.8_x-ww_ecc42bd1";

	fver _atl90_dll_8 = getFileVer(ATL9_8+L"\\atl90.dll",&status);
	fver _msvcm90_dll_8 = getFileVer(CRT9_8+L"\\msvcm90.dll",&status);
	fver _msvcp90_dll_8 = getFileVer(CRT9_8+L"\\msvcp90.dll",&status);
	fver _msvcr90_dll_8 = getFileVer(CRT9_8+L"\\msvcr90.dll",&status);
	fver _mfc90_dll_8 = getFileVer(MFC9_8+L"\\mfc90.dll",&status);
	fver _mfc90u_dll_8 = getFileVer(MFC9_8+L"\\mfc90u.dll",&status);
	fver _mfcm90_dll_8 = getFileVer(MFC9_8+L"\\mfcm90.dll",&status);
	fver _mfcm90u_dll_8 = getFileVer(MFC9_8+L"\\mfcm90u.dll",&status);
	fver _mfc90chs_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90chs.dll",&status);
	fver _mfc90cht_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90cht.dll",&status);
	fver _mfc90deu_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90deu.dll",&status);
	fver _mfc90enu_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90enu.dll",&status);
	fver _mfc90esn_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90esn.dll",&status);
	fver _mfc90esp_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90esp.dll",&status);
	fver _mfc90fra_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90fra.dll",&status);
	fver _mfc90ita_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90ita.dll",&status);
	fver _mfc90jpn_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90jpn.dll",&status);
	fver _mfc90kor_dll_8 = getFileVer(MFCLOC9_8+L"\\mfc90kor.dll",&status);
	fver _vcomp90_dll_8 = getFileVer(OpenMP9_8+L"\\vcomp90.dll",&status);

	fver _atl90_dll = _atl90_dll_8;

	fver _msvcm90_dll = _msvcm90_dll_8; // installed with .NET Framework
	fver _msvcp90_dll = _msvcp90_dll_8; // installed with .NET Framework
	fver _msvcr90_dll = _msvcr90_dll_8; // installed with .NET Framework
	fver _mfc90_dll = _mfc90_dll_8;
	fver _mfc90u_dll = _mfc90u_dll_8;
	fver _mfcm90_dll = _mfcm90_dll_8;
	fver _mfcm90u_dll = _mfcm90u_dll_8;
	fver _mfc90chs_dll = _mfc90chs_dll_8;
	fver _mfc90cht_dll = _mfc90cht_dll_8;
	fver _mfc90deu_dll = _mfc90deu_dll_8;
	fver _mfc90enu_dll = _mfc90enu_dll_8;
	fver _mfc90esn_dll = _mfc90esn_dll_8;
	fver _mfc90esp_dll = _mfc90esp_dll_8;
	fver _mfc90fra_dll = _mfc90fra_dll_8;
	fver _mfc90ita_dll = _mfc90ita_dll_8;
	fver _mfc90jpn_dll = _mfc90jpn_dll_8;
	fver _mfc90kor_dll = _mfc90kor_dll_8;
	fver _vcomp90_dll = _vcomp90_dll_8;

	std::wstring      ATL9_218 = WinSxS+L"\\x86_Microsoft.VC90.ATL_1fc8b3b9a1e18e3b_9.0.21022.218_x-ww_42101c2a";
	std::wstring      CRT9_218 = WinSxS+L"\\x86_Microsoft.VC90.CRT_1fc8b3b9a1e18e3b_9.0.21022.218_x-ww_e1702eb6";
	std::wstring      MFC9_218 = WinSxS+L"\\x86_Microsoft.VC90.MFC_1fc8b3b9a1e18e3b_9.0.21022.218_x-ww_b256a1bb";
	std::wstring   MFCLOC9_218 = WinSxS+L"\\x86_Microsoft.VC90.MFCLOC_1fc8b3b9a1e18e3b_9.0.21022.218_x-ww_22d7157b";
	std::wstring   OpenMP9_218 = WinSxS+L"\\x86_Microsoft.VC90.OpenMP_1fc8b3b9a1e18e3b_9.0.21022.218_x-ww_fda75712";

	fver _atl90_dll_218 = getFileVer(ATL9_218+L"\\atl90.dll",&status);
	fver _msvcm90_dll_218 = getFileVer(CRT9_218+L"\\msvcm90.dll",&status);
	fver _msvcp90_dll_218 = getFileVer(CRT9_218+L"\\msvcp90.dll",&status);
	fver _msvcr90_dll_218 = getFileVer(CRT9_218+L"\\msvcr90.dll",&status);
	fver _mfc90_dll_218 = getFileVer(MFC9_218+L"\\mfc90.dll",&status);
	fver _mfc90u_dll_218 = getFileVer(MFC9_218+L"\\mfc90u.dll",&status);
	fver _mfcm90_dll_218 = getFileVer(MFC9_218+L"\\mfcm90.dll",&status);
	fver _mfcm90u_dll_218 = getFileVer(MFC9_218+L"\\mfcm90u.dll",&status);
	fver _mfc90chs_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90chs.dll",&status);
	fver _mfc90cht_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90cht.dll",&status);
	fver _mfc90deu_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90deu.dll",&status);
	fver _mfc90enu_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90enu.dll",&status);
	fver _mfc90esn_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90esn.dll",&status);
	fver _mfc90esp_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90esp.dll",&status);
	fver _mfc90fra_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90fra.dll",&status);
	fver _mfc90ita_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90ita.dll",&status);
	fver _mfc90jpn_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90jpn.dll",&status);
	fver _mfc90kor_dll_218 = getFileVer(MFCLOC9_218+L"\\mfc90kor.dll",&status);
	fver _vcomp90_dll_218 = getFileVer(OpenMP9_218+L"\\vcomp90.dll",&status);

	if(_atl90_dll_218    >_atl90_dll   ) {_atl90_dll    = _atl90_dll_218;   }
	if(_msvcm90_dll_218  >_msvcm90_dll ) {_msvcm90_dll  = _msvcm90_dll_218; }
	if(_msvcp90_dll_218  >_msvcp90_dll ) {_msvcp90_dll  = _msvcp90_dll_218; }
	if(_msvcr90_dll_218  >_msvcr90_dll ) {_msvcr90_dll  = _msvcr90_dll_218; }
	if(_mfc90_dll_218    >_mfc90_dll   ) {_mfc90_dll    = _mfc90_dll_218;   }
	if(_mfc90u_dll_218   >_mfc90u_dll  ) {_mfc90u_dll   = _mfc90u_dll_218;  }
	if(_mfcm90_dll_218   >_mfcm90_dll  ) {_mfcm90_dll   = _mfcm90_dll_218;  }
	if(_mfcm90u_dll_218  >_mfcm90u_dll ) {_mfcm90u_dll  = _mfcm90u_dll_218; }
	if(_mfc90chs_dll_218 >_mfc90chs_dll) {_mfc90chs_dll = _mfc90chs_dll_218;}
	if(_mfc90cht_dll_218 >_mfc90cht_dll) {_mfc90cht_dll = _mfc90cht_dll_218;}
	if(_mfc90deu_dll_218 >_mfc90deu_dll) {_mfc90deu_dll = _mfc90deu_dll_218;}
	if(_mfc90enu_dll_218 >_mfc90enu_dll) {_mfc90enu_dll = _mfc90enu_dll_218;}
	if(_mfc90esn_dll_218 >_mfc90esn_dll) {_mfc90esn_dll = _mfc90esn_dll_218;}
	if(_mfc90esp_dll_218 >_mfc90esp_dll) {_mfc90esp_dll = _mfc90esp_dll_218;}
	if(_mfc90fra_dll_218 >_mfc90fra_dll) {_mfc90fra_dll = _mfc90fra_dll_218;}
	if(_mfc90ita_dll_218 >_mfc90ita_dll) {_mfc90ita_dll = _mfc90ita_dll_218;}
	if(_mfc90jpn_dll_218 >_mfc90jpn_dll) {_mfc90jpn_dll = _mfc90jpn_dll_218;}
	if(_mfc90kor_dll_218 >_mfc90kor_dll) {_mfc90kor_dll = _mfc90kor_dll_218;}
	if(_vcomp90_dll_218  >_vcomp90_dll ) {_vcomp90_dll  = _vcomp90_dll_218; }

	std::wstring      ATL9_001 = WinSxS+L"\\x86_Microsoft.VC90.ATL_1fc8b3b9a1e18e3b_9.0.30729.1_x-ww_d01483b2";
	std::wstring      CRT9_001 = WinSxS+L"\\x86_Microsoft.VC90.CRT_1fc8b3b9a1e18e3b_9.0.30729.1_x-ww_6f74963e";
	std::wstring      MFC9_001 = WinSxS+L"\\x86_Microsoft.VC90.MFC_1fc8b3b9a1e18e3b_9.0.30729.1_x-ww_405b0943";
	std::wstring   MFCLOC9_001 = WinSxS+L"\\x86_Microsoft.VC90.MFCLOC_1fc8b3b9a1e18e3b_9.0.30729.1_x-ww_b0db7d03";
//	std::wstring   OpenMP9_001 = WinSxS+L"\\";

	fver _atl90_dll_001 = getFileVer(ATL9_001+L"\\atl90.dll",&status);
	fver _msvcm90_dll_001 = getFileVer(CRT9_001+L"\\msvcm90.dll",&status);
	fver _msvcp90_dll_001 = getFileVer(CRT9_001+L"\\msvcp90.dll",&status);
	fver _msvcr90_dll_001 = getFileVer(CRT9_001+L"\\msvcr90.dll",&status);
	fver _mfc90_dll_001 = getFileVer(MFC9_001+L"\\mfc90.dll",&status);
	fver _mfc90u_dll_001 = getFileVer(MFC9_001+L"\\mfc90u.dll",&status);
	fver _mfcm90_dll_001 = getFileVer(MFC9_001+L"\\mfcm90.dll",&status);
	fver _mfcm90u_dll_001 = getFileVer(MFC9_001+L"\\mfcm90u.dll",&status);
	fver _mfc90chs_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90chs.dll",&status);
	fver _mfc90cht_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90cht.dll",&status);
	fver _mfc90deu_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90deu.dll",&status);
	fver _mfc90enu_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90enu.dll",&status);
	fver _mfc90esn_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90esn.dll",&status);
	fver _mfc90esp_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90esp.dll",&status);
	fver _mfc90fra_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90fra.dll",&status);
	fver _mfc90ita_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90ita.dll",&status);
	fver _mfc90jpn_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90jpn.dll",&status);
	fver _mfc90kor_dll_001 = getFileVer(MFCLOC9_001+L"\\mfc90kor.dll",&status);
//	fver _vcomp90_dll_001 = getFileVer(OpenMP9_001+L"\\vcomp90.dll",&status);

	if(_atl90_dll_001    >_atl90_dll   ) {_atl90_dll    = _atl90_dll_001;   }
	if(_msvcm90_dll_001  >_msvcm90_dll ) {_msvcm90_dll  = _msvcm90_dll_001; }
	if(_msvcp90_dll_001  >_msvcp90_dll ) {_msvcp90_dll  = _msvcp90_dll_001; }
	if(_msvcr90_dll_001  >_msvcr90_dll ) {_msvcr90_dll  = _msvcr90_dll_001; }
	if(_mfc90_dll_001    >_mfc90_dll   ) {_mfc90_dll    = _mfc90_dll_001;   }
	if(_mfc90u_dll_001   >_mfc90u_dll  ) {_mfc90u_dll   = _mfc90u_dll_001;  }
	if(_mfcm90_dll_001   >_mfcm90_dll  ) {_mfcm90_dll   = _mfcm90_dll_001;  }
	if(_mfcm90u_dll_001  >_mfcm90u_dll ) {_mfcm90u_dll  = _mfcm90u_dll_001; }
	if(_mfc90chs_dll_001 >_mfc90chs_dll) {_mfc90chs_dll = _mfc90chs_dll_001;}
	if(_mfc90cht_dll_001 >_mfc90cht_dll) {_mfc90cht_dll = _mfc90cht_dll_001;}
	if(_mfc90deu_dll_001 >_mfc90deu_dll) {_mfc90deu_dll = _mfc90deu_dll_001;}
	if(_mfc90enu_dll_001 >_mfc90enu_dll) {_mfc90enu_dll = _mfc90enu_dll_001;}
	if(_mfc90esn_dll_001 >_mfc90esn_dll) {_mfc90esn_dll = _mfc90esn_dll_001;}
	if(_mfc90esp_dll_001 >_mfc90esp_dll) {_mfc90esp_dll = _mfc90esp_dll_001;}
	if(_mfc90fra_dll_001 >_mfc90fra_dll) {_mfc90fra_dll = _mfc90fra_dll_001;}
	if(_mfc90ita_dll_001 >_mfc90ita_dll) {_mfc90ita_dll = _mfc90ita_dll_001;}
	if(_mfc90jpn_dll_001 >_mfc90jpn_dll) {_mfc90jpn_dll = _mfc90jpn_dll_001;}
	if(_mfc90kor_dll_001 >_mfc90kor_dll) {_mfc90kor_dll = _mfc90kor_dll_001;}
//	if(_vcomp90_dll_001  >_vcomp90_dll ) {_vcomp90_dll  = _vcomp90_dll_001; }

	std::wstring      ATL9_4148 = WinSxS+L"\\x86_Microsoft.VC90.ATL_1fc8b3b9a1e18e3b_9.0.30729.4148_x-ww_353599c2";
	std::wstring      CRT9_4148 = WinSxS+L"\\x86_Microsoft.VC90.CRT_1fc8b3b9a1e18e3b_9.0.30729.4148_x-ww_d495ac4e";
	std::wstring      MFC9_4148 = WinSxS+L"\\x86_Microsoft.VC90.MFC_1fc8b3b9a1e18e3b_9.0.30729.4148_x-ww_a57c1f53";
	std::wstring   MFCLOC9_4148 = WinSxS+L"\\x86_Microsoft.VC90.MFCLOC_1fc8b3b9a1e18e3b_9.0.30729.4148_x-ww_15fc9313";
	std::wstring   OpenMP9_4148 = WinSxS+L"\\x86_Microsoft.VC90.OpenMP_1fc8b3b9a1e18e3b_9.0.30729.4148_x-ww_f0ccd4aa";

	fver _atl90_dll_4148 = getFileVer(ATL9_4148+L"\\atl90.dll",&status);
	fver _msvcm90_dll_4148 = getFileVer(CRT9_4148+L"\\msvcm90.dll",&status);
	fver _msvcp90_dll_4148 = getFileVer(CRT9_4148+L"\\msvcp90.dll",&status);
	fver _msvcr90_dll_4148 = getFileVer(CRT9_4148+L"\\msvcr90.dll",&status);
	fver _mfc90_dll_4148 = getFileVer(MFC9_4148+L"\\mfc90.dll",&status);
	fver _mfc90u_dll_4148 = getFileVer(MFC9_4148+L"\\mfc90u.dll",&status);
	fver _mfcm90_dll_4148 = getFileVer(MFC9_4148+L"\\mfcm90.dll",&status);
	fver _mfcm90u_dll_4148 = getFileVer(MFC9_4148+L"\\mfcm90u.dll",&status);
	fver _mfc90chs_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90chs.dll",&status);
	fver _mfc90cht_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90cht.dll",&status);
	fver _mfc90deu_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90deu.dll",&status);
	fver _mfc90enu_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90enu.dll",&status);
	fver _mfc90esn_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90esn.dll",&status);
	fver _mfc90esp_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90esp.dll",&status);
	fver _mfc90fra_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90fra.dll",&status);
	fver _mfc90ita_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90ita.dll",&status);
	fver _mfc90jpn_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90jpn.dll",&status);
	fver _mfc90kor_dll_4148 = getFileVer(MFCLOC9_4148+L"\\mfc90kor.dll",&status);
	fver _vcomp90_dll_4148 = getFileVer(OpenMP9_4148+L"\\vcomp90.dll",&status);

	if(_atl90_dll_4148    >_atl90_dll   ) {_atl90_dll    = _atl90_dll_4148;   }
	if(_msvcm90_dll_4148  >_msvcm90_dll ) {_msvcm90_dll  = _msvcm90_dll_4148; }
	if(_msvcp90_dll_4148  >_msvcp90_dll ) {_msvcp90_dll  = _msvcp90_dll_4148; }
	if(_msvcr90_dll_4148  >_msvcr90_dll ) {_msvcr90_dll  = _msvcr90_dll_4148; }
	if(_mfc90_dll_4148    >_mfc90_dll   ) {_mfc90_dll    = _mfc90_dll_4148;   }
	if(_mfc90u_dll_4148   >_mfc90u_dll  ) {_mfc90u_dll   = _mfc90u_dll_4148;  }
	if(_mfcm90_dll_4148   >_mfcm90_dll  ) {_mfcm90_dll   = _mfcm90_dll_4148;  }
	if(_mfcm90u_dll_4148  >_mfcm90u_dll ) {_mfcm90u_dll  = _mfcm90u_dll_4148; }
	if(_mfc90chs_dll_4148 >_mfc90chs_dll) {_mfc90chs_dll = _mfc90chs_dll_4148;}
	if(_mfc90cht_dll_4148 >_mfc90cht_dll) {_mfc90cht_dll = _mfc90cht_dll_4148;}
	if(_mfc90deu_dll_4148 >_mfc90deu_dll) {_mfc90deu_dll = _mfc90deu_dll_4148;}
	if(_mfc90enu_dll_4148 >_mfc90enu_dll) {_mfc90enu_dll = _mfc90enu_dll_4148;}
	if(_mfc90esn_dll_4148 >_mfc90esn_dll) {_mfc90esn_dll = _mfc90esn_dll_4148;}
	if(_mfc90esp_dll_4148 >_mfc90esp_dll) {_mfc90esp_dll = _mfc90esp_dll_4148;}
	if(_mfc90fra_dll_4148 >_mfc90fra_dll) {_mfc90fra_dll = _mfc90fra_dll_4148;}
	if(_mfc90ita_dll_4148 >_mfc90ita_dll) {_mfc90ita_dll = _mfc90ita_dll_4148;}
	if(_mfc90jpn_dll_4148 >_mfc90jpn_dll) {_mfc90jpn_dll = _mfc90jpn_dll_4148;}
	if(_mfc90kor_dll_4148 >_mfc90kor_dll) {_mfc90kor_dll = _mfc90kor_dll_4148;}
	if(_vcomp90_dll_4148  >_vcomp90_dll ) {_vcomp90_dll  = _vcomp90_dll_4148; }

	std::wstring      ATL9_5570 = WinSxS+L"\\x86_Microsoft.VC90.ATL_1fc8b3b9a1e18e3b_9.0.30729.5570_x-ww_65b7a93a";
	std::wstring      CRT9_5570 = WinSxS+L"\\x86_Microsoft.VC90.CRT_1fc8b3b9a1e18e3b_9.0.30729.5570_x-ww_0517bbc6";
	std::wstring      MFC9_5570 = WinSxS+L"\\x86_Microsoft.VC90.MFC_1fc8b3b9a1e18e3b_9.0.30729.5570_x-ww_d5fe2ecb";
	std::wstring   MFCLOC9_5570 = WinSxS+L"\\x86_Microsoft.VC90.MFCLOC_1fc8b3b9a1e18e3b_9.0.30729.5570_x-ww_467ea28b";
	std::wstring   OpenMP9_5570 = WinSxS+L"\\x86_Microsoft.VC90.OpenMP_1fc8b3b9a1e18e3b_9.0.30729.5570_x-ww_214ee422";

	fver _atl90_dll_5570 = getFileVer(ATL9_5570+L"\\atl90.dll",&status);
	fver _msvcm90_dll_5570 = getFileVer(CRT9_5570+L"\\msvcm90.dll",&status);
	fver _msvcp90_dll_5570 = getFileVer(CRT9_5570+L"\\msvcp90.dll",&status);
	fver _msvcr90_dll_5570 = getFileVer(CRT9_5570+L"\\msvcr90.dll",&status);
	fver _mfc90_dll_5570 = getFileVer(MFC9_5570+L"\\mfc90.dll",&status);
	fver _mfc90u_dll_5570 = getFileVer(MFC9_5570+L"\\mfc90u.dll",&status);
	fver _mfcm90_dll_5570 = getFileVer(MFC9_5570+L"\\mfcm90.dll",&status);
	fver _mfcm90u_dll_5570 = getFileVer(MFC9_5570+L"\\mfcm90u.dll",&status);
	fver _mfc90chs_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90chs.dll",&status);
	fver _mfc90cht_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90cht.dll",&status);
	fver _mfc90deu_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90deu.dll",&status);
	fver _mfc90enu_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90enu.dll",&status);
	fver _mfc90esn_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90esn.dll",&status);
	fver _mfc90esp_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90esp.dll",&status);
	fver _mfc90fra_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90fra.dll",&status);
	fver _mfc90ita_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90ita.dll",&status);
	fver _mfc90jpn_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90jpn.dll",&status);
	fver _mfc90kor_dll_5570 = getFileVer(MFCLOC9_5570+L"\\mfc90kor.dll",&status);
	fver _vcomp90_dll_5570 = getFileVer(OpenMP9_5570+L"\\vcomp90.dll",&status);

	if(_atl90_dll_5570    >_atl90_dll   ) {_atl90_dll    = _atl90_dll_5570;   }
	if(_msvcm90_dll_5570  >_msvcm90_dll ) {_msvcm90_dll  = _msvcm90_dll_5570; }
	if(_msvcp90_dll_5570  >_msvcp90_dll ) {_msvcp90_dll  = _msvcp90_dll_5570; }
	if(_msvcr90_dll_5570  >_msvcr90_dll ) {_msvcr90_dll  = _msvcr90_dll_5570; }
	if(_mfc90_dll_5570    >_mfc90_dll   ) {_mfc90_dll    = _mfc90_dll_5570;   }
	if(_mfc90u_dll_5570   >_mfc90u_dll  ) {_mfc90u_dll   = _mfc90u_dll_5570;  }
	if(_mfcm90_dll_5570   >_mfcm90_dll  ) {_mfcm90_dll   = _mfcm90_dll_5570;  }
	if(_mfcm90u_dll_5570  >_mfcm90u_dll ) {_mfcm90u_dll  = _mfcm90u_dll_5570; }
	if(_mfc90chs_dll_5570 >_mfc90chs_dll) {_mfc90chs_dll = _mfc90chs_dll_5570;}
	if(_mfc90cht_dll_5570 >_mfc90cht_dll) {_mfc90cht_dll = _mfc90cht_dll_5570;}
	if(_mfc90deu_dll_5570 >_mfc90deu_dll) {_mfc90deu_dll = _mfc90deu_dll_5570;}
	if(_mfc90enu_dll_5570 >_mfc90enu_dll) {_mfc90enu_dll = _mfc90enu_dll_5570;}
	if(_mfc90esn_dll_5570 >_mfc90esn_dll) {_mfc90esn_dll = _mfc90esn_dll_5570;}
	if(_mfc90esp_dll_5570 >_mfc90esp_dll) {_mfc90esp_dll = _mfc90esp_dll_5570;}
	if(_mfc90fra_dll_5570 >_mfc90fra_dll) {_mfc90fra_dll = _mfc90fra_dll_5570;}
	if(_mfc90ita_dll_5570 >_mfc90ita_dll) {_mfc90ita_dll = _mfc90ita_dll_5570;}
	if(_mfc90jpn_dll_5570 >_mfc90jpn_dll) {_mfc90jpn_dll = _mfc90jpn_dll_5570;}
	if(_mfc90kor_dll_5570 >_mfc90kor_dll) {_mfc90kor_dll = _mfc90kor_dll_5570;}
	if(_vcomp90_dll_5570  >_vcomp90_dll ) {_vcomp90_dll  = _vcomp90_dll_5570; }

	std::wstring      ATL9_6161 = WinSxS+L"\\x86_Microsoft.VC90.ATL_1fc8b3b9a1e18e3b_9.0.30729.6161_x-ww_92453bb7";
	std::wstring      CRT9_6161 = WinSxS+L"\\x86_Microsoft.VC90.CRT_1fc8b3b9a1e18e3b_9.0.30729.6161_x-ww_31a54e43";
	std::wstring      MFC9_6161 = WinSxS+L"\\x86_Microsoft.VC90.MFC_1fc8b3b9a1e18e3b_9.0.30729.6161_x-ww_028bc148";
	std::wstring   MFCLOC9_6161 = WinSxS+L"\\x86_Microsoft.VC90.MFCLOC_1fc8b3b9a1e18e3b_9.0.30729.6161_x-ww_730c3508";
	std::wstring   OpenMP9_6161 = WinSxS+L"\\x86_Microsoft.VC90.OpenMP_1fc8b3b9a1e18e3b_9.0.30729.6161_x-ww_4ddc769f";

	fver _atl90_dll_6161 = getFileVer(ATL9_6161+L"\\atl90.dll",&status);
	fver _msvcm90_dll_6161 = getFileVer(CRT9_6161+L"\\msvcm90.dll",&status);
	fver _msvcp90_dll_6161 = getFileVer(CRT9_6161+L"\\msvcp90.dll",&status);
	fver _msvcr90_dll_6161 = getFileVer(CRT9_6161+L"\\msvcr90.dll",&status);
	fver _mfc90_dll_6161 = getFileVer(MFC9_6161+L"\\mfc90.dll",&status);
	fver _mfc90u_dll_6161 = getFileVer(MFC9_6161+L"\\mfc90u.dll",&status);
	fver _mfcm90_dll_6161 = getFileVer(MFC9_6161+L"\\mfcm90.dll",&status);
	fver _mfcm90u_dll_6161 = getFileVer(MFC9_6161+L"\\mfcm90u.dll",&status);
	fver _mfc90chs_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90chs.dll",&status);
	fver _mfc90cht_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90cht.dll",&status);
	fver _mfc90deu_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90deu.dll",&status);
	fver _mfc90enu_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90enu.dll",&status);
	fver _mfc90esn_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90esn.dll",&status);
	fver _mfc90esp_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90esp.dll",&status);
	fver _mfc90fra_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90fra.dll",&status);
	fver _mfc90ita_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90ita.dll",&status);
	fver _mfc90jpn_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90jpn.dll",&status);
	fver _mfc90kor_dll_6161 = getFileVer(MFCLOC9_6161+L"\\mfc90kor.dll",&status);
	fver _vcomp90_dll_6161 = getFileVer(OpenMP9_6161+L"\\vcomp90.dll",&status);

	if(_atl90_dll_6161    >_atl90_dll   ) {_atl90_dll    = _atl90_dll_6161;   }
	if(_msvcm90_dll_6161  >_msvcm90_dll ) {_msvcm90_dll  = _msvcm90_dll_6161; }
	if(_msvcp90_dll_6161  >_msvcp90_dll ) {_msvcp90_dll  = _msvcp90_dll_6161; }
	if(_msvcr90_dll_6161  >_msvcr90_dll ) {_msvcr90_dll  = _msvcr90_dll_6161; }
	if(_mfc90_dll_6161    >_mfc90_dll   ) {_mfc90_dll    = _mfc90_dll_6161;   }
	if(_mfc90u_dll_6161   >_mfc90u_dll  ) {_mfc90u_dll   = _mfc90u_dll_6161;  }
	if(_mfcm90_dll_6161   >_mfcm90_dll  ) {_mfcm90_dll   = _mfcm90_dll_6161;  }
	if(_mfcm90u_dll_6161  >_mfcm90u_dll ) {_mfcm90u_dll  = _mfcm90u_dll_6161; }
	if(_mfc90chs_dll_6161 >_mfc90chs_dll) {_mfc90chs_dll = _mfc90chs_dll_6161;}
	if(_mfc90cht_dll_6161 >_mfc90cht_dll) {_mfc90cht_dll = _mfc90cht_dll_6161;}
	if(_mfc90deu_dll_6161 >_mfc90deu_dll) {_mfc90deu_dll = _mfc90deu_dll_6161;}
	if(_mfc90enu_dll_6161 >_mfc90enu_dll) {_mfc90enu_dll = _mfc90enu_dll_6161;}
	if(_mfc90esn_dll_6161 >_mfc90esn_dll) {_mfc90esn_dll = _mfc90esn_dll_6161;}
	if(_mfc90esp_dll_6161 >_mfc90esp_dll) {_mfc90esp_dll = _mfc90esp_dll_6161;}
	if(_mfc90fra_dll_6161 >_mfc90fra_dll) {_mfc90fra_dll = _mfc90fra_dll_6161;}
	if(_mfc90ita_dll_6161 >_mfc90ita_dll) {_mfc90ita_dll = _mfc90ita_dll_6161;}
	if(_mfc90jpn_dll_6161 >_mfc90jpn_dll) {_mfc90jpn_dll = _mfc90jpn_dll_6161;}
	if(_mfc90kor_dll_6161 >_mfc90kor_dll) {_mfc90kor_dll = _mfc90kor_dll_6161;}
	if(_vcomp90_dll_6161  >_vcomp90_dll ) {_vcomp90_dll  = _vcomp90_dll_6161; }
	
	fver t = fver(9,0,21022,8);
		if( _atl90_dll    >= t
		 && _msvcm90_dll  >= t
		 && _msvcp90_dll  >= t
		 && _msvcr90_dll  >= t
		 && _mfc90_dll    >= t
		 && _mfc90u_dll   >= t
		 && _mfcm90_dll   >= t
		 && _mfcm90u_dll  >= t
		 && _mfc90chs_dll >= t
		 && _mfc90cht_dll >= t
		 && _mfc90deu_dll >= t
		 && _mfc90enu_dll >= t
		 && _mfc90esn_dll >= t
		 && _mfc90esp_dll >= t
		 && _mfc90fra_dll >= t
		 && _mfc90ita_dll >= t
		 && _mfc90jpn_dll >= t
		 && _mfc90kor_dll >= t
		 && _vcomp90_dll  >= t ) { f = t; }
	t = fver(9,0,21022,218);
		if( _atl90_dll    >= t
		 && _msvcm90_dll  >= t
		 && _msvcp90_dll  >= t
		 && _msvcr90_dll  >= t
		 && _mfc90_dll    >= t
		 && _mfc90u_dll   >= t
		 && _mfcm90_dll   >= t
		 && _mfcm90u_dll  >= t
		 && _mfc90chs_dll >= t
		 && _mfc90cht_dll >= t
		 && _mfc90deu_dll >= t
		 && _mfc90enu_dll >= t
		 && _mfc90esn_dll >= t
		 && _mfc90esp_dll >= t
		 && _mfc90fra_dll >= t
		 && _mfc90ita_dll >= t
		 && _mfc90jpn_dll >= t
		 && _mfc90kor_dll >= t
		 && _vcomp90_dll  >= t ) { f = t; }
	t = fver(9,0,30729,1);
		if( _atl90_dll    >= t
		 && _msvcm90_dll  >= t
		 && _msvcp90_dll  >= t
		 && _msvcr90_dll  >= t
		 && _mfc90_dll    >= t
		 && _mfc90u_dll   >= t
		 && _mfcm90_dll   >= t
		 && _mfcm90u_dll  >= t
		 && _mfc90chs_dll >= t
		 && _mfc90cht_dll >= t
		 && _mfc90deu_dll >= t
		 && _mfc90enu_dll >= t
		 && _mfc90esn_dll >= t
		 && _mfc90esp_dll >= t
		 && _mfc90fra_dll >= t
		 && _mfc90ita_dll >= t
		 && _mfc90jpn_dll >= t
		 && _mfc90kor_dll >= t
		 && _vcomp90_dll  >= fver() ) { f = t; }
	t = fver(9,0,30729,4148);
		if( _atl90_dll    >= t
		 && _msvcm90_dll  >= t
		 && _msvcp90_dll  >= t
		 && _msvcr90_dll  >= t
		 && _mfc90_dll    >= t
		 && _mfc90u_dll   >= t
		 && _mfcm90_dll   >= t
		 && _mfcm90u_dll  >= t
		 && _mfc90chs_dll >= t
		 && _mfc90cht_dll >= t
		 && _mfc90deu_dll >= t
		 && _mfc90enu_dll >= t
		 && _mfc90esn_dll >= t
		 && _mfc90esp_dll >= t
		 && _mfc90fra_dll >= t
		 && _mfc90ita_dll >= t
		 && _mfc90jpn_dll >= t
		 && _mfc90kor_dll >= t
		 && _vcomp90_dll  >= t ) { f = t; }
	t = fver(9,0,30729,5570);
		if( _atl90_dll    >= t
		 && _msvcm90_dll  >= t
		 && _msvcp90_dll  >= t
		 && _msvcr90_dll  >= t
		 && _mfc90_dll    >= t
		 && _mfc90u_dll   >= t
		 && _mfcm90_dll   >= t
		 && _mfcm90u_dll  >= t
		 && _mfc90chs_dll >= t
		 && _mfc90cht_dll >= t
		 && _mfc90deu_dll >= t
		 && _mfc90enu_dll >= t
		 && _mfc90esn_dll >= t
		 && _mfc90esp_dll >= t
		 && _mfc90fra_dll >= t
		 && _mfc90ita_dll >= t
		 && _mfc90jpn_dll >= t
		 && _mfc90kor_dll >= t
		 && _vcomp90_dll  >= t ) { f = t; }
	t = fver(9,0,30729,6161);
		if( _atl90_dll    >= t
		 && _msvcm90_dll  >= t
		 && _msvcp90_dll  >= t
		 && _msvcr90_dll  >= t
		 && _mfc90_dll    >= t
		 && _mfc90u_dll   >= t
		 && _mfcm90_dll   >= t
		 && _mfcm90u_dll  >= t
		 && _mfc90chs_dll >= t
		 && _mfc90cht_dll >= t
		 && _mfc90deu_dll >= t
		 && _mfc90enu_dll >= t
		 && _mfc90esn_dll >= t
		 && _mfc90esp_dll >= t
		 && _mfc90fra_dll >= t
		 && _mfc90ita_dll >= t
		 && _mfc90jpn_dll >= t
		 && _mfc90kor_dll >= t
		 && _vcomp90_dll  >= t ) { f = t; }

		if(f>=fver(9,0,30729,0)) { vcsp=1; }

	if(debug) {
		printf("Microsoft Visual C++ 2008 DEBUG:\n");
		printf("%s\t%s\n","atl90.dll",_atl90_dll.format().c_str());
		printf("%s\t%s\n","msvcm90.dll",_msvcm90_dll.format().c_str());
		printf("%s\t%s\n","msvcp90.dll",_msvcp90_dll.format().c_str());
		printf("%s\t%s\n","msvcr90.dll",_msvcr90_dll.format().c_str());
		printf("%s\t%s\n","mfc90.dll",_mfc90_dll.format().c_str());
		printf("%s\t%s\n","mfc90u.dll",_mfc90u_dll.format().c_str());
		printf("%s\t%s\n","mfcm90.dll",_mfcm90_dll.format().c_str());
		printf("%s\t%s\n","mfcm90u.dll",_mfcm90u_dll.format().c_str());
		printf("%s\t%s\n","mfc90chs.dll",_mfc90chs_dll.format().c_str());
		printf("%s\t%s\n","mfc90cht.dll",_mfc90cht_dll.format().c_str());
		printf("%s\t%s\n","mfc90deu.dll",_mfc90deu_dll.format().c_str());
		printf("%s\t%s\n","mfc90enu.dll",_mfc90enu_dll.format().c_str());
		printf("%s\t%s\n","mfc90esn.dll",_mfc90esn_dll.format().c_str());
		printf("%s\t%s\n","mfc90esp.dll",_mfc90esp_dll.format().c_str());
		printf("%s\t%s\n","mfc90fra.dll",_mfc90fra_dll.format().c_str());
		printf("%s\t%s\n","mfc90ita.dll",_mfc90ita_dll.format().c_str());
		printf("%s\t%s\n","mfc90jpn.dll",_mfc90jpn_dll.format().c_str());
		printf("%s\t%s\n","mfc90kor.dll",_mfc90kor_dll.format().c_str());
		printf("%s\t%s\n","vcomp90.dll",_vcomp90_dll.format().c_str());
		printf("\n");
		printf("VCREDIST_2008_VERSION = %s\n",f.format().c_str());
		printf("VCREDIST_2008_SPLEVEL = %d\n\n",vcsp);
	}

	return f;
}

fver vcr2010(bool debug) {

	int status=0;
	fver f; int vcsp=0;

	// Identify system paths
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&status);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";
	std::wstring SharedVC = ProgramFiles+L"\\Common Files\\Microsoft Shared\\VC";
	
	// There is no such thing as msvcm100.dll or mfc100esp.dll
	fver _atl100_dll    = getFileVer(System32+L"\\atl100.dll",&status);
	fver _msdia100_dll  = getFileVer(SharedVC+L"\\msdia100.dll",&status);
	fver _msvcp100_dll  = getFileVer(System32+L"\\msvcp100.dll",&status);
	fver _msvcr100_dll  = getFileVer(System32+L"\\msvcr100.dll",&status);
	fver _mfc100_dll    = getFileVer(System32+L"\\mfc100.dll",&status);
	fver _mfc100u_dll   = getFileVer(System32+L"\\mfc100u.dll",&status);
	fver _mfcm100_dll   = getFileVer(System32+L"\\mfcm100.dll",&status);
	fver _mfcm100u_dll  = getFileVer(System32+L"\\mfcm100u.dll",&status);
	fver _mfc100chs_dll = getFileVer(System32+L"\\mfc100chs.dll",&status);
	fver _mfc100cht_dll = getFileVer(System32+L"\\mfc100cht.dll",&status);
	fver _mfc100deu_dll = getFileVer(System32+L"\\mfc100deu.dll",&status);
	fver _mfc100enu_dll = getFileVer(System32+L"\\mfc100enu.dll",&status);
	fver _mfc100esn_dll = getFileVer(System32+L"\\mfc100esn.dll",&status);
	fver _mfc100fra_dll = getFileVer(System32+L"\\mfc100fra.dll",&status);
	fver _mfc100ita_dll = getFileVer(System32+L"\\mfc100ita.dll",&status);
	fver _mfc100jpn_dll = getFileVer(System32+L"\\mfc100jpn.dll",&status);
	fver _mfc100kor_dll = getFileVer(System32+L"\\mfc100kor.dll",&status);
	fver _mfc100rus_dll = getFileVer(System32+L"\\mfc100rus.dll",&status);
	fver _vcomp100_dll  = getFileVer(System32+L"\\vcomp100.dll",&status);
	
	fver t = fver(10,0,30319,1);
		if( _atl100_dll    >= t
		 && _msdia100_dll  >= t
		 && _msvcp100_dll  >= t
		 && _msvcr100_dll  >= t
		 && _mfc100_dll    >= t
		 && _mfc100u_dll   >= t
		 && _mfcm100_dll   >= t
		 && _mfcm100u_dll  >= t
		 && _mfc100chs_dll >= t
		 && _mfc100cht_dll >= t
		 && _mfc100deu_dll >= t
		 && _mfc100enu_dll >= t
		 && _mfc100esn_dll >= t
		 && _mfc100fra_dll >= t
		 && _mfc100ita_dll >= t
		 && _mfc100jpn_dll >= t
		 && _mfc100kor_dll >= t
		 && _mfc100rus_dll >= t
		 && _vcomp100_dll  >= t ) { f = t; }
	t = fver(10,0,30319,415);
		if( _atl100_dll    >= t
		 && _msdia100_dll  >= fver(10,0,30319,1)
		 && _msvcp100_dll  >= t
		 && _msvcr100_dll  >= t
		 && _mfc100_dll    >= t
		 && _mfc100u_dll   >= t
		 && _mfcm100_dll   >= t
		 && _mfcm100u_dll  >= t
		 && _mfc100chs_dll >= t
		 && _mfc100cht_dll >= t
		 && _mfc100deu_dll >= t
		 && _mfc100enu_dll >= t
		 && _mfc100esn_dll >= t
		 && _mfc100fra_dll >= t
		 && _mfc100ita_dll >= t
		 && _mfc100jpn_dll >= t
		 && _mfc100kor_dll >= t
		 && _mfc100rus_dll >= t
		 && _vcomp100_dll  >= t ) { f = t; }
	t = fver(10,0,40219,1);
		if( _atl100_dll    >= t
		 && _msdia100_dll  >= t
		 && _msvcp100_dll  >= t
		 && _msvcr100_dll  >= t
		 && _mfc100_dll    >= t
		 && _mfc100u_dll   >= t
		 && _mfcm100_dll   >= t
		 && _mfcm100u_dll  >= t
		 && _mfc100chs_dll >= t
		 && _mfc100cht_dll >= t
		 && _mfc100deu_dll >= t
		 && _mfc100enu_dll >= t
		 && _mfc100esn_dll >= t
		 && _mfc100fra_dll >= t
		 && _mfc100ita_dll >= t
		 && _mfc100jpn_dll >= t
		 && _mfc100kor_dll >= t
		 && _mfc100rus_dll >= t
		 && _vcomp100_dll  >= t ) { f = t; }
	t = fver(10,0,40219,325);
		if( _atl100_dll    >= t
		 && _msdia100_dll  >= fver(10,0,40219,1)
		 && _msvcp100_dll  >= t
		 && _msvcr100_dll  >= t
		 && _mfc100_dll    >= t
		 && _mfc100u_dll   >= t
		 && _mfcm100_dll   >= t
		 && _mfcm100u_dll  >= t
		 && _mfc100chs_dll >= t
		 && _mfc100cht_dll >= t
		 && _mfc100deu_dll >= t
		 && _mfc100enu_dll >= t
		 && _mfc100esn_dll >= t
		 && _mfc100fra_dll >= t
		 && _mfc100ita_dll >= t
		 && _mfc100jpn_dll >= t
		 && _mfc100kor_dll >= t
		 && _mfc100rus_dll >= t
		 && _vcomp100_dll  >= t ) { f = t; }

	if(f>=fver(10,0,40219,0)) { vcsp=1; }
	
	if(debug) {
		printf("Microsoft Visual C++ 2010 DEBUG:\n");
		printf("%s\t%s\n","atl100.dll",_atl100_dll.format().c_str());
		printf("%s\t%s\n","msdia100.dll",_msdia100_dll.format().c_str());
		printf("%s\t%s\n","msvcp100.dll",_msvcp100_dll.format().c_str());
		printf("%s\t%s\n","msvcr100.dll",_msvcr100_dll.format().c_str());
		printf("%s\t%s\n","mfc100.dll",_mfc100_dll.format().c_str());
		printf("%s\t%s\n","mfc100u.dll",_mfc100u_dll.format().c_str());
		printf("%s\t%s\n","mfcm100.dll",_mfcm100_dll.format().c_str());
		printf("%s\t%s\n","mfcm100u.dll",_mfcm100u_dll.format().c_str());
		printf("%s\t%s\n","mfc100chs.dll",_mfc100chs_dll.format().c_str());
		printf("%s\t%s\n","mfc100cht.dll",_mfc100cht_dll.format().c_str());
		printf("%s\t%s\n","mfc100deu.dll",_mfc100deu_dll.format().c_str());
		printf("%s\t%s\n","mfc100enu.dll",_mfc100enu_dll.format().c_str());
		printf("%s\t%s\n","mfc100esn.dll",_mfc100esn_dll.format().c_str());
		printf("%s\t%s\n","mfc100fra.dll",_mfc100fra_dll.format().c_str());
		printf("%s\t%s\n","mfc100ita.dll",_mfc100ita_dll.format().c_str());
		printf("%s\t%s\n","mfc100jpn.dll",_mfc100jpn_dll.format().c_str());
		printf("%s\t%s\n","mfc100kor.dll",_mfc100kor_dll.format().c_str());
		printf("%s\t%s\n","mfc100rus.dll",_mfc100rus_dll.format().c_str());
		printf("%s\t%s\n","vcomp100.dll",_vcomp100_dll.format().c_str());
		printf("\n");
		printf("VCREDIST_2010_VERSION = %s\n",f.format().c_str());
		printf("VCREDIST_2010_SPLEVEL = %d\n\n",vcsp);
	}

	return f;
}

fver vcr2012(bool debug) {

	int status=0;
	fver f;

	// Identify system paths
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&status);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";

	std::wstring vs2012reg = regQueryValue(L"SOFTWARE\\Classes\\Installer\\Dependencies\\Microsoft.VS.VC_RuntimeMinimumVSU_x86,v11",L"Version",&status);
	fver _vs2012reg     = fver(vs2012reg.c_str());
	fver _atl110_dll    = getFileVer(System32+L"\\atl110.dll",&status);
	fver _msvcp110_dll  = getFileVer(System32+L"\\msvcp110.dll",&status);
	fver _msvcr110_dll  = getFileVer(System32+L"\\msvcr110.dll",&status);
	fver _mfc110_dll    = getFileVer(System32+L"\\mfc110.dll",&status);
	fver _mfc110u_dll   = getFileVer(System32+L"\\mfc110u.dll",&status);
	fver _mfcm110_dll   = getFileVer(System32+L"\\mfcm110.dll",&status);
	fver _mfcm110u_dll  = getFileVer(System32+L"\\mfcm110u.dll",&status);
	fver _mfc110chs_dll = getFileVer(System32+L"\\mfc110chs.dll",&status);
	fver _mfc110cht_dll = getFileVer(System32+L"\\mfc110cht.dll",&status);
	fver _mfc110deu_dll = getFileVer(System32+L"\\mfc110deu.dll",&status);
	fver _mfc110enu_dll = getFileVer(System32+L"\\mfc110enu.dll",&status);
	fver _mfc110esn_dll = getFileVer(System32+L"\\mfc110esn.dll",&status);
	fver _mfc110fra_dll = getFileVer(System32+L"\\mfc110fra.dll",&status);
	fver _mfc110ita_dll = getFileVer(System32+L"\\mfc110ita.dll",&status);
	fver _mfc110jpn_dll = getFileVer(System32+L"\\mfc110jpn.dll",&status);
	fver _mfc110kor_dll = getFileVer(System32+L"\\mfc110kor.dll",&status);
	fver _mfc110rus_dll = getFileVer(System32+L"\\mfc110rus.dll",&status);
	fver _vcomp110_dll  = getFileVer(System32+L"\\vcomp110.dll",&status);
	
	fver t = fver(11,0,50727,1);
		if( _vs2012reg     >= t
		 && _atl110_dll    >= t
		 && _msvcp110_dll  >= t
		 && _msvcr110_dll  >= t
		 && _mfc110_dll    >= t
		 && _mfc110u_dll   >= t
		 && _mfcm110_dll   >= t
		 && _mfcm110u_dll  >= t
		 && _mfc110chs_dll >= t
		 && _mfc110cht_dll >= t
		 && _mfc110deu_dll >= t
		 && _mfc110enu_dll >= t
		 && _mfc110esn_dll >= t
		 && _mfc110fra_dll >= t
		 && _mfc110ita_dll >= t
		 && _mfc110jpn_dll >= t
		 && _mfc110kor_dll >= t
		 && _mfc110rus_dll >= t
		 && _vcomp110_dll  >= t ) { f = t; }
	t = fver(11,0,51106,1);
		if( _vs2012reg     >= fver(11,0,51106,0)
		 && _atl110_dll    >= t
		 && _msvcp110_dll  >= t
		 && _msvcr110_dll  >= t
		 && _mfc110_dll    >= t
		 && _mfc110u_dll   >= t
		 && _mfcm110_dll   >= t
		 && _mfcm110u_dll  >= t
		 && _mfc110chs_dll >= t
		 && _mfc110cht_dll >= t
		 && _mfc110deu_dll >= t
		 && _mfc110enu_dll >= t
		 && _mfc110esn_dll >= t
		 && _mfc110fra_dll >= t
		 && _mfc110ita_dll >= t
		 && _mfc110jpn_dll >= t
		 && _mfc110kor_dll >= t
		 && _mfc110rus_dll >= t
		 && _vcomp110_dll  >= t ) { f = t; }
	t = fver(11,0,60610,1);
		if( _vs2012reg     >= fver(11,0,60610,0)
		 && _atl110_dll    >= t
		 && _msvcp110_dll  >= fver(11,0,51106,1)
		 && _msvcr110_dll  >= fver(11,0,51106,1)
		 && _mfc110_dll    >= t
		 && _mfc110u_dll   >= t
		 && _mfcm110_dll   >= t
		 && _mfcm110u_dll  >= t
		 && _mfc110chs_dll >= t
		 && _mfc110cht_dll >= t
		 && _mfc110deu_dll >= t
		 && _mfc110enu_dll >= t
		 && _mfc110esn_dll >= t
		 && _mfc110fra_dll >= t
		 && _mfc110ita_dll >= t
		 && _mfc110jpn_dll >= t
		 && _mfc110kor_dll >= t
		 && _mfc110rus_dll >= t
		 && _vcomp110_dll  >= fver(11,0,51106,1) ) { f = t; }
	t = fver(11,0,61030,0);
		if( _vs2012reg     >= t
		 && _atl110_dll    >= fver(11,0,60610,1)
		 && _msvcp110_dll  >= fver(11,0,51106,1)
		 && _msvcr110_dll  >= fver(11,0,51106,1)
		 && _mfc110_dll    >= fver(11,0,60610,1)
		 && _mfc110u_dll   >= fver(11,0,60610,1)
		 && _mfcm110_dll   >= fver(11,0,60610,1)
		 && _mfcm110u_dll  >= fver(11,0,60610,1)
		 && _mfc110chs_dll >= fver(11,0,60610,1)
		 && _mfc110cht_dll >= fver(11,0,60610,1)
		 && _mfc110deu_dll >= fver(11,0,60610,1)
		 && _mfc110enu_dll >= fver(11,0,60610,1)
		 && _mfc110esn_dll >= fver(11,0,60610,1)
		 && _mfc110fra_dll >= fver(11,0,60610,1)
		 && _mfc110ita_dll >= fver(11,0,60610,1)
		 && _mfc110jpn_dll >= fver(11,0,60610,1)
		 && _mfc110kor_dll >= fver(11,0,60610,1)
		 && _mfc110rus_dll >= fver(11,0,60610,1)
		 && _vcomp110_dll  >= fver(11,0,51106,1) ) { f = t; }

	if(debug) {
		printf("Microsoft Visual C++ 2012 DEBUG:\n");
		printf("Registry\t%s\n",_vs2012reg.format().c_str());
		printf("%s\t%s\n","atl110.dll",_atl110_dll.format().c_str());
		printf("%s\t%s\n","msvcp110.dll",_msvcp110_dll.format().c_str());
		printf("%s\t%s\n","msvcr110.dll",_msvcr110_dll.format().c_str());
		printf("%s\t%s\n","mfc110.dll",_mfc110_dll.format().c_str());
		printf("%s\t%s\n","mfc110u.dll",_mfc110u_dll.format().c_str());
		printf("%s\t%s\n","mfcm110.dll",_mfcm110_dll.format().c_str());
		printf("%s\t%s\n","mfcm110u.dll",_mfcm110u_dll.format().c_str());
		printf("%s\t%s\n","mfc110chs.dll",_mfc110chs_dll.format().c_str());
		printf("%s\t%s\n","mfc110cht.dll",_mfc110cht_dll.format().c_str());
		printf("%s\t%s\n","mfc110deu.dll",_mfc110deu_dll.format().c_str());
		printf("%s\t%s\n","mfc110enu.dll",_mfc110enu_dll.format().c_str());
		printf("%s\t%s\n","mfc110esn.dll",_mfc110esn_dll.format().c_str());
		printf("%s\t%s\n","mfc110fra.dll",_mfc110fra_dll.format().c_str());
		printf("%s\t%s\n","mfc110ita.dll",_mfc110ita_dll.format().c_str());
		printf("%s\t%s\n","mfc110jpn.dll",_mfc110jpn_dll.format().c_str());
		printf("%s\t%s\n","mfc110kor.dll",_mfc110kor_dll.format().c_str());
		printf("%s\t%s\n","mfc110rus.dll",_mfc110rus_dll.format().c_str());
		printf("%s\t%s\n","vcomp110.dll",_vcomp110_dll.format().c_str());
		printf("\n");
		printf("VCREDIST_2012_VERSION = %s\n\n",f.format().c_str());
	}

	return f;
}

fver vcr2013(bool debug) {

	int status=0;
	fver f;

	// Identify system paths
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&status);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";

	std::wstring vs2013reg1 = regQueryValue(L"SOFTWARE\\Classes\\Installer\\Dependencies\\Microsoft.VS.VC_RuntimeMinimumVSU_x86,v12",L"Version",&status);
	std::wstring vs2013reg2 = regQueryValue(L"SOFTWARE\\Classes\\Installer\\Dependencies\\{f65db027-aff3-4070-886a-0d87064aabb1}",L"Version",&status);
	fver _vs2013reg1 = fver(vs2013reg1.c_str());
	fver _vs2013reg2 = fver(vs2013reg2.c_str());
	fver _vs2013reg  = _vs2013reg1;
	if(_vs2013reg2 > _vs2013reg) { _vs2013reg = _vs2013reg2; }
	fver _mfc120_dll    = getFileVer(System32+L"\\mfc120.dll",&status);
	fver _mfc120chs_dll = getFileVer(System32+L"\\mfc120chs.dll",&status);
	fver _mfc120cht_dll = getFileVer(System32+L"\\mfc120cht.dll",&status);
	fver _mfc120deu_dll = getFileVer(System32+L"\\mfc120deu.dll",&status);
	fver _mfc120enu_dll = getFileVer(System32+L"\\mfc120enu.dll",&status);
	fver _mfc120esn_dll = getFileVer(System32+L"\\mfc120esn.dll",&status);
	fver _mfc120fra_dll = getFileVer(System32+L"\\mfc120fra.dll",&status);
	fver _mfc120ita_dll = getFileVer(System32+L"\\mfc120ita.dll",&status);
	fver _mfc120jpn_dll = getFileVer(System32+L"\\mfc120jpn.dll",&status);
	fver _mfc120kor_dll = getFileVer(System32+L"\\mfc120kor.dll",&status);
	fver _mfc120rus_dll = getFileVer(System32+L"\\mfc120rus.dll",&status);
	fver _mfc120u_dll   = getFileVer(System32+L"\\mfc120u.dll",&status);
	fver _mfcm120_dll   = getFileVer(System32+L"\\mfcm120.dll",&status);
	fver _mfcm120u_dll  = getFileVer(System32+L"\\mfcm120u.dll",&status);
	fver _msvcp120_dll  = getFileVer(System32+L"\\msvcp120.dll",&status);
	fver _msvcr120_dll  = getFileVer(System32+L"\\msvcr120.dll",&status);
	fver _vcomp120_dll  = getFileVer(System32+L"\\vcomp120.dll",&status);
	
	fver t = fver(12,0,21005,1);
		if( _vs2013reg     >= fver(12,0,21005,0)
		 && _mfc120_dll    >= t
		 && _mfc120chs_dll >= t
		 && _mfc120cht_dll >= t
		 && _mfc120deu_dll >= t
		 && _mfc120enu_dll >= t
		 && _mfc120esn_dll >= t
		 && _mfc120fra_dll >= t
		 && _mfc120ita_dll >= t
		 && _mfc120jpn_dll >= t
		 && _mfc120kor_dll >= t
		 && _mfc120rus_dll >= t
		 && _mfc120u_dll   >= t
		 && _mfcm120_dll   >= t
		 && _mfcm120u_dll  >= t
		 && _msvcp120_dll  >= t
		 && _msvcr120_dll  >= t
		 && _vcomp120_dll  >= t ) { f = t; }
	t = fver(12,0,30501,0);
		if( _vs2013reg     >= t
		 && _mfc120_dll    >= fver(12,0,21005,1)
		 && _mfc120chs_dll >= fver(12,0,21005,1)
		 && _mfc120cht_dll >= fver(12,0,21005,1)
		 && _mfc120deu_dll >= fver(12,0,21005,1)
		 && _mfc120enu_dll >= fver(12,0,21005,1)
		 && _mfc120esn_dll >= fver(12,0,21005,1)
		 && _mfc120fra_dll >= fver(12,0,21005,1)
		 && _mfc120ita_dll >= fver(12,0,21005,1)
		 && _mfc120jpn_dll >= fver(12,0,21005,1)
		 && _mfc120kor_dll >= fver(12,0,21005,1)
		 && _mfc120rus_dll >= fver(12,0,21005,1)
		 && _mfc120u_dll   >= fver(12,0,21005,1)
		 && _mfcm120_dll   >= fver(12,0,21005,1)
		 && _mfcm120u_dll  >= fver(12,0,21005,1)
		 && _msvcp120_dll  >= fver(12,0,21005,1)
		 && _msvcr120_dll  >= fver(12,0,21005,1)
		 && _vcomp120_dll  >= fver(12,0,21005,1) ) { f = t; }
	t = fver(12,0,40649,5);
		if( _vs2013reg     >= fver(12,0,40649,0)
		 && _mfc120_dll    >= t
		 && _mfc120chs_dll >= t
		 && _mfc120cht_dll >= t
		 && _mfc120deu_dll >= t
		 && _mfc120enu_dll >= t
		 && _mfc120esn_dll >= t
		 && _mfc120fra_dll >= t
		 && _mfc120ita_dll >= t
		 && _mfc120jpn_dll >= t
		 && _mfc120kor_dll >= t
		 && _mfc120rus_dll >= t
		 && _mfc120u_dll   >= t
		 && _mfcm120_dll   >= t
		 && _mfcm120u_dll  >= t
		 && _msvcp120_dll  >= t
		 && _msvcr120_dll  >= t
		 && _vcomp120_dll  >= t ) { f = t; }
	t = fver(12,0,40660,0);
		if( _vs2013reg     >= t
		 && _mfc120_dll    >= t
		 && _mfc120chs_dll >= t
		 && _mfc120cht_dll >= t
		 && _mfc120deu_dll >= t
		 && _mfc120enu_dll >= t
		 && _mfc120esn_dll >= t
		 && _mfc120fra_dll >= t
		 && _mfc120ita_dll >= t
		 && _mfc120jpn_dll >= t
		 && _mfc120kor_dll >= t
		 && _mfc120rus_dll >= t
		 && _mfc120u_dll   >= t
		 && _mfcm120_dll   >= t
		 && _mfcm120u_dll  >= t
		 && _msvcp120_dll  >= t
		 && _msvcr120_dll  >= t
		 && _vcomp120_dll  >= t ) { f = t; }
	t = fver(12,0,40664,0);
		if( _vs2013reg     >= t
		 && _mfc120_dll    >= t
		 && _mfc120chs_dll >= t
		 && _mfc120cht_dll >= t
		 && _mfc120deu_dll >= t
		 && _mfc120enu_dll >= t
		 && _mfc120esn_dll >= t
		 && _mfc120fra_dll >= t
		 && _mfc120ita_dll >= t
		 && _mfc120jpn_dll >= t
		 && _mfc120kor_dll >= t
		 && _mfc120rus_dll >= t
		 && _mfc120u_dll   >= t
		 && _mfcm120_dll   >= t
		 && _mfcm120u_dll  >= t
		 && _msvcp120_dll  >= t
		 && _msvcr120_dll  >= t
		 && _vcomp120_dll  >= t ) { f = t; }

	if(debug) {
		printf("Microsoft Visual C++ 2013 DEBUG:\n");
		printf("Registry\t%s\n",_vs2013reg.format().c_str());
		printf("%s\t%s\n","mfc120.dll",_mfc120_dll.format().c_str());
		printf("%s\t%s\n","mfc120chs.dll",_mfc120chs_dll.format().c_str());
		printf("%s\t%s\n","mfc120cht.dll",_mfc120cht_dll.format().c_str());
		printf("%s\t%s\n","mfc120deu.dll",_mfc120deu_dll.format().c_str());
		printf("%s\t%s\n","mfc120enu.dll",_mfc120enu_dll.format().c_str());
		printf("%s\t%s\n","mfc120esn.dll",_mfc120esn_dll.format().c_str());
		printf("%s\t%s\n","mfc120fra.dll",_mfc120fra_dll.format().c_str());
		printf("%s\t%s\n","mfc120ita.dll",_mfc120ita_dll.format().c_str());
		printf("%s\t%s\n","mfc120jpn.dll",_mfc120jpn_dll.format().c_str());
		printf("%s\t%s\n","mfc120kor.dll",_mfc120kor_dll.format().c_str());
		printf("%s\t%s\n","mfc120rus.dll",_mfc120rus_dll.format().c_str());
		printf("%s\t%s\n","mfc120u.dll",_mfc120u_dll.format().c_str());
		printf("%s\t%s\n","mfcm120.dll",_mfcm120_dll.format().c_str());
		printf("%s\t%s\n","mfcm120u.dll",_mfcm120u_dll.format().c_str());
		printf("%s\t%s\n","msvcp120.dll",_msvcp120_dll.format().c_str());
		printf("%s\t%s\n","msvcr120.dll",_msvcr120_dll.format().c_str());
		printf("%s\t%s\n","vcomp120.dll",_vcomp120_dll.format().c_str());
		printf("\n");
		printf("VCREDIST_2013_VERSION = %s\n\n",f.format().c_str());
	}

	return f;
}

fver vcr2015(bool debug) {

	int status=0;
	fver f;

	// Identify system paths
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&status);
	std::wstring ProgramFiles = regQueryValue(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion",L"ProgramFilesDir",&status);
	std::wstring WinSxS = SystemRoot + L"\\WinSxS";
	std::wstring System32 = SystemRoot + L"\\system32";

	fver _concrt140_dll    = getFileVer(System32+L"\\concrt140.dll",&status);
	fver _mfc140_dll = getFileVer(System32+L"\\mfc140.dll",&status);
	fver _mfc140chs_dll = getFileVer(System32+L"\\mfc140chs.dll",&status);
	fver _mfc140cht_dll = getFileVer(System32+L"\\mfc140cht.dll",&status);
	fver _mfc140deu_dll = getFileVer(System32+L"\\mfc140deu.dll",&status);
	fver _mfc140enu_dll = getFileVer(System32+L"\\mfc140enu.dll",&status);
	fver _mfc140esn_dll = getFileVer(System32+L"\\mfc140esn.dll",&status);
	fver _mfc140fra_dll = getFileVer(System32+L"\\mfc140fra.dll",&status);
	fver _mfc140ita_dll = getFileVer(System32+L"\\mfc140ita.dll",&status);
	fver _mfc140jpn_dll = getFileVer(System32+L"\\mfc140jpn.dll",&status);
	fver _mfc140kor_dll = getFileVer(System32+L"\\mfc140kor.dll",&status);
	fver _mfc140rus_dll = getFileVer(System32+L"\\mfc140rus.dll",&status);
	fver _mfc140u_dll = getFileVer(System32+L"\\mfc140u.dll",&status);
	fver _mfcm140_dll = getFileVer(System32+L"\\mfcm140.dll",&status);
	fver _mfcm140u_dll = getFileVer(System32+L"\\mfcm140u.dll",&status);
	fver _msvcp140_dll = getFileVer(System32+L"\\msvcp140.dll",&status);
	fver _ucrtbase_dll = getFileVer(System32+L"\\ucrtbase.dll",&status);
	fver _vcamp140_dll = getFileVer(System32+L"\\vcamp140.dll",&status);
	fver _vccorlib140_dll = getFileVer(System32+L"\\vccorlib140.dll",&status);
	fver _vcomp140_dll = getFileVer(System32+L"\\vcomp140.dll",&status);
	fver _vcruntime140_dll = getFileVer(System32+L"\\vcruntime140.dll",&status);
	
	fver t = fver(14,0,23026,0);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10137,0)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }
	t = fver(14,0,23506,0);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10563,0)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }
	t = fver(14,0,23918,0);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10586,15)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }
	t = fver(14,0,24123,0);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10586,15)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }
	t = fver(14,0,24210,0);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10586,15)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }
	t = fver(14,0,24212,0);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10586,15)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }
	t = fver(14,0,24215,1);
		if( _concrt140_dll    >= t
		 && _mfc140_dll       >= t
		 && _mfc140chs_dll    >= t
		 && _mfc140cht_dll    >= t
		 && _mfc140deu_dll    >= t
		 && _mfc140enu_dll    >= t
		 && _mfc140esn_dll    >= t
		 && _mfc140fra_dll    >= t
		 && _mfc140ita_dll    >= t
		 && _mfc140jpn_dll    >= t
		 && _mfc140kor_dll    >= t
		 && _mfc140rus_dll    >= t
		 && _mfc140u_dll      >= t
		 && _mfcm140_dll      >= t
		 && _mfcm140u_dll     >= t
		 && _msvcp140_dll     >= t
		 && _ucrtbase_dll     >= fver(10,0,10586,15)
		 && _vcamp140_dll     >= t
		 && _vccorlib140_dll  >= t
		 && _vcomp140_dll     >= t
		 && _vcruntime140_dll >= t ) { f = t; }

	if(debug) {
		printf("Microsoft Visual C++ 2015 DEBUG:\n");
		printf("%s\t\t%s\n","concrt140.dll",_concrt140_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140.dll",_mfc140_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140chs.dll",_mfc140chs_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140cht.dll",_mfc140cht_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140deu.dll",_mfc140deu_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140enu.dll",_mfc140enu_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140esn.dll",_mfc140esn_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140fra.dll",_mfc140fra_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140ita.dll",_mfc140ita_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140jpn.dll",_mfc140jpn_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140kor.dll",_mfc140kor_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140rus.dll",_mfc140rus_dll.format().c_str());
		printf("%s\t\t%s\n","mfc140u.dll",_mfc140u_dll.format().c_str());
		printf("%s\t\t%s\n","mfcm140.dll",_mfcm140_dll.format().c_str());
		printf("%s\t\t%s\n","mfcm140u.dll",_mfcm140u_dll.format().c_str());
		printf("%s\t\t%s\n","msvcp140.dll",_msvcp140_dll.format().c_str());
		printf("%s\t\t%s\n","ucrtbase.dll",_ucrtbase_dll.format().c_str());
		printf("%s\t\t%s\n","vcamp140.dll",_vcamp140_dll.format().c_str());
		printf("%s\t\t%s\n","vccorlib140.dll",_vccorlib140_dll.format().c_str());
		printf("%s\t\t%s\n","vcomp140.dll",_vcomp140_dll.format().c_str());
		printf("%s\t%s\n","vcruntime140.dll",_vcruntime140_dll.format().c_str());
		printf("\n");
		printf("VCREDIST_2015_VERSION = %s\n\n",f.format().c_str());
	}

	return f;
}
