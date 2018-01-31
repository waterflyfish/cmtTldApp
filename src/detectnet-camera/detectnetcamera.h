/*
 * http://github.com/dusty-nv/jetson-inference
 */
#ifndef DETECTNETCAMERA_H_
#define DETECTNETCAMERA_H_
#include "gst/gst.h"
#include "gst/app/gstappsink.h"
#include "Algorithm.h"
#include  "../communications/messagee/TrackingMessage.h"

#include <NVXIO/Utility.hpp>
#include <NVXIO/ProfilerRange.hpp>

#include <cuda_runtime_api.h>

#include <memory>
#include <string>

#include "Private/Types.hpp"

#include <NVXCUIO/FrameSource.hpp>

#include <NVX/nvx.h>
#include <NVX/nvxcu.h>
#include <NVX/nvx_timer.hpp>

#include <NVXIO/Application.hpp>
#include <NVXIO/ConfigParser.hpp>
#include <NVXCUIO/FrameSource.hpp>
#include <NVXCUIO/Render.hpp>
#include <NVXIO/SyncTimer.hpp>
#include <NVXIO/Utility.hpp>
//
//
//
#include <vector>
#include "gstCamera.h"
//
//
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
//
#include "cudaMappedMemory.h"
#include "cudaNormalize.h"
#include "cudaFont.h"

#include "detectNet.h"


#define DEFAULT_CAMERA -1	// -1 for onboard camera, or change to index of /dev/video V4L2 camera (>=0)
namespace va{
//using namespace nvidiaio;
class detectnetcamera:public Algorithm{
public:
	    detectnetcamera(string anaName, string loggerName);
		virtual ~detectnetcamera();
		bool processMessage(Message* msg);
		void run();
		bool isTracking() {
			return tracking;
		}
		void setTracking(bool tracking) {
			this->tracking = tracking;
		}
        int num=0;
       gstCamera* camera =NULL;
	   detectNet* net =NULL;

	   cudaFont* font = NULL;
       #ifdef ENABLE_MAV_DEBUG_MSG
	        bool isSendDebug() const {
		    return sendDebug;
	}
	   void setSendDebug(bool sendDebug) {
		this->sendDebug = sendDebug;
	}
#endif
private:

	bool tracking;

	/*****************************************/
	 nvxio::ContextGuard vxContext;
	    nvxcuio::FrameSource::Parameters configuration;
	    void * devMem;
	    size_t devMemPitch;

#ifdef ENABLE_MAV_DEBUG_MSG
    bool sendDebug;
    int debugMsgSerial;
#endif
};
}
#endif
