#pragma once

#include "stdAfx.h"
#include <vector>
#include <string>

#include "ExhaustiveRandomGenerator.h"

void addFilesInDirectory(const char * dir, 
						 const std::vector<std::string>& extensions, 
						 std::vector<std::string>& files,
						 bool recurseInSubDirectories) 
{
	WIN32_FIND_DATA FindFileData;
	for(unsigned int i = 0; i < extensions.size(); i++) {
		std::string queryString = std::string(dir) + "\\*." + extensions[i];
		HANDLE hFind = FindFirstFile(queryString.c_str(), &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) {			
			do {
				files.push_back(std::string(std::string(dir) + "\\" + FindFileData.cFileName));
			} while (FindNextFile(hFind, &FindFileData) != 0);
	    
			FindClose(hFind);
		}
	}

	if(recurseInSubDirectories) {
		// get the subdirectories
		std::string queryString = std::string(dir) + "\\*";
		HANDLE hFind = FindFirstFile(queryString.c_str(), &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					std::string fileName = FindFileData.cFileName;
					if((fileName != ".") && (fileName != "..")) {
						std::string subDirPath = std::string(dir) + "\\" + fileName;
						addFilesInDirectory(subDirPath.c_str(), extensions, files, true);
					}
				}
			} while (FindNextFile(hFind, &FindFileData) != 0);
		
			FindClose(hFind);
		}
	}
}

void addPicturesInDirectory(const char * dir, std::vector<std::string>& files, bool recurseInSubDirectories) {
	std::vector<std::string> extensions;
	extensions.push_back("bmp");
	extensions.push_back("jpg");
	extensions.push_back("gif");
	addFilesInDirectory(dir, extensions, files, recurseInSubDirectories);
}	

class RandomFileAccessor {

public:

	void init(const char * inputDirectory, bool recurseInSubDirectories) {
		files_.clear();
		addPicturesInDirectory(inputDirectory, files_, recurseInSubDirectories);
		randomGenerator = Prenzl::ExhaustiveRandomGenerator(files_.size());
	}

	bool hasNoFiles() {
		return files_.empty();
	}

	unsigned int getNumberOfFiles() {
		return (unsigned int)files_.size();
	}

	std::string getFile() {
		return files_[randomGenerator.next()];
	}

private:
	std::vector<std::string> files_;
	Prenzl::ExhaustiveRandomGenerator randomGenerator;
};
