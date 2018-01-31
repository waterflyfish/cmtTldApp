/*
 * SystemController.h
 *
 *  Created on: Jan 10, 2017
 *      Author: patrick
 */

#ifndef SYSTEMCONTROLLER_H_
#define SYSTEMCONTROLLER_H_

#include <string>
#include <vector>
#include <map>
#include <thread>

#include "../tools/Enums.h"
#include "../tools/Messanger.h"
#include "../communications/messagee/Message.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
namespace va {
class SubsystemController;
class SystemController: public Messanger {
public:
//	enum ControllerState {
//		NEW, INIT, RUNNING, STOPPED, DONE
//	};
	SystemController(const string& ctrlName, string loggerName);
	virtual ~SystemController();
	void run();
	ComponentState terminate();
	virtual ComponentState init();
	ComponentState reLoad();
	ComponentState getCtrlState() const {
		return ctrlState;
	}
	void setCtrlState(ComponentState ctrlState);
	virtual bool processMessage(Message* msg);
	virtual std::thread& start();
protected:
	ComponentState stop();

	virtual ComponentState control();
	ComponentState ctrlState;
	bool cycleLimit;
	int cycleCount;
	int cycleMaxCount;
	int cycleSleepTime;
	int intervalCount;
private:

	std::thread thread;
	map<string, SubsystemController*> subsystems;

};
} /* namespace va */

#endif /* SYSTEMCONTROLLER_H_ */
