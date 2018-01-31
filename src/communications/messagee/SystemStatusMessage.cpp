/*
 * SystemStatusMessage.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#include "SystemStatusMessage.h"

namespace va {

SystemStatusMessage::SystemStatusMessage(string msgId, string loggerName, string targetId) :
		StatusMessage(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	concreteType = StatusMessageType::SYSTEM;
	tracking = false;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif

}

SystemStatusMessage::~SystemStatusMessage() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

} /* namespace va */
