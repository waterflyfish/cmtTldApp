/*
 * Algorithm.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: patrick
 */

#include "Algorithm.h"

namespace va {
bool Algorithm::gstreamerFlag=false;
bool Algorithm::trackingFlag=true;
float Algorithm::loc[6];
Algorithm::Algorithm(string algoName, string loggerName) :
				Messanger(algoName, loggerName) {
	sink = NULL;

}

Algorithm::~Algorithm() {
	// TODO Auto-generated destructor stub
}

void Algorithm::init(GstElement* sink) {
	this->sink = sink;
}
void Algorithm::initt(GstElement* sink) {
	this->tee = sink;
}
} /* namespace va */
