#include "stdafx.h"
#include "kb914798.h"
#include "registry.h"
#include <string>
#include <vector>

using namespace std;

bool kb914798_installed() {

	bool Dhcp_correct    =false;
	bool Dnscache_correct=false;
	bool MSDTC_correct   =false;
	bool NetBT_correct   =false;
	bool SCardSvr_correct=false;
	bool SSDPSRV_correct =false;
	bool upnphost_correct=false;
	bool installed       =false;

	int Dhcp_size;
	int Dnscache_size;
	int MSDTC_size;
	int NetBT_size;
	int SCardSvr_size;
	int SSDPSRV_size;
	int upnphost_size;

	std::wstring Dhcp    =L"SYSTEM\\CurrentControlSet\\Services\\Dhcp\\Security";
	std::wstring Dnscache=L"SYSTEM\\CurrentControlSet\\Services\\Dnscache\\Security";
	std::wstring MSDTC   =L"SYSTEM\\CurrentControlSet\\Services\\MSDTC\\Security";
	std::wstring NetBT   =L"SYSTEM\\CurrentControlSet\\Services\\NetBT\\Security";
	std::wstring SCardSvr=L"SYSTEM\\CurrentControlSet\\Services\\SCardSvr\\Security";
	std::wstring SSDPSRV =L"SYSTEM\\CurrentControlSet\\Services\\SSDPSRV\\Security";
	std::wstring upnphost=L"SYSTEM\\CurrentControlSet\\Services\\upnphost\\Security";

	const BYTE Dhcp_chk[] = {
		0x01,0x00,0x14,0x80,0x90,0x00,0x00,0x00,0x9c,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0x60,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x8d,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x0b,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
		0xfd,0x01,0x02,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x2c,0x02,0x00,0x00,0x00,0x00,0x18,0x00,0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,
		0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x00,0x00,0x14,0x00,
		0xfd,0x01,0x02,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00 };

	const BYTE Dnscache_chk[] = {
		0x01,0x00,0x14,0x80,0xa8,0x00,0x00,0x00,0xb4,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0x78,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x8d,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x0b,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
		0x9d,0x01,0x02,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x23,0x02,0x00,0x00,0x00,0x00,0x18,0x00,0xfd,0x01,0x02,0x00,0x01,0x02,0x00,0x00,
		0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x2c,0x02,0x00,0x00,0x00,0x00,0x18,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x20,0x02,0x00,0x00,0x00,0x00,0x14,0x00,0xfd,0x01,0x02,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,
		0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00 };

	const BYTE MSDTC_chk[] = {
		0x01,0x00,0x14,0x80,0xb8,0x00,0x00,0x00,0xc4,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0x88,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x9d,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x00,
		0xfd,0x01,0x02,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,
		0x00,0x00,0x18,0x00,0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
		0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x00,0x00,0x14,0x00,0x8d,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x0b,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
		0xfd,0x01,0x02,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x23,0x02,0x00,0x00,0x00,0x00,0x14,0x00,0x9d,0x00,0x02,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x14,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,
		0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00 };

	const BYTE NetBT_chk[] = {
		0x01,0x00,0x14,0x80,0xe8,0x00,0x00,0x00,0xf4,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0xb8,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x8d,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x0b,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
		0x9d,0x01,0x02,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x23,0x02,0x00,0x00,0x00,0x00,0x18,0x00,0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,
		0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x00,0x00,0x18,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x25,0x02,0x00,0x00,0x00,0x00,0x14,0x00,0xfd,0x01,0x02,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x40,0x00,0x00,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x13,0x00,0x00,0x00,0x00,0x00,0x14,0x00,
		0x40,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x14,0x00,0x00,0x00,
		0x00,0x00,0x18,0x00,0x9d,0x01,0x02,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
		0x20,0x00,0x00,0x00,0x2c,0x02,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,
		0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00 };

	const BYTE SCardSvr_chk[] = {
		0x01,0x00,0x14,0x80,0xa4,0x00,0x00,0x00,0xb0,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0x74,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0xfd,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x00,0x00,0x14,0x00,
		0xfd,0x01,0x02,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x13,0x00,0x00,0x00,
		0x00,0x00,0x18,0x00,0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,
		0x20,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x00,0x00,0x18,0x00,0xff,0x01,0x0f,0x00,
		0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x25,0x02,0x00,0x00,
		0x00,0x00,0x14,0x00,0x9d,0x01,0x02,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x02,
		0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00 };

	const BYTE SSDPSRV_chk[] = {
		0x01,0x00,0x14,0x80,0xbc,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0x8c,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0xff,0x01,0x0f,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x20,0x02,0x00,0x00,0x00,0x00,0x18,0x00,0xfd,0x01,0x02,0x00,0x01,0x02,0x00,0x00,
		0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x23,0x02,0x00,0x00,0x00,0x00,0x18,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x25,0x02,0x00,0x00,0x00,0x00,0x14,0x00,0x9d,0x00,0x02,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x0b,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x70,0x00,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x13,0x00,0x00,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,
		0x12,0x00,0x00,0x00 };

	const BYTE upnphost_chk[] = {
		0x01,0x00,0x14,0x80,0xbc,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0x14,0x00,0x00,0x00,
		0x30,0x00,0x00,0x00,0x02,0x00,0x1c,0x00,0x01,0x00,0x00,0x00,0x02,0x80,0x14,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
		0x02,0x00,0x8c,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0xff,0x01,0x0f,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x00,0x00,0x18,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x20,0x02,0x00,0x00,0x00,0x00,0x18,0x00,0xfd,0x01,0x02,0x00,0x01,0x02,0x00,0x00,
		0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,0x23,0x02,0x00,0x00,0x00,0x00,0x18,0x00,
		0xff,0x01,0x0f,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x05,0x20,0x00,0x00,0x00,
		0x25,0x02,0x00,0x00,0x00,0x00,0x14,0x00,0x9d,0x00,0x02,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x0b,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x8f,0x01,0x02,0x00,
		0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,0x13,0x00,0x00,0x00,0x01,0x01,0x00,0x00,
		0x00,0x00,0x00,0x05,0x12,0x00,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x05,
		0x12,0x00,0x00,0x00 };

	LPBYTE Dhcp_data = regQueryBinaryData(Dhcp,L"Security",&Dhcp_size);
	Dhcp_correct = verifyBinaryData(Dhcp_chk,sizeof(Dhcp_chk),Dhcp_data,Dhcp_size);
//	printDebugBinaryData(Dhcp_data,Dhcp_size,"Dhcp");

	LPBYTE Dnscache_data = regQueryBinaryData(Dnscache,L"Security",&Dnscache_size);
	Dnscache_correct = verifyBinaryData(Dnscache_chk,sizeof(Dnscache_chk),Dnscache_data,Dnscache_size);
//	printDebugBinaryData(Dnscache_data,Dnscache_size,"Dnscache");

	LPBYTE MSDTC_data = regQueryBinaryData(MSDTC,L"Security",&MSDTC_size);
	MSDTC_correct = verifyBinaryData(MSDTC_chk,sizeof(MSDTC_chk),MSDTC_data,MSDTC_size);
//	printDebugBinaryData(MSDTC_data,MSDTC_size,"MSDTC");

	LPBYTE NetBT_data = regQueryBinaryData(NetBT,L"Security",&NetBT_size);
	NetBT_correct = verifyBinaryData(NetBT_chk,sizeof(NetBT_chk),NetBT_data,NetBT_size);
//	printDebugBinaryData(NetBT_data,NetBT_size,"NetBT");

	LPBYTE SCardSvr_data = regQueryBinaryData(SCardSvr,L"Security",&SCardSvr_size);
	SCardSvr_correct = verifyBinaryData(SCardSvr_chk,sizeof(SCardSvr_chk),SCardSvr_data,SCardSvr_size);
//	printDebugBinaryData(SCardSvr_data,SCardSvr_size,"SCardSvr");

	LPBYTE SSDPSRV_data = regQueryBinaryData(SSDPSRV,L"Security",&SSDPSRV_size);
	SSDPSRV_correct = verifyBinaryData(SSDPSRV_chk,sizeof(SSDPSRV_chk),SSDPSRV_data,SSDPSRV_size);
//	printDebugBinaryData(SSDPSRV_data,SSDPSRV_size,"SSDPSRV");

	LPBYTE upnphost_data = regQueryBinaryData(upnphost,L"Security",&upnphost_size);
	upnphost_correct = verifyBinaryData(upnphost_chk,sizeof(upnphost_chk),upnphost_data,upnphost_size);
//	printDebugBinaryData(upnphost_data,upnphost_size,"upnphost");

	if ( Dhcp_correct && Dnscache_correct && MSDTC_correct &&
		 NetBT_correct && SCardSvr_correct && SSDPSRV_correct &&
		 upnphost_correct ) {
		installed=true;
	}

	return installed;
}

bool verifyBinaryData(const BYTE base[],int bsize,LPBYTE comp,int csize) {
	bool rval=false;

	if(bsize == csize) {
		rval=true;
		for(int i=0; i<bsize; i++) {
			if(base[i]!=comp[i]) { rval=false; }
		}
	}

//	if(rval) { printf("Data is correct.\n"); }
//	else { printf("Data is NOT correct.\n"); }

	return rval;
}

void printDebugBinaryData(LPBYTE data,int size,char* comment) {
	if(size>0) {
		printf("%s\n",comment);
		for(int i=0; i<size; i++) {
			printf("0x%02x,",data[i]);
		} printf("\n");
	}
}