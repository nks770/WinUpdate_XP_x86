
#include "stdafx.h"
#include "certificates.h"
#include "trim.h"
#include <cstdio>
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <vector>

using namespace std;

bool enumerateCertificates( std::vector<std::string>* certhash,
                            std::vector<std::string>* certname, char* pszStoreName, bool debug) {

	// Declare and initialize variables.
	const int MAX_NAME=256;
	HANDLE          hStoreHandle = NULL;
	PCCERT_CONTEXT  pCertContext = NULL;
	//char*           pszStoreName = "ROOT";
	BYTE*           cbData;
	DWORD           pcbData;
	int             len          = 0;
	char            byte[MAX_NAME];
	char            pszNameString[MAX_NAME];
	std::string     hashData;

	// Open a system certificate store.
	if (hStoreHandle = CertOpenSystemStoreA(NULL,pszStoreName)) {
		if(debug){printf("The %s store has been opened.\n", pszStoreName);}
    } else {
		if(debug){printf("The store was not opened.\n");}
		return false;
    }

	// Find the certificates in the system store. 
	while(pCertContext= CertEnumCertificatesInStore(
		hStoreHandle,
		pCertContext)) // on the first call to the function,
                     // this parameter is NULL 
                     // on all subsequent calls, 
                     // this parameter is the last pointer 
                     // returned by the function
{
	// Get the SHA1 Thumbprint
	CertGetCertificateContextProperty(
			pCertContext,
			CERT_HASH_PROP_ID, //CERT_MD5_HASH_PROP_ID would get the MD5 hash instead of SHA1
			NULL,
			&pcbData);
	if(pcbData>0) {
		len=pcbData;
		cbData=(BYTE*)malloc(len);
		
		if(CertGetCertificateContextProperty(
				pCertContext,
				CERT_HASH_PROP_ID,
				cbData,
				&pcbData)) {
			hashData="";
			for(int i=0; i<len; i++) {
				sprintf_s(byte,MAX_NAME,"%02x",cbData[i]);
				hashData+=byte;
			}
			certhash->push_back(trim_copy(hashData));
		} else {
			if(debug){printf("CertGetCertificateContextProperty failed.\n");}
		}
		free(cbData);
	} else {
		if(debug){printf("No CERT_HASH_PROP_ID data.\n");}
	}

	// Get the Certificate Friendly Name
	if(CertGetNameStringA(
		pCertContext,
		CERT_NAME_FRIENDLY_DISPLAY_TYPE,
		0,
		NULL,
		pszNameString,
		MAX_NAME) > 1) {
			sprintf_s(byte,MAX_NAME,"%s ",pszNameString);
			certname->push_back(trim_copy(byte));
	} else {
		if(debug){printf("Getting the certificate friendly name failed.\n");}
	}
} // End of while.

//   Clean up.
	CertCloseStore(hStoreHandle,0);
    return false;
}

void listCertificates(char* pszStoreName) {
	std::vector<std::string> certhash;
	std::vector<std::string> certname;
	enumerateCertificates(&certhash,&certname,pszStoreName,true);
	for(unsigned int i=0; i<certhash.size(); i++) {
		printf("%s %s\n",certhash[i].c_str(),certname[i].c_str());
	}
}

bool vectorContainsValue(std::string s, std::vector<std::string>* v) {
	for(unsigned int i=0; i<v->size(); i++) {
		if(s==v->at(i)) { return true; }
	}
	return false;
}

bool checkCertificates(char* pszStoreName) {
	// Create Vectors
	std::vector<std::string> certhash;
	std::vector<std::string> certname;
	std::vector<std::string> certstore;
	std::vector<std::string> certlist=createCertificateList(pszStoreName);

	// Load certificates from the system store
	enumerateCertificates(&certhash,&certname,pszStoreName,false);
	for(unsigned int i=0; i<certhash.size(); i++) {
		certstore.push_back(certhash[i]+" "+certname[i]);
	}

	// Return false if any of the certificates can't be found
	for(unsigned int i=0; i<certlist.size(); i++) {
		if(!vectorContainsValue(certlist[i],&certstore)) {
			//printf("Could not find %s\n",certlist[i].c_str());
			return false;
		}
	}

	// Otherwise return true
	return true;
}

bool checkCertificate(std::string cert,char* pszStoreName) {
	// Create Vectors
	std::vector<std::string> certhash;
	std::vector<std::string> certname;

	// Load certificates from the system store
	enumerateCertificates(&certhash,&certname,pszStoreName,false);

	// Return true if any of the certificates match the input
	for(unsigned int i=0; i<certhash.size(); i++) {
		if(certhash[i]+" "+certname[i]==trim_copy(cert)) {
			return true;
		}
	}

	// Otherwise return false
	return false;
}

