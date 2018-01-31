#pragma once
// MESSAGE TX1_DEBUG_STATUS_FP_TRACKED PACKING

#define MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED 65101

MAVPACKED(
typedef struct __mavlink_tx1_debug_status_fp_tracked_t {
 float error; /*< The tracking method specific error.*/
 float orientation; /*< The orientation of the keypoint.*/
 float scale; /*< The scale of the keypoint.*/
 float strength; /*< The strength of the keypoint.*/
 int32_t tracking_status; /*< The tracking status of the keypoint.*/
 float x; /*< The x coordinate.*/
 float y; /*< The y coordinate.*/
 uint8_t target_system; /*< System ID*/
 uint8_t target_component; /*< Component ID*/
 uint8_t fpset_serial; /*< A Serial that connects fp from the same fpset*/
}) mavlink_tx1_debug_status_fp_tracked_t;

#define MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN 31
#define MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN 31
#define MAVLINK_MSG_ID_65101_LEN 31
#define MAVLINK_MSG_ID_65101_MIN_LEN 31

#define MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC 1
#define MAVLINK_MSG_ID_65101_CRC 1



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_TX1_DEBUG_STATUS_FP_TRACKED { \
    65101, \
    "TX1_DEBUG_STATUS_FP_TRACKED", \
    10, \
    {  { "error", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_tx1_debug_status_fp_tracked_t, error) }, \
         { "orientation", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_tx1_debug_status_fp_tracked_t, orientation) }, \
         { "scale", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_tx1_debug_status_fp_tracked_t, scale) }, \
         { "strength", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_tx1_debug_status_fp_tracked_t, strength) }, \
         { "tracking_status", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_tx1_debug_status_fp_tracked_t, tracking_status) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_tx1_debug_status_fp_tracked_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_tx1_debug_status_fp_tracked_t, y) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 28, offsetof(mavlink_tx1_debug_status_fp_tracked_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 29, offsetof(mavlink_tx1_debug_status_fp_tracked_t, target_component) }, \
         { "fpset_serial", NULL, MAVLINK_TYPE_UINT8_T, 0, 30, offsetof(mavlink_tx1_debug_status_fp_tracked_t, fpset_serial) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_TX1_DEBUG_STATUS_FP_TRACKED { \
    "TX1_DEBUG_STATUS_FP_TRACKED", \
    10, \
    {  { "error", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_tx1_debug_status_fp_tracked_t, error) }, \
         { "orientation", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_tx1_debug_status_fp_tracked_t, orientation) }, \
         { "scale", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_tx1_debug_status_fp_tracked_t, scale) }, \
         { "strength", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_tx1_debug_status_fp_tracked_t, strength) }, \
         { "tracking_status", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_tx1_debug_status_fp_tracked_t, tracking_status) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_tx1_debug_status_fp_tracked_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_tx1_debug_status_fp_tracked_t, y) }, \
         { "target_system", NULL, MAVLINK_TYPE_UINT8_T, 0, 28, offsetof(mavlink_tx1_debug_status_fp_tracked_t, target_system) }, \
         { "target_component", NULL, MAVLINK_TYPE_UINT8_T, 0, 29, offsetof(mavlink_tx1_debug_status_fp_tracked_t, target_component) }, \
         { "fpset_serial", NULL, MAVLINK_TYPE_UINT8_T, 0, 30, offsetof(mavlink_tx1_debug_status_fp_tracked_t, fpset_serial) }, \
         } \
}
#endif

