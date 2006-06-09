#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <exception>

//! Properties Container 
class CProperties {

public:

	//! Empty constructor
	CProperties() {}

	//! Constructor from a string 
	CProperties(const std::string& input) {
		// put the input in the string stream
		std::stringstream sstream;
		sstream << input;
		initFromStream(sstream);
	}

	//! Constructor from an input strean
	CProperties(std::istream& stream) {
		initFromStream(stream);
	}

	void clear() {
		content_.clear();
	}

	//! Initialize properties from an input stream
	/*!
		If the properties were not empty, they are cleared
		before being initialized.
	*/
	void initFromStream(std::istream& stream) {
		// clear the content
		content_.clear();

		// get each line
		std::string line;
		while(std::getline(stream,line)) {
			//look for the '='
			std::string::size_type pos = line.find('=',0);
			if(pos == std::string::npos) {
				// we could not find the '='
				//throw std::exception("invalid line");
				std::cerr << "invalid line";
			}
			else {
				// we found '=' at position pos
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos+1);
				content_[key] = value;
			}
		}
	}

	//! Get value for a key as an integer
	int getValueAsInt(const std::string& key) const {
		std::map<std::string, std::string>::const_iterator itr = content_.find(key);
		if(itr != content_.end()) {
			// we found the key
			return atoi(itr->second.c_str());
		}
		else {
			// not found !
			//throw std::exception((key + " not found").c_str());
			std::cerr << (key + " not found").c_str();
			throw std::exception(); 
		}
	}

	//! Set the value for a key given as an integer
	void setIntValue(const std::string& key, int value) {
		std::stringstream buffer;
		buffer << value;
		content_[key] = buffer.str();
	}

	//! Write content of the properties in a stream
	void writeInStream(std::ostream& stream) const {
		std::map<std::string, std::string>::const_iterator itr;
		for(itr = content_.begin(); itr != content_.end(); ++itr) {
			stream << itr->first << "=" << itr->second << "\n";
		}
	}

	//! Write content of the properties as a string
	std::string toString() const {
		std::stringstream sstream;
		writeInStream(sstream);
		return sstream.str();
	}

private:
	//! The map storing the content
	std::map<std::string, std::string> content_;
};
