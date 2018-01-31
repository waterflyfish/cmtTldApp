/** @file
 *    @brief MAVLink comm protocol testsuite generated from tx1.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef TX1_TESTSUITE_H
#define TX1_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_tx1(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_tx1(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_tx1_status_system(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_STATUS_SYSTEM >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_status_system_t packet_in = {
        5,72,139
    };
    mavlink_tx1_status_system_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        packet1.tracking = packet_in.tracking;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_STATUS_SYSTEM_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_STATUS_SYSTEM_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_system_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_status_system_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_system_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.tracking );
    mavlink_msg_tx1_status_system_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_system_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.tracking );
    mavlink_msg_tx1_status_system_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_status_system_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_system_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.tracking );
    mavlink_msg_tx1_status_system_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1_status_tracking_target_location(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_status_tracking_target_location_t packet_in = {
        17235,17339,17443,17547,17651,17755,41,108
    };
    mavlink_tx1_status_tracking_target_location_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.tl_x = packet_in.tl_x;
        packet1.tl_y = packet_in.tl_y;
        packet1.br_x = packet_in.br_x;
        packet1.br_y = packet_in.br_y;
        packet1.c_x = packet_in.c_x;
        packet1.c_y = packet_in.c_y;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_tracking_target_location_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_status_tracking_target_location_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_tracking_target_location_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y , packet1.c_x , packet1.c_y );
    mavlink_msg_tx1_status_tracking_target_location_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_tracking_target_location_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y , packet1.c_x , packet1.c_y );
    mavlink_msg_tx1_status_tracking_target_location_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_status_tracking_target_location_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_tracking_target_location_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y , packet1.c_x , packet1.c_y );
    mavlink_msg_tx1_status_tracking_target_location_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1_status_possible_target_location(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_STATUS_POSSIBLE_TARGET_LOCATION >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_status_possible_target_location_t packet_in = {
        17235,17339,17443,17547,29,96
    };
    mavlink_tx1_status_possible_target_location_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.tl_x = packet_in.tl_x;
        packet1.tl_y = packet_in.tl_y;
        packet1.br_x = packet_in.br_x;
        packet1.br_y = packet_in.br_y;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_STATUS_POSSIBLE_TARGET_LOCATION_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_STATUS_POSSIBLE_TARGET_LOCATION_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_possible_target_location_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_status_possible_target_location_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_possible_target_location_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y );
    mavlink_msg_tx1_status_possible_target_location_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_possible_target_location_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y );
    mavlink_msg_tx1_status_possible_target_location_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_status_possible_target_location_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_status_possible_target_location_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y );
    mavlink_msg_tx1_status_possible_target_location_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1_control_tracking_start(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_control_tracking_start_t packet_in = {
        17235,17339,17443,17547,29,96
    };
    mavlink_tx1_control_tracking_start_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.tl_x = packet_in.tl_x;
        packet1.tl_y = packet_in.tl_y;
        packet1.br_x = packet_in.br_x;
        packet1.br_y = packet_in.br_y;
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_start_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_control_tracking_start_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_start_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y );
    mavlink_msg_tx1_control_tracking_start_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_start_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y );
    mavlink_msg_tx1_control_tracking_start_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_control_tracking_start_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_start_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component , packet1.tl_x , packet1.tl_y , packet1.br_x , packet1.br_y );
    mavlink_msg_tx1_control_tracking_start_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1_control_tracking_stop(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_tx1_control_tracking_stop_t packet_in = {
        5,72
    };
    mavlink_tx1_control_tracking_stop_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.target_system = packet_in.target_system;
        packet1.target_component = packet_in.target_component;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_stop_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_tx1_control_tracking_stop_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_stop_pack(system_id, component_id, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_tx1_control_tracking_stop_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_stop_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.target_system , packet1.target_component );
    mavlink_msg_tx1_control_tracking_stop_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_tx1_control_tracking_stop_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_tx1_control_tracking_stop_send(MAVLINK_COMM_1 , packet1.target_system , packet1.target_component );
    mavlink_msg_tx1_control_tracking_stop_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_tx1(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_tx1_status_system(system_id, component_id, last_msg);
    mavlink_test_tx1_status_tracking_target_location(system_id, component_id, last_msg);
    mavlink_test_tx1_status_possible_target_location(system_id, component_id, last_msg);
    mavlink_test_tx1_control_tracking_start(system_id, component_id, last_msg);
    mavlink_test_tx1_control_tracking_stop(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // TX1_TESTSUITE_H
