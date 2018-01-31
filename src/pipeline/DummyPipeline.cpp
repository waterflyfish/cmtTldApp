/*
 * DummyPipeline.cpp
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#include "DummyPipeline.h"
#include "../../config/logger_config.h"
#include <thread>
#include <gst/app/gstappsink.h>
namespace va {

DummyPipeline::DummyPipeline(string pipeName, string loggerName, string streamIp, int streamPort) :
		Pipeline(pipeName, loggerName, streamIp, streamPort) {

	pipeline = NULL;
	source = NULL;
	sourceCaps = NULL;
	convert = NULL;
	trackingTee = NULL;
	trackingTeeQueueTracking = NULL;
	trackingTeeQueueStream = NULL;
	trackingConv = NULL;
	trackingSink = NULL;
	trackingConvCaps = NULL;
	encoder = NULL;
	encoderCaps = NULL;
	payloader = NULL;
	convertCaps = NULL;
	sink = NULL;
#ifdef ENABLE_TRACKING
	trackingAdapter = NULL;
#endif
}

DummyPipeline::~DummyPipeline() {

}

Pipeline::PipelineState DummyPipeline::init() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	if ((pipeline = gst_pipeline_new("PIPELINE")) == NULL) { //well yeah - fuckup with the name - fast fast fast it needs to work
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with Error with gst_pipeline_new()");
#endif
		return DONE;
	}
	/******************************************************************************/
	// camera input & convert flip picture
	/******************************************************************************/
	if ((source = gst_element_factory_make("nvcamerasrc", "source")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('v4l2src')");

#endif
		return DONE;
	}
	if ((sourceCaps = gst_caps_from_string(SOURCE_CAPS)) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_caps_from_string(sourceCaps)");
#endif
		return DONE;
	}

	if ((convertCaps = gst_caps_from_string(CONVERT_CAPS)) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_caps_from_string()");
#endif
		return DONE;
	}

	if ((trackingTeeQueueStream = gst_element_factory_make("queue", "trackingTeeQueueStream")) == NULL) {
		LOG4CXX_DEBUG(logger, "TTSimpleDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTeeQueueStream')");
		return DONE;
	}
	if ((trackingTeeQueueTracking = gst_element_factory_make("queue", "trackingTeeQueueTracking")) == NULL) {
		LOG4CXX_DEBUG(logger, "TTSimpleDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTeeQueueTracking')");
		return DONE;
	}




	/******************************************************************************/
	//tee splitting tracking and stream
	/******************************************************************************/
	if ((trackingTee = gst_element_factory_make("tee", "trackingTee")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
#endif
		return DONE;
	}
	/******************************************************************************/
	//tracking part after the tee
	/******************************************************************************/

	if ((trackingConv = gst_element_factory_make("nvvidconv", "trackingConv")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingConv')");
#endif
		return DONE;
	}
	if ((trackingConvCaps = gst_caps_from_string(TRACKING_CONV_CAPS)) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_caps_from_string(trackingConvCaps)");
#endif
		return DONE;
	}

	if ((trackingSink = gst_element_factory_make("appsink", "trackingSink")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingSink')");
#endif
		return DONE;
	}
	/******************************************************************************/
	//streaming part after the tee
	/******************************************************************************/
	if ((convert = gst_element_factory_make("nvvidconv", "convert")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('convert')");
#endif
		return DONE;
	}
	if ((encoder = gst_element_factory_make("omxh264enc", "encoder")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('omxh264enc')");
#endif
		return DONE;
	}
	if ((encoderCaps = gst_caps_from_string(ENCODER_CAPS)) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_caps_from_string()");
#endif
		return DONE;
	}
	if ((payloader = gst_element_factory_make("rtph264pay", "payloader")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('rtph264pay')");
#endif
		return DONE;
	}
	if ((sink = gst_element_factory_make("udpsink", "sink")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('udpsink')");
#endif
		return DONE;
	}
	/******************************************************************************/
	LOG4CXX_INFO(logger, "[" << id <<"]INIT " << SUCCESS);
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"]END INIT");
#endif

	return DONE;
}

Pipeline::PipelineState DummyPipeline::start() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	bool running = false;
	do {
		if (true) {
			g_object_set(G_OBJECT(convert), "flip-method", 2, NULL);
			g_object_set(G_OBJECT(trackingConv), "flip-method", 2, NULL);
			g_object_set(G_OBJECT(sink), "host", streamIp.c_str(), NULL);
			g_object_set(G_OBJECT(sink), "port", streamPort, NULL);
g_object_set(G_OBJECT(sink), "sync", false, NULL);
g_object_set(G_OBJECT(trackingSink), "sync", false, NULL);
g_object_set(G_OBJECT(trackingSink), "wait-on-eos", false, NULL);
gst_app_sink_set_max_buffers(GST_APP_SINK(trackingSink), 1);
	    			gst_app_sink_set_drop(GST_APP_SINK(trackingSink), (gboolean) TRUE);
gst_app_sink_set_emit_signals(GST_APP_SINK(trackingSink),0);
                        g_object_set(G_OBJECT(source), "fpsRange", "120 120", NULL);
			gst_bin_add_many(GST_BIN(pipeline), trackingTeeQueueTracking, trackingTeeQueueStream, source, convert, trackingTee, encoder, payloader, sink, trackingConv, trackingSink, NULL);

			/******************************************************************************/
			//linking src and convert
			/******************************************************************************/
			if (gst_element_link_filtered(source, trackingTee, sourceCaps) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('source -> sourcecaps -> trackingtee')");
#endif
				return DONE;
			}
			/******************************************************************************/
			//linking convert and tee
			/******************************************************************************/
			if (gst_element_link_filtered(convert, encoder, convertCaps) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('trackingtee -> convertcaps ->convert')");
#endif
				return DONE;
			}
			/******************************************************************************/
			//linking tee -> stream part
			/******************************************************************************/
			if (gst_element_link_filtered(trackingTeeQueueStream, convert, convertCaps) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('trackingtee -> convertcaps ->convert')");
#endif
				return DONE;
			}

			if (gst_element_link_many(trackingTee, trackingTeeQueueStream, NULL) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('convert -> encoder')");
#endif
				return DONE;
			}
			if (gst_element_link_filtered(encoder, payloader, encoderCaps) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('encoder -> encoderCaps -> payloader')");
#endif
				return DONE;
			}
			if (gst_element_link_many(payloader, sink, NULL) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('payloader -> sink')");
#endif
				return DONE;
			}
			/******************************************************************************/
			//linking tee -> tracking part
			/******************************************************************************/
			if (gst_element_link_many(trackingTee, trackingTeeQueueTracking, trackingConv, NULL) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('trackingTee -> trackingTeeQueueTracking -> trackingConv')");
#endif
				return DONE;
			}
			if (gst_element_link_filtered(trackingConv, trackingSink, trackingConvCaps) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('trackingConv -> trackingConvCaps -> trackingSink')");
#endif
				return DONE;
			}
			GstBus* bus = NULL;

			if ((bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline))) != NULL) {
				gst_bus_add_watch(bus, onBusMessage, this);
				gst_object_unref(bus);
				bus = NULL;
			}
gst_pipeline_use_clock(GST_PIPELINE(pipeline),nullptr);
			//GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "sdp_beforeStartPipe");
			running = gst_element_set_state(pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE;
			//GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "sdp_afterStartPipe");

			if (sourceCaps != NULL) {
				gst_caps_unref(sourceCaps);
				sourceCaps = NULL;
			}
			if (encoderCaps != NULL) {
				gst_caps_unref(encoderCaps);
				encoderCaps = NULL;
			}
			if (convertCaps != NULL) {
				gst_caps_unref(convertCaps);
				convertCaps = NULL;
			}
			if (trackingConvCaps != NULL) {
				gst_caps_unref(trackingConvCaps);
				trackingConvCaps = NULL;
			}

			if (!running) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::start() failed. deInit() will de called");
#endif			//deInit();
				LOG4CXX_ERROR(logger, "[" << id <<"]START " << FAIL);
				return DONE;
			} else {
				LOG4CXX_INFO(logger, "[" << id <<"]START " << SUCCESS);
#ifdef ENABLE_TRACKING
				runTracking();
#endif
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline ] running");
#endif
			}
		} else {
//			//init();
//			if (true) {
//				start();
//			} else {
//				return false;
//			}
		}
	} while (0);
	return DONE;
}


bool DummyPipeline::processMessage(Message* msg) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif

#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
	return true;
}
Pipeline::PipelineState DummyPipeline::stop() {
	return NEW;
}
GstElement* DummyPipeline::getSink(string sinkName) {
	return trackingSink;
}
GstElement* DummyPipeline::getdetectSink(string sinkName) {
	return pipeline;
}
GstElement* DummyPipeline::getdetectTee(string sinkName) {
	return trackingSink;
}
Pipeline::PipelineState DummyPipeline::detectinit() {
	//	gst-launch-1.0 v4l2src device=/dev/video0 ! 'video/x-raw, width=(int)1920, height=(int)1080, framerate=60/1' ! videoconvert ! omxh264enc
	//			! 'video/x-h264, stream-format=(string)byte-stream' ! h264parse ! rtph264pay ! udpsink host=10.42.0.1 port=5000
	std::ostringstream ss;
	printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRRRRRRRRRRRRr");
	ss<<"nvcamerasrc"<<" ! ";
	ss<<"video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,framerate=30/1"<<" ! ";
	//ss<<"nvvidconv"<<" ! ";
	ss<<"tee name=t"<<" ! ";
	ss<<"queue"<<" ! ";
	ss<<"video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,framerate=30/1"<<" ! ";
ss<<"nvvidconv"<<" ! ";
ss<<"omxh264enc"<<" ! ";
	ss<<"video/x-h264,stream-format=(string)byte-stream,birate=(int)5000"<<" ! ";
	ss<<"rtph264pay"<<" ! ";
	ss<<"udpsink name=vidsink t."<<" ! ";
	ss<<"queue"<<" ! ";
	ss<<"nvvidconv"<<" ! ";
	ss<<"video/x-raw, format=(string)GRAY8"<<" ! ";
	ss<<"appsink name=tracking t."<<" ! ";
//	ss<<"t."<<" ! ";
	ss<<"queue"<<" ! ";
	ss<<"nvvidconv"<<" ! ";
	ss<<"video/x-raw,format=NV12"<<" ! ";
	ss<<"videoconvert"<<" ! ";
	ss<<"appsink name=mysink";
	std::string mLaunchStr;
	mLaunchStr=ss.str();
	printf("%s\n", mLaunchStr.c_str());
	pipeline = gst_parse_launch(mLaunchStr.c_str(), &err);
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=");
	//if (!err) {

	    sink = gst_bin_get_by_name(GST_BIN(pipeline), "vidsink");

	   // detectnetSink = gst_bin_get_by_name(GST_BIN(pipeline), "mysink");
	    trackingSink = gst_bin_get_by_name(GST_BIN(pipeline), "tracking");
	    g_object_set(G_OBJECT(sink), "host", streamIp.c_str(), NULL);
	    g_object_set(G_OBJECT(sink), "port", streamPort, NULL);
	    GstAppSink* appSink = (GstAppSink*) trackingSink;
	    			gst_app_sink_set_max_buffers(appSink, 1);
	    			gst_app_sink_set_drop(appSink, (gboolean) TRUE);
	    GstBus* bus = NULL;

	    			if ((bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline))) != NULL) {
	    				gst_bus_add_watch(bus, onBusMessage, this);
	    				gst_object_unref(bus);
	    				bus = NULL;
	    			}
	    			GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "sdp_beforeStartPipe");
	    			gst_element_set_state(pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE;
	    			GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "sdp_afterStartPipe");

	//}
	return DONE;

}
} /* namespace va */
