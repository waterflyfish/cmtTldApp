/*
 * Algorithm.h
 *
 *  Created on: Apr 13, 2017
 *      Author: patrick
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_
#include "../tools/Messanger.h"
#include "gst/gst.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
namespace va {

class Algorithm: public Messanger {

public:
	enum class AlgorithmType {
		TRACKING
	};
	static bool gstreamerFlag;
        static bool trackingFlag;
	static float loc[];
	Algorithm(string algoName, string loggerName);
	virtual ~Algorithm();
	void init(GstElement* sink);
	void initt(GstElement* tee);
	AlgorithmType getType() const {
		return type;
	}

	void setType(AlgorithmType type) {
		this->type = type;
	}
protected:
	AlgorithmType type;
	GstElement* sink;
	GstElement* tee;

};

} /* namespace va */

#endif /* ALGORITHM_H_ */
