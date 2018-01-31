/*
 * FeaturePointsRequestDebugControlMessage.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: patrick
 */
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
#ifdef ENABLE_MAV_DEBUG_MSG
#include "Fpdcmessage.h"

namespace va {

Fpdcmessage::Fpdcmessage(string msgId, string loggerName, string targetId) :
		ControlMessage(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	start = false;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

Fpdcmessage::~Fpdcmessage() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

void Fpdcmessage::setupFromMavlinkMsg(mavlink_message_t msg) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif

	//LOG4CXX_ERROR(logger, "[" << id <<"] start before: " << start);


	mavlink_tx1_debug_request_fp_tracked_t message;
	mavlink_msg_tx1_debug_request_fp_tracked_decode(&msg, &message);
	concreteType = ControlMessageType::REQ_FP;
	//LOG4CXX_ERROR(logger, "[" << id <<"] send before: " << message.send);
	if(message.send == TX1_DEBUG_STATUS_FP_SEND) {
		start = true;
	} else {
		if(message.send == TX1_DEBUG_STATUS_FP_NOT_SEND) {
			start = false;
		}
	}
	//LOG4CXX_ERROR(logger, "[" << id <<"] start after: " << start);

#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

} /* namespace va */
#endif
