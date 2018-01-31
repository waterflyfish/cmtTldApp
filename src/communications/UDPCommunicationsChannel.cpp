/*
 * UDPCommunicationsChannel.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#include "UDPCommunicationsChannel.h"
#include "messagee/Message.h"
#include "messagee/TrackingMessage.h"
#include "messagee/Fpdcmessage.h"
#include "../tools/Configuration.h"

#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

namespace va {

UDPCommunicationsChannel::UDPCommunicationsChannel(string channelName, string loggerName) :
		CommunicationChannel(channelName, loggerName) {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

UDPCommunicationsChannel::~UDPCommunicationsChannel() {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

bool UDPCommunicationsChannel::init(const multimap<int, pair<int, int>>& msgCfg) {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif

	//LOG4CXX_ERROR(logger, "["<< id <<"] please call the overloaded function with ip config - no time for implementing default values yet");

	this->msgCfg = msgCfg;

//	try {
//		server_addr.sin_family = AF_INET;
//		server_addr.sin_port = htons(std::stoi(listenPort));
//		inet_aton(listenIp.c_str(), &server_addr.sin_addr);
//		bzero(&(server_addr.sin_zero), 8);
//
//		//set the target ip+port
//
//		target_addr.sin_family = AF_INET;
//		target_addr.sin_port = htons(std::stoi(targetPort));
//		inet_aton(targetIp.c_str(), &target_addr.sin_addr);
//		bzero(&(target_addr.sin_zero), 8);
//	} catch (...) {
//		LOG4CXX_WARN(logger, "["<< id <<"] Could not set the UDP address attribute");
//		return false;
//	}

	setChannelState(ComponentState::NEW);

	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");

	return false;
}

bool UDPCommunicationsChannel::init(const multimap<int, pair<int, int>>& msgCfg, const pair<pair<string, int>, pair<string, int>>& ipCfg) {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif

	this->msgCfg = msgCfg;
	try {

		//LOG4CXX_DEBUG(logger, "["<< id <<"] binding server [" << ipCfg.first.first <<"|" << ipCfg.first.second <<"]");

		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(ipCfg.first.second);
		inet_aton(ipCfg.first.first.c_str(), &server_addr.sin_addr);
		bzero(&(server_addr.sin_zero), 8);

		//set the target ip+port

		//LOG4CXX_DEBUG(logger, "["<< id <<"] binding target [" << ipCfg.second.first <<"|" << ipCfg.second.second <<"]");
//
		target_addr.sin_family = AF_INET;
		target_addr.sin_port = htons(ipCfg.second.second);
		inet_aton(ipCfg.second.first.c_str(), &target_addr.sin_addr);
		bzero(&(target_addr.sin_zero), 8);
	} catch (...) {
		//LOG4CXX_WARN(logger, "["<< id <<"] Could not set the UDP address attribute");
		return false;
	}

	setChannelState(ComponentState::INIT);

	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");

	return true;
}

void UDPCommunicationsChannel::run() {
#ifdef DEBUG_BUILD
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	prctl(PR_SET_NAME, id.c_str(), 0, 0, 0);
	if (isOpen) {
		return;
	}
	isOpen = true;
	setChannelState(ComponentState::RUNNING);
	if ((readSock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket");
		LOG4CXX_ERROR(logger, "["<< id <<"] Bind error");
		exit(1);
	}
	if (bind(readSock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Bind");
		LOG4CXX_ERROR(logger, "["<< id <<"] Bind error");
		exit(1);
	}
	while (isOpen) {
		handleMessage(read());
	}
	close(readSock);
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

Message* UDPCommunicationsChannel::read() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	mavlink_message_t message;
	mavlink_status_t status;
	unsigned int temp = 0;
	int bytes_read;
	socklen_t addr_len;
	char recv_data[1024];
	//TODO the printf things need to be logged right.
	addr_len = sizeof(struct sockaddr);

	bytes_read = recvfrom(readSock, recv_data, 1024, 0, (struct sockaddr *) &server_addr, &addr_len);
printf("%s WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\n",recv_data);
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] XXXXXXXXXXXXXXx: " << bytes_read);
#endif
	recv_data[bytes_read] = '\0';

	//fflush(stdout);
	std::string string = std::string(recv_data);
#ifdef DEBUG_BUILD
	//printf("Datagram is: ");
#endif
	for (int i = 0; i < bytes_read; ++i) {
		temp = recv_data[i];
#ifdef DEBUG_BUILD
		//printf("%02x ", (unsigned char) temp);
#endif
		if (mavlink_parse_char(MAVLINK_COMM_0, recv_data[i], &message, &status)) {
			// Packet received
#ifdef DEBUG_BUILD
			printf("\nReceived message with ID %d, sequence: %d from component %d of system %d\n", message.msgid, message.seq,
					message.compid, message.sysid);
#endif
		}
	}
	Message* msg;
	switch (message.msgid) {
	case MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START: {
		TrackingMessage* cMsg = new TrackingMessage(to_string(MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START), LOG_MSG,
				Configuration::getInstance().getPipelineControllerId());
		cMsg->setupFromMavlinkMsg(message);
		msg = dynamic_cast<Message*>(cMsg);
	}
		break;
	case MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP: {
		TrackingMessage* cMsg = new TrackingMessage(to_string(MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP), LOG_MSG,
				Configuration::getInstance().getPipelineControllerId());
		cMsg->setupFromMavlinkMsg(message);
		msg = dynamic_cast<Message*>(cMsg);
	}
		break;
#ifdef ENABLE_MAV_DEBUG_MSG
	case MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED: {
		Fpdcmessage* cMsg = new Fpdcmessage(
				to_string(MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED), LOG_MSG, Configuration::getInstance().getPipelineControllerId());
		cMsg->setupFromMavlinkMsg(message);
		msg = dynamic_cast<Message*>(cMsg);
	}
		break;
#endif
	default: {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "["<< id <<"] Received something else than a Contorl Message");
#endif
	}
		break;
	}

#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
	return msg;
}

bool UDPCommunicationsChannel::send(Message* msg) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	string msgId = msg->getId();
	try {
		for (auto cfg : msgCfg) {
			//LOG4CXX_DEBUG(logger, "["<< id <<"] cfg.first -> " << cfg.first << " msgId " << msgId);
			if (std::stoi(msgId) == cfg.first) {
				//LOG4CXX_DEBUG(logger, "["<< id <<"] after compare: " << cfg.first << " == " << msgId);

				LOG4CXX_DEBUG(logger, "["<< id <<"] afound configuration for msgId ["<<msgId<< "]. sending msg with mavIds ["<< cfg.second.first<<"|" <<cfg.second.second <<"]");

				//int mavSysId, int mavCompId, int targetMavSysId, int targetMavCompId
				mavlink_message_t msgToSend = msg->getMavMsg(Configuration::getInstance().getMavlinkSystemId(), Configuration::getInstance().getMavlinkComponentId(),cfg.second.first, cfg.second.second);
				uint8_t buf[MAV_BUFFER_LENGTH];
				uint16_t len = mavlink_msg_to_send_buffer(buf, &msgToSend);
				char* reBuf = new char[len];
				for (int i = 0; i < len; ++i) {
					reBuf[i] = buf[i];
				}

				//LOG4CXX_DEBUG(logger, "["<< id <<"] BBBBBBBBBBb");

				int sock;
				if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
					perror("socket");
					return -1;
				}
				sendto(sock, reBuf, len, 0, (struct sockaddr *) &target_addr, sizeof(struct sockaddr));
				close(sock);
			}
		}
	} catch (...) {
		LOG4CXX_DEBUG(logger, "["<< id <<"] catch block");
	}

#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
	return true; //TODO what to return here
}

} /* namespace va */
