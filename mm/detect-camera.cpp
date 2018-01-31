//============================================================================
// Name        : main.cpp
// Author      : patrick
// Version     :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>

#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

#include "../tools/Enums.h"
#include "../config/videoapp_config.h"
#include "../controller/SystemController.h"
#include "../tools/Configuration.h"

using namespace std;
using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace va;

//TODO needs to change to a secure failsafe wat to load the logger config file
bool initLogSystem(int argc, char **argv) {
	try {
		string filePath;
		if (argc > 1) {
			cout << "Try to configure logger from file [" << argv[LOG4CXX_CONF_FILE_PATH_POS] << "]" << endl;
			filePath = argv[LOG4CXX_CONF_FILE_PATH_POS];
		} else {
			cout << "Try to configure logger from file [" << LOG4CXX_CONF_PATH << "]" << endl;
			filePath = LOG4CXX_CONF_PATH;
		}
		PropertyConfigurator::configure(filePath);
		return true;
	} catch (Exception&) {
		try {
			cout << "Loading Logger config file failed" << endl;
			BasicConfigurator::configure();
			cout << "Loaded default basic config" << endl;
			return true;
		} catch (Exception&) {
			cout << "Could not initialize Logsystem!" << endl;
			return false;
		}
	}
}

void logBuildVersion(LoggerPtr logger) {

	LOG4CXX_WARN(logger, "--------------------------------------------------------------------------------");
	LOG4CXX_WARN(logger, "--------------------------------------------------------------------------------");
#ifdef DEBUG_BUILD
	LOG4CXX_WARN(logger, "This videoApp version was build with the DEBUG_BUILD flag set!");
	LOG4CXX_WARN(logger, "Please set all loggers in the log4cxx config file to DEBUG to get full debug logging");
	LOG4CXX_WARN(logger, "This may have negative effects on the performance and produce spam like output");
	LOG4CXX_WARN(logger, "This version is not meant for a production enviroment!");
	LOG4CXX_WARN(logger, "If you have only this version - ask patrick what to do!");
#elif RELEASE_BUILD
	LOG4CXX_WARN(logger, "This videoApp version was NOT build with the DEBUG_BUILD flag set!");
#endif
	LOG4CXX_WARN(logger, "--------------------------------------------------------------------------------");
	LOG4CXX_WARN(logger, "--------------------------------------------------------------------------------");

}

int main(int argc, char **argv) {
	std::locale::global(std::locale(""));
	int reVal = 1;
	cout << "Starting VideoApp" << endl;
	bool isLogSystemInited = initLogSystem(argc, argv);
	if (!isLogSystemInited) {
		return 0;
	}
	LoggerPtr logger(Logger::getLogger(LOG_MAIN));
	LOG4CXX_INFO(logger, "VideoApp running");
	LOG4CXX_INFO(logger, "LogSystem initialization - " << SUCCESS);
	logBuildVersion(logger);
	if (!Configuration::getInstance().init(CONF_PATH)) {
		LOG4CXX_ERROR(logger, "Initialization of [" << Configuration::getInstance().getObjectName() <<"]  - " << FAIL);
		return 1;
	}
	SystemController* mainCtrl = new SystemController(Configuration::getInstance().getSystemControllerId(), LOG_MAIN);
	if (mainCtrl->init() != ComponentState::INIT) {
		LOG4CXX_ERROR(logger,
				"Initialization of System-Controller [" << Configuration::getInstance().getSystemControllerId() << "] - " << FAIL);
		return 1;
	} else {
		LOG4CXX_INFO(logger,
				"Initialization of System-Controller [" << Configuration::getInstance().getSystemControllerId() << "] - " << SUCCESS);

		mainCtrl->start().join();
	}
	delete mainCtrl;
	LOG4CXX_INFO(logger, "VideoApp exiting");
	logger->releaseRef();
	return 0;
}

