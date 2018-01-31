/*
 * Messanger.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: patrick
 */

#include "Messanger.h"

namespace va {

Messanger::Messanger(const string& messangerId, string loggerName) : Logable(messangerId, loggerName){
	id = messangerId;

}


Messanger::~Messanger() {

}

void Messanger::setMessageHandleCallback(function<bool(Message*)> callback) {

	msgHandleCallback = callback;
}

bool Messanger::handleMessage(Message* msg) {

	bool reVal = true;
	if(msg->getTargetId() == id) {

		reVal = processMessage(msg);
	} else {

		reVal = msgHandleCallback(msg);
	}

	return reVal;
}

} /* namespace va */