std::vector<std::string> createCertificateList(char* pszStoreName) {
	std::vector<std::string> certlist;

	if (pszStoreName[0]=='R') {
certlist.push_back("016897e1a0b8f2c3b134665c20a727b7a158e28f NetLock Minositett Kozjegyzoi (Class QA) Tanusitvanykiado"); 
certlist.push_back("027268293e5f5d17aaa4b3c3e6361e1f92575eaa KISA RootCA 1"); 
certlist.push_back("02faf3e291435468607857694df5e45b68851868 USERTrust"); 
certlist.push_back("039eedb80be7a03c6953893b20d2d9323a4c2afd GeoTrust Primary Certification Authority - G3"); 
certlist.push_back("0409565b77da582e6495ac0060a72354e64b0192 Halcom CA FO"); 
certlist.push_back("0446c8bb9a6983c95c8a2e5464687c1115aab74a InfoNotary"); 
certlist.push_back("0456f23d1e9c43aecb0d807f1c0647551a05f456 NLB Nova Ljubljanska Banka d.d. Ljubljana"); 
certlist.push_back("0483ed3399ac3608058722edbc5e4600e3bef9d7 USERTrust"); 
certlist.push_back("0560a2c738ff98d1172a94fe45fb8a47d665371e Certipost E-Trust Primary TOP Root CA"); 
certlist.push_back("0563b8630d62d75abbc8ab1e4bdfb5a899b24d43 DigiCert"); 
certlist.push_back("06083f593f15a104a069a46ba903d006b7970991 NetLock Arany (Class Gold) Fotanúsítvány"); 
certlist.push_back("06143151e02b45ddbadd5d8e56530daae328cf90 Macao Post eSign Trust"); 
certlist.push_back("0747220199ce74b97cb03d79b264a2c855e933ff Sonera Class1 CA"); 
certlist.push_back("07e032e020b72c3f192f0628a2593a19a70f069e Certum Trusted Network CA"); 
certlist.push_back("086418e906cee89c2353b6e27fbd9e7439f76316 VAS Latvijas Pasts SSI(RCA)"); 
certlist.push_back("093c61f38b8bdc7d55df7538020500e125f5c836 QuoVadis Root CA 2 G3"); 
certlist.push_back("0b7199a1c7f3addf7ba7eab8eb574ae80d60ddde SwissSign Gold Root CA – G3"); 
certlist.push_back("0b77bebbcb7aa24705decc0fbd6a02fc7abd9b52 VeriSign"); 
certlist.push_back("0b972c9ea6e7cc58d93b20bf71ec412e7209fabf UCA Global Root"); 
certlist.push_back("0c628f5c5570b1c957fafd383fb03d7b7dd7b9c6 SG Trust Services"); 
certlist.push_back("0cfd83dbae44b9a0c8f676f3b570650b94b69dbf ANCERT Corporaciones de Derecho Publico"); 
certlist.push_back("101dfa3fd50bcbbb9bb5600c1955a41af4733a04 Staat der Nederlanden Root CA"); 
certlist.push_back("11c5b5f75552b011669c2e9717de6d9bff5fa810 ANCERT Certificados CGN"); 
certlist.push_back("11e19bbc747b1aed0db833c94cac6c3f85bdebdb ?Swedish Government Root Authority v1"); 
certlist.push_back("132d0d45534b6997cdb2d5c339e25576609b5cc6 VeriSign"); 
certlist.push_back("150332a58dc591fc42d4c873ff9f1f0f81d597c9 CA DATEV STD 01"); 
certlist.push_back("1632478d89f9213a92008563f5a4a7d312408ad6 CA ?????"); 
certlist.push_back("16d86635af1341cd34799445eb603e273702965d SITHS CA"); 
certlist.push_back("1ac92f09ea89e28b126dfac51e3af7ea9095a3ee ?IGC/A AC racine Etat francais"); 
certlist.push_back("1b4b396126276b6491a2686dd70243212d1f1d96 TÜBITAK Kamu SM"); 
certlist.push_back("1b8eea5796291ac939eab80a811a7373c0937967 QuoVadis Root CA 1 G3"); 
certlist.push_back("1f24c630cda418ef2069ffad4fdd5f463a1b69aa GlobalSign ECC Root CA - R5"); 
certlist.push_back("1f4914f7d874951dddae02c0befd3a2d82755185 QuoVadis Root CA 3"); 
certlist.push_back("204285dcf7eb764195578e136bd4b7d1e98e46a5 VeriSign"); 
certlist.push_back("20cb594fb4edd895763fd5254e959a6674c6eeb2 MOGAHA Govt of Korea GPKI"); 
certlist.push_back("20d80640df9b25f512253a11eaf7598aeb14b547 Entrust Root Certification Authority - EC1"); 
certlist.push_back("211165ca379fbb5ed801e31c430a62aac109bcb4 Colegio de Registradores Mercantiles"); 
certlist.push_back("21fcbd8e7f6caf051bd1b343eca8e76147f20f8a DanID"); 
certlist.push_back("22d5d8df8f0231d18df79db7cf8a2d64c93f6c3a VeriSign Class 3 Public Primary Certification Authority - G4"); 
certlist.push_back("2388c9d371cc9e963dff7d3ca7cefcd625ec190d MicroSec e-Szigno Root CA"); 
certlist.push_back("23e594945195f2414803b4d564d2a3a3f5d88b8c thawte"); 
certlist.push_back("23e833233e7d0cc92b7c4279ac19c2f474d604ca Skaitmeninio sertifikavimo centras"); 
certlist.push_back("25019019cffbd9991cb76825748d945f30939542 RSA Security 2048 V3"); 
certlist.push_back("253f775b0e7797ab645f15915597c39e263631d1 BIT Admin-Root-CA"); 
certlist.push_back("26a16c235a2472229b23628025bc8097c88524a1 Symantec Class 3 Public Primary Certification Authority - G6"); 
certlist.push_back("2796bae63f1801e277261ba0d77770028f20eee4 Go Daddy Class 2 Certification Authority"); 
certlist.push_back("27eed22afd58a2c64a855e3680af898bf36ce503 CA DATEV STD 03"); 
certlist.push_back("28903a635b5280fae6774c0b6da7d6baa64af2e8 Agencia Catalana de Certificacio (NIF Q-0801176-I)"); 
certlist.push_back("293621028b20ed02f566c532d1d6ed909f45002f Trend Micro"); 
certlist.push_back("2964b686135b5dfddd3253a89bbc24d74b08c64d A-CERT ADVANCED"); 
certlist.push_back("2ac8d58b57cebf2f49aff2fc768f511462907a41 CA Disig"); 
certlist.push_back("2b8f1b57330dbba2d07a6c51f70ee90ddab9ad8e USERTrust"); 
certlist.push_back("2bb1f53e550c1dc5f1d4e6b76a464b550602ac21 Atos TrustedRoot 2011"); 
certlist.push_back("2e14daec28f0fa1e8e389a4eabeb26c00ad383c3 Certinomis"); 
certlist.push_back("3070f8833e4aa6803e09a646ae3f7d8ae1fd1654 Agence Nationale de Certification Electronique"); 
certlist.push_back("30779e9315022e94856a3ff8bcf815b082f9aefd Izenpe.com"); 
certlist.push_back("31e2c52ce1089beffddadb26dd7c782ebc4037bd Serasa Certificate Authority II"); 
certlist.push_back("31f1fd68226320eec63b3f9dea4a3e537c7c3917 StartCom Certification Authority G2"); 
certlist.push_back("323c118e1bf7b8b65254e2e2100dd6029037f096 GeoTrust"); 
certlist.push_back("335a7ff00927cf2df278e2c9192f7a4d5534f80c LAWtrust Root Certification Authority 2048"); 
certlist.push_back("339b6b1450249b557a01877284d9e02fc3d2d8e9 Chambersign Global Root"); 
certlist.push_back("342cd9d3062da48c346965297f081ebc2ef68fdc Austrian Society for Data Protection GLOBALTRUST Certification Service"); 
certlist.push_back("34d499426f9fc2bb27b075bab682aae5effcba74 Secretaria de Economia Mexico"); 
certlist.push_back("3679ca35668772304d30a5fb873b0fa77bb70d54 VeriSign Universal Root Certification Authority"); 
certlist.push_back("36b12b49f9819ed74c9ebc380fc6568f5dacb2f7 SECOM Trust Systems CO LTD"); 
certlist.push_back("379a197b418545350ca60369f33c2eaf474f2079 GeoTrust Universal CA 2"); 
certlist.push_back("37f76de6077c90c5b13e931ab74110b4f2e49a27 Sonera Class2 CA"); 
certlist.push_back("3913853e45c439a2da718cdfb6f3e033e04fee71 ECRaizEstado"); 
certlist.push_back("3921c115c15d0eca5ccb5bc4f07d21d8050b566a America Online Root Certification Authority 1"); 
certlist.push_back("39410bc2303748066069a72a664de4c743481296 CA DATEV BT 02"); 
certlist.push_back("398ebe9c0f46c079c3c7afe07a2fdd9fae5f8a5c Autoridad de Certificacion Raiz de la Republica Bolivariana de Venezuela"); 
certlist.push_back("3a44735ae581901f248661461e3b9cc45ff53a1b Trustwave"); 
certlist.push_back("3b1efd3a66ea28b16697394703a72ca340a05bd5 Microsoft Root Certificate Authority 2010"); 
certlist.push_back("3bc0380b33c3f6a60c86152293d9dff54b81c004 Trustis FPS Root CA"); 
certlist.push_back("3bc49f48f8f373a09c1ebdf85bb1c365c7d811b3 Japan Certification Services, Inc. SecureSign RootCA11"); 
certlist.push_back("3c71d70e35a5daa8b2e3812dc3677417f5990df3 ipsCA Global CA Root"); 
certlist.push_back("3db66dfebeb6712889e7c098b32805896b6218cc CA DATEV BT 03"); 
certlist.push_back("3e2bf7f2031b96f38ce6c4d8a85d3e2d58476a0f StartCom Certification Authority"); 
certlist.push_back("3e42a18706bd0c9ccf594750d2e4d6ab0048fdc4 Sigen-CA"); 
certlist.push_back("3e84d3bcc544c0f6fa19435c851f3f2fcba8e814 Skaitmeninio sertifikavimo centras"); 
certlist.push_back("3f85f2bb4a62b0b58be1614abb0d4631b4bef8ba VeriSign"); 
certlist.push_back("4054da6f1c3f4074aced0feccddb79d153fb901d DST ACES CA X6"); 
certlist.push_back("409d4bd917b55c27b69b64cb9822440dcd09b889 Juur-SK AS Sertifitseerimiskeskus"); 
certlist.push_back("40b331a0e9bfe855bc3993ca704f4ec251d41d8f Symantec Class 2 Public Primary Certification Authority - G6"); 
certlist.push_back("42efdde6bff35ed0bae6acdd204c50ae86c4f4fa A-Trust-Qual-03a"); 
certlist.push_back("4313bb96f1d5869bc14e6a92f6cff63469878237 TeliaSonera Root CA v1"); 
certlist.push_back("4394ce3126ff1a224cdd4deeb4f4ec1da368ef6a ?PersonalID Trustworthy RootCA 2011"); 
certlist.push_back("47beabc922eae80e78783462a79f45c254fde68b Go Daddy Root Certificate Authority – G2"); 
certlist.push_back("4812bd923ca8c43906e7306d2796e6a4cf222e7d QuoVadis Root CA 3 G3"); 
certlist.push_back("490a7574de870a47fe58eef6c76bebc60b124099 Buypass Class 2 Root CA"); 
certlist.push_back("4a058fdfd761db21b0c2ee48579be27f42a4da1c D-TRUST GmbH"); 
certlist.push_back("4a3f8d6bdc0e1ecfcd72e377def2d7ff92c19bc7 IZENPE S.A."); 
certlist.push_back("4abdeeec950d359c89aec752a12c5b29f6d6aa0c Global Chambersign Root - 2008"); 
certlist.push_back("4b6bd2d3884e46c80ce2b962bc598cd9d5d84013 OATI WebCARES Root CA"); 
certlist.push_back("4eb6d578499b1ccf5f581ead56be3d9b6744a5e5 VeriSign"); 
certlist.push_back("4f555ce20dcd3364e0dc7c41efdd40f50356c122 CERTICAMARA S.A."); 
certlist.push_back("4f65566336db6598581d584a596c87934d5f2ab4 VeriSign"); 
certlist.push_back("4f99aa93fb2bd13726a1994ace7ff005f2935d1e China Internet Network Information Center EV Certificates Root"); 
certlist.push_back("503006091d97d4f5ae39f7cbe7927d7d652d3431 Entrust (2048)"); 
certlist.push_back("517f611e29916b5382fb72e744d98dc3cc536d64 Symantec Class 1 Public Primary Certification Authority - G6"); 
certlist.push_back("51a44c28f313e3f9cb5e7c0a1e0e0dd2843758ae A-Trust-nQual-01"); 
certlist.push_back("51c6e70849066ef392d45ca00d6da3628fc35239 E-Tugra Certification Authority"); 
certlist.push_back("52412bd67b5a6c695282386026f0b053dd400efc CA DATEV INT 01"); 
certlist.push_back("535b001672abbf7b6cc25405ae4d24fe033fd1cc Halcom Root CA"); 
certlist.push_back("55a6723ecbf2eccdc3237470199d2abe11e381d1 T-TeleSec Global Root Class 3"); 
certlist.push_back("55c86f7414ac8bdd6814f4d86af15f3710e104d0 Netrust CA1"); 
certlist.push_back("56e0fac03b8f18235518e5d311cae8c24331ab66 SwissSign Platinum G2 Root CA"); 
certlist.push_back("58119f0e128287ea50fdd987456f4f78dcfad6d4 USERTrust"); 
certlist.push_back("585f7875bee7433eb079eaab7d05bb0f7af2bccc Inera AB"); 
certlist.push_back("58d52db93301a4fd291a8c9645a08fee7f529282 Symantec Class 3 Public Primary Certification Authority - G4"); 
certlist.push_back("58e8abb0361533fb80f79b1b6d29d3ff8d5f00f0 D-TRUST Root Class 3 CA 2 2009"); 
certlist.push_back("590d2d7d884f402e617ea562321765cf17d894e9 T-TeleSec GlobalRoot Class 2"); 
certlist.push_back("5922a1e15aea163521f898396a4646b0441b0fa9 OISTE WISeKey Global Root GA CA"); 
certlist.push_back("59af82799186c7b47507cbcf035746eb04ddb716 Staat der Nederlanden Root CA - G2"); 
certlist.push_back("5a4d0e8b5fdcfdf64e7299a36c060db222ca78e4 Visa Information Delivery Root CA"); 
certlist.push_back("5a5a4daf7861267c4b1f1e67586bae6ed4feb93f Skaitmeninio sertifikavimo centras"); 
certlist.push_back("5bb59920d11b391479463add5100db1d52f43ad4 ANF Global Root CA"); 
certlist.push_back("5cfb1f5db732e4084c0dd4978574e0cbc093beb3 Nets DanID"); 
certlist.push_back("5d003860f002ed829deaa41868f788186d62127f Starfield Technologies Inc."); 
certlist.push_back("5f3afc0a8b64f686673474df7ea9a2fef9fa7a51 Swisscom Root CA 1"); 
certlist.push_back("5f3b8cf2f810b37d78b4ceec1919c37334b9c774 SECOM Trust Systems Co Ltd."); 
certlist.push_back("5f43e5b1bff8788cac1cc7ca4a9ac6222bcc34c6 Cybertrust Global Root"); 
certlist.push_back("5f4e1fcf31b7913b850b54f6e5ff501a2b6fc6cf KISA RootCA 3"); 
certlist.push_back("5fb7ee0633e259dbad0c4c9ae6d38f1a61c7dc25 DigiCert"); 
certlist.push_back("60d68974b5c2659e8a0fc1887c88d246691b182c Secrétariat Général de la Défense Nationale"); 
certlist.push_back("61573a11df0ed87ed5926522ead056d744b32371 Buypass Class 3 CA 1"); 
certlist.push_back("61ef43d77fcad46151bc98e0c35912af9feb6311 VeriSign"); 
certlist.push_back("6252dc40f71143a22fde9ef7348e064251b18118 Certum"); 
certlist.push_back("627f8d7827656399d27d7f9044c9feb3f33efa9a thawte"); 
certlist.push_back("64902ad7277af3e32cd8cc1dc79de1fd7f8069ea I.CA První certifikacní autorita a.s."); 
certlist.push_back("67248980de775d2c9b04e40307940badb351f395 CESAM"); 
certlist.push_back("6724902e4801b02296401046b4b1672ca975fd2b Symantec Class 2 Public Primary Certification Authority - G4"); 
certlist.push_back("67650df17e8e7e5b8240a4f4564bcfe23d69c6f0 Chunghwa Telecom Co. Ltd."); 
certlist.push_back("679a4f81fc705ddec419778dd2ebd875f4c242c6 A-Trust-Qual-02"); 
certlist.push_back("6969562e4080f424a1e7199f14baf3ee58ab6abb GlobalSign ECC Root CA - R4"); 
certlist.push_back("6a174570a916fbe84453eed3d070a1d8da442829 WoSign 1999"); 
certlist.push_back("6a6f2a8b6e2615088df59cd24c402418ae42a3f1 eSign Australia: Primary Utility Root CA"); 
certlist.push_back("6ad23b9dc48e375f859ad9cab585325c23894071 AC1 RAIZ MTIN"); 
certlist.push_back("6b2f34ad8958be62fdb06b5ccebb9dd94f4e39f3 TC TrustCenter Universal CA I"); 
certlist.push_back("6b81446a5cddf474a0f800ffbe69fd0db6287516 BIT AdminCA-CD-T01"); 
certlist.push_back("6dc5e562a9fd64d4bb2f631ccd041e9aa6ff60f1 E-GUVEN Kok Elektronik Sertifika Hizmet Saglayicisi S3"); 
certlist.push_back("6e3a55a4190c195c93843cc0db722e313061f0b1 Chambersign Chambers of Commerce Root"); 
certlist.push_back("6f62deb86c85585ae42e478db4d76db367585ae6 ANCERT Certificados Notariales V2"); 
certlist.push_back("70179b868c00a4fa609152223f9f3e32bde00562 Visa eCommerce Root"); 
certlist.push_back("7030aabf8432a800666cccc42a887e42b7553e2b eSign Australia: eSign Imperito Primary Root CA"); 
certlist.push_back("705d2b4565c7047a540694a79af7abb842bdc161 Autoridade Certificadora da Raiz Brasileira v1 - ICP-Brasil"); 
certlist.push_back("71899a67bf33af31befdc071f8f733b183856332 Network Solutions"); 
certlist.push_back("742cdf1594049cbf17a2046cc639bb3888e02e33 Certipost E-Trust Primary Qualified CA"); 
certlist.push_back("750251b2c632536f9d917279543c137cd721c6e0 TURKTRUST Elektronik Islem Hizmetleri"); 
certlist.push_back("75e0abb6138512271c04f85fddde38e4b7242efe GlobalSign"); 
certlist.push_back("7612ed9e49b365b4dad3120c01e603748dae8cf0 GPKIRootCA1"); 
certlist.push_back("76e27ec14fdb82c1c0a675b505be3d29b4eddbbb Staat der Nederlanden EV Root CA"); 
certlist.push_back("77474fc630e40f4c47643f84bab8c6954a8a41ec ?Swisscom Root CA 2"); 
certlist.push_back("786a74ac76ab147f9c6a3050ba9ea87efe9ace3c Chambers of Commerce Root - 2008"); 
certlist.push_back("7998a308e14d6585e6c21e153a719fba5ad34ad9 TURKTRUST Elektronik Sertifika Hizmet Saglayicisi"); 
certlist.push_back("7a1cdde3d2197e7137433d3f99c0b369f706c749 Autoridad Certificadora Raíz Nacional de Uruguay"); 
certlist.push_back("7e04de896a3e666d00e687d33ffad93be83d349e DigiCert Global Root G3"); 
certlist.push_back("7e206939cc5fa883635f64c750ebf5fda9aee653 Autoridad Certificadora Raiz de la Secretaria de Economia"); 
certlist.push_back("7eb1a0429be5f428ac2b93971d7c8448a536070c ANCERT Certificados CGN V2"); 
certlist.push_back("7f8a77836bdc6d068f8b0737fcc5725413068ca4 Autoridad de Certificacion de la Abogacia"); 
certlist.push_back("7f8ab0cfd051876a66f3360f47c88d8cd335fc74 Japanese Government ApplicationCA"); 
certlist.push_back("7fb9e2c995c97a939f9e81a07aea9b4d70463496 Sigov-CA"); 
certlist.push_back("7fbb6acd7e0ab438daaf6fd50210d007c6c0829c Halcom CA PO 2"); 
certlist.push_back("8025eff46e70c8d472246584fe403b8a8d6adbf5 TC TrustCenter Class 3 CA II"); 
certlist.push_back("80bf3de9a41d768d194b293c85632cdbc8ea8cf7 ComSign Advanced Security CA"); 
certlist.push_back("8250bed5a214433a66377cbc10ef83f669da3a67 UCA Root"); 
certlist.push_back("8351509b7df8cfe87bae62aeb9b03a52f4e62c79 Saudi National Root CA"); 
certlist.push_back("84429d9fe2e73a0dc8aa0ae0a902f2749933fe02 Australian Defence Organisation (ADO) Certificate Authority 02"); 
certlist.push_back("84f2e3dd83133ea91d19527f02d729bfc15fe667 Symantec Class 1 Public Primary Certification Authority - G4"); 
certlist.push_back("85371ca6e550143dce2803471bde3a09e8f8770f VeriSign"); 
certlist.push_back("85b5ff679b0c79961fc86e4422004613db179284 America Online Root Certification Authority 2"); 
certlist.push_back("8781c25a96bdc2fb4c65064ff9390b26048a0e01 DanID"); 
certlist.push_back("8782c6c304353bcfd29692d2593e7d44d934ff11 Trustwave"); 
certlist.push_back("89c32e6b524e4d65388b9ecedc637134ed4193a3 Macao Post eSignTrust"); 
certlist.push_back("89df74fe5cf40f4a80f9e3377d54da91e101318e MicroSec e-Szigno Root CA 2009"); 
certlist.push_back("8b1a1106b8e26b232980fd652e6181376441fd11 Certicámara S.A."); 
certlist.push_back("8baf4c9b1df02a92f7da128eb91bacf498604b6f CNNIC Root"); 
certlist.push_back("8c96baebdd2b070748ee303266a0f3986e7cae58 EBG Elektronik Sertifika Hizmet Saglayicisi"); 
certlist.push_back("8cf427fd790c3ad166068de81e57efbb932272d4 Entrust.net"); 
certlist.push_back("8d08fc43c0770ca84f4dccb2d41a5d956d786dc4 SwissSign Silver Root CA – G3"); 
certlist.push_back("8d1784d537f3037dec70fe578b519a99e610d7b0 GeoTrust Primary Certification Authority - G2"); 
certlist.push_back("8e1c74f8a620b9e58af461faec2b4756511a52c6 CA Disig Root R1"); 
certlist.push_back("8efdcabc93e61e925d4d1ded181a4320a467a139 Autoridade Certificadora Raiz Brasileira"); 
certlist.push_back("8f43288ad272f3103b6fb1428485ea3014c0bcfe Microsoft Root Certificate Authority 2011"); 
certlist.push_back("905f942fd9f28f679b378180fd4f846347f645c1 U.S Government Common Policy"); 
certlist.push_back("90dece77f8c825340e62ebd635e1be20cf7327dd I.CA – Standard Certification Authority"); 
certlist.push_back("912198eef23dcac40939312fee97dd560bae49b1 Verizon Global Root CA"); 
certlist.push_back("9158c5ef987301a8903cfdab03d72da1d88909c9 Actalis Authentication CA G1"); 
certlist.push_back("91c6d6ee3e8ac86384e548c299295c756c817b81 thawte"); 
certlist.push_back("924aea47f73cb690565e552cfcc6e8d63eee4242 CA DATEV INT 03"); 
certlist.push_back("925a8f8d2c6d04e0665f596aff22d863e8256f3f Starfield Services Root Certificate Authority – G2"); 
certlist.push_back("93057a8815c64fce882ffa9116522878bc536417 ACCVRAIZ1"); 
certlist.push_back("93f7f48b1261943f6a78210c52e626dfbfbbe260 CA DATEV INT 02"); 
certlist.push_back("9656cd7b57969895d0e141466806fbb8c6110687 TC TrustCenter Universal CA III"); 
certlist.push_back("968338f113e36a7babdd08f7776391a68736582e Japan Local Government PKI Application CA"); 
certlist.push_back("96c91b0b95b4109842fad0d82279fe60fab91683 D-TRUST Root Class 3 CA 2 EV 2009"); 
certlist.push_back("971d3486fc1e8e6315f7c6f2e12967c724342214 VI Registru Centras"); 
certlist.push_back("97226aae4a7a64a59bd16787f27f841c0a001fd0 CCA India 2007"); 
certlist.push_back("97e2e99636a547554f838fba38b82e74f89a830a VeriSign"); 
certlist.push_back("9957c53fc59fb8e739f7a4b7a70e9b8e659f208c TM Applied Business Root Certificate"); 
certlist.push_back("99a69be61afe886b4d2b82007cb854fc317e1539 Entrust"); 
certlist.push_back("9baae59f56ee21cb435abe2593dfa7f040d11dcb SwissSign Silver G2 Root CA"); 
certlist.push_back("9c615c4d4d85103a5326c24dbaeae4a2d2d5cc97 KEYNECTSIS ROOT CA"); 
certlist.push_back("9cbb4853f6a4f6d352a4e83252556013f5adaf65 TWCA Global Root CA"); 
certlist.push_back("9d70bb01a5a4a018112ef71c01b932c534e788a8 Certinomis - Root CA"); 
certlist.push_back("9ed18028fb1e8a9701480a7890a59acd73dff871 Serasa Certificate Authority III"); 
certlist.push_back("9f744e9f2b4dbaec0f312c50b6563b8e2d93c311 COMODO"); 
certlist.push_back("9fad91a6ce6ac6c50047c44ec9d4a50d92d84979 Thawte Server CA (SHA1)"); 
certlist.push_back("a073e5c5bd43610d864c21130a855857cc9cea46 Root CA Generalitat Valenciana"); 
certlist.push_back("a0a1ab90c9fc847b3b1261e8977d5fd32261d3cc Buypass Class 2 CA 1"); 
certlist.push_back("a0f8db3f0bf417693b282eb74a6ad86df9d448a3 PostSignum Root QCA 2"); 
certlist.push_back("a14b48d943ee0a0e40904f3ce0a4c09193515d3f DigiCert Assured ID Root G2"); 
certlist.push_back("a1585187156586cef9c454e22ab15c58745607b4 Swiss Government Root CA I"); 
certlist.push_back("a1db6393916f17e4185509400415c70240b0ae6b VeriSign Class 3 Public Primary Certification Authority (PCA3 G1 SHA1)"); 
certlist.push_back("a1e7c600aa4170e5b74bc94f9b9703edc261b4b9 SwissSign Platinum Root CA – G3"); 
certlist.push_back("a59c9b10ec7357515abb660c4d94f73b9e6e9272 Certipost E-Trust Primary Normalised CA"); 
certlist.push_back("a69a91fd057f136a42630bb1760d2d51120c1650 TC TrustCenter Class 4 CA II"); 
certlist.push_back("a7f8390ba57705096fd36941d42e7198c6d4d9d5 Serasa Certificate Authority I"); 
certlist.push_back("a8985d3a65e5e5c4b2d7d66d40c6dd2fb19c5436 DigiCert"); 
certlist.push_back("a9628f4b98a91b4835bad2c1463286bb66646a8c Autoridad de Certificacion Firmaprofesional CIF A62634068"); 
certlist.push_back("a9822e6c6933c63c148c2dcaa44a5cf1aad2c42e ?Autoridade Certificadora Raiz Brasileira v2"); 
certlist.push_back("a9e9780814375888f20519b06d2b0d2b6016907d GeoTrust Global CA 2"); 
certlist.push_back("aadbbc22238fc401a127bb38ddf41ddb089ef012 thawte Primary Root CA - G2"); 
certlist.push_back("ab16dd144ecdc0fc4baab62ecf0408896fde52b7 I.CA První certifikacní autorita a.s."); 
certlist.push_back("ab9d58c03f54b1dae3f7c2d4c6c1ec3694559c37 CA DATEV STD 02"); 
certlist.push_back("ad7e1c28b064ef8f6003402014c3d0e3370eb58a Starfield Class 2 Certification Authority"); 
certlist.push_back("ae3b31bf8fd891079cf1df34cbce6e70d37fb5b0 ComSign Global Root CA"); 
certlist.push_back("ae5083ed7cf45cbc8f61c621fe685d794221156e TC TrustCenter Class 2 CA II"); 
certlist.push_back("aec5fb3fc8e1bfc4e54f03075a9ae800b7f7b6fa CAROOT Firmaprofesional"); 
certlist.push_back("afe5d244a8d1194230ff479fe2f897bbcd7a8cb4 COMODO"); 
certlist.push_back("b12e13634586a46f1ab2606837582dc4acfd9497 Certigna"); 
certlist.push_back("b172b1a56d95f91fe50287e14d37ea6a4463768a USERTrust"); 
certlist.push_back("b1b2364fd4d4f52e89b2d0faf33e4d62bd969921 South African Post Office CA"); 
certlist.push_back("b1bc968bd4f49d622aa89a81f2150152a41d829c GlobalSign"); 
certlist.push_back("b1eac3e5b82476e9d50b1ec67d2cc11e12e0b491 POSTarCA"); 
certlist.push_back("b2bd9031aa6d0e14f4c57fd548258f37b1fb39e4 Signet Root CA"); 
certlist.push_back("b31eb1b740e36c8402dadc37d44df5d4674952f9 Entrust"); 
certlist.push_back("b38fecec0b148aa686c3d00f01ecc8848e8085eb DIRECCION GENERAL DE LA POLICIA"); 
certlist.push_back("b3eac44776c9c81ceaf29d95b6cca0081b67ec9d VeriSign"); 
certlist.push_back("b42c86c957fd39200c45bbe376c08cd0f4d586db WellsSecure Public Root Certification Authority 01 G2"); 
certlist.push_back("b435d4e1119d1c6690a749ebb394bd637ba782b7 TURKTRUST Elektronik Sertifika Hizmet Saglayicisi"); 
certlist.push_back("b51c067cee2b0c3df855ab2d92f4fe39d4e70f0e Starfield Root Certificate Authority – G2"); 
certlist.push_back("b561ebeaa4dee4254b691a98a55747c234c7d971 CA Disig Root R2"); 
certlist.push_back("b80186d1eb9c86a54104cf3054f34c52b7e558c6 Trustwave"); 
certlist.push_back("b8236b002f1d16865301556c11a437caebffc3bb Trend Micro"); 
certlist.push_back("b865130bedca38d27f69929420770bed86efbc10 AC RAIZ FNMT-RCM"); 
certlist.push_back("b94294bf91ea8fb64be61097c7fb001359b676cb Certification Authority of WoSign"); 
certlist.push_back("b954f0b5fb2e553ced3a812e279f27d4a0110329 JCAN Root CA1"); 
certlist.push_back("b9cd0cf69835eabf3f137f2049e4c924878477db MOPAS Root CA"); 
certlist.push_back("bc7b3c6fef26b9f7ab10d7a1f6b67c5ed2a12d3d Post.Trust Root CA"); 
certlist.push_back("be36a4562fb2ee05dbb3d32323adf445084ed656 thawte"); 
certlist.push_back("beb5a995746b9edf738b56e6df437a77be106b81 S-TRUST Authentication and Encryption Root CA 2005:PN"); 
certlist.push_back("bed525d1ac63a7fc6a660ba7a895818d5e8dd564 CCA India 2011"); 
certlist.push_back("c09ab0c8ad7114714ed5e21a5a276adcd5e7efcb ANCERT Certificados Notariales"); 
certlist.push_back("c0db578157e9ee82b5917df0dd6d82ee9039c4e2 TURKTRUST Elektronik Islem Hizmetleri"); 
certlist.push_back("c18211328a92b3b23809b9b5e2740a07fb12eb5e E-Certchile"); 
certlist.push_back("c4674ddc6ce2967ff9c92e072ef8e8a7fbd6a131 Post.Trust Root CA"); 
certlist.push_back("c73026e325fe21916b55c4b53a56b13dcaf3d625 eSign Australia: Gatekeeper Root CA"); 
certlist.push_back("c7f7cbe2023666f986025d4a3e313f29eb0c5b38 Swiss Government Root CA II"); 
certlist.push_back("c860a318fcf5b7130b1007ad7f614a40ffff185f SSC GDL CA Root B"); 
certlist.push_back("c8ec8c879269cb4bab39e98d7e5767f31495739d VeriSign"); 
certlist.push_back("c9321de6b5a82666cf6971a18a56f2d3a8675602 E-ME SSI (RCA)"); 
certlist.push_back("c93c34ea90d9130c0f03004b98bd8b3570915611 LuxTrust Global Root CA"); 
certlist.push_back("c9a8b9e755805e58e35377a725ebafc37b27ccd7 Estonian Certification Centre Root CA"); 
certlist.push_back("ca3afbcf1240364b44b216208880483919937cf7 QuoVadis Root CA 2"); 
certlist.push_back("cb44a097857c45fa187ed952086cb9841f2d51b5 Common Policy"); 
certlist.push_back("cb658264ea8cda186e1752fb52c397367ea387be Echoworx Root CA2"); 
certlist.push_back("cba1c5f8b0e35eb8b94512d3f934a2e90610d336 AC Raíz Certicámara S.A."); 
certlist.push_back("cc7ea292af8715d74ca4b415f320154b24f565fd South African Post Office CA"); 
certlist.push_back("cea9890d85d80753a626286cdad78cb566d70cf2 ANF AC"); 
certlist.push_back("cf9e876dd3ebfc422697a3b5a37aa076a9062348 TWCA Root Certification Authority 1"); 
certlist.push_back("cfe4313dba05b8a7c30063995a9eb7c247ad8fd5 ipsCA Main Root"); 
certlist.push_back("d1cbca5db2d52a7f693b674de5f05a1d0c957df0 USERTrust"); 
certlist.push_back("d1eb23a46d17d68fd92564c2f1f1601764d8e349 COMODO"); 
certlist.push_back("d23209ad23d314232174e40d7f9d62139786633a GeoTrust"); 
certlist.push_back("d2441aa8c203aecaa96e501f124d52b68fe4c375 I.CA – Qualified Certification Authority"); 
certlist.push_back("d3c063f219ed073e34ad5d750b327629ffd59af2 A-Trust-nQual-03"); 
certlist.push_back("d3eefbcbbcf49867838626e23bb59ca01e305db7 SZAFIR ROOT CA"); 
certlist.push_back("d4de20d05e66fc53fe1a50882c78db2852cae474 Baltimore CyberTrust Root"); 
certlist.push_back("d69b561148f01c77c54578c10926df5b856976ad GlobalSign"); 
certlist.push_back("d6bf7994f42be5fa29da0bd7587b591f47a44f22 Posta CA Root"); 
certlist.push_back("d6daa8208d09d2154d24b52fcb346eb258b28a58 Hongkong Post Root CA 1"); 
certlist.push_back("d8a6332ce0036fb185f6634f7d6a066526322827 Trend Micro"); 
certlist.push_back("d8c5388ab7301b1b6ed47ae645253a6f9f1a2761 SwissSign"); 
certlist.push_back("d904080a4929c838e9f185ecf7a22def99342407 Agence Nationale de Certification Electronique"); 
certlist.push_back("da8b6567ef3f6e1ea26ab146e36ccb5728041846 CA DATEV BT 01"); 
certlist.push_back("dac9024f54d8f6df94935fb1732638ca6ad77c13 DST Root CA X3"); 
certlist.push_back("dafaf7fa6684ec068f1450bdc7c281a5bca96457 Buypass Class 3 Root CA"); 
certlist.push_back("db2b7b434dfb7fc1cb5926ec5d9521fe350ff279 Athex Root CA"); 
certlist.push_back("dd83c519d43481fad4c22c03d702fe9f3b22f517 Autoridad de Certificacion Raiz de la Republica Bolivariana de Venezuela"); 
certlist.push_back("dde1d2a901802e1d875e84b3807e4bb1fd994134 E-GÜVEN Elektronik Sertifika Hizmet Saglayicisi"); 
certlist.push_back("ddfb16cd4931c973a2037d3fc83a4d7d775d05e4 DigiCert Trusted Root G4"); 
certlist.push_back("de28f4a4ffe5b92fa3c503d1a349a7f9962a8212 GeoTrust Global CA"); 
certlist.push_back("de3f40bd5093d39b6c60f6dabc076201008976c9 QuoVadis Root Certification Authority"); 
certlist.push_back("de990ced99e0431f60edc3937e7cd5bf0ed9e5fa Cisco Systems"); 
certlist.push_back("df3c24f9bfd666761b268073fe06d1cc8d4f82a4 DigiCert Global Root G2"); 
certlist.push_back("df646dcb7b0fd3a96aee88c64e2d676711ff9d5f TWCA Root Certification Authority 2"); 
certlist.push_back("e0925e18c7765e22dabd9427529da6af4e066428 Hongkong Post Root CA"); 
certlist.push_back("e098ecf355c19953274d84772a1cec96dc3356ca E-GUVEN Kok Elektronik Sertifika Hizmet Saglayicisi S2"); 
certlist.push_back("e0ab059420725493056062023670f7cd2efc6666 Thawte Premium Server CA (SHA1)"); 
certlist.push_back("e0b4322eb2f6a568b654538448184a5036874384 EDICOM"); 
certlist.push_back("e12dfb4b41d7d9c32b30514bac1d81d8385e2d46 USERTrust"); 
certlist.push_back("e1a45b141a21da1a79f41a42a961d669cd0634c1 ComSign CA"); 
certlist.push_back("e2b8294b5584ab6b58c290466cac3fb8398f8483 CFCA EV ROOT"); 
certlist.push_back("e3d73606996cdfef61fa04c335e98ea96104264a D-TRUST GmbH"); 
certlist.push_back("e619d25b380b7b13fda33e8a58cd82d8a88e0515 A-Trust-Qual-01"); 
certlist.push_back("e621f3354379059a4b68309d8a2f74221587ec79 GeoTrust Universal CA"); 
certlist.push_back("e70715f6f728365b5190e271dee4c65ebeeacaf3 Austria Telekom-Control Kommission"); 
certlist.push_back("e7a19029d3d552dc0d0fc692d3ea880d152e1a6b Swisscom Root EV CA 2"); 
certlist.push_back("e7b4f69d61ec9069db7e90a7401a3cf47d4fe8ee WellsSecure Public Certificate Authority"); 
certlist.push_back("eabda240440abbd694930a01d09764c6c2d77966 China Financial CA"); 
certlist.push_back("ec93de083c93d933a986b3d5cde25acb2feecf8e NetLock Platina (Class Platinum) Fotanúsítvány"); 
certlist.push_back("ed8dc8386c4886aeee079158aac3bfe658e394b4 Trustis EVS Root CA"); 
certlist.push_back("edb3cb5fb419a185066267e5791554e1e28b6399 South African Post Office CA"); 
certlist.push_back("ee29d6ea98e632c6e527e0906f0280688bdf44dc Chambersign Public Notary Root"); 
certlist.push_back("ee869387fffd8349ab5ad14322588789a457b012 COMODO"); 
certlist.push_back("f00fc37d6a1c9261fb6bc1c218498c5aa4dc51fb GPKI ApplicationCA2 Root"); 
certlist.push_back("f138a330a4ea986beb520bb11035876efb9d7f1c Digidentity BV"); 
certlist.push_back("f17f6fb631dc99e3a3c87ffe1cf1811088d96033 TÜRKTRUST Elektronik Sertifika Hizmet Saglayicisi"); 
certlist.push_back("f18b538d1be903b6a6f056435b171589caf36bf2 thawte Primary Root CA - G3"); 
certlist.push_back("f373b387065a28848af2f34ace192bddc78e9cac Actalis Authentication Root CA"); 
certlist.push_back("f48b11bfdeabbe94542071e641de6bbe882b40b9 TW Government Root Certification Authority"); 
certlist.push_back("f517a24f9a48c6c9f8a200269fdc0f482cab3089 DigiCert Assured ID Root G3"); 
certlist.push_back("f5c27cf5fff3029acf1a1a4bec7ee1964c77d784 CertRSA01"); 
certlist.push_back("f9b5b632455f9cbeec575f80dce96e2cc7b278b7 Trend Micro"); 
certlist.push_back("f9cd0e2cda7624c18fbdf0f0abb645b8f7fed57a ComSign Secured CA"); 
certlist.push_back("f9dd19266b2043f1fe4b3dcb0190aff11f31a69d Correo Uruguayo - Root CA"); 
certlist.push_back("fa0882595f9ca6a11eccbeaf65c764c0ccc311d0 CertEurope"); 
certlist.push_back("faa7d9fb31b746f200a85e65797613d816e063b5 VRK Gov. Root CA"); 
certlist.push_back("faaa27b8caf5fdf5cda98ac3378572e04ce8f2e0 Spanish Property & Commerce Registry CA"); 
certlist.push_back("fab7ee36972662fb2db02af6bf03fde87c4b2f9b certSIGN Root CA"); 
certlist.push_back("fd1ed1e2021b0b9f73e8eb75ce23436bbcc746eb D-TRUST GmbH"); 
certlist.push_back("fe45659b79035b98a161b5512eacda580948224d Hellenic Academic and Research Institutions RootCA 2011"); 
certlist.push_back("feb8c432dcf9769aceae3dd8908ffd288665647d SECOM Trust Systems CO LTD");
}
else if (pszStoreName[0]=='D') {
certlist.push_back("fa6660a94ab45f6a88c0d7874d89a863d74dee97 Microsoft Enforced Licensing Registration Authority CA (SHA1)");
certlist.push_back("f92be5266cc05db2dc0dc3f2dc74e02defd949cb e-islem.kktcmerkezbankasi.org");
certlist.push_back("f8a54e03aadc5692b850496a4c4630ffeaa29d83 DigiNotar Services 1024 CA");
certlist.push_back("f5a874f3987eb0a9961a564b669a9050f770308a Microsoft Online Svcs BPOS CA2");
certlist.push_back("e9809e023b4512aa4d4d53f40569c313c1d0294d Microsoft Online Svcs BPOS EMEA CA2");
certlist.push_back("e95dd86f32c771f0341743ebd75ec33c74a3ded9 Microsoft Online Svcs BPOS APAC CA3");
certlist.push_back("e38a2b7663b86796436d8df5898d9faa6835b238 Microsoft Genuine Windows Phone Public Preview CA01");
certlist.push_back("e1f3591e769865c4e447acc37eafc9e2bfe4c576 MCSHOLDING TEST");
certlist.push_back("d8ce8d07f9f19d2569c2fb854401bc99c1eb7c3b Microsoft Online Svcs BPOS APAC CA2");
certlist.push_back("d43153c8c25f0041287987250f1e3cabac8c2177 Microsoft Online Svcs BPOS APAC CA1");
certlist.push_back("d2dbf71823b2b8e78f5958096150bfcb97cc388a NIC CA 2014");
certlist.push_back("d0bb3e3dfbfb86c0eee2a047e328609e6e1f185e Microsoft Online Svcs BPOS APAC CA5");
certlist.push_back("d018b62dc518907247df50925bb09acf4a5cb3ad login.yahoo.com");
certlist.push_back("cea586b2ce593ec7d939898337c57814708ab2be login.live.com");
certlist.push_back("c69f28c825139e65a646c434aca5a1d200295db1 *.EGO.GOV.TR");
certlist.push_back("c6796490cdeeaab31aed798752ecd003e6866cb2 NIC CA 2011");
certlist.push_back("c060ed44cbd881bd0ef86c0ba287ddcf8167478c DigiNotar Root CA");
certlist.push_back("bed412b1334d7dfceba3015e5f9f905d571c45cf Microsoft IPTVe CA");
certlist.push_back("b86e791620f759f17b8d25e38ca8be32e7d5eac2 DigiNotar Cyber CA");
certlist.push_back("b533345d06f64516403c00da03187d3bfef59156 DigiNotar PKIoverheid CA Overheid");
certlist.push_back("a81706d31e6f5c791cd9d3b1b9c63464954ba4f5 Microsoft Online Svcs CA5");
certlist.push_back("a7b5531ddc87129e2c3bb14767953d6745fb14a6 Microsoft Online Svcs BPOS EMEA CA3");
certlist.push_back("a35a8c727e88bcca40a3f9679ce8ca00c26789fd Microsoft Online Svcs BPOS EMEA CA1");
certlist.push_back("a221d360309b5c3c4097c44cc779acc5a9845b66 Microsoft Online Svcs CA1");
certlist.push_back("a1505d9843c826dd67ed4ea5209804bdbb0df502 Microsoft Online CA001");
certlist.push_back("9845a431d51959caf225322b4a4fe9f223ce6d15 DigiNotar Cyber CA");
certlist.push_back("8e5bd50d6ae686d65252f843a9d4b96d197730ab Digisign Server ID - (Enrich)");
certlist.push_back("8977e8569d2a633af01d0394851681ce122683a6 Microsoft Online Svcs CA3");
certlist.push_back("86e817c81a5ca672fe000f36f878c19518d6f844 DigiNotar Root CA");
certlist.push_back("838ffd509de868f481c29819992e38a4f7082873 Microsoft Online Svcs BPOS EMEA CA6");
certlist.push_back("80962ae4d6c5b442894e95a13e4a699e07d694cf login.yahoo.com");
certlist.push_back("7613bf0ba261006cac3ed2ddbef343425357f18b Microsoft Online Svcs BPOS CA1");
certlist.push_back("6690c02b922cbd3ff0d0a5994dbd336592887e3f Microsoft Online Svcs CA4");
certlist.push_back("6431723036fd26dea502792fa595922493030f97 mail.google.com");
certlist.push_back("63feae960baa91e343ce2bd8b71798c76bdb77d0 login.yahoo.com");
certlist.push_back("61793fcbfa4f9008309bba5ff12d2cb29cd4151a global trustee");
certlist.push_back("5de83ee82ac5090aea9d6ac4e7a6e213f946e179 DigiNotar PKIoverheid CA Organisatie - G2");
certlist.push_back("5d5185df1eb7dc76015422ec8138a5724bee2886 Microsoft Online Svcs CA4");
certlist.push_back("5ce339465f41a1e423149f65544095404de6ebe2 AC DG Trésor SSL");
certlist.push_back("587b59fb52d8a683cbe1ca00e6393d7bb923bc92 Microsoft Online Svcs BPOS CA2");
certlist.push_back("51c3247d60f356c7ca3baf4c3f429dac93ee7b74 Digisign Server ID (Enrich)");
certlist.push_back("4ed8aa06d1bc72ca64c47b1dfe05acc8d51fc76f Microsoft Online Svcs BPOS CA2");
certlist.push_back("4df13947493cff69cde554881c5f114e97c3d03b Microsoft Online Svcs CA5");
certlist.push_back("4d8547b7f864132a7f62d9b75b068521f10b68e3 *.google.com");
certlist.push_back("4822824ece7ed1450c039aa077dc1f8ae3489bbf NIC Certifying Authority");
certlist.push_back("471c949a8143db5ad5cdf1c972864a2504fa23c9 login.skype.com");
certlist.push_back("43d9bcb568e039d073a74a71d8511f7476089cc3 DigiNotar Root CA G2");
certlist.push_back("40aa38731bd189f9cdb5b9dc35e2136f38777af4 DigiNotar PKIoverheid CA Overheid en Bedrijven");
certlist.push_back("3a850044d8a195cd401a680c012cb0a3b5f8dc08 Microsoft Enforced Licensing Intermediate PCA");
certlist.push_back("3a26012171855d4020c973bec3f4f9da45bd2b83 CN=Microsoft Online Svcs BPOS APAC CA4");
certlist.push_back("374d5b925b0bd83494e656eb8087127275db83ce Microsoft Online Svcs CA3");
certlist.push_back("367d4b3b4fcbbc0b767b2ec0cdb2a36eab71a4eb DigiNotar Root CA");
certlist.push_back("330d8d3fd325a0e5fddda27013a2e75e7130165f Microsoft Online Svcs BPOS EMEA CA4");
certlist.push_back("305f8bd17aa2cbc483a4c41b19a39a0c75da39d6 addons.mozilla.org");
certlist.push_back("2b84bfbb34ee2ef949fe1cbe30aa026416eb2216 DigiNotar Cyber CA");
certlist.push_back("2a83e9020591a55fc6ddad3fb102794c52b24e70 Microsoft Enforced Licensing Intermediate PCA");
certlist.push_back("23ef3384e21f70f034c467d4cba6eb61429f174e Microsoft Online Svcs CA1");
certlist.push_back("1916a2af346d399f50313c393200f14140456616 www.google.com");
certlist.push_back("09ff2cc86ceefa8a8bb3f2e3e84d6da3fabbf63e Microsoft Online Svcs CA6");
certlist.push_back("09271dd621ebd3910c2ea1d059f99b8181405a17 Microsoft Online Svcs BPOS EMEA CA5");
certlist.push_back("08e4987249bc450748a4a78133cbf041a3510033 www.live.fi");
certlist.push_back("08738a96a4853a52acef23f782e8e1fea7bced02 Microsoft Online Svcs BPOS APAC CA6");
}
return certlist;
}
