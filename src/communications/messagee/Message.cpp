/*
 * Message.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#include "Message.h"
#include "../../tools/MavlinkMessageFactory.h"
#include "../../../config/dummy_config.h"

namespace va {

Message::Message(string msgId, string loggerName, string targetId) :
		Logable(msgId, loggerName) {
	id = msgId;
	this->targetId = targetId;
	type = MessageType::UNKNOWN;
}

Message::~Message() {

}

void Message::logMessage() {

}

mavlink_message_t Message::getMavMsg(int mavSysId, int mavCompId, int targetMavSysId, int targetMavCompId) {
	if(this->type == Message::MessageType::INTERNAL) {
		LOG4CXX_WARN(logger, "Cannot convert Message::MessageType::INTERNAL to a mavlink message");

		throw std::runtime_error("Error - see the logs");
	}
	return getMavlinkMessage(*this, mavSysId, mavCompId, targetMavSysId, targetMavCompId);

}

} /* namespace va */
