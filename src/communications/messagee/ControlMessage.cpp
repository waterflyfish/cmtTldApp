/*
 * ControlMessage.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: patrick
 */

#include "ControlMessage.h"

namespace va {

ControlMessage::ControlMessage(string msgId, string loggerName, string targetId) :
		Message(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	type = MessageType::CONTROL;
	concreteType = ControlMessageType::UNKNOWN;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

ControlMessage::~ControlMessage() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

} /* namespace va */
