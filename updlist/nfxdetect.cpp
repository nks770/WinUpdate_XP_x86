#include "stdafx.h"
#include "nfxdetect.h"
#include "registry.h"
#include <string>
#include <vector>

using namespace std;

void nfxDetect(int* nfxVersions) {

	int i=0;
	int error=0;
	std::wstring data;

	for(i=0; i<8; i++) {
		nfxVersions[i]=-1;
	}
	int* v_nfx10=nfxVersions+0;
	int* v_nfx10mce=nfxVersions+1;
	int* v_nfx11=nfxVersions+2;
	int* v_nfx20=nfxVersions+3;
	int* v_nfx30=nfxVersions+4;
	int* v_nfx35=nfxVersions+5;
	int* v_nfx40c=nfxVersions+6;
	int* v_nfx40f=nfxVersions+7;

	// .NET Framework v1.0
	//const std::wstring nfx10a=L"SOFTWARE\\Microsoft\\.NETFramework\\Policy\\v1.0";
	const std::wstring nfx10=L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{78705f0d-e8db-4b2d-8193-982bdda15ecd}";
	//data=regQueryValue(nfx10a,L"3705",&error);
	//if(!error) { *v_nfx10=0; }
	data=regQueryValue(nfx10,L"Version",&error);
	if(!error) {
		if(data==L"1,0,3705,0") { *v_nfx10=0; }
		if(data==L"1,0,3705,1") { *v_nfx10=1; }
		if(data==L"1,0,3705,2") { *v_nfx10=2; }
		if(data==L"1,0,3705,3") { *v_nfx10=3; }
	}
	
	// .NET Framework v1.0 (Media Center Edition and Tablet PC Edition)
	const std::wstring nfx10_mce=L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{FDC11A6F-17D1-48f9-9EA3-9051954BAA24}";
	//data=regQueryValue(nfx10a,L"3705",&error);
	//if(!error) { *v_nfx10mce=0; }
	data=regQueryValue(nfx10_mce,L"Version",&error);
	if(!error) {
		if(data==L"1,0,3705,0") { *v_nfx10mce=0; }
		if(data==L"1,0,3705,1") { *v_nfx10mce=1; }
		if(data==L"1,0,3705,2") { *v_nfx10mce=2; }
		if(data==L"1,0,3705,3") { *v_nfx10mce=3; }
	}

	// .NET Framework v1.1
	const std::wstring nfx11=L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v1.1.4322";
	data=regQueryValue(nfx11,L"Install",&error);
	if(!error) { *v_nfx11=0; }
	data=regQueryValue(nfx11,L"SP",&error);
	if(!error) {
		if(data==L"0") { *v_nfx11=0; }
		if(data==L"1") { *v_nfx11=1; }
	}

	// .NET Framework v2.0
	const std::wstring nfx20=L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v2.0.50727";
	data=regQueryValue(nfx20,L"Install",&error);
	if(!error) { *v_nfx20=0; }
	data=regQueryValue(nfx20,L"SP",&error);
	if(!error) {
		if(data==L"0") { *v_nfx20=0; }
		if(data==L"1") { *v_nfx20=1; }
		if(data==L"2") { *v_nfx20=2; }
	}

	// .NET Framework v3.0
	const std::wstring nfx30=L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.0";
	const std::wstring nfx30s=nfx30+L"\\Setup";
	data=regQueryValue(nfx30s,L"InstallSuccess",&error);
	if(!error) { *v_nfx30=0; }
	data=regQueryValue(nfx30,L"SP",&error);
	if(!error) {
		if(data==L"0") { *v_nfx30=0; }
		if(data==L"1") { *v_nfx30=1; }
		if(data==L"2") { *v_nfx30=2; }
	}

	// .NET Framework v3.5
	const std::wstring nfx35=L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v3.5";
	data=regQueryValue(nfx35,L"Install",&error);
	if(!error) { *v_nfx35=0; }
	data=regQueryValue(nfx35,L"SP",&error);
	if(!error) {
		if(data==L"0") { *v_nfx35=0; }
		if(data==L"1") { *v_nfx35=1; }
	}

	// .NET Framework v4.0 Client Profile
	const std::wstring nfx40c=L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Client";
	data=regQueryValue(nfx40c,L"Install",&error);
	if(!error) { *v_nfx40c=0; }
	data=regQueryValue(nfx40c,L"Version",&error);
	if(!error) {
		if(data==L"4.0.30319.0") { *v_nfx40c=0; }
	}

	// .NET Framework v4.0 Full Profile
	const std::wstring nfx40f=L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full";
	data=regQueryValue(nfx40f,L"Install",&error);
	if(!error) { *v_nfx40f=0; }
	data=regQueryValue(nfx40f,L"Version",&error);
	if(!error) {
		if(data==L"4.0.30319.0") { *v_nfx40f=0; }
	}

}

