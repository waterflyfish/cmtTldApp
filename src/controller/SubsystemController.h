/*
 * SubsystemController.h
 *
 *  Created on: Jan 10, 2017
 *      Author: patrick
 */

#ifndef SUBSYSTEMCONTROLLER_H_
#define SUBSYSTEMCONTROLLER_H_

#include <string>
#include <map>


#include "tools.h"
#include "SystemController.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
using namespace std;


namespace va {

class SubsystemController: public SystemController {
public:
	enum class SubsystemType {
		PIPELINE, COMMUNICATIONS, UNKNOWN
	};
	SubsystemController(const string& ctrlName, string loggerName);
	virtual ~SubsystemController();
	virtual ComponentState init() = 0;

	static SubsystemType getTypeFromString(string type) {
		switch (str2int(type.c_str())) {
		case str2int("pipeline"):
			return SubsystemType::PIPELINE;
			break;
		case str2int("communications"):
			return SubsystemType::COMMUNICATIONS;
			break;
		default:
			break;
		}
	}

	bool controlsMessanger(const string& messangerId);
	virtual bool processMessage(Message* msg) = 0;
protected:

	map<string, Messanger*> controlledMessangers;

	SubsystemType subSysType;
	virtual ComponentState control() = 0;


};

} /* namespace va */

#endif /* SUBSYSTEMCONTROLLER_H_ */
