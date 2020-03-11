#ifndef WINSKU_H
#define WINSKU_H

#include "stdafx.h"
#include <string>

using namespace std;

enum winsku { UNKNOWN_OS        =    0,
	          XP_PRO            =    1,
              XP_PRON           =    2,
              XP_HOME           =    4,
			  XP_HOMEN          =    8,
			  XP_STARTER        =   16,
			  XP_TABLET         =   32,
			  XP_MCE2002        =   64,
			  XP_MCE2004        =  128,
			  XP_MCE2005        =  256,
			  XPE_FLP           =  512,
			  XPE_POSREADY2009  = 1024,
			  XPE_WES2009       = 2048 };

enum winsp  { SP_UNKNOWN = 0,
              SP0 = 1,
              SP1 = 2,
			  SP2 = 4,
			  SP3 = 8 };

std::string getSKUName(winsku sku);
std::string getSPName(winsp sp);
winsku getSKU();
winsp getSP();
int sp_to_int(winsp sp);

#endif