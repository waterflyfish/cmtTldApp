/*
 * PipelineController.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#include "PipelineController.h"

#include "gst/gst.h"
#include "../tools/Configuration.h"
#include "../pipeline/Pipeline.h"
#include "../pipeline/DummyPipeline.h"
#include "../pipeline/FileDummyPipeline.h"
#include "../pipeline/EconCameraPipeline.h"
#include "../algorithm/Algorithm.h"
#include "../algorithm/visionWorks/objectTracker/VWObjectTrackerAdapter.h"
#include "../communications/SerialCommunication.h"
#include "../detectnet-camera/detectnetcamera.h"
namespace va {

PipelineController::PipelineController(string ctrlName, string loggerName) :
		SubsystemController(ctrlName, loggerName) {
	filePipe = false;
	subSysType = SubsystemType::PIPELINE;
	dummyRunning = false;
	trackingAlgo = NULL;
	pipe = NULL;
	tracking = 1;
}

PipelineController::~PipelineController() {

}

ComponentState PipelineController::init() {

	cycleSleepTime = PIPECTRL_SLEEP_CYCLE;
#ifdef SYSCTRL_LIMIT_CYCLES
	cycleMaxCount = PIPECTRL_MAX_CYCLES;
#endif
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] initializing videostreaming");
#endif
	if (!initializeVideoStreaming()) {
		LOG4CXX_ERROR(logger, "[" << id <<"] initializing videostreaming - " << FAIL);
		setCtrlState(ComponentState::NEW);
		//return here
	} else {
		LOG4CXX_INFO(logger, "[" << id <<"] initializing videostreaming - " << SUCCESS);
		setCtrlState(ComponentState::INIT);
	}
#if PIPE_TYPE == 0
	Pipeline::PipelineType pipeType = Pipeline::PipelineType::CSI;
#elif PIPE_TYPE == 1
	Pipeline::PipelineType pipeType = Pipeline::PipelineType::USB;
#endif

	bool preObj = false;

#ifdef DEBUG_BUILD

	if (Configuration::getInstance().dbg_getFilePipeFilePath() != "") {
		pipeType = Pipeline::PipelineType::FILE;
		LOG4CXX_DEBUG(logger, "[" << id <<"] file path set");
	} else {
		LOG4CXX_DEBUG(logger, "[" << id <<"] not file path set: " << Configuration::getInstance().dbg_getFilePipeFilePath());
	}

	tracking = Configuration::getInstance().dbg_getEnableTracking();

	LOG4CXX_DEBUG(logger, "[" << id <<"] tracking is: " << tracking);

#endif
//FIXME move the run function to do not start before that is running
	if (pipeType == Pipeline::PipelineType::USB) {
		EconCameraPipeline* pipe = new EconCameraPipeline("pipeline", LOG_PIPE, Configuration::getInstance().pipe_getStreamTarget().first,
				Configuration::getInstance().pipe_getStreamTarget().second);
		this->pipe = dynamic_cast<Pipeline*>(pipe);
		controlledMessangers.insert(pair<string, Messanger*>("pipe", dynamic_cast<Messanger*>(pipe)));
		pipe->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
		//pipe->detectinit();
		pipe->init();
		pipe->start();
	}
	if (pipeType == Pipeline::PipelineType::CSI) {
		DummyPipeline* pipe = new DummyPipeline("pipeline", LOG_PIPE, Configuration::getInstance().pipe_getStreamTarget().first,
				Configuration::getInstance().pipe_getStreamTarget().second);
		this->pipe = dynamic_cast<Pipeline*>(pipe);
		controlledMessangers.insert(pair<string, Messanger*>("pipe", dynamic_cast<Messanger*>(pipe)));
		pipe->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
		//pipe->detectinit();
		pipe->init();
		pipe->start();
	}
	if (pipeType == Pipeline::PipelineType::FILE) {
		FileDummyPipeline* pipe = new FileDummyPipeline("pipeline", LOG_PIPE, Configuration::getInstance().pipe_getStreamTarget().first,
				Configuration::getInstance().pipe_getStreamTarget().second);
		this->pipe = dynamic_cast<Pipeline*>(pipe);
		controlledMessangers.insert(pair<string, Messanger*>("pipe", dynamic_cast<Messanger*>(pipe)));
		pipe->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
		pipe->detectinit();
                //pipe->init();
		//pipe->start();
	}
		if (tracking) {
 				VWObjectTrackerAdapter* trackingAlgo = new VWObjectTrackerAdapter("tracking", LOG_ANA);
 				SerialCommunication* serialLink = new SerialCommunication("tracking", LOG_ANA);
 				serialLink->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
 				//std::thread serialThread = std::thread(&SerialCommunication::run, serialLink);
 				//serialThread.detach();
 				this->trackingAlgo = dynamic_cast<Algorithm*>(trackingAlgo);
 				controlledMessangers.insert(pair<string, Messanger*>("tracking", dynamic_cast<Messanger*>(trackingAlgo)));
 				trackingAlgo->setSerial(serialLink);
 				trackingAlgo->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
 				trackingAlgo->init(dynamic_cast<Pipeline*>(controlledMessangers.at("pipe"))->getSink("tracking"));
 				std::thread t = std::thread(&VWObjectTrackerAdapter::run, trackingAlgo);
 				t.detach();
 			}
	//===================================================================================
	//add the thread for detectnet
     detectnetcamera* detectnetAlgo = new detectnetcamera("tracking", LOG_ANA);
     this->detectnetAlgo=dynamic_cast<Algorithm*>(detectnetAlgo);
    // controlledMessangers.insert(pair<string, Messanger*>("detectnet", dynamic_cast<Messanger*>(detectnetAlgo)));
     detectnetAlgo->setMessageHandleCallback(std::bind(&va::SystemController::handleMessage, this, placeholders::_1));
     detectnetAlgo->init(dynamic_cast<Pipeline*>(controlledMessangers.at("pipe"))->getdetectSink("mysink"));
     		//std::thread tt = std::thread(&detectnetcamera::run, detectnetAlgo);
     		//tt.detach();

	return ctrlState;
}

ComponentState PipelineController::control() {
	LOG4CXX_DEBUG(logger, "pipe ctrl control");
	return ctrlState;
}

bool PipelineController::initializeVideoStreaming() {
	GError* error = NULL;
	if (!gst_init_check(NULL, NULL, &error)) {
		LOG4CXX_ERROR(logger, "gst_init_check() failed: " << error->message);
		g_error_free(error);
		return false;
	}
	return true;
}

bool PipelineController::processMessage(Message* msg) {
printf("PipelineController::processMessage");
	msg->logMessage(); // TODO this a debug log that is unneccessary here - totally.

	switch (msg->getType()) {
	case Message::MessageType::STATUS: {
	}
		break;
	case Message::MessageType::CONTROL: {

		VWObjectTrackerAdapter* algo = dynamic_cast<VWObjectTrackerAdapter*>(controlledMessangers.at("tracking")); //
		algo->processMessage(msg);

	}
		break;
	default:
		break;
	}
	return true;
}

bool PipelineController::getTrackingState() {
	if (tracking) {
		VWObjectTrackerAdapter* trackingAlgo = dynamic_cast<VWObjectTrackerAdapter*>(this->trackingAlgo);
		return trackingAlgo->isTracking();
	}
	return false;
}

} /* namespace va */

