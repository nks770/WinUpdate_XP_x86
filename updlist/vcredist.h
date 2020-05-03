
#ifndef VCREDIST_H
#define VCREDIST_H

#include "stdafx.h"
#include <string>
#include <vector>
#include "fver.h"

void vcsummary();
void installVC(std::vector<std::string>* name, std::vector<std::string>* exe, int sp);

fver vcr2005(bool debug);
fver vcr2008(bool debug);
fver vcr2010(bool debug);
fver vcr2012(bool debug);
fver vcr2013(bool debug);
fver vcr2015(bool debug);

#endif