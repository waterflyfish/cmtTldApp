/*
 * PipelineController.h
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#ifndef PIPELINECONTROLLER_H_
#define PIPELINECONTROLLER_H_

#include "SubsystemController.h"
#include "../algorithm/Algorithm.h"
#include "../pipeline/Pipeline.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
namespace va {

class PipelineController: public SubsystemController {
public:
	PipelineController(string ctrlName, string loggerNam);
	virtual ~PipelineController();

	virtual ComponentState init();
	bool getTrackingState();

protected:
	SubsystemType subSysType;
	virtual ComponentState control();
	virtual bool processMessage(Message* msg);

private:
	bool filePipe;
	bool initializeVideoStreaming();
	bool dummyRunning;
	Algorithm* trackingAlgo;
	Pipeline* pipe;
	Algorithm* detectnetAlgo;
	bool tracking;
};

} /* namespace va */

#endif /* PIPELINECONTROLLER_H_ */
