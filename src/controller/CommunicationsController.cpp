/*
 * CommunicationsController.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#include <thread>
#include "../tools/Configuration.h"
#include "CommunicationsController.h"
#include "../communications/messagee/StatusMessage.h"
#include "../communications/UDPCommunicationsChannel.h"
#include "../communications/CommunicationChannel.h"
#include "../../config/dummy_config.h"

namespace va {

CommunicationsController::CommunicationsController(string ctrlName, string loggerName) :
		SubsystemController(ctrlName, loggerName) {

	subSysType = SubsystemType::COMMUNICATIONS;

}

CommunicationsController::~CommunicationsController() {

}

ComponentState CommunicationsController::init() {

	cycleSleepTime = COMCTRL_SLEEP_CYCLE;
#ifdef SYSCTRL_LIMIT_CYCLES
	cycleMaxCount = COMCTRL_MAX_CYCLES;
#endif

	map<string, pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>> > udpCfg = Configuration::getInstance().com_getUdpChannelConfigurations();

	if (!udpCfg.empty()) {
		for (auto chanCfg : udpCfg) {
			string chanId = chanCfg.first;
			LOG4CXX_DEBUG(logger, "Processing UDPCommunicationChannel [" << chanId <<"]");
			pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>> setupParams = chanCfg.second;
			pair<pair<string, int>, pair<string, int>> ipCfg = setupParams.first;
			pair<string, int> listenIpCfg = ipCfg.first;
			pair<string, int> targetIpCfg = ipCfg.second;
			LOG4CXX_DEBUG(logger, "[" << chanId <<"] listen config: [" << listenIpCfg.first <<":" << listenIpCfg.second << "]");
			LOG4CXX_DEBUG(logger, "[" << chanId <<"] target config: [" << targetIpCfg.first <<":" << targetIpCfg.second << "]");
			multimap<int, pair<int, int>> msgCfg = setupParams.second;

			UDPCommunicationsChannel* channel = new UDPCommunicationsChannel(chanId, LOG_COM);
			channel->init(msgCfg, ipCfg);
			channel->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
			comChannels.insert(pair<string, CommunicationChannel*>(chanId, dynamic_cast<CommunicationChannel*>(channel)));
			controlledMessangers.insert(pair<string, Messanger*>(chanId, dynamic_cast<Messanger*>(channel)));

		}
	}

//
//	const auto rootNode = config->get_root_node();
//	Element* rootElem = dynamic_cast<Element*>(rootNode);
//	NodeSet chans = rootNode->find(string("//*[name()='").append(XML_COM_CHAN_ELEM_NAME).append("']"));
//	for (auto chan : chans) {
//		Element* chanElem = dynamic_cast<Element*>(chan);
//		string chanId = chanElem->get_attribute_value("id");
//		string chanType = chanElem->get_attribute_value("type");
//
//		string useMav = chanElem->get_attribute_value("mav");
//		string sendDebug = chanElem->get_attribute_value("debug");
//		bool useMav_b = false;
//		bool sendDebug_b = false;
//		if(useMav == "1") {
//			useMav_b = true;
//		}
//		if (sendDebug == "1") {
//			sendDebug_b = true;
//		}
//
//#ifdef DEBUG_BUILD
//
//#endif
//
//#ifdef DEBUG_BUILD
//		LOG4CXX_DEBUG(logger, "Processing CommunicationChannel [id/type] --> [" << chanId << "/" << chanType <<"]");
//#endif
//		CommunicationChannel* channel = NULL;
//		switch (CommunicationChannel::stringToType(chanType)) {
//		case CommunicationChannel::UDP:
//
//			channel = new UDPCommunicationsChannel(chanId, LOG_COM, useMav_b, sendDebug_b);
//			break;
//		default:
//			//TODO log error, break
//			break;
//		}
//		if (channel != NULL) {
//			if (!channel->init(chanElem)) {
//#ifdef DEBUG_BUILD
//				LOG4CXX_DEBUG(logger, "Error initalizing CommunicationChannel [id/type] --> [" << chanId << "/" << chanType <<"]");
//#endif
//				//delete channel;
//			} else {
//				channel->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
//				comChannels.insert(pair<string, CommunicationChannel*>(chanId, channel));
//				controlledMessangers.insert(pair<string, Messanger*>(chanId, dynamic_cast<Messanger*>(channel)));
//			}
//		} else {
//#ifdef DEBUG_BUILD
//			LOG4CXX_DEBUG(logger,
//					"Cannot process CommunicationChannel [id/type] --> [" << chanId << "/" << chanType <<"] - no implementation for type [" << chanType <<"]");
//#endif
//		}
//	}
//	LOG4CXX_INFO(logger, "[" << id <<"] Successfully initialized " << comChannels.size() << " CommunicationChannels");
	setCtrlState(ComponentState::INIT);

	return ctrlState;
}

ComponentState CommunicationsController::control() {

	std::thread t;
	for (auto chan : comChannels) {
		switch (chan.second->getChannelState()) {
		case ComponentState::NEW:
			LOG4CXX_WARN(logger, "[" << id <<"] CommunicationsChannel [" << chan.first <<"] state is NEW!")
			; //TODO use translatestate function!
			break;
		case ComponentState::INIT: {

			chan.second->setChannelState(ComponentState::RUNNING);
			t = std::thread(&va::CommunicationChannel::run, chan.second);
			t.detach();
		}
			break;
		case ComponentState::RUNNING:

			break;
		default:
			break;
		}
	}
	LOG4CXX_DEBUG(logger, "com ctrl control");
	return ctrlState;
}

bool CommunicationsController::processMessage(Message* msg) {
	LOG4CXX_DEBUG(logger, "com ctrl process msg");
	msg->logMessage(); // TODO this a debug log that is unneccessary here - totally.
	switch (msg->getType()) {
	case Message::MessageType::STATUS: {
		for (auto chan : comChannels) {
			msg->setTargetId(chan.first);
			chan.second->processMessage(msg);
		}
	}
		break;
	case Message::MessageType::CONTROL: {
	}

		break;
	default:
		break;
	}
	return true;
}

} /* namespace va */

