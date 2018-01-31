#pragma once
// MESSAGE TX1_STATUS_TRACKING_TARGET_LOCATION PACKING

#define MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION 65001

MAVPACKED(
typedef struct __mavlink_tx1_status_tracking_target_location_t {
 int16_t tl_x; /*< top-left x coordinate*/
 int16_t tl_y; /*< top-left y coordinate*/
 int16_t br_x; /*< bottom-right x coordinate*/
 int16_t br_y; /*< bottom-right y coordinate*/
 int16_t c_x; /*< center x coordinate*/
 int16_t c_y; /*< center y coordinate*/
 uint8_t target_system; /*< System ID*/
 uint8_t target_component; /*< Component ID*/
}) mavlink_tx1_status_tracking_target_location_t;

#define MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN 14
#define MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN 14
#define MAVLINK_MSG_ID_65001_LEN 14
#define MAVLINK_MSG_ID_65001_MIN_LEN 14

#define MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC 179
#define MAVLINK_MSG_ID_65001_CRC 179



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_TX1_STATUS_TRACKING_TARGET_LOCATION { \
    65001, \
    "TX1_STATUS_TRACKING_TARGET_LOCATION", \
    8, \
    {  { "tl_x", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_tx1_status_tracking_target_location_t, tl_x) }, \
         { "tl_y", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_tx1_status_tracking_target_location_t, tl_y) }, \
         { "br_x", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_tx1_status_tracking_target_location_t, br_x) }, \
         { "br_y", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_tx1_status_tracking_target_location_t, br_y) }, \
         { "c_x", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_tx1_status_tracking_target_location_t, c_x) }, \
         { "c_y", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_tx1_status_tracking_target_location_t, c_y) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_tx1_status_tracking_target_location_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_tx1_status_tracking_target_location_t, target_component) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_TX1_STATUS_TRACKING_TARGET_LOCATION { \
    "TX1_STATUS_TRACKING_TARGET_LOCATION", \
    8, \
    {  { "tl_x", NULL, MAVLINK_TYPE_INT16_T, 0, 0, offsetof(mavlink_tx1_status_tracking_target_location_t, tl_x) }, \
         { "tl_y", NULL, MAVLINK_TYPE_INT16_T, 0, 2, offsetof(mavlink_tx1_status_tracking_target_location_t, tl_y) }, \
         { "br_x", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_tx1_status_tracking_target_location_t, br_x) }, \
         { "br_y", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_tx1_status_tracking_target_location_t, br_y) }, \
         { "c_x", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_tx1_status_tracking_target_location_t, c_x) }, \
         { "c_y", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_tx1_status_tracking_target_location_t, c_y) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_tx1_status_tracking_target_location_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_tx1_status_tracking_target_location_t, target_component) }, \
         } \
}
#endif

