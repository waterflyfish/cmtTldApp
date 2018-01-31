/*
 * Messanger.h
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#ifndef MESSANGER_H_
#define MESSANGER_H_

#include <functional>

#include "../communications/messagee/Message.h"
#include "Logable.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
using namespace std;

namespace va {
class Messanger : public Logable{
public:
	Messanger(const string& messangerId, string loggerName);
	virtual ~Messanger();
	void setMessageHandleCallback(function<bool(Message*)> callback);
	virtual bool handleMessage(Message* msg);
	const string& getId() const {
		return id;
	}

protected:
	virtual bool processMessage(Message* msg) = 0;
	function<bool(Message*)> msgHandleCallback;
	string id;
};

} /* namespace va */

#endif /* MESSANGER_H_ */
