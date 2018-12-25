#include "SDK.h"

#define WPN_LISTS_PATH "C:/Users/Buxthefag/Desktop/WeaponDump.hpp"
#define SIG_LISTS_PATH "C:/Users/Buxthefag/Desktop/SigDump.hpp"

void DumpWeapons() {
	cout << "Dumping Weapons!" << endl;

	if (FileExists(WPN_LISTS_PATH)) {
		cout << "File Exists, refreshing..." << endl;
		remove(WPN_LISTS_PATH);
	}

	typedef const char*(__cdecl* Wid2Alias)(int i);

	Wid2Alias pWpnName = NULL;

	DWORD dwLoc = (DWORD)sigs::GetClientSignature("55 8B EC 8B 45 08 83 F8 6E 73 0D 85 C0 78 09 8B 04 85 ? ? ? ? 5D");
	DWORD dwFinal = (DWORD)(dwLoc);

	pWpnName = (Wid2Alias)dwFinal;

	string start = "enum globalweapons{";
	string end = "}; ";
	string buf[1000] = {};

	if (dwFinal != NULL) {
		int max = 0;

		for (int i = 0; i <= 1000; i++) {

			if (!pWpnName(i)) continue;

			buf[i].append(pWpnName(i));
			buf[i].append(" = ");
			buf[i].append(std::to_string(i));
			buf[i].append(", ");
			buf[i].append("//0x0");
			buf[i].append(std::to_string(i));

			++max;
		}

		ofstream wpnlist;
		wpnlist.open(WPN_LISTS_PATH);

		if (wpnlist.good() != true) cout << "Wpnlist stream is not good!" << endl;

		wpnlist << "#include <Windows.h>" << endl;
		wpnlist << "" << endl;
		wpnlist << "/*" << endl;
		wpnlist << "*" << endl;
		wpnlist << "*									WARNING!" << endl;
		wpnlist << "* THIS STRUCTS HAVE BEEN AUTOMATICALLY CREATED BY THE CITADELWORKS INTELLIGENT SDK" << endl;
		wpnlist << "* PLEASE, REPORT ANY INACCURACIES AND ERROR CODES / CRASHES / AND OR OTHER BUGS / COMPATIBLITY ISSUES TO THE HOLDER OF THIS LIBRARY." << endl;
		wpnlist << "*/" << endl;

		wpnlist << start << endl;

		for (int i = 0; i <= max; i++) {
			wpnlist << buf[i] << endl;
		}

		end.append("//Size: ");
		end.append("0x0");
		end.append(std::to_string(max));
		
		wpnlist << end << endl;

		wpnlist.close();
	}
}

bool FileExists(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}

void DumpSig2Offs() {
	DWORD m_dwClientModeLocation = sigs::GetClientSignature("8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC");
	DWORD m_dwClientModeAddress = m_dwClientModeLocation + 0x2;
	DWORD m_dwClientModeOffset = (DWORD)I::Client - m_dwClientModeAddress;

	ofstream siglist;
	siglist.open(SIG_LISTS_PATH);

	if (siglist.good() != true) cout << "Siglist stream is not good!" << endl;

	siglist << "#include <Windows.h>" << endl;
	siglist << "" << endl;
	siglist << "/*" << endl;
	siglist << "*" << endl;
	siglist << "*									WARNING!" << endl;
	siglist << "* THIS STRUCTS HAVE BEEN AUTOMATICALLY CREATED BY THE CITADELWORKS INTELLIGENT SDK" << endl;
	siglist << "* PLEASE, REPORT ANY INACCURACIES AND ERROR CODES / CRASHES / AND OR OTHER BUGS / COMPATIBLITY ISSUES TO THE HOLDER OF THIS LIBRARY." << endl;
	siglist << "*/" << endl;

	string start = "enum siglist{";
	string end = "}; ";
	string buf[1000] = {};

	int max = 0;

	buf[max].append("CLIENT_MODE_PTR");
	buf[max].append(" = 0x");
	buf[max].append(std::to_string(m_dwClientModeOffset));
	buf[max].append(", //0x");
	buf[max].append(std::to_string(max));
	++max;

	siglist << start << endl;

	for (int i = 0; i <= max; i++) {
		siglist << buf[i] << endl;
	}

	siglist << end << endl;

	siglist.close();
}

#ifdef _HL2DM_MP_v1

CBaseEntity* IBasePhysCannon::GetHeldEntity() {
	return I::EntList->GetClientEntityFromHandle(*(int*)(this + 0x958));
}

#endif