#include "stdafx.h"
#include "script.h"
#include <vector>
#include <string>

using namespace std;

void scriptElements(std::vector<std::string>* name,std::vector<std::string>* exe) {

	unsigned int i=0;
	unsigned int j=(unsigned int)name->size();

	if((unsigned int)exe->size()<j){
		j=(unsigned int)exe->size();
	}

	for(i=0; i<j; i++) {
		printf("echo Installing %s (update %d of %d)...\n",escapeString(name->at(i)).c_str(),i+1,j);
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

	if(j==0) {
		printf("<List Empty>\n\n");
	} else {
		printf("%d updates are missing.\n\n",j);
		for(unsigned int i=0; i<j; i++) {
			printf("%s\n",name->at(i).c_str());
		}
	}
}

std::string escapeString(const std::string str) {
	std::string rval;
	for(unsigned int i=0; i<str.length(); i++) {
		if(str[i]=='&') { rval.append("^&"); }
		else if(str[i]=='|') { rval.append("^|"); }
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