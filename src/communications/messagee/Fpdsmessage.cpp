/*
 * FeaturePointsTrackedDebugStatusMessage.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: patrick
 */

#include "Fpdsmessage.h"

namespace va {

Fpdsmessage::Fpdsmessage(string msgId, string loggerName, string targetId):
				StatusMessage(msgId, loggerName, targetId) {

	concreteType = StatusMessageType::DBG_FP_TR;
	serial = 0;
	x = -1;
	y = -1;
	error = -1;
	orientation = -1;
	scale = -1;
	strength = -1;
	tracking_status = -1;


}

Fpdsmessage::~Fpdsmessage() {

}

} /* namespace va */

