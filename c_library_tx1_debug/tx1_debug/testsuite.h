/** @file
 *    @brief MAVLink comm protocol testsuite generated from tx1_debug.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef TX1_DEBUG_TESTSUITE_H
#define TX1_DEBUG_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_tx1(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_tx1_debug(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_tx1(system_id, component_id, last_msg);
    mavlink_test_tx1_debug(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"
#include "../tx1/testsuite.h"


static void mavlink_test_tx1_debug_request_fp_tracked(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_debug_request_fp_tracked_t packet_in = {
        5,72,139
    };
    mavlink_tx1_debug_request_fp_tracked_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.send = packet_in.send;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_DEBUG_REQUEST_FP_TRACKED_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_request_fp_tracked_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_debug_request_fp_tracked_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_request_fp_tracked_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.send );
    mavlink_msg_tx1_debug_request_fp_tracked_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_request_fp_tracked_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.send );
    mavlink_msg_tx1_debug_request_fp_tracked_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_debug_request_fp_tracked_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_request_fp_tracked_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.send );
    mavlink_msg_tx1_debug_request_fp_tracked_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1_debug_status_fp_tracked(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_debug_status_fp_tracked_t packet_in = {
        17.0,45.0,73.0,101.0,963498296,157.0,185.0,89,156,223
    };
    mavlink_tx1_debug_status_fp_tracked_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.error = packet_in.error;
        packet1.orientation = packet_in.orientation;
        packet1.scale = packet_in.scale;
        packet1.strength = packet_in.strength;
        packet1.tracking_status = packet_in.tracking_status;
        packet1.x = packet_in.x;
        packet1.y = packet_in.y;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.fpset_serial = packet_in.fpset_serial;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_status_fp_tracked_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_debug_status_fp_tracked_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_status_fp_tracked_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.fpset_serial , packet1.error , packet1.orientation , packet1.scale , packet1.strength , packet1.tracking_status , packet1.x , packet1.y );
    mavlink_msg_tx1_debug_status_fp_tracked_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_status_fp_tracked_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.fpset_serial , packet1.error , packet1.orientation , packet1.scale , packet1.strength , packet1.tracking_status , packet1.x , packet1.y );
    mavlink_msg_tx1_debug_status_fp_tracked_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_debug_status_fp_tracked_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_debug_status_fp_tracked_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.fpset_serial , packet1.error , packet1.orientation , packet1.scale , packet1.strength , packet1.tracking_status , packet1.x , packet1.y );
    mavlink_msg_tx1_debug_status_fp_tracked_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1_debug(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_tx1_debug_request_fp_tracked(system_id, component_id, last_msg);
    mavlink_test_tx1_debug_status_fp_tracked(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // TX1_DEBUG_TESTSUITE_H
