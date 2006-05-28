#pragma once
#include "stdafx.h"
#include <string>

#define VendorName "CART"		// predefined stuff
#define AppName "PrenzlSaver"							// for registry access
#define RegKeyName "Software\\" VendorName "\\" AppName

class RegistryAccessor {
public:
	RegistryAccessor() {
		DWORD dummy;			// don't really care if the key exists or not, just open it
		if (RegCreateKeyEx(HKEY_CURRENT_USER, RegKeyName, 0, "", 0, KEY_READ | KEY_WRITE,
			NULL, &hKey, &dummy))
		{
			// hmm, couldn't open key...  User might be able to do something about this
			// so we'll go ahead and report it
			MessageBox(NULL, "Error opening/creating registry key.  Check that you "
				"are not out of hard disk space and have permission to write to the "
				"HKEY_CURRENT_USER key (if running Windows NT).  Other than that, good "
				"luck :-/", "PrenzlSaver", MB_OK | MB_ICONSTOP);

			throw std::exception("Registry could not be opened");		// still can't recover...
		}
	}

	~RegistryAccessor() {
		RegCloseKey(hKey);
	}

	DWORD readDWORD(const char* regName, DWORD defaultValue = 0) {
		LONG err;
		DWORD type, size, ret;

		size = sizeof(DWORD);
		err = RegQueryValueEx(hKey, regName, 0, &type, (UCHAR*)&ret, &size);

		if (!err && (type == REG_DWORD)) {
			return ret;
		}
		else {
			return defaultValue;
		}
	}

	void writeDWORD(const char* regName, DWORD value) {
		RegSetValueEx(hKey, regName, 0, REG_DWORD, (UCHAR*)&value, sizeof(DWORD));
	}

	std::string readString(const char* regName) {
		char buffer[256];

		LONG err;
		DWORD type, size;
		size = 255;
		err = RegQueryValueEx(hKey, regName, 0, &type, (UCHAR*)buffer, &size);

		if (!err && (type == REG_SZ)) {
			buffer[255] = 0;
			return std::string(buffer);
		}
		else {
			return std::string("");
		}

	}

	void writeString(const char* regName, const char * content) {
		RegSetValueEx(hKey, regName, 0, REG_SZ, (UCHAR*)content, (DWORD)(strlen(content) + 1));
	}

private:
	HKEY hKey;
};