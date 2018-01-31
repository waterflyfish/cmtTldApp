/*
 * Pipeline.h
 *
 *  Created on: Apr 13, 2017
 *      Author: patrick
 */

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <string>
#include <map>
#include "../tools/Configuration.h"
#include "../algorithm/Algorithm.h"
#include "../tools/Messanger.h"
#include "gst/gst.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
namespace va {

class Pipeline: public Messanger {
public:
	enum PipelineState {
		NEW, INIT, RUNNING, STOPPED, DONE
	};
	enum PipelineType {
		CSI, USB, FILE
	};
	Pipeline(string pipeName, string loggerName, string streamIp, int streamPort);
	virtual ~Pipeline();
	virtual PipelineState start() = 0;
	virtual PipelineState init() = 0;
	virtual PipelineState stop() = 0;
	virtual bool processMessage(Message* msg) = 0;
	virtual GstElement* getSink(string sinkName) = 0;
	virtual GstElement* getdetectSink(string sinkName) = 0;
	virtual GstElement* getdetectTee(string sinkName) = 0;
protected:
	std::string streamIp;
	int streamPort;
	void onBusMessage(GstMessage* msg) {
		switch (GST_MESSAGE_TYPE(msg)) {
		case GST_MESSAGE_EOS:
			stop();
			break;
		case GST_MESSAGE_ERROR:
			do {
				gchar* debug;
				GError* error;
				gst_message_parse_error(msg, &error, &debug);
				g_free(debug);
				g_error_free(error);
			} while (0);
			//stop();
			break;
		default:
			break;
		}
	}
	static gboolean onBusMessage(GstBus* bus, GstMessage* msg, gpointer data) {
		Pipeline* pThis = (Pipeline*) data;
		pThis->onBusMessage(msg);
		return TRUE;
	}
};

} /* namespace va */

#endif /* PIPELINE_H_ */
