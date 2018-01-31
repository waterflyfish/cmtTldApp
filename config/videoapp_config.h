/*
 * videoapp_config.h
 *
 *  Created on: Jan 4, 2017
 *      Author: patrick
 */

#ifndef VIDEOAPP_CONFIG_H_
#define VIDEOAPP_CONFIG_H_

#include "logger_config.h"
#include "controller_config.h"

// Helper macros
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Some strings for logging output
#define SUCCESS "SUCCESS"
#define FAIL "FAIL"

//Mavlink options
#define MAV_BUFFER_LENGTH 2041

// Config options.
#define LOG4CXX_CONF_FILE_PATH_POS 1 //TODO REMOVE
#define CONF_PATH "/home/nvidia/videoApp/config/va.conf.xml"
#define CONF_SYS "SysConf"
#define CONF_PIPE "PipeConf"
#define CONF_COM "ComConf"
#define CONF_DBG "DbgConf"

// PipelineType options.
#define PIPE_TYPE 0 //1  USB 0 CSI

// Default values for the visionworks object tracker.
#define VXOT_BB_DECREASING_RATIO_DEF 1.0
#define VXOT_PYR_LEVELS_DEF 6
#define VXOT_LK_NUM_ITERS_DEF 20
#define VXOT_LK_WIN_SIZE_DEF 10
#define VXOT_DETECTOR_CELL_SIZE_DEF 3
#define VXOT_MAX_CORNERS_DEF 5000
#define VXOT_FAST_TYPE_DEF 9
#define VXOT_FAST_THRESHOLD_DEF 40
#define VXOT_HARRIS_K_DEF 0.04
#define VXOT_HARRIS_THRESHOLD_DEF 100.0
#define VXOT_MAX_CORNERS_IN_CELL_DEF 5
#define VXOT_X_NUM_OF_CELLS_DEF 2
#define VXOT_Y_NUM_OF_CELLS_DEF 2
#define VXOT_USE_FAST_DETECTOR_DEF true

#endif /* VIDEOAPP_CONFIG_H_ */
