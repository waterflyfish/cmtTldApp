/*
 * PossibleTargetMessage.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#include "PossibleTargetMessage.h"

namespace va {

PossibleTargetMessage::PossibleTargetMessage(string msgId, string loggerName, string targetId) :
		StatusMessage(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	concreteType = StatusMessageType::POSSIBLE;
	tlX = -1;
	tlY = -1;
	brX = -1;
	brY = -1;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif

}

PossibleTargetMessage::~PossibleTargetMessage() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}


} /* namespace va */
