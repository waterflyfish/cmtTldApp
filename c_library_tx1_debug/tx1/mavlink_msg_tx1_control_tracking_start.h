#pragma once
// MESSAGE TX1_CONTROL_TRACKING_START PACKING

#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START 65003

MAVPACKED(
typedef struct __mavlink_tx1_control_tracking_start_t {
 int16_t tl_x; /*< top-left x coordinate*/
 int16_t tl_y; /*< top-left y coordinate*/
 int16_t br_x; /*< bottom-right x coordinate*/
 int16_t br_y; /*< bottom-right y coordinate*/
 uint8_t target_system; /*< System ID*/
 uint8_t target_component; /*< Component ID*/
}) mavlink_tx1_control_tracking_start_t;

#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN 10
#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN 10
#define MAVLINK_MSG_ID_65003_LEN 10
#define MAVLINK_MSG_ID_65003_MIN_LEN 10

#define MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC 81
#define MAVLINK_MSG_ID_65003_CRC 81



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_TX1_CONTROL_TRACKING_START { \
    65003, \
    "TX1_CONTROL_TRACKING_START", \
    6, \
    {  { "tl_x", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_tx1_control_tracking_start_t, tl_x) }, \
         { "tl_y", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_tx1_control_tracking_start_t, tl_y) }, \
         { "br_x", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_tx1_control_tracking_start_t, br_x) }, \
         { "br_y", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_tx1_control_tracking_start_t, br_y) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_tx1_control_tracking_start_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_tx1_control_tracking_start_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_TX1_CONTROL_TRACKING_START { \
    "TX1_CONTROL_TRACKING_START", \
    6, \
    {  { "tl_x", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_tx1_control_tracking_start_t, tl_x) }, \
         { "tl_y", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_tx1_control_tracking_start_t, tl_y) }, \
         { "br_x", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_tx1_control_tracking_start_t, br_x) }, \
         { "br_y", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_tx1_control_tracking_start_t, br_y) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_tx1_control_tracking_start_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_tx1_control_tracking_start_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a tx1_control_tracking_start message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param tl_x top-left x coordinate
 * @param tl_y top-left y coordinate
 * @param br_x bottom-right x coordinate
 * @param br_y bottom-right y coordinate
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_start_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, int16_t tl_x, int16_t tl_y, int16_t br_x, int16_t br_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN];
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_uint8_t(buf, 8, target_system);
    _mav_put_uint8_t(buf, 9, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN);
#else
    mavlink_tx1_control_tracking_start_t packet;
    packet.tl_x = tl_x;
    packet.tl_y = tl_y;
    packet.br_x = br_x;
    packet.br_y = br_y;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
}

