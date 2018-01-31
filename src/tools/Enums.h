/*
 * ComponentStateEnum.h
 *
 *  Created on: Apr 28, 2017
 *      Author: patrick
 */

#ifndef ENUMS_H_
#define ENUMS_H_

namespace va {
enum class ComponentState {
	NEW, INIT, RUNNING, STOPPED, DONE, ERROR
};

//SubsystemController.h
enum class SubsystemType {
	PIPELINE, COMMUNICATIONS, UNKNOWN
};
//CommunicationChannel.h
enum class CommunicationChannelType {
	UDP, SERIAL, FIFO, UNKNOWN
};

enum class PipelineType {
	CSI, USB, FILE
};
// Algorithm.h
enum class AlgorithmType {
	TRACKING
};
//Message.h
enum class MessageType {
	CONTROL, STATUS, INTERNAL, UNKNOWN
};
//ControlMessage.h
enum class ControlMessageType {
	TRACKING_START, TRACKING_STOP, UNKNOWN
#ifdef ENABLE_MAV_DEBUG_MSG
	,REQ_FP
#endif
};
// StatusMessage.h
enum class StatusMessageType {
	SYSTEM, TARGET, POSSIBLE, UNKNOWN
#ifdef ENABLE_MAV_DEBUG_MSG
	, DBG_FP_TR
#endif
};
}

#endif /* ENUMS_H_ */
