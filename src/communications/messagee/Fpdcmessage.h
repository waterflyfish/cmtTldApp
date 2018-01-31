/*
 * FeaturePointsRequestDebugControlMessage.h
 *
 *  Created on: Mar 4, 2017
 *      Author: patrick
 */
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
#ifdef ENABLE_MAV_DEBUG_MSG

#ifndef FPDCMESSAGE_H_
#define FPDCMESSAGE_H_


#include "ControlMessage.h"
#include "Message.h"

namespace va {

class Fpdcmessage: public ControlMessage {
public:
	Fpdcmessage(string msgId, string loggerName, string targetId);
	virtual ~Fpdcmessage();

	void setupFromMavlinkMsg(mavlink_message_t msg);

	bool isStart() const {
		return start;
	}

	void setStart(bool start) {
		this->start = start;
	}

private:
	bool start;

};

} /* namespace va */


#endif 

#endif