/**
 * @brief Pack a tx1_debug_status_fp_tracked message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param fpset_serial A Serial that connects fp from the same fpset
 * @param error The tracking method specific error.
 * @param orientation The orientation of the keypoint.
 * @param scale The scale of the keypoint.
 * @param strength The strength of the keypoint.
 * @param tracking_status The tracking status of the keypoint.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_debug_status_fp_tracked_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t target_system, uint8_t target_component, uint8_t fpset_serial, float error, float orientation, float scale, float strength, int32_t tracking_status, float x, float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN];
    _mav_put_float(buf, 0, error);
    _mav_put_float(buf, 4, orientation);
    _mav_put_float(buf, 8, scale);
    _mav_put_float(buf, 12, strength);
    _mav_put_int32_t(buf, 16, tracking_status);
    _mav_put_float(buf, 20, x);
    _mav_put_float(buf, 24, y);
    _mav_put_uint8_t(buf, 28, target_system);
    _mav_put_uint8_t(buf, 29, target_component);
    _mav_put_uint8_t(buf, 30, fpset_serial);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN);
#else
    mavlink_tx1_debug_status_fp_tracked_t packet;
    packet.error = error;
    packet.orientation = orientation;
    packet.scale = scale;
    packet.strength = strength;
    packet.tracking_status = tracking_status;
    packet.x = x;
    packet.y = y;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.fpset_serial = fpset_serial;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
}

/**
 * @brief Pack a tx1_debug_status_fp_tracked message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param target_system System ID
 * @param target_component Component ID
 * @param fpset_serial A Serial that connects fp from the same fpset
 * @param error The tracking method specific error.
 * @param orientation The orientation of the keypoint.
 * @param scale The scale of the keypoint.
 * @param strength The strength of the keypoint.
 * @param tracking_status The tracking status of the keypoint.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_tx1_debug_status_fp_tracked_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t target_system,uint8_t target_component,uint8_t fpset_serial,float error,float orientation,float scale,float strength,int32_t tracking_status,float x,float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN];
    _mav_put_float(buf, 0, error);
    _mav_put_float(buf, 4, orientation);
    _mav_put_float(buf, 8, scale);
    _mav_put_float(buf, 12, strength);
    _mav_put_int32_t(buf, 16, tracking_status);
    _mav_put_float(buf, 20, x);
    _mav_put_float(buf, 24, y);
    _mav_put_uint8_t(buf, 28, target_system);
    _mav_put_uint8_t(buf, 29, target_component);
    _mav_put_uint8_t(buf, 30, fpset_serial);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN);
#else
    mavlink_tx1_debug_status_fp_tracked_t packet;
    packet.error = error;
    packet.orientation = orientation;
    packet.scale = scale;
    packet.strength = strength;
    packet.tracking_status = tracking_status;
    packet.x = x;
    packet.y = y;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.fpset_serial = fpset_serial;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
}

/**
 * @brief Encode a tx1_debug_status_fp_tracked struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param tx1_debug_status_fp_tracked C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_debug_status_fp_tracked_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_tx1_debug_status_fp_tracked_t* tx1_debug_status_fp_tracked)
{
    return mavlink_msg_tx1_debug_status_fp_tracked_pack(system_id, component_id, msg, tx1_debug_status_fp_tracked->target_system, tx1_debug_status_fp_tracked->target_component, tx1_debug_status_fp_tracked->fpset_serial, tx1_debug_status_fp_tracked->error, tx1_debug_status_fp_tracked->orientation, tx1_debug_status_fp_tracked->scale, tx1_debug_status_fp_tracked->strength, tx1_debug_status_fp_tracked->tracking_status, tx1_debug_status_fp_tracked->x, tx1_debug_status_fp_tracked->y);
}

/**
 * @brief Encode a tx1_debug_status_fp_tracked struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tx1_debug_status_fp_tracked C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_tx1_debug_status_fp_tracked_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_tx1_debug_status_fp_tracked_t* tx1_debug_status_fp_tracked)
{
    return mavlink_msg_tx1_debug_status_fp_tracked_pack_chan(system_id, component_id, chan, msg, tx1_debug_status_fp_tracked->target_system, tx1_debug_status_fp_tracked->target_component, tx1_debug_status_fp_tracked->fpset_serial, tx1_debug_status_fp_tracked->error, tx1_debug_status_fp_tracked->orientation, tx1_debug_status_fp_tracked->scale, tx1_debug_status_fp_tracked->strength, tx1_debug_status_fp_tracked->tracking_status, tx1_debug_status_fp_tracked->x, tx1_debug_status_fp_tracked->y);
}

/**
 * @brief Send a tx1_debug_status_fp_tracked message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System ID
 * @param target_component Component ID
 * @param fpset_serial A Serial that connects fp from the same fpset
 * @param error The tracking method specific error.
 * @param orientation The orientation of the keypoint.
 * @param scale The scale of the keypoint.
 * @param strength The strength of the keypoint.
 * @param tracking_status The tracking status of the keypoint.
 * @param x The x coordinate.
 * @param y The y coordinate.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_tx1_debug_status_fp_tracked_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint8_t fpset_serial, float error, float orientation, float scale, float strength, int32_t tracking_status, float x, float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN];
    _mav_put_float(buf, 0, error);
    _mav_put_float(buf, 4, orientation);
    _mav_put_float(buf, 8, scale);
    _mav_put_float(buf, 12, strength);
    _mav_put_int32_t(buf, 16, tracking_status);
    _mav_put_float(buf, 20, x);
    _mav_put_float(buf, 24, y);
    _mav_put_uint8_t(buf, 28, target_system);
    _mav_put_uint8_t(buf, 29, target_component);
    _mav_put_uint8_t(buf, 30, fpset_serial);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED, buf, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
#else
    mavlink_tx1_debug_status_fp_tracked_t packet;
    packet.error = error;
    packet.orientation = orientation;
    packet.scale = scale;
    packet.strength = strength;
    packet.tracking_status = tracking_status;
    packet.x = x;
    packet.y = y;
    packet.target_system = target_system;
    packet.target_component = target_component;
    packet.fpset_serial = fpset_serial;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED, (const char *)&packet, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
#endif
}

/**
 * @brief Send a tx1_debug_status_fp_tracked message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_tx1_debug_status_fp_tracked_send_struct(mavlink_channel_t chan, const mavlink_tx1_debug_status_fp_tracked_t* tx1_debug_status_fp_tracked)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_tx1_debug_status_fp_tracked_send(chan, tx1_debug_status_fp_tracked->target_system, tx1_debug_status_fp_tracked->target_component, tx1_debug_status_fp_tracked->fpset_serial, tx1_debug_status_fp_tracked->error, tx1_debug_status_fp_tracked->orientation, tx1_debug_status_fp_tracked->scale, tx1_debug_status_fp_tracked->strength, tx1_debug_status_fp_tracked->tracking_status, tx1_debug_status_fp_tracked->x, tx1_debug_status_fp_tracked->y);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED, (const char *)tx1_debug_status_fp_tracked, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
#endif
}

#if MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_tx1_debug_status_fp_tracked_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t target_system, uint8_t target_component, uint8_t fpset_serial, float error, float orientation, float scale, float strength, int32_t tracking_status, float x, float y)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, error);
    _mav_put_float(buf, 4, orientation);
    _mav_put_float(buf, 8, scale);
    _mav_put_float(buf, 12, strength);
    _mav_put_int32_t(buf, 16, tracking_status);
    _mav_put_float(buf, 20, x);
    _mav_put_float(buf, 24, y);
    _mav_put_uint8_t(buf, 28, target_system);
    _mav_put_uint8_t(buf, 29, target_component);
    _mav_put_uint8_t(buf, 30, fpset_serial);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED, buf, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
#else
    mavlink_tx1_debug_status_fp_tracked_t *packet = (mavlink_tx1_debug_status_fp_tracked_t *)msgbuf;
    packet->error = error;
    packet->orientation = orientation;
    packet->scale = scale;
    packet->strength = strength;
    packet->tracking_status = tracking_status;
    packet->x = x;
    packet->y = y;
    packet->target_system = target_system;
    packet->target_component = target_component;
    packet->fpset_serial = fpset_serial;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED, (const char *)packet, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_MIN_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_CRC);
#endif
}
#endif

#endif

// MESSAGE TX1_DEBUG_STATUS_FP_TRACKED UNPACKING


/**
 * @brief Get field target_system from tx1_debug_status_fp_tracked message
 *
 * @return System ID
 */
