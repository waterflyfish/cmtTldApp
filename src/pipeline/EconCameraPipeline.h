/*
 * EconCameraPipeline.h
 *
 *  Created on: Apr 20, 2017
 *      Author: patrick
 */

#ifndef ECONCAMERAPIPELINE_H_
#define ECONCAMERAPIPELINE_H_

#include "Pipeline.h"

namespace va {

class EconCameraPipeline: public Pipeline {
public:
	EconCameraPipeline(string pipeName, string loggerName, string streamIp, int streamPort);
	virtual ~EconCameraPipeline();

	PipelineState start();
	PipelineState init();
	PipelineState stop();
	PipelineState detectinit();
	bool processMessage(Message* msg);
	GstElement* getSink(string sinkName);
	GstElement* getdetectSink(string sinkName);
	GstElement* getdetectTee(string sinkName);
//	gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw, width=(int)1920, height=(int)1080, framerate=60/1' ! videoconvert ! omxh264enc
//			! 'video/x-h264, stream-format=(string)byte-stream' ! h264parse ! rtph264pay ! udpsink host=10.42.0.1 port=5000
	GError* err = NULL;
	GstElement* pipeline;
	GstElement* source;
	GstCaps* sourceCaps;

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
	GstElement* detectnetSink;

private:

};

} /* namespace va */

#endif /* ECONCAMERAPIPELINE_H_ */
