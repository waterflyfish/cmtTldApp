/*
 * FileDummyPipeline.h
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#ifndef FILEDUMMYPIPELINE_H_
#define FILEDUMMYPIPELINE_H_

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



namespace va {

class FileDummyPipeline: public Pipeline {
public:
	FileDummyPipeline(string pipeName, string loggerName, string streamIp, int streamPort);
	virtual ~FileDummyPipeline();
	PipelineState start();
		PipelineState init();
		PipelineState detectinit();
		PipelineState stop();
		bool processMessage(Message* msg);
		GstElement* getSink(string sinkName);
		GstElement* getdetectSink(string sinkName);
		GstElement* getdetectTee(string sinkName);
private:

//	gst-launch-1.0 filesrc location=/home/ubuntu/videoApp/video.mp4 ! decodebin ! nvvidconv !
//	omxh264enc ! "video/x-h264, stream-format=(string)byte-stream"  ! rtph264pay ! udpsink host=10.0.0.66 port=5600

	std::string filePath;
	GstElement* pipeline;
	GstElement* source;
	GstElement* decodeBin;
	GstElement* convert;
	GstCaps* convCaps;
	GstElement* trackingTee;

	GstElement* trackingConv;
	GstElement* trackingSink;
	GstCaps* trackingConvCaps;
	GstElement* encoder;
	GstCaps* encoderCaps;
	GstElement* payloader;
	GstElement* sink;
	//=======================================
		//add the pipeline for detectnet
	GError* err = NULL;
		GstElement* detectnetQueue;
		GstElement* detectnetSink;
		GstElement* detectnetTee;
		GstElement* trackQueue;
		GstElement* mainQueue;
		GstElement* detectnetDecode;
		GstElement* detectnetConv;
		GstElement* detectnetRate;
		GstCaps* detectnetfCaps;
		GstCaps* detectnetsCaps;
	void onBusMessage(GstMessage* msg) {
			switch (GST_MESSAGE_TYPE(msg)) {
			case GST_MESSAGE_EOS:
				//stop();
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"]FUCKING EOS CATCHED");
#endif
				break;
			case GST_MESSAGE_ERROR:
				do {
					gchar* debug;
					GError* error;
					gst_message_parse_error(msg, &error, &debug);
					g_free(debug);
#ifdef DEBUG_BUILD
					LOG4CXX_DEBUG(logger, "[" << id <<"] error" << error->message);
#endif
					g_error_free(error);
				} while (0);
				//stop();
				break;
			default:
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"] SOME BUS MESSAGE");
#endif
				break;
			}
		}
		static gboolean onBusMessage(GstBus* bus, GstMessage* msg, gpointer data) {
			FileDummyPipeline* pThis = (FileDummyPipeline*) data;
			pThis->onBusMessage(msg);
			return TRUE;
		}
};

} /* namespace va */

#endif /* FILEDUMMYPIPELINE_H_ */