/**
 * @brief Pack a tx1_status_tracking_target_location message
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
 * @param c_x center x coordinate
 * @param c_y center y coordinate
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_status_tracking_target_location_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, int16_t tl_x, int16_t tl_y, int16_t br_x, int16_t br_y, int16_t c_x, int16_t c_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN];
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_int16_t(buf, 8, c_x);
    _mav_put_int16_t(buf, 10, c_y);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN);
#else
    mavlink_tx1_status_tracking_target_location_t packet;
    packet.tl_x = tl_x;
    packet.tl_y = tl_y;
    packet.br_x = br_x;
    packet.br_y = br_y;
    packet.c_x = c_x;
    packet.c_y = c_y;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
}

/**
 * @brief Pack a tx1_status_tracking_target_location message on a channel
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
 * @param c_x center x coordinate
 * @param c_y center y coordinate
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_status_tracking_target_location_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,int16_t tl_x,int16_t tl_y,int16_t br_x,int16_t br_y,int16_t c_x,int16_t c_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN];
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_int16_t(buf, 8, c_x);
    _mav_put_int16_t(buf, 10, c_y);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN);
#else
    mavlink_tx1_status_tracking_target_location_t packet;
    packet.tl_x = tl_x;
    packet.tl_y = tl_y;
    packet.br_x = br_x;
    packet.br_y = br_y;
    packet.c_x = c_x;
    packet.c_y = c_y;
    packet.target_system = target_system;
    packet.target_component = target_component;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
}

/**
 * @brief Encode a tx1_status_tracking_target_location struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param tx1_status_tracking_target_location C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_status_tracking_target_location_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_tx1_status_tracking_target_location_t* tx1_status_tracking_target_location)
{
    return mavlink_msg_tx1_status_tracking_target_location_pack(system_id, component_id, msg, tx1_status_tracking_target_location->target_system, tx1_status_tracking_target_location->target_component, tx1_status_tracking_target_location->tl_x, tx1_status_tracking_target_location->tl_y, tx1_status_tracking_target_location->br_x, tx1_status_tracking_target_location->br_y, tx1_status_tracking_target_location->c_x, tx1_status_tracking_target_location->c_y);
}

/**
 * @brief Encode a tx1_status_tracking_target_location struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tx1_status_tracking_target_location C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_status_tracking_target_location_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_tx1_status_tracking_target_location_t* tx1_status_tracking_target_location)
{
    return mavlink_msg_tx1_status_tracking_target_location_pack_chan(system_id, component_id, chan, msg, tx1_status_tracking_target_location->target_system, tx1_status_tracking_target_location->target_component, tx1_status_tracking_target_location->tl_x, tx1_status_tracking_target_location->tl_y, tx1_status_tracking_target_location->br_x, tx1_status_tracking_target_location->br_y, tx1_status_tracking_target_location->c_x, tx1_status_tracking_target_location->c_y);
}

/**
 * @brief Send a tx1_status_tracking_target_location message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param tl_x top-left x coordinate
 * @param tl_y top-left y coordinate
 * @param br_x bottom-right x coordinate
 * @param br_y bottom-right y coordinate
 * @param c_x center x coordinate
 * @param c_y center y coordinate
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_tx1_status_tracking_target_location_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, int16_t tl_x, int16_t tl_y, int16_t br_x, int16_t br_y, int16_t c_x, int16_t c_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN];
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_int16_t(buf, 8, c_x);
    _mav_put_int16_t(buf, 10, c_y);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION, buf, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
#else
    mavlink_tx1_status_tracking_target_location_t packet;
    packet.tl_x = tl_x;
    packet.tl_y = tl_y;
    packet.br_x = br_x;
    packet.br_y = br_y;
    packet.c_x = c_x;
    packet.c_y = c_y;
    packet.target_system = target_system;
    packet.target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION, (const char *)&packet, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
#endif
}

/**
 * @brief Send a tx1_status_tracking_target_location message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_tx1_status_tracking_target_location_send_struct(mavlink_channel_t chan, const mavlink_tx1_status_tracking_target_location_t* tx1_status_tracking_target_location)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_tx1_status_tracking_target_location_send(chan, tx1_status_tracking_target_location->target_system, tx1_status_tracking_target_location->target_component, tx1_status_tracking_target_location->tl_x, tx1_status_tracking_target_location->tl_y, tx1_status_tracking_target_location->br_x, tx1_status_tracking_target_location->br_y, tx1_status_tracking_target_location->c_x, tx1_status_tracking_target_location->c_y);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION, (const char *)tx1_status_tracking_target_location, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
#endif
}

#if MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_tx1_status_tracking_target_location_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, int16_t tl_x, int16_t tl_y, int16_t br_x, int16_t br_y, int16_t c_x, int16_t c_y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int16_t(buf, 0, tl_x);
    _mav_put_int16_t(buf, 2, tl_y);
    _mav_put_int16_t(buf, 4, br_x);
    _mav_put_int16_t(buf, 6, br_y);
    _mav_put_int16_t(buf, 8, c_x);
    _mav_put_int16_t(buf, 10, c_y);
    _mav_put_uint8_t(buf, 12, target_system);
    _mav_put_uint8_t(buf, 13, target_component);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION, buf, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
#else
    mavlink_tx1_status_tracking_target_location_t *packet = (mavlink_tx1_status_tracking_target_location_t *)msgbuf;
    packet->tl_x = tl_x;
    packet->tl_y = tl_y;
    packet->br_x = br_x;
    packet->br_y = br_y;
    packet->c_x = c_x;
    packet->c_y = c_y;
    packet->target_system = target_system;
    packet->target_component = target_component;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION, (const char *)packet, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_MIN_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_CRC);
#endif
}
#endif

#endif

// MESSAGE TX1_STATUS_TRACKING_TARGET_LOCATION UNPACKING


/**
 * @brief Get field target_system from tx1_status_tracking_target_location message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_tx1_status_tracking_target_location_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field target_component from tx1_status_tracking_target_location message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_tx1_status_tracking_target_location_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Get field tl_x from tx1_status_tracking_target_location message
 *
 * @return top-left x coordinate
 */
