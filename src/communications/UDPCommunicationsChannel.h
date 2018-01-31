/*
 * UDPCommunicationsChannel.h
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#ifndef UDPCOMMUNICATIONSCHANNEL_H_
#define UDPCOMMUNICATIONSCHANNEL_H_

#include "CommunicationChannel.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
using namespace std;

namespace va {

class UDPCommunicationsChannel : public CommunicationChannel{
public:
	UDPCommunicationsChannel(string channelName, string loggerName);
	virtual ~UDPCommunicationsChannel();

	//bool processMessage(Message* msg);
	Message* read();
	bool send(Message* msg);
	void run();
	bool init(const multimap<int, pair<int, int>>& msgCfg);
	bool init(const multimap<int, pair<int, int>>& msgCfg, const pair<pair<string, int>, pair<string, int>>& ipCfg);
	//void close();


private:
	struct sockaddr_in server_addr;
	struct sockaddr_in target_addr;

	int readSock;

};

} /* namespace va */

#endif /* UDPCOMMUNICATIONSCHANNEL_H_ */