void nfxUpgrade(int* nfxsp, bool* nfxinstall, std::vector<std::string> *notifications) {
	int* v_nfx10=nfxsp+0;
	int* v_nfx10mce=nfxsp+1;
	int* v_nfx11=nfxsp+2;
	int* v_nfx20=nfxsp+3;
	int* v_nfx30=nfxsp+4;
	int* v_nfx35=nfxsp+5;
	int* v_nfx40c=nfxsp+6;
	int* v_nfx40f=nfxsp+7;

	bool* i_nfx10=nfxinstall+0;    *i_nfx10=false;
	bool* i_nfx10mce=nfxinstall+1; *i_nfx10mce=false;
	bool* i_nfx11=nfxinstall+2;    *i_nfx11=false;
	bool* i_nfx20=nfxinstall+3;    *i_nfx20=false;
	bool* i_nfx30=nfxinstall+4;    *i_nfx30=false;
	bool* i_nfx35=nfxinstall+5;    *i_nfx35=false;
	bool* i_nfx40c=nfxinstall+6;   *i_nfx40c=false;
	bool* i_nfx40f=nfxinstall+7;   *i_nfx40f=false;

	//if ((*v_nfx10>=0 && *v_nfx10<3) && *v_nfx40c<0 && *v_nfx40f<0) { *i_nfx10=true; }
	if (*v_nfx10>=0 && *v_nfx10<3) {
		if(*v_nfx40c<0 && *v_nfx40f<0) {
				*i_nfx10=true;
		} else {
//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Cannot install Microsoft .NET Framework")
			                               +"|1.0 SP3 (KB867461) because .NET"
										   +"|Framework 4.0 is installed."
							               +"| "
							               +"|Please uninstall .NET Framework 4.0 and try"
							               +"|again.");
		}
	}
	if (*v_nfx11>=0 && *v_nfx11<1) { *i_nfx11=true; }
	if (*v_nfx20>=0 && *v_nfx20<2) { *i_nfx20=true; }
	if (*v_nfx30>=0 && *v_nfx30<2) { *i_nfx20=true; *i_nfx30=true; *i_nfx35=true; }
	if (*v_nfx35>=0 && *v_nfx35<1) { *i_nfx20=true; *i_nfx30=true; *i_nfx35=true; }
}

void nfxInstallCheck(int* nfxsp, bool* nfxinstall, int sp, std::vector<std::string> *notifications) {
	int* v_nfx10=nfxsp+0;
	int* v_nfx10mce=nfxsp+1;
	int* v_nfx11=nfxsp+2;
	int* v_nfx20=nfxsp+3;
	int* v_nfx30=nfxsp+4;
	int* v_nfx35=nfxsp+5;
	int* v_nfx40c=nfxsp+6;
	int* v_nfx40f=nfxsp+7;

	bool* i_nfx10=nfxinstall+0;    *i_nfx10=false;
	bool* i_nfx10mce=nfxinstall+1; *i_nfx10mce=false;
	bool* i_nfx11=nfxinstall+2;    *i_nfx11=false;
	bool* i_nfx20=nfxinstall+3;    *i_nfx20=false;
	bool* i_nfx30=nfxinstall+4;    *i_nfx30=false;
	bool* i_nfx35=nfxinstall+5;    *i_nfx35=false;
	bool* i_nfx40c=nfxinstall+6;   *i_nfx40c=false;
	bool* i_nfx40f=nfxinstall+7;   *i_nfx40f=false;

	bool netfx4_issue=false;

	if (*v_nfx10<3 && *v_nfx10mce<0) {
		if(*v_nfx40c<0 && *v_nfx40f<0) {
				*i_nfx10=true;
		} else {
//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Cannot install Microsoft .NET Framework")
			                               +"|1.0 because .NET Framework 4.0 is"
										   +"|installed."
							               +"| "
							               +"|Please uninstall .NET Framework 4.0 and try"
							               +"|again.");
		}
	}
	if (*v_nfx11<1 ) { *i_nfx11=true; }
	/*if (*v_nfx20<0 || *v_nfx30<0 || *v_nfx35<0 ) { *i_nfx20=true; *i_nfx30=true; *i_nfx35=true; }*/
	if (*v_nfx20<2 ) { *i_nfx20=true; }
	if (*v_nfx30<2 ) { *i_nfx30=true; }
	if (*v_nfx35<1 ) { *i_nfx35=true; }
	if (*v_nfx40c<0 ) {
		if(sp==3) {
			*i_nfx40c=true; *i_nfx40f=true;
		} else {
			netfx4_issue=true;
		}
	}
	if (*v_nfx40f<0 ) {
		if(sp==3) {
			*i_nfx40f=true;
		} else {
			netfx4_issue=true;
		}
	}

	if(netfx4_issue && sp==2 ) {
		//                                    ....V....1....V....2....V....3....V....4....V....5
		notifications->push_back(std::string("Cannot install Microsoft .NET Framework 4.0")
			                               +"|because it requires Windows XP Service Pack 3."
							               +"| "
							               +"|NOTE: Technically, the installer would succeed"
							               +"|on Service Pack 2, but such a configuration is"
							               +"|not supported and might cause issues.");
	}

}


