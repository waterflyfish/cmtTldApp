/*
 * StatusMessage.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: patrick
 */

#include "StatusMessage.h"

namespace va {

StatusMessage::StatusMessage(string msgId, string loggerName, string targetId) :
		Message(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	type = MessageType::STATUS;
	concreteType = StatusMessageType::UNKNOWN;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

StatusMessage::~StatusMessage() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

} /* namespace va */


