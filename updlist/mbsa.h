
#ifndef MBSA_H
#define MBSA_H

#include "stdafx.h"
#include <string>
#include <vector>

using namespace std;

void compareMBSA(const std::wstring filePath, std::vector<std::string>* update_list);

void removeDuplicates(std::vector<std::string>* v);
void matchVectors(std::vector<std::string>* a, int* ai, std::vector<std::string>* b, int* bi);
void matchSummary(std::vector<std::string>* a, int* ai, std::vector<std::string>* b, int* bi);
void printUnmatchedUpdates(std::vector<std::string>* update_list, int* list_index);
void printMatchedUpdates(std::vector<std::string>* update_list, int* list_index);
int parseMBSAOutput(const std::wstring filePath, std::vector<std::string>* missing_updates);

#endif