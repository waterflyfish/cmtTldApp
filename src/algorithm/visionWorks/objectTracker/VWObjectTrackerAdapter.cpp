/*
 * VWObjectTrackerAdapter.cpp
 *
 *  Created on: Feb 11, 2017
 *      Author: patrick
 */

#include <sys/prctl.h>

#include <thread>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <memory>
#include <stdlib.h>
using namespace std;
#include <string>
#include <NVXIO/Utility.hpp>
#include <NVXIO/ProfilerRange.hpp>
#include <cuda_runtime_api.h>
#include "Private/Types.hpp"
#include <NVXCUIO/FrameSource.hpp>
#include <NVX/nvx.h>
#include <NVX/nvxcu.h>
#include <NVX/nvx_timer.hpp>
#include <NVXIO/Application.hpp>
#include <NVXCUIO/FrameSource.hpp>
#include <NVXIO/Utility.hpp>

#include "object_tracker_nvxcu.hpp"
#include "object_tracker_with_features_info_nvxcu.hpp"

#include "VWObjectTrackerAdapter.h"
#include "../../../tools/Configuration.h"
#include "../../../../config/logger_config.h"
#include "../../../communications/messagee/ControlMessage.h"
#include "../../../communications/messagee/TrackingMessage.h"
#include "../../../communications/messagee/TrackedTargetMessage.h"

#include "../../../communications/messagee/Fpdsmessage.h"
#include "../../../communications/messagee/Fpdcmessage.h"
#include "../../../../config/videoapp_config.h"

