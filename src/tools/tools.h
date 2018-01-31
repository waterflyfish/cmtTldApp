/*
 * tools.h
 *
 *  Created on: Jan 16, 2017
 *      Author: patrick
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <map>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <string>

#include "log4cxx/logger.h"

#include "../../config/videoapp_config.h"

using namespace std;
using namespace log4cxx;

//@see http://stackoverflow.com/questions/5888022/split-string-by-single-spaces
static unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch) {
	unsigned int pos = txt.find(ch);
	unsigned int initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}
	unsigned int x = txt.size();
	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, x) - initialPos));

	return strs.size();
}

static std::vector<std::string> splitCommandString(std::string command) {
	std::vector<std::string> result;
	split(command, result, ' ');
	return result;
}

static std::map<std::string, std::string> parseQueryString(std::string command) {
	std::vector<std::string> tmp;
	split(command, tmp, '&');
	std::map<std::string, std::string> result;
	for (auto paramPair : tmp) {
		std::vector<std::string> param;
		split(paramPair, param, '=');
		result.insert(std::pair<std::string, std::string>(param[0], param[1]));
	}
	return result;
}

static bool fileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

static std::string getStringValueFromMap(std::map<std::string, std::string> map, std::string key, std::string defaultValue) {
	return map.count(key) ? map.at(key) : defaultValue;
}

constexpr unsigned int str2int(const char* str, int h = 0) {
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

static std::string getTime() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%I-%M-%S", timeinfo);
	std::string str(buffer);
	return str;
}
static std::string getDate() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%d-%m-%Y", timeinfo);
	std::string str(buffer);
	return str;
}

static std::string buildGstDotName(std::string pipePrefix, std::string dotName, int count) {
	return pipePrefix + std::string("_") + dotName + std::string("_") + std::to_string(count);
}

static const bool statFile(string filePath) {
	LoggerPtr logger(Logger::getLogger(LOG_TOOLS));
	struct stat fileStatus;
	if (stat(filePath.c_str(), &fileStatus) == -1) {
		LOG4CXX_ERROR(logger, "loadConfigFile() stat [" << filePath << "] - FAILED"); //TODO this does not knwo what file type it is! config file??? etc
		if ( errno == ENOENT) {
			LOG4CXX_ERROR(logger, "loadConfigFile() - [" << filePath << "] Path file_name does not exist, or path is an empty string.");
		} else if ( errno == ENOTDIR) {
			LOG4CXX_ERROR(logger, "loadConfigFile() - [" << filePath << "] A component of the path is not a directory.");
		} else if ( errno == ELOOP) {
			LOG4CXX_ERROR(logger, "loadConfigFile() - [" << filePath << "] Too many symbolic links encountered while traversing the path.");
		} else if ( errno == EACCES) {
			LOG4CXX_ERROR(logger, "loadConfigFile() - [" << filePath << "] Permission denied.");
		} else if ( errno == ENAMETOOLONG) {
			LOG4CXX_ERROR(logger, "loadConfigFile() - [" << filePath << "] File can not be read");
		}
		return false;
	}
	return true;
}

#endif /* TOOLS_H_ */
