/*
 * dummy_config.h
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#ifndef DUMMY_CONFIG_H_
#define DUMMY_CONFIG_H_

#include "videoapp_config.h"

/*
 * THIS FILE ONLY EXISTS TO MAKE THINGS WORK FAST. EVERYTHING IN HERE NEEDS TO BE IMPLEMENTED AS READ FROM CONFIG FILES ETC!
 */

#define VID_WIDTH 1280 //TODO
#define VID_HEIGHT 720 //TODO
#define FRAME_RATE 30 //TODO
#define TRACK_WIDTH 1280 //TODO
#define TRACK_HEIGHT 720 //TODO

#define SOURCE_CAPS "video/x-raw(memory:NVMM), width=(int)" STR(VID_WIDTH) ", height=(int)" STR(VID_HEIGHT) ", format=(string)NV12, framerate=(fraction)" STR(FRAME_RATE) "/1"
#define CONVERT_CAPS "video/x-raw(memory:NVMM), width=(int)" STR(VID_WIDTH) ", height=(int)" STR(VID_HEIGHT) ", format=(string)NV12, framerate=(fraction)" STR(FRAME_RATE) "/1"
#define TRACKING_CONV_CAPS "video/x-raw, format=(string)GRAY8"/*, width=(int)" STR(TRACK_WIDTH) ", height=(int)" STR(TRACK_HEIGHT) */
#define ENCODER_CAPS "video/x-h264, stream-format=(string)byte-stream, bitrate=(int)50000 "//, framerate=" STR(FRAME_RATE) "/1"
#define DETECT_CAPS "video/x-raw(memory:NVMM),width=1280,height=720,format=NV12,framerate=(fraction)30/1"
#define DETECTS_CAPS "video/x-raw,framerate=30/1"
#endif /* DUMMY_CONFIG_H_ */
