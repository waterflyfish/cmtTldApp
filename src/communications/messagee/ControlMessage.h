/*
 * ControlMessage.h
 *
 *  Created on: Jan 23, 2017
 *      Author: patrick
 */

#ifndef CONTROLMESSAGE_H_
#define CONTROLMESSAGE_H_
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
#include "Message.h"

namespace va {

class ControlMessage: public Message {
public:
	enum class ControlMessageType {
		TRACKING_START, TRACKING_STOP, UNKNOWN
#ifdef ENABLE_MAV_DEBUG_MSG
		,REQ_FP
#endif
	};
	ControlMessage(string msgId, string loggerName, string targetId);
	virtual ~ControlMessage();
	virtual void setupFromMavlinkMsg(mavlink_message_t msg) = 0;

	ControlMessageType getConcreteType() const {
		return concreteType;
	}

protected:
	ControlMessageType concreteType;
};

} /* namespace va */

#endif /* CONTROLMESSAGE_H_ */
