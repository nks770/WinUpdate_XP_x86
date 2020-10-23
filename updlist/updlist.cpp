// updlist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>

#include "banner.h"
#include "help.h"
#include "fver.h"
#include "registry.h"
#include "winsku.h"
#include "winupdate.h"
#include "cmpupdate.h"
#include "script.h"
#include "mbsa.h"
#include "options.h"
#include "nfxdetect.h"
#include "nfxupdate.h"
#include "certificates.h"
#include "notify.h"
#include "vcredist.h"

using namespace std;

#define PROGRAM_DATE "22-Oct-2020"

int _tmain(int argc, _TCHAR* argv[]) {

	int status=0;

	// Parse arguments
	int i=0;
	bool help=false;
	bool batch=false;
	bool mbsa=false; std::wstring mbsa_file;
	bool netfx_mode=false;
	bool install_netfx=false;
	bool install_mbsa=false;
	bool install_sp=false;
	bool qfe=false;
	bool install_vcredist=false;
	bool component_install=false;
	bool list_cert_root=false;
	bool list_cert_disallowed=false;
	bool allow_reboot=true;
	int sp_to_be_installed=0;

	bool vcdebug2005 = false;
	bool vcdebug2008 = false;
	bool vcdebug2010 = false;
	bool vcdebug2012 = false;
	bool vcdebug2013 = false;
	bool vcdebug2015 = false;
	bool vcdebug     = false;

	std::vector<std::string> notifications;

	for(i=0;i<argc;i++) {
		if(!wcscmp(argv[i],L"--help")) { help=true; }
		if(!wcscmp(argv[i],L"-help")) { help=true; }
		if(!wcscmp(argv[i],L"-h")) { help=true; }
		if(!wcscmp(argv[i],L"-H")) { help=true; }
		if(!wcscmp(argv[i],L"-?")) { help=true; }
		if(!wcscmp(argv[i],L"/?")) { help=true; }
		if(!wcscmp(argv[i],L"--help")) { help=true; }
		if(!wcscmp(argv[i],L"--batch")) { batch=true; }
		if(!wcscmp(argv[i],L"--netfx")) { netfx_mode=true; }
		if(!wcscmp(argv[i],L"--comp")) { component_install=true; }
		if(!wcscmp(argv[i],L"--install-netfx")) { install_netfx=true; }
		if(!wcscmp(argv[i],L"--install-mbsa")) { install_mbsa=true; }
		if(!wcscmp(argv[i],L"--install-sp")) { install_sp=true; }
		if(!wcscmp(argv[i],L"--enable-qfe")) { qfe=true; }
		if(!wcscmp(argv[i],L"--list-cert-root")) { list_cert_root=true; }
		if(!wcscmp(argv[i],L"--list-cert-disallowed")) { list_cert_disallowed=true; }
		if(!wcscmp(argv[i],L"--noreboot")) { allow_reboot=false; }
		if(!wcscmp(argv[i],L"--vcdebug2005")) { vcdebug2005=true; }
		if(!wcscmp(argv[i],L"--vcdebug2008")) { vcdebug2008=true; }
		if(!wcscmp(argv[i],L"--vcdebug2010")) { vcdebug2010=true; }
		if(!wcscmp(argv[i],L"--vcdebug2012")) { vcdebug2012=true; }
		if(!wcscmp(argv[i],L"--vcdebug2013")) { vcdebug2013=true; }
		if(!wcscmp(argv[i],L"--vcdebug2015")) { vcdebug2015=true; }
		if(!wcscmp(argv[i],L"--vcdebug")) { vcdebug=true; }
		if(!wcscmp(argv[i],L"--vcredist")) { install_vcredist=true; }
		if(!wcscmp(argv[i],L"--mbsa")) {
			mbsa=true;
			if(++i<argc) {
				mbsa_file=std::wstring(argv[i]);
			}
		}
	}

	if( help ) {
		show_help(PROGRAM_DATE);
		return 0;
	}
	if( list_cert_root ) {
		printf("Certificates:\n");
		listCertificates("Root");
		printf("End of certificates.\n");
		return 0;
	}
	else if( list_cert_disallowed ) {
		printf("Certificates:\n");
		listCertificates("Disallowed");
		printf("End of certificates.\n");
		return 0;
	}
	else if( vcdebug2005 ) { vcr2005(true); return 0; }
	else if( vcdebug2008 ) { vcr2008(true); return 0; }
	else if( vcdebug2010 ) { vcr2010(true); return 0; }
	else if( vcdebug2012 ) { vcr2012(true); return 0; }
	else if( vcdebug2013 ) { vcr2013(true); return 0; }
	else if( vcdebug2015 ) { vcr2015(true); return 0; }
	else if( vcdebug     ) { vcsummary(); return 0; }

	// Validate selected options
	if(batch && mbsa) {
		printf("ERROR: '--batch' and '--mbsa' are mutually exclusive.\n\n");
		return 1;
	}

	// Get System Metrics
	winsku sku=getSKU();
	winsp splevel=getSP();
	int CannotFindSystemRoot=0;
	std::wstring SystemRoot = regQueryValue(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",L"SystemRoot",&CannotFindSystemRoot);
	std::string ProductName=getSKUName(sku)+" "+getSPName(splevel);
	if(!install_sp && !install_mbsa) {
		servicePackNotifications(sp_to_int(splevel),&notifications);
	}

	// Determine optional components
	bool compInstalled[COMPONENT_COUNT];
	bool compInstall[COMPONENT_COUNT];
	detectOptions(compInstalled);
	argumentOptions(argc,argv,compInstalled,compInstall,COMPONENT_COUNT,
			(component_install || install_netfx || install_mbsa
			     || netfx_mode || install_sp || install_vcredist), // Conditions to not mark anything with '[>]'
			sku,sp_to_int(splevel),&notifications);

	// Detect .NET Framework parameters
	int nfxServicePack[NFX_VERSION_COUNT];
	bool nfxInstall[NFX_VERSION_COUNT];
	nfxDetect(nfxServicePack);
	nfxUpgrade(nfxServicePack,nfxInstall,sp_to_int(splevel),&notifications);

	// Construct Update vector
	std::vector<std::string> name;
	std::vector<std::string> exe;
	std::vector<std::string> nfx_name;
	std::vector<std::string> nfx_exe;

	if(component_install) {
		componentUpdates(&name,&exe,sku,sp_to_int(splevel),&notifications);
	} else if(install_sp) {
		sp_to_be_installed=installServicePack(&name,&exe,sku,sp_to_int(splevel),&notifications);
	} else if(install_mbsa) {
		installMBSA(&name,&exe,sku);
	} else if(install_vcredist) {
		installVC(&name,&exe,sp_to_int(splevel));
	} else if(install_netfx) {
		nfxInstallCheck(nfxServicePack,nfxInstall,sp_to_int(splevel),&notifications);
		nfxInstallation(&nfx_name,&nfx_exe,sku,sp_to_int(splevel),nfxServicePack,nfxInstall,&notifications);
	} else {
		windowsUpdates(&name,&exe,sku,sp_to_int(splevel),compInstall,qfe,&notifications);
		nfxUpdates(&nfx_name,&nfx_exe,sku,sp_to_int(splevel),nfxServicePack,nfxInstall,&notifications);
	}

	// Combined update list
	std::vector<std::string> all_name;
	all_name.reserve(name.size()+nfx_name.size());
	all_name.insert(all_name.end(),name.begin(),name.end());
	all_name.insert(all_name.end(),nfx_name.begin(),nfx_name.end());
	removeDuplicates(&all_name);

	// Print program banner
	banner(batch,PROGRAM_DATE,ProductName);

	// Compare MBSA, if option selected
	if(mbsa) {
		systemComponentVersions(false);
		displayOptions(compInstalled,compInstall,false,COMPONENT_COUNT,sp_to_int(splevel));
		compareMBSA(mbsa_file,&all_name);
	} else {

		if(netfx_mode || install_netfx) {
			nfxSummary(nfxServicePack,nfxInstall,batch);
		} else if (install_mbsa) {
			printf("%s   Install minimum updates to run MBSA 2.2.\n",batch?"echo ":"");
			printf("%s\n",batch?"echo.":"");
		} else if (install_sp) {
			systemComponentVersions(batch);
			if(sp_to_be_installed>0) {
				printf("%s   Install Service Pack %d\n",batch?"echo ":"",sp_to_be_installed);
			}
			printf("%s\n",batch?"echo.":"");
			printList2(&name,batch);
		} else if( component_install ) {
			systemComponentVersions(batch);
			printList2(&name,batch);
		} else {
			systemComponentVersions(batch);
			displayOptions(compInstalled,compInstall,batch,COMPONENT_COUNT,sp_to_int(splevel));
			if(qfe) { printf("%sQFE Hotfixes are enabled.\n",batch?"echo.":""); }
		}
		showNotifications(batch,&notifications);
		if(batch) {
			if(netfx_mode || install_netfx) {
				createUpdateScript(&nfx_name,&nfx_exe,allow_reboot,false);
			} else {
				createUpdateScript(&name,&exe,allow_reboot,false);
			}
		} else {
			printList(&all_name);
		}

	}

	return 0;
}
