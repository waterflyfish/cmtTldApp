/*
 * controller_config.h
 *
 *  Created on: Jan 10, 2017
 *      Author: patrick
 */

#ifndef CONTROLLER_CONFIG_H_
#define CONTROLLER_CONFIG_H_

/******************************************************************************/
/* SystemController configurations */
/******************************************************************************/
//#define SYSCTRL_LIMIT_CYCLES  //turn runtime limitting on/off
#define SYSCTRL_PRINT_RUN_CYCLES 5 // how often print the cycle-count for runtime limits - in seconds

/* Main SystemController configuration */
#define MAINCTRL_SLEEP_CYCLE 5 //how long to sleep after each cycle - in seconds
#define MAINCTRL_MAX_CYCLES 120 //how many cycles to run before terminating

/* CommunicationsController SubsystemController Config */
#define COMCTRL_SLEEP_CYCLE 5 //how long to sleep after each cycle - in seconds
#define COMCTRL_MAX_CYCLES 120 //how many cycles to run before terminating

/* PipelineController SubsystemController Config */
#define PIPECTRL_SLEEP_CYCLE 5 //how long to sleep after each cycle - in seconds
#define PIPECTRL_MAX_CYCLES 120 //how many cycles to run before terminating
/******************************************************************************/

#define COM_CHAN_SLEEP 2

#endif /* CONTROLLER_CONFIG_H_ */
