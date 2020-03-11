
#ifndef NFXUPDATE_H
#define NFXUPDATE_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "winsku.h"

void nfxInstallation(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
					 int* nfxdata, bool* nfxinstall, std::vector<std::string> *notifications);

void nfxUpdates(std::vector<std::string>* name, std::vector<std::string>* exe,winsku sku,int sp,
				int* nfxdata, bool* nfxinstall, std::vector<std::string> *notifications);

#endif