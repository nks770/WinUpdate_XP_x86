
#ifndef SCRIPT_H
#define SCRIPT_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "winsku.h"

void scriptElements(std::vector<std::string>* name,std::vector<std::string>* exe);

void createUpdateScript(std::vector<std::string>* name,std::vector<std::string>* exe,
						const bool RebootPermitted, const bool isNullAvailable);

void printList(std::vector<std::string>* name);
void printList2(std::vector<std::string>* name, bool batch);

std::string escapeString(const std::string str);
std::string escapeString(const std::string str, bool sw);

#endif