void nfxSummary(int* nfxdata, bool* nfxinstall, bool batchmode) {

	//for(int i=0; i<8; i++) { printf("%d ",nfxdata[i]); }
	//printf("\n");

	printf("%s .NET Framework Version   SP Level             Edition\n",batchmode?"echo ":"");
	printf("%s------------------------ ---------- ------------------------------\n",batchmode?"echo ":"");
	if(nfxdata[1]>-1) {
		printf("%s [%s] .NET Framework 1.0      SP%1d     MediaCenter/TabletPC Edition\n",batchmode?"echo ":"",nfxinstall[1]?"U":"X",nfxdata[1]);
	} else if(nfxdata[0]>-1) {
		printf("%s [%s] .NET Framework 1.0      SP%1d\n",batchmode?"echo ":"",nfxinstall[0]?"U":"X",nfxdata[0]);
	} else {
		printf("%s [%s] .NET Framework 1.0       -\n",batchmode?"echo ":"",nfxinstall[0]?"I":" ");
	}

	if(nfxdata[2]>-1) {
		printf("%s [%s] .NET Framework 1.1      SP%1d\n",batchmode?"echo ":"",nfxinstall[2]?"U":"X",nfxdata[2]);
	} else {
		printf("%s [%s] .NET Framework 1.1       -\n",batchmode?"echo ":"",nfxinstall[2]?"I":" ");
	}

	if(nfxdata[3]>-1) {
		printf("%s [%s] .NET Framework 2.0      SP%1d\n",batchmode?"echo ":"",nfxinstall[3]?"U":"X",nfxdata[3]);
	} else {
		printf("%s [%s] .NET Framework 2.0       -\n",batchmode?"echo ":"",nfxinstall[3]?"I":" ");
	}
	
	if(nfxdata[4]>-1) {
		printf("%s [%s] .NET Framework 3.0      SP%1d\n",batchmode?"echo ":"",nfxinstall[4]?"U":"X",nfxdata[4]);
	} else {
		printf("%s [%s] .NET Framework 3.0       -\n",batchmode?"echo ":"",nfxinstall[4]?"I":" ");
	}
	
	if(nfxdata[5]>-1) {
		printf("%s [%s] .NET Framework 3.5      SP%1d\n",batchmode?"echo ":"",nfxinstall[5]?"U":"X",nfxdata[5]);
	} else {
		printf("%s [%s] .NET Framework 3.5       -\n",batchmode?"echo ":"",nfxinstall[5]?"I":" ");
	}

	if(nfxdata[7]>-1) {
		printf("%s [%s] .NET Framework 4.0      SP%1d     Full Profile\n",batchmode?"echo ":"",nfxinstall[7]?"U":"X",nfxdata[7]);
	} else if(nfxdata[6]>-1 && nfxinstall[7]) {
		printf("%s [U] .NET Framework 4.0      SP%1d     Client Profile -> Full Prof.\n",batchmode?"echo ":"",nfxdata[6]);
	} else if(nfxdata[6]>-1) {
		printf("%s [%s] .NET Framework 4.0      SP%1d     Client Profile\n",batchmode?"echo ":"",nfxinstall[6]?"U":"X",nfxdata[6]);
	} else if(nfxinstall[6] || nfxinstall[7]) {
		printf("%s [I] .NET Framework 4.0       -      %s Profile\n",batchmode?"echo ":"",nfxinstall[7]?"Full":"Client");
	} else {
		printf("%s [ ] .NET Framework 4.0       -\n",batchmode?"echo ":"");
	}

	printf("%s\n",batchmode?"echo.":"");
}
