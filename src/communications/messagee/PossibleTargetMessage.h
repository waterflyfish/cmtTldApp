/*
 * PossibleTargetMessage.h
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#ifndef POSSIBLETARGETMESSAGE_H_
#define POSSIBLETARGETMESSAGE_H_

#include "StatusMessage.h"

namespace va {

class PossibleTargetMessage: public StatusMessage {
public:
	PossibleTargetMessage(string msgId, string loggerName, string targetId);
	virtual ~PossibleTargetMessage();

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

private:
	int tlX;
	int tlY;
	int brX;
	int brY;
};

} /* namespace va */

#endif /* POSSIBLETARGETMESSAGE_H_ */
