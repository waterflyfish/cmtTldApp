/*
 * FileDummyPipeline.cpp
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#include "FileDummyPipeline.h"
#include "../../config/logger_config.h"
#include <thread>

namespace va {
static void newPadCB(GstElement * element, GstPad* pad, gpointer data) {

//	GstPad* vidpad;

	GstElement* parent = GST_ELEMENT_PARENT(element);

	gchar *name;
	name = gst_pad_get_name(pad);
	//LOG("A new pad %s was created: " << name);

	GstCaps* caps;
	GstStructure *str;
	caps = gst_pad_query_caps(pad, NULL);
	//LOG("CAPS: " << gst_caps_to_string(caps));
	str = gst_caps_get_structure(caps, 0);

	if (!g_strrstr(gst_structure_get_name(str), "video")) {
		//LOG("NO VIDEO PAD!!!!");
		gst_caps_unref(caps);

		gst_element_remove_pad(element, pad);
		//LOG("removed a pad");
		gst_object_unref(pad);
	}

	GstCaps * p_caps = gst_pad_get_pad_template_caps(pad);
	gchar * description = gst_caps_to_string(p_caps);
	//LOG(p_caps << ", " << description);
	//LOG(description);
	GstElement * p_convert = GST_ELEMENT(data);
	if (gst_element_link_pads(element, name, p_convert, NULL) == true) {
		//LOG("newPadCB : succeed to link elements\n");
		GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(parent), GST_DEBUG_GRAPH_SHOW_ALL, "pipe");
	} else {
		//LOG("newPadCB : failed to link elements\n");
	}
	g_free(name);
}

FileDummyPipeline::FileDummyPipeline(string pipeName, string loggerName, string streamIp, int streamPort) :
		Pipeline(pipeName, loggerName, streamIp, streamPort) {
	//this->filePath = filePath;
	pipeline = NULL;
	source = NULL;
	decodeBin = NULL;
	//sourceCaps = NULL;
	convert = NULL;
	trackingTee = NULL;

	trackingConv = NULL;
	trackingSink = NULL;
	trackingConvCaps = NULL;
	encoder = NULL;
	encoderCaps = NULL;
	payloader = NULL;
	//convertCaps = NULL;
	sink = NULL;
	//=======================================
		//add the pipeline for detectnet
		detectnetQueue=NULL;
		 detectnetSink=NULL;
	detectnetTee=NULL;
		trackQueue=NULL;
		mainQueue=NULL;
		 detectnetDecode=NULL;
		 detectnetRate=NULL;
		 detectnetConv=NULL;
		 detectnetfCaps=NULL;
		 detectnetsCaps=NULL;
}

FileDummyPipeline::~FileDummyPipeline() {

}
Pipeline::PipelineState FileDummyPipeline::detectinit() {
	std::ostringstream ss;
	printf("EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRRRRRRRRRRRRr");
	ss<<"filesrc location=/home/ubuntu/videoApp/video.mp4"<<" ! ";
	ss<<"decodebin"<<" ! ";
	//ss<<"nvvidconv"<<" ! ";
	ss<<"tee name=t"<<" ! ";
	ss<<"queue"<<" ! ";
	ss<<"nvvidconv"<<" ! ";
	ss<<"videorate ! video/x-raw,format=I420"<<" ! ";
	ss<<"omxh264enc"<<" ! ";
	ss<<"video/x-h264,stream-format=byte-stream"<<" ! ";
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
	ss<<"appsink name=mysink";
	std::string mLaunchStr;
	mLaunchStr=ss.str();
	printf("%s\n", mLaunchStr.c_str());
	pipeline = gst_parse_launch(mLaunchStr.c_str(), &err);
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=");
	//if (!err) {

	    sink = gst_bin_get_by_name(GST_BIN(pipeline), "vidsink");

	    detectnetSink = gst_bin_get_by_name(GST_BIN(pipeline), "mysink");
	    trackingSink = gst_bin_get_by_name(GST_BIN(pipeline), "tracking");
	    g_object_set(G_OBJECT(sink), "host", streamIp.c_str(), NULL);
	    g_object_set(G_OBJECT(sink), "port", streamPort, NULL);
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
Pipeline::PipelineState FileDummyPipeline::init() {
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
	if ((source = gst_element_factory_make("filesrc", "source")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('filesrc')");

#endif
		return DONE;
	}
	if ((decodeBin = gst_element_factory_make("decodebin", "decodebin")) == NULL) {
		LOG4CXX_DEBUG(logger, "TTSimpleDynamicPipeline::init() failed. Error with gst_element_factory_make('decodebin')");
		return DONE;
	}
	if ((convert = gst_element_factory_make("nvvidconv", "convert")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('convert')");
#endif
		return DONE;
	}

	if ((trackingTee = gst_element_factory_make("tee", "trackingTee")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
#endif
		return DONE;
	}

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

	if ((convCaps = gst_caps_from_string("video/x-raw(memory:NVMM), format=(string)I420")) == NULL) {
		//LOG_DEBUG("CSIDynamicPipeline::init() failed. Error with gst_caps_from_string(convCaps)");
		return DONE;
	}

	if ((trackingSink = gst_element_factory_make("appsink", "trackingSink")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingSink')");
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
	//==================================================================================================
	if ((detectnetTee = gst_element_factory_make("tee", "detectnetTee")) == NULL) {
#ifdef DEBUG_BUILD
		LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
#endif
		return DONE;
	}
	if ((detectnetSink = gst_element_factory_make("appsink", "mysink")) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
	#endif
			return DONE;
		}
	if ((detectnetRate = gst_element_factory_make("videorate", "detectnetRate")) == NULL) {
		#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
		#endif
				return DONE;
			}
	if ((detectnetQueue = gst_element_factory_make("queue", "detectnetQueue")) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
	#endif
			return DONE;
		}
	if ((mainQueue = gst_element_factory_make("queue", "mainQueue")) == NULL) {
		#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
		#endif
				return DONE;
			}
	if ((trackQueue = gst_element_factory_make("queue", "trackQueue")) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
	#endif
			return DONE;
		}
	if ((detectnetDecode = gst_element_factory_make("decodebin", "detectnetDecode")) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
	#endif
			return DONE;
		}
	if ((detectnetConv = gst_element_factory_make("nvvidconv", "detectnetConv")) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_element_factory_make('trackingTee')");
	#endif
			return DONE;
		}
	if ((detectnetfCaps = gst_caps_from_string(DETECT_CAPS)) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_caps_from_string(trackingConvCaps)");
	#endif
			return DONE;
		}
	if ((detectnetsCaps = gst_caps_from_string(DETECTS_CAPS)) == NULL) {
	#ifdef DEBUG_BUILD
			LOG4CXX_DEBUG(logger, "[" << id <<"]CSIDynamicPipeline::init() failed. Error with gst_caps_from_string(trackingConvCaps)");
	#endif
			return DONE;
		}
	//==================================================================================================
	LOG4CXX_INFO(logger, "[" << id <<"]INIT " << SUCCESS);
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"]END INIT");
#endif

	return DONE;
}

Pipeline::PipelineState FileDummyPipeline::start() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "[" << id <<"] called");
#endif
	bool running = false;
	do {
		if (true) {
			g_object_set(G_OBJECT(sink), "host", streamIp.c_str(), NULL);
			g_object_set(G_OBJECT(sink), "port", streamPort, NULL);
			g_object_set(G_OBJECT(source), "location", Configuration::getInstance().dbg_getFilePipeFilePath().c_str(), NULL);
			g_object_set(G_OBJECT(trackingSink),"sync",false,NULL);
			g_signal_connect(decodeBin, "pad-added", G_CALLBACK(newPadCB), convert);

			gst_bin_add_many(GST_BIN(pipeline), source, decodeBin, convert, trackingTee, mainQueue,encoder, payloader, sink, trackQueue,trackingConv,
					trackingSink,NULL);
			//gst_bin_add_many(GST_BIN(pipeline), trackingTee, detectnetConv,detectnetSink,NULL);
//===========================================================================================================================
//			if (gst_element_link_many(source,decodeBin, NULL) != (gboolean) TRUE) {
//#ifdef DEBUG_BUILD
//				LOG4CXX_DEBUG(logger,
//						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('convert -> encoder')");
//#endif
//				return DONE;
//			}
			if (gst_element_link_many(source,decodeBin,NULL) != (gboolean) TRUE) {
			#ifdef DEBUG_BUILD
							LOG4CXX_DEBUG(logger,
									"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('convert -> encoder')");
			#endif
							return DONE;
						}
//			if (gst_element_link_many(detectnetTee,detectnetQueue, NULL) != (gboolean) TRUE) {
//			#ifdef DEBUG_BUILD
//							LOG4CXX_DEBUG(logger,
//									"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('convert -> encoder')");
//			#endif
//							return DONE;
//						}
//			if (gst_element_link_filtered(detectnetQueue,detectnetDecode, detectnetfCaps) != (gboolean) TRUE) {
//			#ifdef DEBUG_BUILD
//							LOG4CXX_DEBUG(logger,
//									"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('encoder -> encoderCaps -> payloader')");
//			#endif
//							return DONE;
////						}
//			if (gst_element_link_many(trackingTee,detectnetConv, NULL) != (gboolean) TRUE) {
//						#ifdef DEBUG_BUILD
//										LOG4CXX_DEBUG(logger,
//												"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('convert -> encoder')");
//						#endif
//										return DONE;
//									}
//			if (gst_element_link_filtered(detectnetConv,detectnetSink, detectnetsCaps) != (gboolean) TRUE) {
//					#ifdef DEBUG_BUILD
//									LOG4CXX_DEBUG(logger,
//											"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('encoder -> encoderCaps -> payloader')");
//					#endif
//									return DONE;
//								}
//===========================================================================================================================
//			if (gst_element_link_many(source, decodeBin, NULL) != (gboolean) TRUE) {
//#ifdef DEBUG_BUILD
//				LOG4CXX_DEBUG(logger,
//						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('convert -> encoder')");
//#endif
//				return DONE;
//			}
			if (gst_element_link_filtered(convert, trackingTee, convCaps) != (gboolean) TRUE) {
#ifdef DEBUG_BUILD
				LOG4CXX_DEBUG(logger,
						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('encoder -> encoderCaps -> payloader')");
#endif
				return DONE;
			}

			if (gst_element_link_many(trackingTee, mainQueue,encoder, NULL) != (gboolean) TRUE) {
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

			if (gst_element_link_many(trackingTee, trackQueue,trackingConv, NULL) != (gboolean) TRUE) {
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
			//====================================================================================

//			if (gst_element_link_many(trackingTee, detectnetQueue,detectnetConv,detectnetRate, NULL) != (gboolean) TRUE) {
//#ifdef DEBUG_BUILD
//				LOG4CXX_DEBUG(logger,
//						"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_many ('trackingTee -> trackingTeeQueueTracking -> trackingConv')");
//#endif
//				return DONE;
//			}
//			if (gst_element_link_filtered(detectnetRate, detectnetSink, detectnetsCaps) != (gboolean) TRUE) {
//			#ifdef DEBUG_BUILD
//							LOG4CXX_DEBUG(logger,
//									"[" << id <<"]CSIDynamicPipeline::start() failed. Error with gst_element_link_filtered('trackingConv -> trackingConvCaps -> trackingSink')");
//			#endif
//							return DONE;
//						}
			//======================================================================================
			GstBus* bus = NULL;

			if ((bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline))) != NULL) {
				gst_bus_add_watch(bus, onBusMessage, this);
				gst_object_unref(bus);
				bus = NULL;
			}
			GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "sdp_beforeStartPipe");
			running =gst_element_set_state(pipeline, GST_STATE_PLAYING) != GST_STATE_CHANGE_FAILURE;
			GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "sdp_afterStartPipe");

			if (encoderCaps != NULL) {
				gst_caps_unref(encoderCaps);
				encoderCaps = NULL;
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
		//GST_DEBUG_BIN_TO_DOT_FILE(GST_BIN(pipeline), GST_DEBUG_GRAPH_SHOW_ALL, "three");
	} while (0);
	return DONE;
}

bool FileDummyPipeline::processMessage(Message* msg) {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
	return true;
}
Pipeline::PipelineState FileDummyPipeline::stop() {
	return DONE;
}
GstElement* FileDummyPipeline::getSink(string sinkName) {
	return trackingSink;
//	if(sinkName.compare("mysink")){
//		printf("#########################################################################################################################<<");
//		return detectnetSink;
//	}else{
//		printf("!!!!!!!!!!!!!!!!!!!!!!!1#########################################################################################################################<<");
//		return trackingSink;
//	}

}
GstElement* FileDummyPipeline::getdetectSink(string sinkName) {
	//return detectnetSink;
	return pipeline;
//	if(sinkName.compare("mysink")){
//		printf("#########################################################################################################################<<");
//		return detectnetSink;
//	}else{
//		printf("!!!!!!!!!!!!!!!!!!!!!!!1#########################################################################################################################<<");
//		return trackingSink;
//	}

}
GstElement* FileDummyPipeline::getdetectTee(string sinkName) {
	//return detectnetSink;
	return trackingTee;
//	if(sinkName.compare("mysink")){
//		printf("#########################################################################################################################<<");
//		return detectnetSink;
//	}else{
//		printf("!!!!!!!!!!!!!!!!!!!!!!!1#########################################################################################################################<<");
//		return trackingSink;
//	}

}
} /* namespace va */

