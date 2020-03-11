#include "stdafx.h"
#include "notify.h"
#include "script.h"
#include <string>
#include <vector>

using namespace std;

void servicePackNotifications(int sp, std::vector<std::string> *data) {
	if(sp<3) {
//                       ....V....1....V....2....V....3....V....4....V....5
		data->push_back(std::string("A NEWER SERVICE PACK IS AVAILABLE!")+
			           "| |Service Pack 2 is installed, but Service Pack"+
			           "|3 is available.  We recommend you install the"+
					   "|latest Service Pack before applying other"+
					   "|patches.  You can use 'install_sp.bat' to"+
					   "|install Service Pack 3."+
					   "| |Otherwise, we will continue installing all"+
					   "|patches relevant for Service Pack 2.");
	}
}

void showNotifications(bool batch, std::vector<std::string> *data) {

	int dsiz=(int)data->size();
	int i;
	int j;
	int k;
	int l;
	int m[256];
	int n;

	if(dsiz>0) {
		for(i=0; i<dsiz; i++) {
			k=(int)data->at(i).size();
			l=0; m[0]=0;
			for(j=0; j<k; j++) {
				if(data->at(i).at(j)=='|') {
					m[++l]=++j;
				}
			}
			m[l+1]=k+1;

			printf("%s\n",batch?"echo.":"");
			printf("%s %s\n",batch?"echo ":"",escapeString("__________________________________________________",batch).c_str());
			printf("%s %s\n",batch?"echo ":"",escapeString("|                  A L E R T !                   |",batch).c_str());
			printf("%s %s\n",batch?"echo ":"",escapeString("|                                                |",batch).c_str());
			for(j=0; j<=l; j++) {
				n=46-(m[j+1]-m[j]-1);
				if(n<0){n=0;}
				printf("%s %s\n",batch?"echo ":"",
					escapeString(std::string("| ")+data->at(i).substr(m[j],m[j+1]-m[j]-1)+std::string(n,' ')+" |",batch).c_str());
			}
			printf("%s %s\n",batch?"echo ":"",escapeString("|                                                |",batch).c_str());
			if(batch){
			printf("%s %s\n",batch?"echo ":"",escapeString("| [ Enter ] to proceed     [ Ctrl-C ] to cancel  |",batch).c_str());
			}
			printf("%s %s\n",batch?"echo ":"",escapeString("|________________________________________________|",batch).c_str());
			printf("%s\n",batch?"echo.":"");
			if(batch) {
				printf("echo.\npause\n");
			}
		}
	}

}