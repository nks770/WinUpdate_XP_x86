#ifndef KB914798_H
#define KB914798_H

#include "stdafx.h"
#include <windows.h>

bool kb914798_installed();

// Example usage:
//	printf("KB914798: %s\n",kb914798_installed()?"SUCCESS":"FAIL");

bool verifyBinaryData(const BYTE base[],int bsize,LPBYTE comp,int csize);
void printDebugBinaryData(LPBYTE data,int size,char* comment);

#endif