/*
 * Pipeline.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: patrick
 */

#include "Pipeline.h"

namespace va {

Pipeline::Pipeline(string pipeName, string loggerName, string streamIp, int streamPort) :
		Messanger(pipeName, loggerName) {
	this->streamIp = streamIp;
	this->streamPort = streamPort;

}

Pipeline::~Pipeline() {
	// TODO Auto-generated destructor stub
}

} /* namespace va */
