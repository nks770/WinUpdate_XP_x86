#include "stdafx.h"
//#include <stdio.h>
#include <vector>
#include <cassert>
#include <string>
#include <shlwapi.h> // Linker requires shlwapi.lib
//#include <windows.h> // Linker requires version.lib

#include "mbsa.h"

using namespace std;

void removeDuplicates(std::vector<std::string>* v) {
	unsigned int vs = (unsigned int)v->size();
	for(unsigned int i=0; i<vs; i++) {
		for(unsigned int j=i+1; j<vs; j++) {
			//printf("Comparing %d - %d (out of %d)\n",i,j,vs);
			if(v->at(i)==v->at(j)) {
				//printf("Removed duplicate at %d\n",j);
				v->erase(v->begin()+(j--)); vs--;
			}
		}
	}
}

void compareMBSA(const std::wstring filePath, std::vector<std::string>* update_list) {
	
	std::vector<std::string> mbsa_list;
	
	//printf("          ##########################\n");
	printf("          ## MBSA Comparison mode ##\n");
	//printf("          ##########################\n");

	int mbsa_status=parseMBSAOutput(filePath,&mbsa_list);
	printf("\n File = %ls (%s)\n\n",filePath.c_str(),mbsa_status==0?"OK":"ERROR");
	if(mbsa_status==1){printf("ERROR: The file could not be found.\n");}
	if(mbsa_status==2){printf("ERROR: The file could not be read, or contained no data.\n");}

	// Allocate memory
	int* update_index=(int*)malloc((1+update_list->size())*sizeof(int));
	int* mbsa_index=(int*)malloc((1+mbsa_list.size())*sizeof(int));

	// Match data
	matchVectors(update_list,update_index,&mbsa_list,mbsa_index);
	matchSummary(update_list,update_index,&mbsa_list,mbsa_index);

	// Print lists
	printf("MBSA-only updates:\n");
	printUnmatchedUpdates(&mbsa_list,mbsa_index);
	printf("\nUpdates from this tool only:\n");
	printUnmatchedUpdates(update_list,update_index);

	//printf("\nMBSA both:\n");
	//printMatchedUpdates(&mbsa_list,mbsa_index);
	//printf("\nTool both:\n");
	//printMatchedUpdates(update_list,update_index);

	// De-allocate memory
	free(update_index);
	free(mbsa_index);
}

void matchVectors(std::vector<std::string>* a, int* ai, std::vector<std::string>* b, int* bi) {
	
	unsigned int i=0;
	unsigned int j=0;
	
	// Get sizes of vectors
	unsigned int a_size=(unsigned int)a->size();
	unsigned int b_size=(unsigned int)b->size();

	// Initialize match arrays
	for(i=0; i<a_size; i++) { ai[i]=-1; }
	for(i=0; i<b_size; i++) { bi[i]=-1; }

	// Search for matches
	for(i=0; i<a_size; i++) {
		for(j=0; j<b_size; j++) {
			if(a->at(i) == b->at(j)) {
				ai[i]=j;
				//printf("SEARCH 1: (%03d) %s = %d\n",i,a->at(i).c_str(),j);
				break;
			}
		}
	}
	for(j=0; j<b_size; j++) {
		for(i=0; i<a_size; i++) {
			if(b->at(j) == a->at(i)) {
				bi[j]=i;
				//printf("SEARCH 2: (%03d) %s = %d\n",j,b->at(j).c_str(),i);
				break;
			}
		}
	}

	// Check
	for(i=0; i<a_size; i++) {
		if(ai[i] != -1) {
			assert(bi[ai[i]]==i);
		}
	}
	for(j=0; j<b_size; j++) {
		if(bi[j] != -1) {
			assert(ai[bi[j]]==j);
		}
	}
}

void matchSummary(std::vector<std::string>* a, int* ai, std::vector<std::string>* b, int* bi) {
	
	unsigned int updates_only=0;
	unsigned int mbsa_only=0;
	unsigned int updates_both=0;
	unsigned int mbsa_both=0;

	// Get sizes of vectors
	unsigned int updates_size=(unsigned int)a->size();
	unsigned int mbsa_size=(unsigned int)b->size();

	unsigned int i=0;

	for(i=0; i<updates_size; i++) {
		if(ai[i]==-1) { updates_only++; }
		else { updates_both++; }
	}
	for(i=0; i<mbsa_size; i++) {
		if(bi[i]==-1) { mbsa_only++; }
		else { mbsa_both++; }
	}
	assert(updates_both==mbsa_both);
	//printf("updates_both=%d\tmbsa_both=%d\n",updates_both,mbsa_both);

	printf("SUMMARY\n");
	printf("Updates identified by this tool = %d\n",updates_only);
	printf("Updates identified by MBSA      = %d\n",mbsa_only);
	printf("Updates identified by both      = %d\n",updates_both);
	printf("\n");

}

void printUnmatchedUpdates(std::vector<std::string>* update_list, int* list_index) {
	unsigned int updates_size=(unsigned int)update_list->size();
	for(unsigned int i=0; i<updates_size; i++) {
		if(list_index[i] == -1) {
			printf("%s\n",update_list->at(i).c_str());
		}
	}
}
void printMatchedUpdates(std::vector<std::string>* update_list, int* list_index) {
	unsigned int updates_size=(unsigned int)update_list->size();
	for(unsigned int i=0; i<updates_size; i++) {
		if(list_index[i] != -1) {
			printf("%s\n",update_list->at(i).c_str());
		}
	}
}

int parseMBSAOutput(const std::wstring filePath, std::vector<std::string>* missing_updates) {

	std::vector<std::string> mbsa;
	unsigned int j=0;

	// Verify that MBSA file exists
	bool mbsafile_exists=(PathFileExistsW(filePath.c_str())!=0);
		if(!mbsafile_exists){ return 1; }

	// Load contents of MBSA file
	FILE* mbsafile = NULL;
	char buffer[1024];
	if (mbsafile_exists && !_wfopen_s(&mbsafile,filePath.c_str(),L"r")) {
		while(!feof(mbsafile) && j<2048) {
			fgets(buffer,sizeof(buffer),mbsafile);
			mbsa.push_back(std::string(buffer));
			j++; // Put a limit on the amount of data to read from the file.
		}
	fclose(mbsafile);
	}

	// Return error if nothing was loaded from the file.
	if(mbsa.size()==0) { return 2; }

	// Create list of missing updates.
	std::string s;
	for(j=0; j<mbsa.size(); j++) {
		if(mbsa[j].find("| Missing |") != std::string::npos) {
			// Extract update name
			s=mbsa[j].substr(mbsa[j].find("| Missing |")+12,std::string::npos);
			s=s.substr(0,s.find(" |"));

			// Remove the '&amp;' substitution
			while(s.find("&amp;")!=std::string::npos) {
				s=s.substr(0,s.find("&amp;"))+"&"+s.substr(s.find("&amp;")+5,std::string::npos);
			}
			
			// Append to vector
			missing_updates->push_back(s);
		}
	}
	return 0;
}
