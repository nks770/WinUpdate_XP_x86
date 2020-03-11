
#ifndef FVER_H
#define FVER_H

#include "stdafx.h"
#include <string>

using namespace std;

class fver {
private:
	unsigned int verData[4];

public:
	std::string format() const;
	std::string format2() const;
	int major() const;

	// Operators
	bool operator<(const fver& b) const;
	bool operator>(const fver& b) const;
	bool operator<=(const fver& b) const;
	bool operator>=(const fver& b) const;
	bool operator==(const fver& b) const;
	bool operator!=(const fver& b) const;

	// Constructors and destructors
	fver();
	fver(unsigned int major, unsigned int minor, unsigned int build, unsigned int revision);
	//fver(char* verstring);
	fver(const wchar_t* verstring);
	~fver();
};

fver getFileVer(const std::wstring& lptstrFilename,int* status);
__int64 getFileSize(std::wstring name);
bool fileExists(std::wstring name);

#endif