static inline uint8_t mavlink_msg_tx1_debug_status_fp_tracked_get_target_system(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  28);
}

/**
 * @brief Get field target_component from tx1_debug_status_fp_tracked message
 *
 * @return Component ID
 */
static inline uint8_t mavlink_msg_tx1_debug_status_fp_tracked_get_target_component(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  29);
}

/**
 * @brief Get field fpset_serial from tx1_debug_status_fp_tracked message
 *
 * @return A Serial that connects fp from the same fpset
 */
static inline uint8_t mavlink_msg_tx1_debug_status_fp_tracked_get_fpset_serial(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  30);
}

/**
 * @brief Get field error from tx1_debug_status_fp_tracked message
 *
 * @return The tracking method specific error.
 */
static inline float mavlink_msg_tx1_debug_status_fp_tracked_get_error(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field orientation from tx1_debug_status_fp_tracked message
 *
 * @return The orientation of the keypoint.
 */
static inline float mavlink_msg_tx1_debug_status_fp_tracked_get_orientation(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field scale from tx1_debug_status_fp_tracked message
 *
 * @return The scale of the keypoint.
 */
static inline float mavlink_msg_tx1_debug_status_fp_tracked_get_scale(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field strength from tx1_debug_status_fp_tracked message
 *
 * @return The strength of the keypoint.
 */
static inline float mavlink_msg_tx1_debug_status_fp_tracked_get_strength(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field tracking_status from tx1_debug_status_fp_tracked message
 *
 * @return The tracking status of the keypoint.
 */
static inline int32_t mavlink_msg_tx1_debug_status_fp_tracked_get_tracking_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field x from tx1_debug_status_fp_tracked message
 *
 * @return The x coordinate.
 */
static inline float mavlink_msg_tx1_debug_status_fp_tracked_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field y from tx1_debug_status_fp_tracked message
 *
 * @return The y coordinate.
 */
static inline float mavlink_msg_tx1_debug_status_fp_tracked_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Decode a tx1_debug_status_fp_tracked message into a struct
 *
 * @param msg The message to decode
 * @param tx1_debug_status_fp_tracked C-struct to decode the message contents into
 */
static inline void mavlink_msg_tx1_debug_status_fp_tracked_decode(const mavlink_message_t* msg, mavlink_tx1_debug_status_fp_tracked_t* tx1_debug_status_fp_tracked)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    tx1_debug_status_fp_tracked->error = mavlink_msg_tx1_debug_status_fp_tracked_get_error(msg);
    tx1_debug_status_fp_tracked->orientation = mavlink_msg_tx1_debug_status_fp_tracked_get_orientation(msg);
    tx1_debug_status_fp_tracked->scale = mavlink_msg_tx1_debug_status_fp_tracked_get_scale(msg);
    tx1_debug_status_fp_tracked->strength = mavlink_msg_tx1_debug_status_fp_tracked_get_strength(msg);
    tx1_debug_status_fp_tracked->tracking_status = mavlink_msg_tx1_debug_status_fp_tracked_get_tracking_status(msg);
    tx1_debug_status_fp_tracked->x = mavlink_msg_tx1_debug_status_fp_tracked_get_x(msg);
    tx1_debug_status_fp_tracked->y = mavlink_msg_tx1_debug_status_fp_tracked_get_y(msg);
    tx1_debug_status_fp_tracked->target_system = mavlink_msg_tx1_debug_status_fp_tracked_get_target_system(msg);
    tx1_debug_status_fp_tracked->target_component = mavlink_msg_tx1_debug_status_fp_tracked_get_target_component(msg);
    tx1_debug_status_fp_tracked->fpset_serial = mavlink_msg_tx1_debug_status_fp_tracked_get_fpset_serial(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN? msg->len : MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN;
        memset(tx1_debug_status_fp_tracked, 0, MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED_LEN);
    memcpy(tx1_debug_status_fp_tracked, _MAV_PAYLOAD(msg), len);
#endif
}
