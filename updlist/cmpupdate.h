
#ifndef CMPUPDATE_H
#define CMPUPDATE_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "winsku.h"

void componentUpdates(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp);
void installMBSA(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku);

int installServicePack(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
					   std::vector<std::string> *notifications);

#endif