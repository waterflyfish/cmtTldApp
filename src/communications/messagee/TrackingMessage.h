/*
 * TrackingMessage.h
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#ifndef TRACKINGMESSAGE_H_
#define TRACKINGMESSAGE_H_

#include "ControlMessage.h"


namespace va {

class TrackingMessage: public ControlMessage {
public:
	TrackingMessage(string msgId, string loggerName, string targetId);
	virtual ~TrackingMessage();
	void setupFromMavlinkMsg(mavlink_message_t msg);

	int getBrX() const {
		return brX;
	}

	int getBrY() const {
		return brY;
	}

	int getTlX() const {
		return tlX;
	}

	int getTlY() const {
		return tlY;
	}

	void setBrX(int brX) {
		this->brX = brX;
	}

	void setBrY(int brY) {
		this->brY = brY;
	}

	bool isStart() const {
		return start;
	}

	void setStart(bool start) {
		this->start = start;
	}

	void setTlX(int tlX) {
		this->tlX = tlX;
	}

	void setTlY(int tlY) {
		this->tlY = tlY;
	}

private:
	int tlX;
	int tlY;
	int brX;
	int brY;
	bool start;
};

} /* namespace va */

#endif /* TRACKINGMESSAGE_H_ */
