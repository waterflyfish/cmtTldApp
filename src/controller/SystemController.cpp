/*
 * SystemController.cpp
 *
 *  Created on: Jan 10, 2017
 *      Author: patrick
 */

#include <sys/prctl.h>
#include <thread>
#include <limits>
#include <vector>

#include "../../config/videoapp_config.h"
#include "SystemController.h"
#include "SubsystemController.h"
#include "PipelineController.h"
#include "CommunicationsController.h"
#include <limits>
#include "../tools/Configuration.h"
#include "../communications/messagee/SystemStatusMessage.h"
#include "../tools/Debug.h"

namespace va {

SystemController::SystemController(const string& ctrlName, string loggerName) :
		Messanger(ctrlName, loggerName) {
	cycleCount = 0;
	cycleSleepTime = -1;
	cycleMaxCount = -1;
	cycleLimit = -1;
	intervalCount = 0;
	ctrlState = ComponentState::NEW;

}

SystemController::~SystemController() {

	if (subsystems.size() > 0) {
		for (auto subsystemCtrl : subsystems) {
			if (subsystemCtrl.second->getCtrlState() != ComponentState::DONE) {
				subsystemCtrl.second->terminate();
			}
			delete subsystemCtrl.second;
		}
	}
}

void SystemController::run() {
	prctl(PR_SET_NAME, id.c_str(), 0, 0, 0);
	if (cycleMaxCount < 1) {
		cycleMaxCount = std::numeric_limits<int>::max();
		cycleCount = std::numeric_limits<int>::min();
	}
	if (!subsystems.empty()) {
		LOG4CXX_DEBUG(logger, "subsys not empty");

		for (auto subSys : subsystems) {
			subSys.second->start();
		}
	}

#ifdef SYSCTRL_LIMIT_CYCLES
	LOG4CXX_WARN(logger,
			"[" << id <<"] is limited to " << cycleMaxCount << " - " << cycleSleepTime << "s cycles ("<< cycleMaxCount * cycleSleepTime <<"s)");
	while (cycleCount < cycleMaxCount && ctrlState != DONE) {
#else
	while (ctrlState != ComponentState::DONE) {
#endif
		if (ctrlState != ComponentState::STOPPED) {

			if (ctrlState == ComponentState::INIT) {
				setCtrlState(ComponentState::RUNNING);
			}
			if (ctrlState == ComponentState::RUNNING) {
				control();
			}
		}
#ifdef SYSCTRL_LIMIT_CYCLES
#ifdef SYSCTRL_PRINT_RUN_CYCLES
		if (cycleCount % SYSCTRL_PRINT_RUN_CYCLES == 0) {
			LOG4CXX_INFO(logger, "[" << id <<"] cycles: " << cycleCount);
#endif
		}
#endif
		++cycleCount;
		std::this_thread::sleep_for(std::chrono::seconds(cycleSleepTime));
#ifdef SYSCTRL_LIMIT_CYCLES
		if (cycleCount == cycleMaxCount) {
			LOG4CXX_INFO(logger, "[" << id <<"] cycle limit "<< cycleMaxCount <<" reached - terminating SystemController");
			terminate();
		}
#endif
		++intervalCount;
		if (intervalCount > 1000) { //this  should avoid overflows of int_max
			intervalCount = 0;
		}
	}
}

ComponentState SystemController::init() {
	this->setMessageHandleCallback(std::bind(&va::SystemController::processMessage, this, placeholders::_1));
	cycleSleepTime = MAINCTRL_SLEEP_CYCLE;
#ifdef SYSCTRL_LIMIT_CYCLES
	cycleMaxCount = MAINCTRL_MAX_CYCLES;
#endif

	SubsystemController* subCtrlP;
	subCtrlP = new PipelineController(Configuration::getInstance().getPipelineControllerId(), LOG_PIPE);
	if (subCtrlP->init() == ComponentState::INIT) {
		dynamic_cast<SystemController*>(subCtrlP)->setMessageHandleCallback(
				std::bind(&va::Messanger::handleMessage, this, placeholders::_1));
		subsystems.insert(pair<string, SubsystemController*>(Configuration::getInstance().getPipelineControllerId(), subCtrlP));
	} else {
		delete subCtrlP;
	}
	SubsystemController* subCtrlC;
	subCtrlC = new CommunicationsController(Configuration::getInstance().getCommunicationsControllerId(), LOG_COM);

	//if (subCtrlC->init() == ComponentState::INIT) {
		//dynamic_cast<SystemController*>(subCtrlC)->setMessageHandleCallback(
				//std::bind(&va::Messanger::handleMessage, this, placeholders::_1));
		//subsystems.insert(pair<string, SubsystemController*>(Configuration::getInstance().getCommunicationsControllerId(), subCtrlC));
	//} else {
		//delete subCtrlC;
	//}

	setCtrlState(ComponentState::INIT);

	return ctrlState;
}

ComponentState SystemController::terminate() {
	if (ctrlState != ComponentState::STOPPED) {
		setCtrlState(stop());
	}
	if (subsystems.size() > 0) {
		for (auto subsystemCtrl : subsystems) {
			subsystemCtrl.second->terminate();
		}
	}

	setCtrlState(ComponentState::DONE);
	return ctrlState;
}

void SystemController::setCtrlState(ComponentState ctrlState) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	if (ctrlState != this->ctrlState) {

#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"] set ctrlState to " << va::getComponentStateString(ctrlState));
#endif
		this->ctrlState = ctrlState;
	}
#ifdef DEBUG_BUILD
	else {
		LOG4CXX_DEBUG(logger, "[" << id <<"] state is already " << va::getComponentStateString(this->ctrlState));
#endif
}
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] exiting");
#endif
}

