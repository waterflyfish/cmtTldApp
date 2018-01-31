/*
 * CommunicationsController.h
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#ifndef COMMUNICATIONSCONTROLLER_H_
#define COMMUNICATIONSCONTROLLER_H_


#include "SubsystemController.h"
#include "../communications/CommunicationChannel.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
namespace va {

class CommunicationsController: public SubsystemController {
public:
	CommunicationsController(string ctrlName, string loggerName);
	virtual ~CommunicationsController();
	virtual ComponentState init();
protected:
	SubsystemType subSysType;
	virtual ComponentState control();
	virtual bool processMessage( Message* msg);

private:
	map<string, CommunicationChannel*> comChannels;
};

} /* namespace va */

#endif /* COMMUNICATIONSCONTROLLER_H_ */
