
#ifndef CERTIFICATES_H
#define CERTIFICATES_H

#include "stdafx.h"
#include <string>
#include <vector>

bool enumerateCertificates( std::vector<std::string>* certhash,
                            std::vector<std::string>* certname, char* pszStoreName, bool debug);
void listCertificates(char* pszStoreName);
bool vectorContainsValue(std::string s, std::vector<std::string>* v);

bool checkCertificates(char* pszStoreName);
bool checkCertificate(std::string cert,char* pszStoreName);

std::vector<std::string> createCertificateList(char* pszStoreName);

#endif