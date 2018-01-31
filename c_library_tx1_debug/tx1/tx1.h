/** @file
 *  @brief MAVLink comm protocol generated from tx1.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_TX1_H
#define MAVLINK_TX1_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_TX1.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#undef MAVLINK_THIS_XML_IDX
#define MAVLINK_THIS_XML_IDX 2

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{65000, 237, 3, 3, 0, 1}, {65001, 179, 14, 3, 12, 13}, {65002, 237, 10, 3, 8, 9}, {65003, 81, 10, 3, 8, 9}, {65004, 83, 2, 3, 0, 1}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_TX1

// ENUM DEFINITIONS


/** @brief Tracking state of the videoApp */
#ifndef HAVE_ENUM_TX1_TRACKING_STATE
#define HAVE_ENUM_TX1_TRACKING_STATE
typedef enum TX1_TRACKING_STATE
{
   TX1_TRACKING_STATE_RUNNING=0, /* TX1 is tracking right now | */
   TX1_TRACKING_STATE_STOPPED=1, /* Tracking not running | */
   TX1_TRACKING_STATE_ENUM_END=2, /*  | */
} TX1_TRACKING_STATE;
#endif

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_tx1_status_system.h"
#include "./mavlink_msg_tx1_status_tracking_target_location.h"
#include "./mavlink_msg_tx1_status_possible_target_location.h"
#include "./mavlink_msg_tx1_control_tracking_start.h"
#include "./mavlink_msg_tx1_control_tracking_stop.h"

// base include
#include "../common/common.h"

#undef MAVLINK_THIS_XML_IDX
#define MAVLINK_THIS_XML_IDX 2

#if MAVLINK_THIS_XML_IDX == MAVLINK_PRIMARY_XML_IDX
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_TX1_STATUS_SYSTEM, MAVLINK_MESSAGE_INFO_TX1_STATUS_TRACKING_TARGET_LOCATION, MAVLINK_MESSAGE_INFO_TX1_STATUS_POSSIBLE_TARGET_LOCATION, MAVLINK_MESSAGE_INFO_TX1_CONTROL_TRACKING_START, MAVLINK_MESSAGE_INFO_TX1_CONTROL_TRACKING_STOP}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_TX1_H
