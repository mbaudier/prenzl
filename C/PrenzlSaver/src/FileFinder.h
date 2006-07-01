#pragma once

#include "stdAfx.h"
#include <vector>
#include <string>

void addFilesInDirectory(const char * dir, const char * extension, std::vector<std::string>& files) {
	WIN32_FIND_DATA FindFileData;
	std::string queryString = std::string(dir) + "\\*." + extension;
	HANDLE hFind = FindFirstFile(queryString.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {
		files.push_back(std::string(std::string(dir) + "\\" + FindFileData.cFileName));
		
		while (FindNextFile(hFind, &FindFileData) != 0) {
			files.push_back(std::string(std::string(dir) + "\\" + FindFileData.cFileName));
		}
    
		FindClose(hFind);
   }
}

void addPicturesInDirectory(const char * dir, std::vector<std::string>& files) {
	addFilesInDirectory(dir, "bmp", files);
	addFilesInDirectory(dir, "jpg", files);
	addFilesInDirectory(dir, "gif", files);
}

class RandomFileAccessor {
public:
	RandomFileAccessor() 
		: nbFileAccessed_(0)
	{
	}

	void init(const char * inputDirectory) {
		files_.clear();
		addPicturesInDirectory(inputDirectory, files_);
		filesAccessed_ = std::vector<bool>(files_.size(), false);
		nbFileAccessed_ = 0;
	}

	bool hasNoFiles() {
		return files_.empty();
	}

	unsigned int getNumberOfFiles() {
		return (unsigned int)files_.size();
	}

	std::string getFile() {
	/*	if(hasNoFiles()) {
			throw std::exception("No Files Found");
		}*/
		unsigned int nbFiles = (unsigned int)files_.size();
		if(nbFileAccessed_ == nbFiles) {
			// all files were accessed, reinit
			filesAccessed_ = std::vector<bool>(nbFiles, false);
			nbFileAccessed_ = 0;
		}
		unsigned int randomIncrement = rand() % nbFiles;
		unsigned int circularIndex = 0;
		unsigned int incr=0;
		while(true) {
			// find the next unaccessed file
			while(filesAccessed_[circularIndex]) {
				incrementCircularIndex(circularIndex);
			}
			if(incr == randomIncrement) {
				filesAccessed_[circularIndex] = true;
				nbFileAccessed_++;
				return files_[circularIndex];
			}
			incr++;
			incrementCircularIndex(circularIndex);
		}	
	}

private:
	void incrementCircularIndex(unsigned int& index) {
		index++;
		if(index >= files_.size()) {
			index -= (unsigned int)files_.size();
		}
	}

private:
	std::vector<std::string> files_;
	std::vector<bool> filesAccessed_;
	unsigned int nbFileAccessed_;
};
