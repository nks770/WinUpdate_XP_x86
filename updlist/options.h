
#ifndef OPTIONS_H
#define OPTIONS_H

#include "stdafx.h"
#include "winsku.h"
#include <vector>
#include <string>

#define COMPONENT_COUNT 19

void detectOptions(bool* components);
void argumentOptions(int argc, _TCHAR* argv[], bool* installed, bool* components, const int ncomp, bool disable_install,
					 winsku sku, int sp, std::vector<std::string> *notifications);
void displayOptions(bool* installed, bool* install, bool batchmode, const int ncomp);
void systemComponentVersions(bool batchmode);

#endif