ComponentState SystemController::stop() {
	setCtrlState(ComponentState::STOPPED);
	return ctrlState;
}

std::thread& SystemController::start() {

	thread = std::thread(&va::SystemController::run, this);

	return thread;

}
ComponentState SystemController::reLoad() {
	stop();
	init();
	return ctrlState;
}

ComponentState SystemController::control() {
	LOG4CXX_DEBUG(logger, "[" << id <<"]");
	if (Configuration::getInstance().getStatusMessagesGenerationConfig().count(MAVLINK_MSG_ID_TX1_STATUS_SYSTEM) > 0) {
		int interval = Configuration::getInstance().getStatusMessagesGenerationConfig().at(MAVLINK_MSG_ID_TX1_STATUS_SYSTEM);
		if (intervalCount % interval == 0) {
			LOG4CXX_DEBUG(logger, "[" << id <<"] intervalCount % interval == 0 -> sending message");
			SystemStatusMessage* sysCtrlMsg = new SystemStatusMessage(to_string(MAVLINK_MSG_ID_TX1_STATUS_SYSTEM), LOG_MSG,
					Configuration::getInstance().getCommunicationsControllerId());
			PipelineController* pipeCtrl = dynamic_cast<PipelineController*>(subsystems.at(
					Configuration::getInstance().getPipelineControllerId()));
			sysCtrlMsg->setTracking(pipeCtrl->getTrackingState());
			handleMessage(dynamic_cast<Message*>(sysCtrlMsg));
			sysCtrlMsg = NULL;
			delete sysCtrlMsg;
		} else {
			LOG4CXX_DEBUG(logger, "[" << id <<"] intervalCount % interval != 0 -> NOT sending message");
		}
	}
	return ctrlState;
}

bool SystemController::processMessage(Message* msg) {
	if (msg->getTargetId() == id) {
		//TODO do something with the message that ius targeted here for this controller
		msg->logMessage(); // TODO this a debug log that is unneccessary here - totally.
	} else {
		for (auto subsys : subsystems) {
			if (subsys.second->getId() == msg->getTargetId() || subsys.second->controlsMessanger(msg->getTargetId())) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"] handing Message to SubsystemController [" << subsys.second->getId() << "]");
#endif
				subsys.second->processMessage(msg);
			}
		}
	}
	return true;

}

} /* namespace va */

