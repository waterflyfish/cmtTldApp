#pragma once
// MESSAGE TX1_CONTROL_TRACKING_STOP PACKING

#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP 65004

MAVPACKED(
typedef struct __mavlink_tx1_control_tracking_stop_t {
 uint8_t target_system; /*< System ID*/
 uint8_t target_component; /*< Component ID*/
}) mavlink_tx1_control_tracking_stop_t;

#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN 2
#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN 2
#define MAVLINK_MSG_ID_65004_LEN 2
#define MAVLINK_MSG_ID_65004_MIN_LEN 2

#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC 83
#define MAVLINK_MSG_ID_65004_CRC 83



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_TX1_CONTROL_TRACKING_STOP { \
    65004, \
    "TX1_CONTROL_TRACKING_STOP", \
    2, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_tx1_control_tracking_stop_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_tx1_control_tracking_stop_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_TX1_CONTROL_TRACKING_STOP { \
    "TX1_CONTROL_TRACKING_STOP", \
    2, \
    {  { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_tx1_control_tracking_stop_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_tx1_control_tracking_stop_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a tx1_control_tracking_stop message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_stop_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN);
#else
    mavlink_tx1_control_tracking_stop_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
}

/**
 * @brief Pack a tx1_control_tracking_stop message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_stop_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN);
#else
    mavlink_tx1_control_tracking_stop_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
}

/**
 * @brief Encode a tx1_control_tracking_stop struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param tx1_control_tracking_stop C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_stop_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_tx1_control_tracking_stop_t* tx1_control_tracking_stop)
{
    return mavlink_msg_tx1_control_tracking_stop_pack(system_id, component_id, msg, tx1_control_tracking_stop->target_system, tx1_control_tracking_stop->target_component);
}

/**
 * @brief Encode a tx1_control_tracking_stop struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tx1_control_tracking_stop C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_stop_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_tx1_control_tracking_stop_t* tx1_control_tracking_stop)
{
    return mavlink_msg_tx1_control_tracking_stop_pack_chan(system_id, component_id, chan, msg, tx1_control_tracking_stop->target_system, tx1_control_tracking_stop->target_component);
}

/**
 * @brief Send a tx1_control_tracking_stop message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_tx1_control_tracking_stop_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN];
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP, buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
#else
    mavlink_tx1_control_tracking_stop_t packet;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP, (const char *)&packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
#endif
}

/**
 * @brief Send a tx1_control_tracking_stop message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_tx1_control_tracking_stop_send_struct(mavlink_channel_t chan, const mavlink_tx1_control_tracking_stop_t* tx1_control_tracking_stop)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_tx1_control_tracking_stop_send(chan, tx1_control_tracking_stop->target_system, tx1_control_tracking_stop->target_component);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP, (const char *)tx1_control_tracking_stop, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
#endif
}

#if MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_tx1_control_tracking_stop_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, target_system);
    _mav_put_uint8_t(buf, 1, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP, buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
#else
    mavlink_tx1_control_tracking_stop_t *packet = (mavlink_tx1_control_tracking_stop_t *)msgbuf;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP, (const char *)packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_CRC);
#endif
}
#endif

#endif

// MESSAGE TX1_CONTROL_TRACKING_STOP UNPACKING


/**
 * @brief Get field target_system from tx1_control_tracking_stop message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_tx1_control_tracking_stop_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field target_component from tx1_control_tracking_stop message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_tx1_control_tracking_stop_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Decode a tx1_control_tracking_stop message into a struct
 *
 * @param msg The message to decode
 * @param tx1_control_tracking_stop C-struct to decode the message contents into
 */
static inline void mavlink_msg_tx1_control_tracking_stop_decode(const mavlink_message_t* msg, mavlink_tx1_control_tracking_stop_t* tx1_control_tracking_stop)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    tx1_control_tracking_stop->target_system = mavlink_msg_tx1_control_tracking_stop_get_target_system(msg);
    tx1_control_tracking_stop->target_component = mavlink_msg_tx1_control_tracking_stop_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN? msg->len : MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN;
        memset(tx1_control_tracking_stop, 0, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_STOP_LEN);
    memcpy(tx1_control_tracking_stop, _MAV_PAYLOAD(msg), len);
#endif
}
