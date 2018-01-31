/*
 * Logable.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#include "Logable.h"

namespace va {

Logable::Logable(const string& objName, string loggerName) {
	logger = (log4cxx::Logger::getLogger(loggerName));
	objectName = objName;
}

Logable::Logable() {
}

Logable::~Logable() {
	logger->releaseRef();
}

} /* namespace va */
