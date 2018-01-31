/*
 * TrackingMessage.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#include "TrackingMessage.h"

namespace va {

TrackingMessage::TrackingMessage(string msgId, string loggerName, string targetId) :
		ControlMessage(msgId, loggerName, targetId) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	start = false;
	concreteType = ControlMessageType::UNKNOWN;
	tlX = -1;
	tlY = -1;
	brX = -1;
	brY = -1;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

TrackingMessage::~TrackingMessage() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

void TrackingMessage::setupFromMavlinkMsg(mavlink_message_t msg) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	switch (msg.msgid) {
	case MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START: {
		mavlink_tx1_control_tracking_start_t message;
		mavlink_msg_tx1_control_tracking_start_decode(&msg, &message);
		concreteType = ControlMessageType::TRACKING_START;
		start = true;
		tlX = message.tl_x;
		tlY = message.tl_y;
		brX = message.br_x;
		brY = message.br_y;
	}
		break;
	case MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP: {
		mavlink_tx1_control_tracking_stop_t message;
		mavlink_msg_tx1_control_tracking_stop_decode(&msg, &message);
		concreteType = ControlMessageType::TRACKING_STOP;
		start = false;
	}
		break;
	default:
		break;
	}
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

} /* namespace va */
