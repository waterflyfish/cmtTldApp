/*
 * DummyPipeline.h
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#ifndef DUMMYPIPELINE_H_
#define DUMMYPIPELINE_H_

/*
 * this class is full of bullshit! its to make thinjgs work - not to be
 * sensefull, nice or whatever. it makes streaming work and tracking.
 * it will be gine soon
 *
 * DO NOT RELAY TO ANY OF THIS CODE! the code is bullshit! thanks - patrick
 */

#include "../tools/Messanger.h"
#include "gst/gst.h"

#include "../communications/messagee/TrackingMessage.h"
#include "../communications/messagee/Message.h"

#include "../../config/dummy_config.h"
#include "Pipeline.h"

#ifdef ENABLE_TRACKING
#include "../analyze/visionWorks/objectTracker/VWObjectTrackerAdapter.h"
#endif


namespace va {

class DummyPipeline: public Pipeline {
public:
	DummyPipeline(string pipeName, string loggerName, string streamIp, int streamPort);
	virtual ~DummyPipeline();
	PipelineState start();
		PipelineState init();
		PipelineState stop();
		bool processMessage(Message* msg);
		GstElement* getSink(string sinkName);
		GstElement* getdetectSink(string sinkName);
		GstElement* getdetectTee(string sinkName);
		PipelineState detectinit();
private:
#ifdef ENABLE_TRACKING
	VWObjectTrackerAdapter* trackingAdapter;
#endif
	GstElement* pipeline;
	GstElement* source;
	GstCaps* sourceCaps;
	GstElement* convert;
	GstCaps* convertCaps;
	GstElement* trackingTee;
	GstElement* trackingTeeQueueTracking;
	GstElement* trackingTeeQueueStream;
	GstElement* trackingConv;
	GstElement* trackingSink;
	GstCaps* trackingConvCaps;
	GstElement* encoder;
	GstCaps* encoderCaps;
	GstElement* payloader;
	GstElement* sink;
	GError* err = NULL;
};

} /* namespace va */

#endif /* DUMMYPIPELINE_H_ */
