/*
 * CommunicationChannel.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#include <thread>
#include "CommunicationChannel.h"
#include "../communications/messagee/StatusMessage.h"
#include "../tools/Debug.h"

namespace va {

CommunicationChannel::CommunicationChannel(string channelName, string loggerName) :
		Messanger(channelName, loggerName) {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	isOpen = false;
	chanState = ComponentState::NEW;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

CommunicationChannel::~CommunicationChannel() {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

void CommunicationChannel::run() {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	if (isOpen) {
		return;
	}
	isOpen = true;
	setChannelState(ComponentState::RUNNING);
	while (isOpen) {
		handleMessage(read()); //TODO return value verarbeitung
		std::this_thread::sleep_for(std::chrono::seconds(COM_CHAN_SLEEP)); //TODO is this neccessary? it should be a blocking call in the prev statement
	}
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

bool CommunicationChannel::processMessage(Message* msg) {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	bool retVal = true;
	switch (msg->getType()) {
	case Message::MessageType::STATUS: {
		send(msg);
	}
		break;
	case Message::MessageType::CONTROL:
		//TODO must these messages be send somewhere?
		break;
	default:
		break;
	}
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
	return true;

}

void CommunicationChannel::setChannelState(ComponentState chanState) {

	if (chanState != this->chanState) {

		this->chanState = chanState;
	}

}
}
