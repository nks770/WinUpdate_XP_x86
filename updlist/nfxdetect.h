
#ifndef NFXDETECT_H
#define NFXDETECT_H

#include "stdafx.h"
#include <vector>
#include <string>

#define NFX_VERSION_COUNT 8

void nfxDetect(int* nfxVersions);
void nfxUpgrade(int* nfxsp, bool* nfxinstall, std::vector<std::string> *notifications);
void nfxInstallCheck(int* nfxsp, bool* nfxinstall, int sp, std::vector<std::string> *notifications);
void nfxSummary(int* nfxdata, bool* nfxinstall, bool batchmode);

#endif