/*
 * SubsystemController.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: patrick
 */

#include "SubsystemController.h"

namespace va {

SubsystemController::SubsystemController(const string& ctrlName, string loggerName) :
		SystemController(ctrlName, loggerName) {
	subSysType = SubsystemType::UNKNOWN;
}

SubsystemController::~SubsystemController() {

}

bool SubsystemController::controlsMessanger(const string& messangerId) {
	return (controlledMessangers.count(messangerId) == 1);
}

} /* namespace va */