namespace nvidiaio {
void convertFrame(vx_context vxContext, vx_image frame, vx_image decoded_frame, bool is_cuda, void *& devMem, size_t & devMemPitch) {
	nvxio::ProfilerRange range(nvxio::COLOR_ARGB_FUSCHIA, "ConvertFrame (NVXIO)");

	vx_df_image decodedFormat, frameFormat;
	vx_imagepatch_addressing_t decodedImageAddrs[2];
	vx_memory_type_e mem_type = is_cuda ? (vx_memory_type_e) NVX_MEMORY_TYPE_CUDA : VX_MEMORY_TYPE_HOST;

	NVXIO_SAFE_CALL(vxQueryImage(decoded_frame, VX_IMAGE_ATTRIBUTE_FORMAT, (void *) &decodedFormat, sizeof(decodedFormat)));
	NVXIO_SAFE_CALL(vxQueryImage(frame, VX_IMAGE_ATTRIBUTE_FORMAT, (void *) &frameFormat, sizeof(frameFormat)));
	bool needConvert = frameFormat != decodedFormat;
	bool canCopyDirectly = !needConvert || (decodedFormat == VX_DF_IMAGE_NV12 && frameFormat == VX_DF_IMAGE_U8);

	vx_rectangle_t rect = { }, uv_rect;
	vx_map_id decodedMapIDs[2];
	void * decodedPtrs[2];

	NVXIO_SAFE_CALL(vxGetValidRegionImage(decoded_frame, &rect));
	NVXIO_SAFE_CALL(
			vxMapImagePatch(decoded_frame, &rect, 0, &decodedMapIDs[0], &decodedImageAddrs[0], &decodedPtrs[0], VX_READ_ONLY, mem_type, 0));

	if (decodedFormat == VX_DF_IMAGE_NV12) {
		uv_rect = rect;
		uv_rect.start_y >>= 1;
		uv_rect.end_y >>= 1;
		uv_rect.start_x >>= 1;
		uv_rect.end_x >>= 1;

		NVXIO_SAFE_CALL(
				vxMapImagePatch(decoded_frame, &uv_rect, 1, &decodedMapIDs[1], &decodedImageAddrs[1], &decodedPtrs[1], VX_READ_ONLY,
						mem_type, 0));
	}

	// allocate CUDA memory to copy decoded image to
	if (!is_cuda && !canCopyDirectly) {
		if (!devMem) {
			size_t height = decodedImageAddrs[0].dim_y;

			if (decodedFormat == VX_DF_IMAGE_NV12)
				height += height >> 1;

			// we assume that decoded image will have no more than 4 channels per pixel
			NVXIO_ASSERT(cudaSuccess == cudaMallocPitch(&devMem, &devMemPitch, decodedImageAddrs[0].dim_x * 4, height));
		}
	}

	vx_image decodedImage = nullptr;
	cudaStream_t stream = nullptr;

	// 1. create vx_image wrapper
	if (!canCopyDirectly) {
		NVXIO_ASSERT(needConvert);

		if (is_cuda) {
			// a. create vx_image wrapper from CUDA pointer
			decodedImage = vxCreateImageFromHandle(vxContext, decodedFormat, decodedImageAddrs, decodedPtrs, NVX_MEMORY_TYPE_CUDA);
		} else {
			NVXIO_ASSERT(devMem);

			void * devMems[2] = { devMem, nullptr };

			// a. upload decoded image to CUDA buffer
			NVXIO_CUDA_SAFE_CALL(
					cudaMemcpy2DAsync(devMems[0], devMemPitch, decodedPtrs[0], decodedImageAddrs[0].stride_y,
							decodedImageAddrs[0].dim_x * decodedImageAddrs[0].stride_x, decodedImageAddrs[0].dim_y, cudaMemcpyHostToDevice,
							stream));

			if (decodedFormat == VX_DF_IMAGE_NV12) {
				devMems[1] = (uint8_t *) devMem + devMemPitch * decodedImageAddrs[0].dim_y;

				NVXIO_CUDA_SAFE_CALL(
						cudaMemcpy2DAsync(devMems[1], devMemPitch, decodedPtrs[1], decodedImageAddrs[1].stride_y,
								decodedImageAddrs[1].dim_x * decodedImageAddrs[1].stride_x, decodedImageAddrs[1].dim_y,
								cudaMemcpyHostToDevice, stream));
			}

			NVXIO_CUDA_SAFE_CALL(cudaStreamSynchronize(stream));

			// b. create vx_image wrapper for decoded buffer
			decodedImageAddrs[0].stride_y = decodedImageAddrs[1].stride_y = static_cast<vx_int32>(devMemPitch);

			decodedImage = vxCreateImageFromHandle(vxContext, decodedFormat, decodedImageAddrs, devMems, NVX_MEMORY_TYPE_CUDA);
		}

		NVXIO_CHECK_REFERENCE(decodedImage);
	}

	// 2. convert / copy to dst image
	if (canCopyDirectly) {
		cudaMemcpyKind copyKind = is_cuda ? cudaMemcpyDeviceToDevice : cudaMemcpyHostToDevice;

		void * framePtr;
		vx_map_id frameMapID;
		vx_imagepatch_addressing_t frameAddr = { };

		NVXIO_SAFE_CALL(vxMapImagePatch(frame, &rect, 0, &frameMapID, &frameAddr, &framePtr, VX_WRITE_ONLY, NVX_MEMORY_TYPE_CUDA, 0));

		NVXIO_CUDA_SAFE_CALL(
				cudaMemcpy2DAsync(framePtr, frameAddr.stride_y, decodedPtrs[0], decodedImageAddrs[0].stride_y,
						decodedImageAddrs[0].dim_x * decodedImageAddrs[0].stride_x, decodedImageAddrs[0].dim_y, copyKind, stream));

		NVXIO_SAFE_CALL(vxUnmapImagePatch(frame, frameMapID));

		NVXIO_CUDA_SAFE_CALL(cudaStreamSynchronize(stream));
	} else {
		NVXIO_CHECK_REFERENCE(decodedImage);
		NVXIO_SAFE_CALL(vxuColorConvert(vxContext, decodedImage, frame));
		NVXIO_SAFE_CALL(vxReleaseImage(&decodedImage));
	}

	// release decoded_frame
	{
		NVXIO_SAFE_CALL(vxUnmapImagePatch(decoded_frame, decodedMapIDs[0]));

		if (decodedFormat == VX_DF_IMAGE_NV12) {
			NVXIO_SAFE_CALL(vxUnmapImagePatch(decoded_frame, decodedMapIDs[1]));
		}
	}
}

vx_image wrapNVXIOImage(vx_context context, const image_t & image) {
	vx_imagepatch_addressing_t addrs[NVIDIAIO_NB_MAX_PLANES];
	addrs[0].dim_x = image.width;
	addrs[0].dim_y = image.height;
	addrs[0].scale_x = addrs[0].scale_y = VX_SCALE_UNITY;
	addrs[0].stride_x = image.format == NVXCU_DF_IMAGE_RGB ? 3 * sizeof(uint8_t) :
						image.format == NVXCU_DF_IMAGE_RGBX ? 4 * sizeof(uint8_t) : image.format == NVXCU_DF_IMAGE_U8 ? sizeof(uint8_t) :
						image.format == NVXCU_DF_IMAGE_NV12 ? sizeof(uint8_t) : 0;
	addrs[0].stride_y = image.planes[0].pitch_in_bytes;

	if (image.format == NVXCU_DF_IMAGE_NV12) {
		addrs[1].dim_x = image.width >> 1;
		addrs[1].dim_y = image.height >> 1;
		addrs[1].scale_x = addrs[1].scale_y = VX_SCALE_UNITY;
		addrs[1].stride_x = sizeof(uint16_t);
		addrs[1].stride_y = image.planes[1].pitch_in_bytes;
	}

	void * devPtrs[NVIDIAIO_NB_MAX_PLANES] = { image.planes[0].ptr, image.planes[1].ptr };

	vx_image image_vx = vxCreateImageFromHandle(context, static_cast<vx_df_image>(image.format), addrs, devPtrs, NVX_MEMORY_TYPE_CUDA);
	NVXIO_CHECK_REFERENCE(image_vx);

	return image_vx;
}

nvxcuio::FrameSource::FrameStatus extractFrameParams(const nvxcuio::FrameSource::Parameters & configuration, GstCaps * bufferCaps,
		gint & width, gint & height, gint & fps, gint & depth) {
	// fail out if no caps
	assert(gst_caps_get_size(bufferCaps) == 1);
	GstStructure * structure = gst_caps_get_structure(bufferCaps, 0);
//std::cout<<"CONFIGURATIONNNNNNNNNNNNNNNNNNNNNNNNNNNN=============================================="<<configuration<<std::endl;
	// fail out if width or height are 0
	if (!gst_structure_get_int(structure, "width", &width)) {
		//NVXIO_PRINT("Failed to retrieve width");
		return nvxcuio::FrameSource::CLOSED;
	}
	if (!gst_structure_get_int(structure, "height", &height)) {
		//  NVXIO_PRINT("Failed to retrieve height");
		return nvxcuio::FrameSource::CLOSED;
	}

	// NVXIO_ASSERT(configuration.frameWidth == static_cast<uint32_t>(width));
	// NVXIO_ASSERT(configuration.frameHeight == static_cast<uint32_t>(height));

	gint num = 0, denom = 1;
	if (!gst_structure_get_fraction(structure, "framerate", &num, &denom)) {
		// NVXIO_PRINT("Cannot query video fps");
		return nvxcuio::FrameSource::CLOSED;
	} else
		fps = static_cast<float>(num) / denom;

	depth = 0;
	const gchar * name = gst_structure_get_name(structure);
	nvxcu_df_image_e vx_format = NVXCU_DF_IMAGE_NONE;

#if GST_VERSION_MAJOR == 0
	if (!name)
	return nvxcuio::FrameSource::CLOSED;

	if (strcasecmp(name, "video/x-raw-gray") == 0)
	{
		gint bpp = 0;
		if (!gst_structure_get_int(structure, "bpp", &bpp))
		{
			NVXIO_PRINT("Failed to retrieve BPP");
			return nvxcuio::FrameSource::CLOSED;
		}

		if (bpp == 8)
		{
			depth = 1;
			vx_format = NVXCU_DF_IMAGE_U8;
		}
	}
	else if (strcasecmp(name, "video/x-raw-rgb") == 0)
	{
		gint bpp = 0;
		if (!gst_structure_get_int(structure, "bpp", &bpp))
		{
			NVXIO_PRINT("Failed to retrieve BPP");
			return nvxcuio::FrameSource::CLOSED;
		}

		if (bpp == 24)
		{
			depth = 3;
			vx_format = NVXCU_DF_IMAGE_RGB;
		}
		else if (bpp == 32)
		{
			depth = 4;
			vx_format = NVXCU_DF_IMAGE_RGBX;
		}
	}
	else if (strcasecmp(name, "video/x-raw-yuv") == 0)
	{
		guint32 fourcc = 0u;

		if (!gst_structure_get_fourcc(structure, "format", &fourcc))
		{
			NVXIO_PRINT("Failed to retrieve FOURCC");
			return nvxcuio::FrameSource::CLOSED;
		}

		if (fourcc == GST_MAKE_FOURCC('N', 'V', '1', '2'))
		vx_format = NVXCU_DF_IMAGE_NV12;
	}
#else
	const gchar * format = gst_structure_get_string(structure, "format");

	if (!name || !format)
		return nvxcuio::FrameSource::CLOSED;

	if (strcasecmp(name, "video/x-raw") == 0) {
		if (strcasecmp(format, "RGBA") == 0) {
                        
			vx_format = NVXCU_DF_IMAGE_RGBX;
			depth = 4;
		} else if (strcasecmp(format, "RGB") == 0) {
			vx_format = NVXCU_DF_IMAGE_RGB;
			depth = 3;
		} else if (strcasecmp(format, "GRAY8") == 0) {
			vx_format = NVXCU_DF_IMAGE_U8;
			depth = 1;
		} else if (strcasecmp(format, "NV12") == 0)
			vx_format = NVXCU_DF_IMAGE_NV12;
	}
#endif

	NVXIO_ASSERT(configuration.format == NVXCU_DF_IMAGE_NONE || configuration.format == vx_format);

	return nvxcuio::FrameSource::OK;
}

}