/**
 * @brief Pack a tx1_control_tracking_start message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 * @param tl_x top-left x coordinate
 * @param tl_y top-left y coordinate
 * @param br_x bottom-right x coordinate
 * @param br_y bottom-right y coordinate
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_start_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,int16_t tl_x,int16_t tl_y,int16_t br_x,int16_t br_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN];
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_uint8_t(buf, 8, target_system);
    _mav_put_uint8_t(buf, 9, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN);
#else
    mavlink_tx1_control_tracking_start_t packet;
    packet.tl_x = tl_x;
    packet.tl_y = tl_y;
    packet.br_x = br_x;
    packet.br_y = br_y;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
}

/**
 * @brief Encode a tx1_control_tracking_start struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param tx1_control_tracking_start C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_start_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_tx1_control_tracking_start_t* tx1_control_tracking_start)
{
    return mavlink_msg_tx1_control_tracking_start_pack(system_id, component_id, msg, tx1_control_tracking_start->target_system, tx1_control_tracking_start->target_component, tx1_control_tracking_start->tl_x, tx1_control_tracking_start->tl_y, tx1_control_tracking_start->br_x, tx1_control_tracking_start->br_y);
}

/**
 * @brief Encode a tx1_control_tracking_start struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tx1_control_tracking_start C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_control_tracking_start_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_tx1_control_tracking_start_t* tx1_control_tracking_start)
{
    return mavlink_msg_tx1_control_tracking_start_pack_chan(system_id, component_id, chan, msg, tx1_control_tracking_start->target_system, tx1_control_tracking_start->target_component, tx1_control_tracking_start->tl_x, tx1_control_tracking_start->tl_y, tx1_control_tracking_start->br_x, tx1_control_tracking_start->br_y);
}

/**
 * @brief Send a tx1_control_tracking_start message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param tl_x top-left x coordinate
 * @param tl_y top-left y coordinate
 * @param br_x bottom-right x coordinate
 * @param br_y bottom-right y coordinate
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_tx1_control_tracking_start_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, int16_t tl_x, int16_t tl_y, int16_t br_x, int16_t br_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN];
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_uint8_t(buf, 8, target_system);
    _mav_put_uint8_t(buf, 9, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START, buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
#else
    mavlink_tx1_control_tracking_start_t packet;
    packet.tl_x = tl_x;
    packet.tl_y = tl_y;
    packet.br_x = br_x;
    packet.br_y = br_y;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START, (const char *)&packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
#endif
}

/**
 * @brief Send a tx1_control_tracking_start message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_tx1_control_tracking_start_send_struct(mavlink_channel_t chan, const mavlink_tx1_control_tracking_start_t* tx1_control_tracking_start)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_tx1_control_tracking_start_send(chan, tx1_control_tracking_start->target_system, tx1_control_tracking_start->target_component, tx1_control_tracking_start->tl_x, tx1_control_tracking_start->tl_y, tx1_control_tracking_start->br_x, tx1_control_tracking_start->br_y);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START, (const char *)tx1_control_tracking_start, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
#endif
}

#if MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_tx1_control_tracking_start_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, int16_t tl_x, int16_t tl_y, int16_t br_x, int16_t br_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_uint8_t(buf, 8, target_system);
    _mav_put_uint8_t(buf, 9, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START, buf, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
#else
    mavlink_tx1_control_tracking_start_t *packet = (mavlink_tx1_control_tracking_start_t *)msgbuf;
    packet->tl_x = tl_x;
    packet->tl_y = tl_y;
    packet->br_x = br_x;
    packet->br_y = br_y;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START, (const char *)packet, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_MIN_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_CRC);
#endif
}
#endif

#endif

// MESSAGE TX1_CONTROL_TRACKING_START UNPACKING


/**
 * @brief Get field target_system from tx1_control_tracking_start message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_tx1_control_tracking_start_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field target_component from tx1_control_tracking_start message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_tx1_control_tracking_start_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field tl_x from tx1_control_tracking_start message
 *
 * @return top-left x coordinate
 */
static inline int16_t mavlink_msg_tx1_control_tracking_start_get_tl_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  0);
}

/**
 * @brief Get field tl_y from tx1_control_tracking_start message
 *
 * @return top-left y coordinate
 */
static inline int16_t mavlink_msg_tx1_control_tracking_start_get_tl_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  2);
}

/**
 * @brief Get field br_x from tx1_control_tracking_start message
 *
 * @return bottom-right x coordinate
 */
static inline int16_t mavlink_msg_tx1_control_tracking_start_get_br_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field br_y from tx1_control_tracking_start message
 *
 * @return bottom-right y coordinate
 */
static inline int16_t mavlink_msg_tx1_control_tracking_start_get_br_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Decode a tx1_control_tracking_start message into a struct
 *
 * @param msg The message to decode
 * @param tx1_control_tracking_start C-struct to decode the message contents into
 */
static inline void mavlink_msg_tx1_control_tracking_start_decode(const mavlink_message_t* msg, mavlink_tx1_control_tracking_start_t* tx1_control_tracking_start)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    tx1_control_tracking_start->tl_x = mavlink_msg_tx1_control_tracking_start_get_tl_x(msg);
    tx1_control_tracking_start->tl_y = mavlink_msg_tx1_control_tracking_start_get_tl_y(msg);
    tx1_control_tracking_start->br_x = mavlink_msg_tx1_control_tracking_start_get_br_x(msg);
    tx1_control_tracking_start->br_y = mavlink_msg_tx1_control_tracking_start_get_br_y(msg);
    tx1_control_tracking_start->target_system = mavlink_msg_tx1_control_tracking_start_get_target_system(msg);
    tx1_control_tracking_start->target_component = mavlink_msg_tx1_control_tracking_start_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN? msg->len : MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN;
        memset(tx1_control_tracking_start, 0, MAVLINK_MSG_ID_TX1_CONTROL_TRACKING_START_LEN);
    memcpy(tx1_control_tracking_start, _MAV_PAYLOAD(msg), len);
#endif
}