static inline int16_t mavlink_msg_tx1_status_tracking_target_location_get_tl_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  0);
}

/**
 * @brief Get field tl_y from tx1_status_tracking_target_location message
 *
 * @return top-left y coordinate
 */
static inline int16_t mavlink_msg_tx1_status_tracking_target_location_get_tl_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  2);
}

/**
 * @brief Get field br_x from tx1_status_tracking_target_location message
 *
 * @return bottom-right x coordinate
 */
static inline int16_t mavlink_msg_tx1_status_tracking_target_location_get_br_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field br_y from tx1_status_tracking_target_location message
 *
 * @return bottom-right y coordinate
 */
static inline int16_t mavlink_msg_tx1_status_tracking_target_location_get_br_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Get field c_x from tx1_status_tracking_target_location message
 *
 * @return center x coordinate
 */
static inline int16_t mavlink_msg_tx1_status_tracking_target_location_get_c_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field c_y from tx1_status_tracking_target_location message
 *
 * @return center y coordinate
 */
static inline int16_t mavlink_msg_tx1_status_tracking_target_location_get_c_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Decode a tx1_status_tracking_target_location message into a struct
 *
 * @param msg The message to decode
 * @param tx1_status_tracking_target_location C-struct to decode the message contents into
 */
static inline void mavlink_msg_tx1_status_tracking_target_location_decode(const mavlink_message_t* msg, mavlink_tx1_status_tracking_target_location_t* tx1_status_tracking_target_location)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    tx1_status_tracking_target_location->tl_x = mavlink_msg_tx1_status_tracking_target_location_get_tl_x(msg);
    tx1_status_tracking_target_location->tl_y = mavlink_msg_tx1_status_tracking_target_location_get_tl_y(msg);
    tx1_status_tracking_target_location->br_x = mavlink_msg_tx1_status_tracking_target_location_get_br_x(msg);
    tx1_status_tracking_target_location->br_y = mavlink_msg_tx1_status_tracking_target_location_get_br_y(msg);
    tx1_status_tracking_target_location->c_x = mavlink_msg_tx1_status_tracking_target_location_get_c_x(msg);
    tx1_status_tracking_target_location->c_y = mavlink_msg_tx1_status_tracking_target_location_get_c_y(msg);
    tx1_status_tracking_target_location->target_system = mavlink_msg_tx1_status_tracking_target_location_get_target_system(msg);
    tx1_status_tracking_target_location->target_component = mavlink_msg_tx1_status_tracking_target_location_get_target_component(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN? msg->len : MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN;
        memset(tx1_status_tracking_target_location, 0, MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION_LEN);
    memcpy(tx1_status_tracking_target_location, _MAV_PAYLOAD(msg), len);
#endif
}
