/*
 * VWObjectTrackerAdapter.h
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#ifndef VWOBJECTTRACKERADAPTER_H_
#define VWOBJECTTRACKERADAPTER_H_
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
#include "gst/gst.h"
#include "gst/app/gstappsink.h"
#include "Algorithm.h"
#include  "../../../communications/messagee/TrackingMessage.h"

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

#include "object_tracker_nvxcu.hpp"
#include "object_tracker_with_features_info_nvxcu.hpp"
#include "../communications/SerialCommunication.h"
#include <vector>
#include <QMutex>
namespace va {

using namespace nvidiaio;

class VWObjectTrackerAdapter: public Algorithm{
public:
	VWObjectTrackerAdapter(string anaName, string loggerName);
	virtual ~VWObjectTrackerAdapter();
	bool processMessage(Message* msg);
	void run();

	void setSerial(SerialCommunication* serialLink);
	bool isTracking() {
		return tracking;
	}
	void setTracking(bool tracking) {
		this->tracking = tracking;
	}
	int num =0;
        QMutex* waitMutex;
#ifdef ENABLE_MAV_DEBUG_MSG
	bool isSendDebug() const {
		return sendDebug;
	}

	void setSendDebug(bool sendDebug) {
		this->sendDebug = sendDebug;
	}
#endif
private:
	SerialCommunication* serialLink;
	struct FeaturePointsVisualizationStyle
	{
	    float min_weight;
	    float max_weight;
	    uint8_t color[4];
	    unsigned char radius;
	    unsigned char thickness;
	};
	bool tracking;
	nvxcuio::FrameSource::FrameStatus fetch(const image_t & image, uint32_t timeout = 5u);
	void start(TrackingMessage* msg);
	void stop();
	nvxcu::ObjectTrackerWithFeaturesInfo* tracker;
        //std::unique_ptr<nvxcu::ObjectTrackerWithFeaturesInfo> tracker;
	nvxcu::ObjectTrackerWithFeaturesInfo::TrackedObject* obj;
	typedef std::vector<nvxcu::ObjectTrackerWithFeaturesInfo::FeaturePoint> FeaturePointsVector;
	typedef std::vector<nvxcu::ObjectTrackerWithFeaturesInfo::TrackedObject*> TrackedObjectPointersVector;
//=========================================================
	std::vector< std::vector<nvxcu_point3f_t> >prepareCirclesForFeaturePointsDrawing(const std::vector<FeaturePointsVector>& objects_features_info,
	                                      const std::vector<FeaturePointsVisualizationStyle>& features_styles);
	nvxcu_plain_array_t createArray(uint32_t capacity, nvxcu_array_item_type_e itemType);
	//=========================================================
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
} /* namespace va */

#endif /* VWOBJECTTRACKERADAPTER_H_ */
