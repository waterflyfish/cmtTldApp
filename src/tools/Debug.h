/*
 * Debug.h
 *
 *  Created on: May 6, 2017
 *      Author: patrick
 */

#ifdef DEBUG_BUILD

#ifndef DEBUG_H_
#define DEBUG_H_

#include <string>
#include "Enums.h"

namespace va {

static string getComponentStateString(ComponentState componentState) {
	switch (componentState) {
	case ComponentState::NEW:
		return "NEW";
		break;
	case ComponentState::INIT:
		return "INIT";
		break;
	case ComponentState::RUNNING:
		return "RUNNING";
		break;
	case ComponentState::STOPPED:
		return "STOPPED";
		break;
	case ComponentState::DONE:
		return "DONE";
		break;
	case ComponentState::ERROR:
		return "ERROR";
		break;
	default:
		break;
	}
}

}

#endif /* DEBUG_H_ */
#endif
