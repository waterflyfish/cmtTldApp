/*
 * MavlinkMessageFactory.h
 *
 *  Created on: Feb 14, 2017
 *      Author: patrick
 */

#ifndef MAVLINKMESSAGEFACTORY_H_
#define MAVLINKMESSAGEFACTORY_H_
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
#include "../communications/messagee/Message.h"
#include "../communications/messagee/ControlMessage.h"
#include "../communications/messagee/StatusMessage.h"
#include "../communications/messagee/PossibleTargetMessage.h"
#include "../communications/messagee/SystemStatusMessage.h"
#include "../communications/messagee/TrackedTargetMessage.h"
#include "../communications/messagee/TrackingMessage.h"

#include "../communications/messagee/Fpdcmessage.h"
#include "../communications/messagee/Fpdsmessage.h"


namespace va {

static mavlink_message_t getMavlinkMessage(const Message& msg, int mavSysId, int mavCompid, int targetMavSysId, int targetMavCompId) {
	mavlink_message_t mavMsg;
	switch (msg.getType()) {
	case Message::MessageType::CONTROL: {
		const ControlMessage& ctrlMsg = dynamic_cast<const ControlMessage&>(msg);
		switch (ctrlMsg.getConcreteType()) {
		case ControlMessage::ControlMessageType::TRACKING_START: {
			const TrackingMessage& message = dynamic_cast<const TrackingMessage&>(ctrlMsg);
			mavlink_msg_tx1_control_tracking_start_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId, targetMavCompId,
					message.getTlX(), message.getTlY(), message.getBrX(), message.getBrY());
		}
			break;
		case ControlMessage::ControlMessageType::TRACKING_STOP: {
			const TrackingMessage& message = dynamic_cast<const TrackingMessage&>(ctrlMsg);
			mavlink_msg_tx1_control_tracking_stop_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId, targetMavCompId);
		}
			break;
#ifdef ENABLE_MAV_DEBUG_MSG
		case ControlMessage::ControlMessageType::REQ_FP: {
			const Fpdcmessage& message = dynamic_cast<const Fpdcmessage&>(ctrlMsg);
			mavlink_msg_tx1_debug_request_fp_tracked_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId, targetMavCompId,
					message.isStart());
		}
			break;
#endif
		default:
			break;
		}
	}
		break;
	case Message::MessageType::STATUS: {
		const StatusMessage& statMsg = dynamic_cast<const StatusMessage&>(msg);
		switch (statMsg.getConcreteType()) {
		case StatusMessage::StatusMessageType::SYSTEM: {
			const SystemStatusMessage& message = dynamic_cast<const SystemStatusMessage&>(statMsg);
			mavlink_msg_tx1_status_system_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId, targetMavCompId,
					message.isTracking());
		}
			break;
		case StatusMessage::StatusMessageType::POSSIBLE: {
			const PossibleTargetMessage& message = dynamic_cast<const PossibleTargetMessage&>(statMsg);
			mavlink_msg_tx1_status_possible_target_location_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId,
					targetMavCompId, message.getTlX(), message.getTlY(), message.getBrX(), message.getBrY());
		}
			break;
		case StatusMessage::StatusMessageType::TARGET: {
			const TrackedTargetMessage& message = dynamic_cast<const TrackedTargetMessage&>(statMsg);
			mavlink_msg_tx1_status_tracking_target_location_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId,
					targetMavCompId, message.getTlX(), message.getTlY(), message.getBrX(), message.getBrY(), message.getCenterX(),
					message.getCenterY());
		}
			break;
#ifdef ENABLE_MAV_DEBUG_MSG
		case StatusMessage::StatusMessageType::DBG_FP_TR: {
			const Fpdsmessage& message = dynamic_cast<const Fpdsmessage&>(statMsg);
			mavlink_msg_tx1_debug_status_fp_tracked_pack(mavSysId, mavCompid, &mavMsg, targetMavSysId, targetMavCompId,
					message.getSerial(), message.getError(), message.getOrientation(), message.getScale(), message.getStrength(),
					message.getTrackingStatus(), message.getX(), message.getY());
		}
			break;
#endif
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	return mavMsg;
}

}

#endif /* MAVLINKMESSAGEFACTORY_H_ */
