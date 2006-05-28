#pragma once

#include "stdAfx.h"
#include <vector>
#include <string>

void addFilesInDirectory(const char * dir, const char * extension, std::vector<std::string>& files) {
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;

	std::string queryString = std::string(dir) + "\\*." + extension;

	hFind = FindFirstFile(queryString.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		files.push_back(std::string(std::string(dir) + "\\" + FindFileData.cFileName));
		
		while (FindNextFile(hFind, &FindFileData) != 0) {
			files.push_back(std::string(std::string(dir) + "\\" + FindFileData.cFileName));
		}
    
		dwError = GetLastError();
		FindClose(hFind);
/*		if (dwError != ERROR_NO_MORE_FILES) 
		{
			printf ("FindNextFile error. Error is %u\n", dwError);
			return (-1);
		}*/
   }
}

void addPicturesInDirectory(const char * dir, std::vector<std::string>& files) {
	addFilesInDirectory(dir, "bmp", files);
	addFilesInDirectory(dir, "jpg", files);
	addFilesInDirectory(dir, "gif", files);
}
