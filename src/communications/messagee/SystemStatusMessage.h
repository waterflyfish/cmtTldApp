/*
 * SystemStatusMessage.h
 *
 *  Created on: Jan 25, 2017
 *      Author: patrick
 */

#ifndef SYSTEMSTATUSMESSAGE_H_
#define SYSTEMSTATUSMESSAGE_H_

#include "StatusMessage.h"

namespace va {

class SystemStatusMessage: public StatusMessage {
public:
	SystemStatusMessage(string msgId, string loggerName, string targetId);
	virtual ~SystemStatusMessage();

	bool isTracking() const {
		return tracking;
	}

	void setTracking(bool tracking) {
		this->tracking = tracking;
	}

	bool tracking;
};

} /* namespace va */

#endif /* SYSTEMSTATUSMESSAGE_H_ */
