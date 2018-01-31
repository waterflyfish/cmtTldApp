/*
 * CommunicationChannel.h
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#ifndef COMMUNICATIONCHANNEL_H_
#define COMMUNICATIONCHANNEL_H_

#include <algorithm>
#include <string>

#include "../tools/Enums.h"
#include "../tools/Messanger.h"
#include "../tools/tools.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5

namespace va {

class CommunicationChannel: public Messanger {
public:
	enum CommunicationChannelType {
		UDP, SERIAL, FIFO, UNKNOWN
	};
//	enum CommunicationChannelState {
//		NEW, INIT, RUNNING, CLOSE
//	};
	CommunicationChannel(string channelName, string loggerName);
	virtual ~CommunicationChannel();

	static CommunicationChannelType stringToType(string chanType) {
		std::transform(chanType.begin(), chanType.end(), chanType.begin(), ::tolower);
		switch (str2int(chanType.c_str())) {
		case str2int("udp"):
			return UDP;
			break;
		case str2int("serial"):
			return SERIAL;
			break;
		case str2int("fifo"):
			return FIFO;
			break;
		default:
			return UNKNOWN;
			break;
		}
	}
	;

	virtual void run();
	virtual bool init(const multimap<int, pair<int, int>>& msgCfg) = 0;
	virtual bool processMessage(Message* msg);

	ComponentState getChannelState() const {
		return chanState;
	}
	void setChannelState(ComponentState chanState);
protected:
	virtual Message* read() = 0;
	virtual bool send(Message* msg) = 0;

	ComponentState chanState;
	bool isOpen;
	multimap<int, pair<int, int>> msgCfg;
private:

};

} /* namespace va */

#endif /* COMMUNICATIONCHANNEL_H_ */
