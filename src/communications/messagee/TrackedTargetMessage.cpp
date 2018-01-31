/*
 * TrackedTargetMessage.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#include "TrackedTargetMessage.h"

namespace va {

TrackedTargetMessage::TrackedTargetMessage(string msgId, string loggerName, string targetId) :
		StatusMessage(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	concreteType = StatusMessageType::TARGET;
	tlX = -1;
	tlY = -1;
	brX = -1;
	brY = -1;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

TrackedTargetMessage::~TrackedTargetMessage() {

}

int TrackedTargetMessage::getCenterX() const{
    return status;
	//return (brX - tlX)/2;
}

int TrackedTargetMessage::getCenterY() const {

    return num;
	//return (brY - tlY)/2;
}

} /* namespace va */
