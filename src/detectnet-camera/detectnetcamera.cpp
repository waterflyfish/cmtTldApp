/*
 * http://github.com/dusty-nv/jetson-inference
 */


#include "ControlMessage.h"
#include "TrackingMessage.h"
#include "TrackedTargetMessage.h"
#include "Configuration.h"
#include "logger_config.h"

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "gstCamera.h"
//#include "glTexture.h"
//#include "glDisplay.h"
#include "cudaMappedMemory.h"
#include "cudaNormalize.h"
#include "cudaFont.h"
#include "detectnetcamera.h"
#include "detectNet.h"
namespace va{
detectnetcamera::detectnetcamera(string anaName, string loggerName):
				Algorithm(anaName, loggerName), vxContext(), devMem(nullptr), devMemPitch(0ul){
	type = AlgorithmType::TRACKING;

		tracking = false;
		sink = NULL;
		sendDebug = false;
		debugMsgSerial = 0;
}
detectnetcamera::~detectnetcamera() {
	//LOG4CXX_DEBUG(logger, "["<< id <<"] called");

	//LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");

}

bool detectnetcamera::processMessage(Message* msg) {

	LOG4CXX_DEBUG(logger, "["<< id <<"] called");

	msg->logMessage(); // TODO this a debug log that is unneccessary here - totally.
	switch (msg->getType()) {
	case Message::MessageType::STATUS:

	case Message::MessageType::CONTROL: {
		ControlMessage* ctrlMsg = dynamic_cast<ControlMessage*>(msg);
		switch (ctrlMsg->getConcreteType()) {
		case ControlMessage::ControlMessageType::TRACKING_START:
			//start(dynamic_cast<TrackingMessage*>(msg));
			break;
		case ControlMessage::ControlMessageType::TRACKING_STOP:
			//stop();
			break;

		case ControlMessage::ControlMessageType::REQ_FP: {
			//FeaturePointsRequestDebugControlMessage* fpMsg = dynamic_cast<FeaturePointsRequestDebugControlMessage*>(msg);
			//LOG4CXX_ERROR(logger, "["<< id <<"] ControlMessage::ControlMessageType::REQ_FP:  " + fpMsg->isStart());
//			if (fpMsg->isStart()) {
//				this->sendDebug = true;
//			} else {
//
//				if (!fpMsg->isStart()) {
//					this->sendDebug = false;
//				}
//			}
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
	LOG4CXX_DEBUG(logger, "["<< id <<"] exiting");
	return true;
}


void detectnetcamera::run(){
//
////		if( signal(SIGINT, sig_handler) == SIG_ERR )
////			printf("\ncan't catch SIGINT\n");
//
//
//		/*
//		 * create the camera device
//		 */
		gstCamera* camera = gstCamera::Create(DEFAULT_CAMERA);

		if( !camera )
		{
			printf("\ndetectnet-camera:  failed to initialize video device\n");

		}
		camera->setGst(sink);
		//camera->setTee(tee);
        camera->init();
		printf("\ndetectnet-camera:  successfully initialized video device\n");
		printf("    width:  %u\n", camera->GetWidth());
		printf("   height:  %u\n", camera->GetHeight());
		printf("    depth:  %u (bpp)\n\n", camera->GetPixelDepth());
//
//
//		/*
//		 * create detectNet
//		 */
		int argc =1;
		char* argv[argc];
		argv[0] ="./detectnet-camera";
		net = detectNet::Create(argc, argv);

		if( !net )
		{
			printf("detectnet-camera:   failed to initialize imageNet\n");

		}


		/*
		 * allocate memory for output bounding boxes and class confidence
		 */
		const uint32_t maxBoxes = net->GetMaxBoundingBoxes();		printf("maximum bounding boxes:  %u\n", maxBoxes);
		const uint32_t classes  = net->GetNumClasses();

		float* bbCPU    = NULL;
		float* bbCUDA   = NULL;
		float* confCPU  = NULL;
		float* confCUDA = NULL;

		if( !cudaAllocMapped((void**)&bbCPU, (void**)&bbCUDA, maxBoxes * sizeof(float4)) ||
		    !cudaAllocMapped((void**)&confCPU, (void**)&confCUDA, maxBoxes * classes * sizeof(float)) )
		{
			printf("detectnet-console:  failed to alloc output memory\n");

		}

//
//		/*
//		 * create openGL window
//		 */
////
////		       glDisplay* display = glDisplay::Create();
////		       glTexture* texture = NULL;
////
////		if( !display ) {
////			printf("\ndetectnet-camera:  failed to create openGL display\n");
////		}
////		else
////		{
////			texture = glTexture::Create(camera->GetWidth(), camera->GetHeight(), GL_RGBA32F_ARB/*GL_RGBA8*/);
////
////			if( !texture )
////				printf("detectnet-camera:  failed to create openGL texture\n");
////		}
//
//
//		/*
//		 * create font
//		 */
//		cudaFont* font = cudaFont::Create();
//
//
//		/*
//		 * start streaming
//		 */
		//Algorithm::gstreamerFlag=false;
		if( !camera->Open() )
		{
			printf("\ndetectnet-camera:  failed to open camera for streaming\n");

		}
		Algorithm::gstreamerFlag=true;
//
//		printf("\ndetectnet-camera:  camera open for streaming\n");
//
//
//		/*
//		 * processing loop
//		 */
//		float confidence = 0.0f;
//
		while( true )
		{

	                printf("QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ\n");
	                void* imgCPU  = NULL;
			void* imgCUDA = NULL;

			// get the latest frame
			if( !camera->Capture(&imgCPU, &imgCUDA, 1000) )
				printf("\ndetectnet-camera:  failed to capture frame\n");

			// convert from YUV to RGBA
			void* imgRGBA = NULL;

			if( !camera->ConvertRGBA(imgCUDA, &imgRGBA) )
				printf("detectnet-camera:  failed to convert from NV12 to RGBA\n");

			// classify image with detectNet
			int numBoundingBoxes = maxBoxes;

			if( net->Detect((float*)imgRGBA, camera->GetWidth(), camera->GetHeight(), bbCPU, &numBoundingBoxes, confCPU))
			{
				printf("%i bounding boxes detected\n", numBoundingBoxes);

				int lastClass = 0;
				int lastStart = 0;
                num++;
                printf("%i vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n",num);
				for( int n=0; n < numBoundingBoxes; n++ )
				{
					const int nc = confCPU[n*2+1];
					float* bb = bbCPU + (n * 4);

					printf("bounding box %i   (%f, %f)  (%f, %f)  w=%f  h=%f\n", n, bb[0], bb[1], bb[2], bb[3], bb[2] - bb[0], bb[3] - bb[1]);
					  if(bb[0]>=Algorithm::loc[0]&&bb[1]<=Algorithm::loc[1]&&bb[2]<=Algorithm::loc[2]&&bb[3]>=Algorithm::loc[3]){
					                                                             Algorithm::loc[0]=bb[0];
					                                                             Algorithm::loc[1]=bb[1];
					                                                             Algorithm::loc[2]=bb[2];
					                                                             Algorithm::loc[3]=bb[3];
					                                                             Algorithm::loc[4]=1;
					                                                             Algorithm::loc[5]=1;
					                                          }
//=========================================================================================================================================
//					TrackedTargetMessage* targetMsg = new TrackedTargetMessage(
//													to_string(MAVLINK_MSG_ID_TX1_STATUS_TRACKING_TARGET_LOCATION), LOG_MSG,
//													Configuration::getInstance().getCommunicationsControllerId());
//					                        targetMsg->setCenterX(1);
//					                        targetMsg->setCenterY(num);
//											targetMsg->setTlX(bb[0]);
//											targetMsg->setTlY(bb[1]);
//											targetMsg->setBrX(bb[2]);
//											targetMsg->setBrY(bb[3]);
//											handleMessage(dynamic_cast<Message*>(targetMsg));
//											targetMsg = NULL;
//											delete targetMsg;
//========================================================================================================================================
					if( nc != lastClass || n == (numBoundingBoxes - 1) )
					{
						if( !net->DrawBoxes((float*)imgRGBA, (float*)imgRGBA, camera->GetWidth(), camera->GetHeight(),
							                        bbCUDA + (lastStart * 4), (n - lastStart) + 1, lastClass) )
							printf("detectnet-console:  failed to draw boxes\n");

						lastClass = nc;
						lastStart = n;

						CUDA(cudaDeviceSynchronize());
					}
				}

				/*if( font != NULL )
				{
					char str[256];
					sprintf(str, "%05.2f%% %s", confidence * 100.0f, net->GetClassDesc(img_class));

					font->RenderOverlay((float4*)imgRGBA, (float4*)imgRGBA, camera->GetWidth(), camera->GetHeight(),
									    str, 10, 10, make_float4(255.0f, 255.0f, 255.0f, 255.0f));
				}*/
//
//				if( display != NULL )
//				{
//					char str[256];
//					sprintf(str, "TensorRT build %x | %s | %04.1f FPS", NV_GIE_VERSION, net->HasFP16() ? "FP16" : "FP32", display->GetFPS());
//					sprintf(str, "GIE build %x | %s | %04.1f FPS | %05.2f%% %s", NV_GIE_VERSION, net->GetNetworkName(), display->GetFPS(), confidence * 100.0f, net->GetClassDesc(img_class));
//					display->SetTitle(str);
//				}
			}


			// update display
//			if( display != NULL )
//			{
//				display->UserEvents();
//				display->BeginRender();
//
//				if( texture != NULL )
//				{
//					// rescale image pixel intensities for display
//					CUDA(cudaNormalizeRGBA((float4*)imgRGBA, make_float2(0.0f, 255.0f),
//									   (float4*)imgRGBA, make_float2(0.0f, 1.0f),
//			 						   camera->GetWidth(), camera->GetHeight()));
//
//					// map from CUDA to openGL using GL interop
//					void* tex_map = texture->MapCUDA();
//
//					if( tex_map != NULL )
//					{
//						cudaMemcpy(tex_map, imgRGBA, texture->GetSize(), cudaMemcpyDeviceToDevice);
//						texture->Unmap();
//					}
//
//					// draw the texture
//					texture->Render(100,100);
//				}
//
//				display->EndRender();
//			}
		}

		printf("\ndetectnet-camera:  un-initializing video device\n");


		/*
		 * shutdown the camera device
		 */
		if( camera != NULL )
		{
			delete camera;
			camera = NULL;
		}

//		if( display != NULL )
//		{
//			delete display;
//			display = NULL;
//		}

		printf("detectnet-camera:  video device has been un-initialized.\n");
		printf("detectnet-camera:  this concludes the test of the video device.\n");

}

}




