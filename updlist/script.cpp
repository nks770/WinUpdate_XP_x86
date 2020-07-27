#include "stdafx.h"
#include "script.h"
#include <vector>
#include <string>

using namespace std;

void scriptElements(std::vector<std::string>* name,std::vector<std::string>* exe) {

	unsigned int i=0;
	unsigned int j=(unsigned int)name->size();

	std::string f;

	if((unsigned int)exe->size()<j){
		j=(unsigned int)exe->size();
	}

	if(j<10) { f="echo [%d of %d] %s\n"; }
	else if(j<100) { f="echo [%2d of %d] %s\n"; }
	else if(j<1000) { f="echo [%3d of %d] %s\n"; }

	for(i=0; i<j; i++) {
		//printf("echo Installing %s (update %d of %d)...\n",escapeString(name->at(i)).c_str(),i+1,j);
		printf(f.c_str(),i+1,j,escapeString(name->at(i)).c_str());
		printf("%s\n",exe->at(i).c_str());
	}
}

void createUpdateScript(std::vector<std::string>* name,std::vector<std::string>* exe,
						const bool RebootPermitted, const bool isNullAvailable) {

	unsigned int j=(unsigned int)name->size();

	if((unsigned int)exe->size()<j){
		j=(unsigned int)exe->size();
	}

	printf("echo.\n");

	if(j==0) {

		printf("echo The machine is already up to date.\n");
		printf("echo.\n");
		printf("echo Press enter to exit.\n");
		if(!isNullAvailable) { printf("pause\n"); }
		else {printf("pause>NUL\n"); }

	} else {
		if(!isNullAvailable) {
			printf("echo Ready to install %d %s.\n",j,j==1?"update":"updates");
			printf("echo.\n");
			printf("pause\n");
		} else {
			printf("echo Ready to install %d %s. Press enter to begin.\n",j,j==1?"update":"updates");
			printf("echo.\n");
			printf("pause>NUL\n");
		}
		scriptElements(name,exe);

		if(RebootPermitted && j>0) {
			printf("echo Press enter to reboot.\n");
			if(!isNullAvailable) { printf("pause\n"); }
			else {printf("pause>NUL\n"); }
			printf("echo Rebooting the system...\n");
			printf("shutdown.exe -r -t 0\n");
		} else {
			printf("echo Press enter to exit.\n");
			if(!isNullAvailable) { printf("pause\n"); }
			else {printf("pause>NUL\n"); }
		}
	}
}

void printList(std::vector<std::string>* name) {

	unsigned int j=(unsigned int)name->size();
	unsigned int jj=j;

	// Workaround to not display QChain.exe as a missing update when listing the missing updates.
	for(unsigned int i=0; i<j; i++) {
		if(name->at(i)=="QChain.exe command-line tool (Q815062)") {
			jj--;
		}
	}

	if(jj==0) {
		printf("<List Empty>\n\n");
	} else {
		printf("%d updates are missing.\n\n",jj);
		for(unsigned int i=0; i<j; i++) {
			if(name->at(i)!="QChain.exe command-line tool (Q815062)") {
				printf("%s\n",name->at(i).c_str());
			}
		}
	}
}

void printList2(std::vector<std::string>* name, bool batch) {

	unsigned int j=(unsigned int)name->size();

	printf("%sThe following %d update%s will be installed:\n",batch?"echo ":"",j,j==1?"":"s");
	if(j==0) {
		printf("%s%s\n",batch?"echo ":"",escapeString(std::string("  * <None>"),batch).c_str());
	} else {
		for(unsigned int i=0; i<j; i++) {
			printf("%s%s\n",batch?"echo ":"",escapeString(std::string("  * ")+name->at(i),batch).c_str());
		}
	}
	
	//printf("%s\n",batch?"echo.":"");
}

std::string escapeString(const std::string str) {
	std::string rval;
	for(unsigned int i=0; i<str.length(); i++) {
		if(str[i]=='&') { rval.append("^&"); }
		else if(str[i]=='|') { rval.append("^|"); }
		else if(str[i]=='<') { rval.append("^<"); }
		else if(str[i]=='>') { rval.append("^>"); }
		else { rval.push_back(str[i]); }
	}
	return rval;
}

std::string escapeString(const std::string str, bool sw) {
	if(sw) {
		return escapeString(str);
	} else {
		return str;
	}
}