namespace va {

using namespace nvidiaio;
/******************************************************************************************************************************************/
nvxcu_pitch_linear_image_t createImage(uint32_t width, uint32_t height, nvxcu_df_image_e format) {
	NVXIO_ASSERT(format == NVXCU_DF_IMAGE_U8 || format == NVXCU_DF_IMAGE_RGB || format == NVXCU_DF_IMAGE_RGBX);

	nvxcu_pitch_linear_image_t image = { };

	image.base.format = format;
	image.base.image_type = NVXCU_PITCH_LINEAR_IMAGE;
	image.base.width = width;
	image.base.height = height;

	size_t pitch = 0ul;
	uint32_t cn = format == NVXCU_DF_IMAGE_U8 ? 1u : format == NVXCU_DF_IMAGE_RGB ? 3u : 4u;

	NVXIO_CUDA_SAFE_CALL(cudaMallocPitch(&image.planes[0].dev_ptr, &pitch, image.base.width * cn, image.base.height));
	image.planes[0].pitch_in_bytes = static_cast<uint32_t>(pitch);

	return image;
}

void releaseImage(nvxcu_pitch_linear_image_t * image) {
	NVXIO_CUDA_SAFE_CALL(cudaFree(image->planes[0].dev_ptr));
}

nvxcuio::FrameSource::FrameStatus VWObjectTrackerAdapter::fetch(const image_t & image, uint32_t /*timeout*/) {
	nvxio::ProfilerRange range(nvxio::COLOR_ARGB_FUSCHIA, "FrameSource::fetch (NVXIO)");

	//handleGStreamerMessages();

	if (gst_app_sink_is_eos(GST_APP_SINK(sink))) {
//		close();
//		return nvxcuio::FrameSource::CLOSED;
	}
//
//#if GST_VERSION_MAJOR == 0
//	std::unique_ptr<GstBuffer, GStreamerObjectDeleter> bufferHolder(
//			gst_app_sink_pull_buffer(GST_APP_SINK(sink)));
//	GstBuffer* buffer = bufferHolder.get();
//#else
	GstSample* sample;
//	std::unique_ptr<GstSample, GStreamerObjectDeleter> sample;
//
//	if (sampleFirstFrame) {
//		sample = std::move(sampleFirstFrame);
//		NVXIO_ASSERT(!sampleFirstFrame);
//	} else
//		sample.reset(gst_app_sink_pull_sample(GST_APP_SINK(sink)));
	sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
//
//	if (!sample) {
//		close();
//		return nvxcuio::FrameSource::CLOSED;
//	}
//
//	GstBuffer * buffer = gst_sample_get_buffer(sample.get());

	GstBuffer * buffer = gst_sample_get_buffer(sample); //!!
//#endif
//
//#if GST_VERSION_MAJOR == 0
//	std::unique_ptr<GstCaps, GStreamerObjectDeleter> bufferCapsHolder(gst_buffer_get_caps(buffer));
//	GstCaps * bufferCaps = bufferCapsHolder.get();
//#else
			//GstCaps * bufferCaps = gst_sample_get_caps(sample.get());
	GstCaps * bufferCaps = gst_sample_get_caps(sample); //!!!
//#endif
//
	gint width, height, fps, depth;
	if (extractFrameParams(configuration, bufferCaps, width, height, fps, depth) == nvxcuio::FrameSource::CLOSED) {
//		close();
//		return nvxcuio::FrameSource::CLOSED;
		LOG4CXX_ERROR(logger, "["<< id <<"] exactFrameParams");

	}

	vx_image decodedImage = nullptr;
#if GST_VERSION_MAJOR == 0
	void * decodedPtr = GST_BUFFER_DATA(buffer);
#else
	GstMapInfo info;

	gboolean success = gst_buffer_map(buffer, &info, (GstMapFlags) GST_MAP_READ);
	if (!success) {
		LOG4CXX_ERROR(logger, "["<< id <<"] !success");
		//close();
		return nvxcuio::FrameSource::CLOSED;
	}

	void * decodedPtr = info.data;
#endif
	vx_image image_vx = wrapNVXIOImage(vxContext, image);

//	if (configuration.format == NVXCU_DF_IMAGE_U8 || configuration.format == NVXCU_DF_IMAGE_RGB
//			|| configuration.format == NVXCU_DF_IMAGE_RGBX) {
	vx_imagepatch_addressing_t decodedImageAddr;
	decodedImageAddr.dim_x = width;
	decodedImageAddr.dim_y = height;
	decodedImageAddr.scale_x = decodedImageAddr.scale_y = VX_SCALE_UNITY;
	decodedImageAddr.stride_x = depth;
	// GStreamer uses as stride width rounded up to the nearest multiple of 4
	decodedImageAddr.stride_y = ((width * depth + 3) >> 2) << 2;

	decodedImage = vxCreateImageFromHandle(vxContext, NVXCU_DF_IMAGE_U8, &decodedImageAddr, &decodedPtr, VX_MEMORY_TYPE_HOST);
	NVXIO_CHECK_REFERENCE(decodedImage);
//	} else if (configuration.format == NVXCU_DF_IMAGE_NV12) {
//		vx_imagepatch_addressing_t decodedImageAddrs[2];
//
//		decodedImageAddrs[0].dim_x = width;
//		decodedImageAddrs[0].dim_y = height;
//		decodedImageAddrs[0].scale_x = decodedImageAddrs[0].scale_y = VX_SCALE_UNITY;
//		decodedImageAddrs[0].stride_x = sizeof(uint8_t);
//		decodedImageAddrs[0].stride_y = ((width + 3) >> 2) << 2;
//
//		decodedImageAddrs[1].dim_x = decodedImageAddrs[0].dim_x >> 1;
//		decodedImageAddrs[1].dim_y = decodedImageAddrs[0].dim_y >> 1;
//		decodedImageAddrs[1].scale_x = decodedImageAddrs[1].scale_y = VX_SCALE_UNITY;
//		decodedImageAddrs[1].stride_x = sizeof(uint16_t);
//		decodedImageAddrs[1].stride_y = decodedImageAddrs[0].stride_y;
//
//		void * ptrs[] = { decodedPtr, (void *) ((guint8 *) decodedPtr + decodedImageAddrs[0].stride_y * decodedImageAddrs[0].dim_y) };
//		decodedImage = vxCreateImageFromHandle(vxContext, VX_DF_IMAGE_NV12, decodedImageAddrs, ptrs, VX_MEMORY_TYPE_HOST);
//		NVXIO_CHECK_REFERENCE(decodedImage);
//	} else {
//		NVXIO_THROW_EXCEPTION("Unsupported image format");
//	}

	convertFrame(vxContext, image_vx, decodedImage, false, devMem, devMemPitch);

	gst_buffer_unmap(buffer, &info);
gst_sample_unref(sample);

	NVXIO_SAFE_CALL(vxReleaseImage(&decodedImage));
	NVXIO_SAFE_CALL(vxReleaseImage(&image_vx));

	return nvxcuio::FrameSource::OK;
}

/******************************************************************************************************************************************/

;

VWObjectTrackerAdapter::VWObjectTrackerAdapter(string anaName, string loggerName) :
		Algorithm(anaName, loggerName), vxContext(), devMem(nullptr), devMemPitch(0ul) {
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
	type = AlgorithmType::TRACKING;
	obj = NULL;
	tracker = NULL;
	tracking = false;
	sink = NULL;
	sendDebug = false;
	debugMsgSerial = 0;
        waitMutex  = new QMutex();
	nvxio::Application &app = nvxio::Application::get();
	nvxcu::KeypointObjectTrackerParams params;

	params.bb_decreasing_ratio = Configuration::getInstance().pipe_getVxOt_bbDecreasingratio();
	params.pyr_levels = Configuration::getInstance().pipe_getVxOt_pyrLevels();
	params.lk_num_iters = Configuration::getInstance().pipe_getVxOt_lkNumIters();
	params.lk_win_size = Configuration::getInstance().pipe_getVxOt_lkWinSize();
	params.detector_cell_size = Configuration::getInstance().pipe_getVxOt_detectorCellSize();
	params.max_corners = Configuration::getInstance().pipe_getVxOt_maxCorners();
	params.fast_type = Configuration::getInstance().pipe_getVxOt_fastType();
	params.fast_threshold = Configuration::getInstance().pipe_getVxOt_fastThreshold();
	params.harris_k = Configuration::getInstance().pipe_getVxOt_harrisK();
	params.harris_threshold = Configuration::getInstance().pipe_getVxOt_harrisThreshold();
	params.max_corners_in_cell = Configuration::getInstance().pipe_getVxOt_maxCornersInCell();
	params.x_num_of_cells = Configuration::getInstance().pipe_getVxOt_xNumOfCells();
	params.y_num_of_cells = Configuration::getInstance().pipe_getVxOt_yNumOfCells();
	params.use_fast_detector = Configuration::getInstance().pipe_getVxOt_useFastDetector();

	tracker = nvxcuCreateKeypointObjectTrackerWithFeaturesInfo(params);

	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");

}

VWObjectTrackerAdapter::~VWObjectTrackerAdapter() {
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");

	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");

}

bool VWObjectTrackerAdapter::processMessage(Message* msg) {

	LOG4CXX_DEBUG(logger, "["<< id <<"] called");

	msg->logMessage(); // TODO this a debug log that is unneccessary here - totally.
	cout<<"VWOBJECTTRACKERADAPTERKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK===================="<<endl;
	switch (msg->getType()) {
	case Message::MessageType::STATUS:

	case Message::MessageType::CONTROL: {
		ControlMessage* ctrlMsg = dynamic_cast<ControlMessage*>(msg);
		switch (ctrlMsg->getConcreteType()) {
		case ControlMessage::ControlMessageType::TRACKING_START:
			start(dynamic_cast<TrackingMessage*>(msg));
			break;
		case ControlMessage::ControlMessageType::TRACKING_STOP:
			stop();
			break;

		case ControlMessage::ControlMessageType::REQ_FP: {
			Fpdcmessage* fpMsg = dynamic_cast<Fpdcmessage*>(msg);
			//LOG4CXX_ERROR(logger, "["<< id <<"] ControlMessage::ControlMessageType::REQ_FP:  " + fpMsg->isStart());
			if (fpMsg->isStart()) {
				this->sendDebug = true;
			} else {

				if (!fpMsg->isStart()) {
					this->sendDebug = false;
				}
			}
		}
			break;

		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
	return true;
}
//==========================================================================================================================
nvxcu_plain_array_t VWObjectTrackerAdapter::createArray(uint32_t capacity, nvxcu_array_item_type_e itemType)
{
    NVXIO_ASSERT(itemType == NVXCU_TYPE_POINT4F ||
                 itemType == NVXCU_TYPE_POINT3F);

    nvxcu_plain_array_t array = { };
    array.base.array_type = NVXCU_PLAIN_ARRAY;
    array.base.capacity = capacity;
    array.base.item_type = itemType;

    size_t elemSize = itemType == NVXCU_TYPE_POINT4F ? sizeof(nvxcu_point4f_t) :
                                                       sizeof(nvxcu_point3f_t);

    size_t arraySize = elemSize * array.base.capacity;
    NVXIO_CUDA_SAFE_CALL( cudaMalloc(&array.dev_ptr, arraySize + sizeof(uint32_t)) );
    array.num_items_dev_ptr = reinterpret_cast<uint32_t *>(static_cast<uint8_t *>(array.dev_ptr) + arraySize);

    return array;
}
std::vector< std::vector<nvxcu_point3f_t> >VWObjectTrackerAdapter::prepareCirclesForFeaturePointsDrawing(const std::vector<FeaturePointsVector>& objects_features_info,const std::vector<FeaturePointsVisualizationStyle>& features_styles)
{
    size_t num_styles = features_styles.size();

    std::vector< std::vector<nvxcu_point3f_t> > circles_vector_for_each_style(num_styles);

    for(const auto& fp_info : objects_features_info)
    {
        for (const auto& fp: fp_info)
        {
            auto iter = std::find_if(features_styles.begin(), features_styles.end(),
                                     [&](const FeaturePointsVisualizationStyle& cur_style)
                                     {
                                         return (fp.weight >= cur_style.min_weight && fp.weight <= cur_style.max_weight);
                                     });

            if (iter != features_styles.end())
            {
                int32_t index_found_style = std::distance(features_styles.begin(), iter);
                const auto& kp = fp.point_on_current_frame;
                circles_vector_for_each_style[index_found_style].push_back({kp.x, kp.y, (float)features_styles[index_found_style].radius});
            }
        }
    }

    return circles_vector_for_each_style;
}
void VWObjectTrackerAdapter::run() {
//	Algorithm::loc[0]=-1;
//	Algorithm::loc[1]=-1;
//	Algorithm::loc[2]=-1;
//	Algorithm::loc[3]=-1;
//	Algorithm::loc[4]=-1;
//	Algorithm::loc[5]=-1;
//	while(!Algorithm::gstreamerFlag){
//		printf("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCcHHHHHHHHHHHHHHHHHHHHHHHHHHEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEENNNNNNNNNNNNNNNNNNNNNNNNNNNNN");
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//
//	}
	//====================================================================================================================================
	int fd;
	int err;
	fd = serialLink->UART0_Open(fd,"/dev/ttyTHS2");
	 do{
	                  err = serialLink->UART0_Init(fd,115200,0,8,1,'N');
	                 printf("Set Port Exactly!\n");
	      }while(-1== err || -1 == fd);
	 serialLink->setFd(fd);
         std::thread serialThread = std::thread(&SerialCommunication::run, serialLink);
        serialThread.detach();
	 //==================================================================================================================================
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");
	prctl(PR_SET_NAME, id.c_str(), 0, 0, 0);
	if (sink != NULL) {
		gst_app_sink_set_max_buffers((GstAppSink*) sink, 1);
		gst_app_sink_set_drop((GstAppSink*) sink, (gboolean) TRUE);
	} else {
		LOG4CXX_ERROR(logger, "["<< id <<"] sink == NULL, please call init(sink) before calling run(). Stop tracking now");
		return;
	}
	if (!tracker) {
		LOG4CXX_ERROR(logger, "["<< id <<"] exiting Can't initialize object tracker algorithm. Stop tracking now");
		return;
	}
	bool preObj = false;
	if (!(Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().first.first == 0
			&& Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().first.second == 0
			&& Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().second.first == 0
			&& Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().second.second == 0)) {
		preObj = true;
		LOG4CXX_DEBUG(logger, "[" << id <<"] preObj is not 0,0,0,0");
	}
	if (preObj) {
		nvxcu_rectangle_t initialObjectRect = { (unsigned int) Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().first.first,
				(unsigned int) Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().first.second,
				(unsigned int) Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().second.first,
				(unsigned int) Configuration::getInstance().dbg_getVvxOt_preDefinedRectangle().second.second };
		obj = tracker->addObject(initialObjectRect);
		tracking = true;
	}
	int frameCount = 0;
//======================================================================================================
	nvxcu_plain_array_t circles_array = createArray(500, NVXCU_TYPE_POINT3F);

      int i=0;
	//=================================================================================================
	while (true) {
		//LOG4CXX_DEBUG(logger, "["<< id <<"] RUNNING IN RUN()");

                  while(!Algorithm::trackingFlag){
           printf("Tracking function is closed!!!\n");
           std::this_thread::sleep_for(std::chrono::seconds(5));
}
 //printf("Tracking function is opened!!!\n");
#ifdef DEBUG_BUILD

#endif
		//=====================================================================================================================================
//		if( Algorithm::loc[5]==1){
//		nvxcu_rectangle_t initialObjectRect = { (unsigned int) Algorithm::loc[0], (unsigned int)  Algorithm::loc[1], (unsigned int)  Algorithm::loc[2],
//					(unsigned int) Algorithm::loc[3] };
//			obj = tracker->addObject(initialObjectRect);
//		}
//		 Algorithm::loc[5]=-1;
		 //==================================================================================================================================
	
                    waitMutex->lock();

                    if (obj != NULL) {
			nvxcu_pitch_linear_image_t grayScaleFrame = createImage(1280, 720, NVXCU_DF_IMAGE_U8); //FIXME non static width/height
			if (fetch(nvidiaio::image_t(grayScaleFrame)) == nvxcuio::FrameSource::FrameStatus::OK) {

                                i++;
                               // printf("COUNT= %d/n",i);
				LOG4CXX_DEBUG(logger, "["<< id <<"] FETCH FRAME OK");
                                printf("DAO ZZHE\n");
				tracker->process(&grayScaleFrame);
                                printf("AFTER DAO\n");
                                if(obj)printf("NOT NULL\n");
                                if(!obj){
                                           printf("NULL\n");
                                           continue;
}
				nvxcu::ObjectTracker::ObjectStatus status = obj->getStatus();
printf("AFTER 1\n");
				nvxcu_rectangle_t rect = obj->getLocation();
printf("AFTER 2\n");
//				std::cout<<"????????????????????????????????????????????????????????????????????????????????????====::"<<rect.start_x<<std::endl;
//				std::cout<<"////////////////////////////////////////////////////////////////////////////////////====::"<<rect.end_x<<std::endl;
				++frameCount;
//if(frameCount==100){
   // printf("REBOOT\n");
  //  system("reboot");
//}
				int intervall = Configuration::getInstance().getStatusMessagesGenerationConfig().at(
											MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION);
				if (frameCount % intervall == 0) {
					num++;
				std::cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^????????????????????????????????????????????????????????????????????????????????????====::"<<num<<std::endl;
				}
				switch (status) {
				case nvxcu::ObjectTracker::ObjectStatus::TRACKED: {
					LOG4CXX_INFO(logger, "["<< id <<"] Image width:"<<grayScaleFrame.base.width<<"Image height:"<<grayScaleFrame.base.height);
					LOG4CXX_INFO(logger, "["<< id <<"] OBJ STATUS IS [TRACKED | "<<status<<"]");
					int interval = Configuration::getInstance().getStatusMessagesGenerationConfig().at(
							MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION);

					LOG4CXX_ERROR(logger, "["<< id <<"] frameCount/interval 65001: " << frameCount << "/" <<interval);

					if (frameCount % interval == 0) {

						TrackedTargetMessage* targetMsg = new TrackedTargetMessage(
								to_string(MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION), LOG_MSG,
								Configuration::getInstance().getCommunicationsControllerId());
						cout<<"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQCCCCCCCCCCCCCCCC=========="<<Configuration::getInstance().getCommunicationsControllerId()<<endl;
						targetMsg->setTlX(rect.start_x);
						targetMsg->setTlY(rect.start_y);
						targetMsg->setBrX(rect.end_x);
						targetMsg->setBrY(rect.end_y);
						targetMsg->setCenterX(0);
						//if( Algorithm::loc[4]==1)handleMessage(dynamic_cast<Message*>(targetMsg));
						//handleMessage(dynamic_cast<Message*>(targetMsg));
						serialLink->UART0_Send(fd,dynamic_cast<Message*>(targetMsg));
						targetMsg = NULL;
						delete targetMsg;
					}
					interval = Configuration::getInstance().getStatusMessagesGenerationConfig().at(
							MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED);

					LOG4CXX_ERROR(logger, "["<< id <<"] frameCount/interval 65101: " << frameCount << "/" <<interval);

					//if (frameCount % interval == 0) {
						++debugMsgSerial;
						std::cout<<"SIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZESIZE"<<std::endl;
						const nvxcu::ObjectTrackerWithFeaturesInfo::FeaturePointSet& obj_fp_info = obj->getFeaturePointSet();
						//==================================================================================================
						 std::vector<FeaturePointsVector> objects_features_info;
						 size_t N = obj_fp_info.getSize();
						     std::vector< nvxcu::ObjectTrackerWithFeaturesInfo::FeaturePoint> dst(N);
						     for (size_t n = 0; n < N; n++)
						     {
						         dst[n] = obj_fp_info.getFeaturePoint(n);
						     }
						 objects_features_info.push_back( dst );
						 static const std::vector<FeaturePointsVisualizationStyle> features_visualization_styles =
						         {
						             FeaturePointsVisualizationStyle{0.00, 0.30, {0, 0, 255, 100}, 2, 1},
						             FeaturePointsVisualizationStyle{0.30, 0.60, {0, 255, 0, 100}, 2, 1},
						             FeaturePointsVisualizationStyle{0.60, 0.85, {255, 0, 0, 100}, 2, 1},
						             FeaturePointsVisualizationStyle{0.85, 1.00, {255, 0, 0, 255}, 2, 2}
						         };
						     size_t num_feature_styles = features_visualization_styles.size();

						     std::vector< std::vector<nvxcu_point3f_t> > circles_vector_for_each_style =
						             prepareCirclesForFeaturePointsDrawing(objects_features_info,
						                                                   features_visualization_styles);

						     NVXIO_ASSERT(circles_vector_for_each_style.size() == num_feature_styles);
						     NVXIO_ASSERT(circles_array.base.item_type == NVXCU_TYPE_POINT3F);
						     std::cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"<<std::endl;
						     for (size_t n = 0; n < num_feature_styles; n++)
						        {
						            const auto& circles = circles_vector_for_each_style[n];
						            //const auto& cur_style = features_visualization_styles[n];

						            //const auto& color = cur_style.color;
						            //nvxcuio::Render::CircleStyle circle_style = { {color[0], color[1], color[2], color[3]}, cur_style.thickness};

						            uint32_t numItems = static_cast<uint32_t>(circles.size());

						            if (!circles.empty())
						            {
						                size_t arraySize = numItems * sizeof(nvxcu_point3f_t);

						                NVXIO_ASSERT(numItems <= circles_array.base.capacity);
						                NVXIO_CUDA_SAFE_CALL( cudaMemcpy(circles_array.dev_ptr, &circles[0], arraySize, cudaMemcpyHostToDevice) );
						                Fpdsmessage* fpMsg = new Fpdsmessage(
						               						            									to_string(MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED), LOG_MSG,
						               						            									Configuration::getInstance().getCommunicationsControllerId());
						               						            std::cout<<"(((((((((((((((((((((((((((((((((((((((((((((((((((("<<circles[0].x<<std::endl;
						               						            							fpMsg->setSerial(debugMsgSerial);
						               //						            							fpMsg->setError(0);
						               //						            							fpMsg->setOrientation(0);
						               //						            							fpMsg->setScale(0);
						               //						            							fpMsg->setStrength(0);
						               //						            							fpMsg->setTrackingStatus(0);
						               						            							fpMsg->setX(circles[0].x);
						               						            							fpMsg->setY(circles[0].y);
						               						            							//if( Algorithm::loc[4]==1)handleMessage(dynamic_cast<Message*>(fpMsg));
						               						            							//handleMessage(dynamic_cast<Message*>(fpMsg));
						               						            							fpMsg = NULL;
						               						            							delete fpMsg;
						            }

						            NVXIO_CUDA_SAFE_CALL( cudaMemcpy(circles_array.num_items_dev_ptr, &numItems, sizeof(numItems), cudaMemcpyHostToDevice) );


						        }
						     std::cout<<"*****************************************************************************"<<std::endl;
						//=====================================================================================================
//						int size = obj_fp_info.getSize();
//						std::cout<<"SIZESIZEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE====================="<<size<<std::endl;
//						LOG4CXX_DEBUG(logger, "["<< id <<"] POINTS SIZE: " << size);
//						for (int i = 0; i < size; ++i) {
//							nvxcu::ObjectTrackerWithFeaturesInfo::FeaturePoint fp = obj_fp_info.getFeaturePoint(i); //FIXME rename the messages to non-debug! change the mavlink definitions etc
//							LOG4CXX_DEBUG(logger,
//									"["<< id <<"] POINT[ " << i << "].x:" << fp.point_on_current_frame.x << "POINT[ " << i << "].x:" << fp.point_on_current_frame.y);
//							FeaturePointsTrackedDebugStatusMessage* fpMsg = new FeaturePointsTrackedDebugStatusMessage(
//									to_string(MAVLINK_MSG_ID_TX1_DEBUG_STATUS_FP_TRACKED), LOG_MSG,
//									Configuration::getInstance().getCommunicationsControllerId());
//							fpMsg->setSerial(debugMsgSerial);
//							fpMsg->setError(fp.point_on_current_frame.error);
//							fpMsg->setOrientation(fp.point_on_current_frame.orientation);
//							fpMsg->setScale(fp.point_on_current_frame.scale);
//							fpMsg->setStrength(fp.point_on_current_frame.strength);
//							fpMsg->setTrackingStatus(fp.point_on_current_frame.tracking_status);
//							fpMsg->setX(fp.point_on_current_frame.x);
//							fpMsg->setY(fp.point_on_current_frame.y);
//							//handleMessage(dynamic_cast<Message*>(fpMsg));
//							fpMsg = NULL;
//							delete fpMsg;
//						}

//					} else {
//						LOG4CXX_DEBUG(logger, "["<< id <<"]interval modulo != 0 NOT SENDING MESSAGE");
//					}
					LOG4CXX_INFO(logger,
							"["<< id <<"] TRACKING COORDS [ " << rect.start_x << " | " << rect.start_y << " | " << rect.end_x << " | " <<rect.end_y<<" ]");
				}
					break;
				case nvxcu::ObjectTracker::ObjectStatus::TEMPORARY_LOST: {

					LOG4CXX_DEBUG(logger, "["<< id <<"] OBJ STATUS IS [TEMPORARY_LOST | "<<status<<"]");

					LOG4CXX_WARN(logger, "["<< id <<"] STOPPING OBJECT TRACKING NOW");
//					nvxcu_rectangle_t rect1 = obj->getLocation();
//					nvxcu_rectangle_t ret2 = {rect1.start_x+1,rect1.start_y,rect1.end_x,rect1.end_y};
//					obj = tracker->addObject(ret2);
//                    std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<ret2.start_x<<std::endl;
					stop();
				}
					break;
				case nvxcu::ObjectTracker::ObjectStatus::LOST: {

					LOG4CXX_DEBUG(logger, "["<< id <<"] OBJ STATUS IS [LOST | "<<status<<"]");

					LOG4CXX_WARN(logger, "["<< id <<"] STOPPING OBJECT TRACKING NOW");
//					nvxcu_rectangle_t rect1 = obj->getLocation();
//					nvxcu_rectangle_t ret2 = {rect1.start_x+5,rect1.start_y+5,rect1.end_x+5,rect1.end_y+5};
//					obj = tracker->addObject(ret2);
//					std::cout<<"=========================================================================================="<<ret2.start_x<<std::endl;
					stop();
				}
					break;
				default:
					break;
				}
			} else {
				LOG4CXX_WARN(logger, "["<< id <<"] FETCH FRAME FAIL");
			}

			//LOG4CXX_DEBUG(logger, "["<< id <<"] Frames processed: " << frameCount);

			releaseImage(&grayScaleFrame);
		} else {

			//LOG4CXX_DEBUG(logger, "["<< id <<"] obj == NULL - do nothing, wait for tracking to start");
                        
			std::this_thread::sleep_for(std::chrono::milliseconds(1));

		}
waitMutex->unlock();

	}

#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}
void VWObjectTrackerAdapter::start(TrackingMessage* msg) {
waitMutex->lock();
printf("JIN QU\n");
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	stop();
        if((unsigned int) msg->getTlX()==-100){
          Algorithm::trackingFlag=true;
}
        else if((unsigned int) msg->getTlX()==-101){
          Algorithm::trackingFlag=false;
}
       else if((unsigned int) msg->getTlX()==-200){
           // printf("REBOOT\n");
        system("reboot");
}else{
       // printf("PRVE\n");
	nvxcu_rectangle_t initialObjectRect = { (unsigned int) msg->getTlX(), (unsigned int) msg->getTlY(), (unsigned int) msg->getBrX(),
			(unsigned int) msg->getBrY() };
//printf("AFTER\n");

	tracker->removeAllObjects();
	obj = tracker->addObject(initialObjectRect);
if(!obj)printf("PRE NULL\n");
printf("CHU LAI\n");
waitMutex->unlock();
//std::this_thread::sleep_for(std::chrono::seconds(5));
//    Algorithm::loc[0]=initialObjectRect.start_x;
//    Algorithm::loc[1]=initialObjectRect.start_y;
//    Algorithm::loc[2]=initialObjectRect.end_x;
//    Algorithm::loc[3]=initialObjectRect.end_y;
//std::cout<<"Algorithm::loc[0]+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<Algorithm::loc[0]<<std::endl;
	tracking = true;
}
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}

void VWObjectTrackerAdapter::stop() {
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] called");
#endif
	tracker->removeAllObjects();
	obj = NULL;
	//delete obj;
	tracking = false;
#ifdef DEBUG_BUILD
	LOG4CXX_DEBUG(logger, "["<< id <<"] All Objects removed from Tracker, obj == NULL");
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
#endif
}
void VWObjectTrackerAdapter::setSerial(SerialCommunication* serialLink){
   this->serialLink = serialLink;
}
}
/* namespace va */

//const  nvxcu::ObjectTrackerWithFeaturesInfo::FeaturePointSet& obj_fp_info = tr_obj->getFeaturePointSet();
