/*
 * StatusMessage.h
 *
 *  Created on: Jan 23, 2017
 *      Author: patrick
 */

#ifndef STATUSMESSAGE_H_
#define STATUSMESSAGE_H_

#include "Message.h"

namespace va {

class StatusMessage: public Message {
public:
	enum class StatusMessageType {
		SYSTEM, TARGET, POSSIBLE, UNKNOWN
#ifdef ENABLE_MAV_DEBUG_MSG
		, DBG_FP_TR
#endif
	};
	StatusMessage(string msgId, string loggerName, string targetId);
	virtual ~StatusMessage();

	StatusMessageType getConcreteType() const {
		return concreteType;
	}

protected:
	StatusMessageType concreteType;
};

} /* namespace va */

#endif /* STATUSMESSAGE_H_ */
