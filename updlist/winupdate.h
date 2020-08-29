
#ifndef WINUPDATE_H
#define WINUPDATE_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "winsku.h"

void windowsUpdates(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
					bool* options,bool qfe,std::vector<std::string> *notifications);

#endif