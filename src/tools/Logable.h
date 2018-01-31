/*
 * Logable.h
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#ifndef LOGABLE_H_
#define LOGABLE_H_

#include <string>

#include "log4cxx/logger.h"
#include "log4cxx/helpers/exception.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
using namespace std;
using namespace log4cxx;

namespace va {

class Logable {
public:
	Logable(const string& objame, string loggerName);
	Logable();
	virtual ~Logable();
	log4cxx::LoggerPtr logger;
	const string& getObjectName() const {
		return objectName;
	}
protected:
	void setObjectName(const string& objectName) {
		this->objectName = objectName;
	}

private:
	string objectName;
};

} /* namespace va */

#endif /* LOGABLE_H_ */
