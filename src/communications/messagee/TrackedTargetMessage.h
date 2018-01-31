/*
 * TrackedTargetMessage.h
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#ifndef TRACKEDTARGETMESSAGE_H_
#define TRACKEDTARGETMESSAGE_H_

#include "StatusMessage.h"

namespace va {

class TrackedTargetMessage: public StatusMessage {
public:
	TrackedTargetMessage(string msgId, string loggerName, string targetId);
	virtual ~TrackedTargetMessage();

	void setCenterX(int status){
		this->status=status;
	}
	void setCenterY(int num){
		this->num=num;
	}
	int getBrX() const {
		return brX;
	}

	void setBrX(int brX) {
		this->brX = brX;
	}

	int getBrY() const {
		return brY;
	}

	void setBrY(int brY) {
		this->brY = brY;
	}

	int getTlX() const {
		return tlX;
	}

	void setTlX(int tlX) {
		this->tlX = tlX;
	}

	int getTlY() const {
		return tlY;
	}

	void setTlY(int tlY) {
		this->tlY = tlY;
	}
	int getCenterX() const;
	int getCenterY() const;
private:
	int tlX;
	int tlY;
	int brX;
	int brY;
    int status;
    int num;
};

} /* namespace va */

#endif /* TRACKEDTARGETMESSAGE_H_ */
