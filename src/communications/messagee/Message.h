/*
 * Message.h
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <string>

#include "../../tools/Logable.h"

#ifdef ENABLE_MAV_DEBUG_MSG
#include "c_library_tx1_debug/tx1_debug/mavlink.h"
#else
#include "c_library_tx1/tx1/mavlink.h"
#endif
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
using namespace std;

namespace va {

class Message : public Logable{
public:
	enum class MessageType {
		CONTROL, STATUS, INTERNAL, UNKNOWN
	};
	Message(string msgId, string loggerName, string targetId);
	virtual ~Message();
	virtual void logMessage();
	mavlink_message_t getMavMsg(int mavSysId, int mavCompId, int targetMavSysId, int targetMavCompId);
	const string& getId() const {
		return id;
	}
	const string& getTargetId() const {
		return targetId;
	}
	MessageType getType() const {
		return type;
	}
	void setId(const string& id) {
		this->id = id;
	}
	void setTargetId(const string& targetId) {
		this->targetId = targetId;
	}
	void setType(MessageType type) {
		this->type = type;
	}
protected:
	MessageType type;
	string id;
	string targetId;
};

} /* namespace va */

#endif /* MESSAGE_H_ */
