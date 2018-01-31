
cmake_minimum_required(VERSION 2.8)
project(tt)

# setup tensorRT flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")	# -std=gnu++11
set(BUILD_DEPS "YES" CACHE BOOL "If YES, will install dependencies into sandbox.  Automatically reset to NO after dependencies are installed.")


# if this is the first time running cmake, perform pre-build dependency install script (or if the user manually triggers re-building the dependencies)
#if( ${BUILD_DEPS} )
	#message("Launching pre-build dependency installer script...")

	#execute_process(COMMAND sh ../CMakePreBuild.sh
				#WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
				#RESULT_VARIABLE PREBUILD_SCRIPT_RESULT)

	#set(BUILD_DEPS "NO" CACHE BOOL "If YES, will install dependencies into sandbox.  Automatically reset to NO after dependencies are installed." FORCE)
	#message("Finished installing dependencies")
#endif()


# Qt is used to load images (installed by ubuntu-desktop)
find_package(Qt4 COMPONENTS QTGui REQUIRED)
include(${QT_USE_FILE})
message("Libssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss:${QT_LIBRARIES}")
add_definitions(${QT_DEFINITIONS})

LINK_DIRECTORIES("/usr/lib")
# setup CUDA
find_package(CUDA)
message("-- system archKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKkk:  ${CUDA_LIBRARIES}")
set(
	CUDA_NVCC_FLAGS
	${CUDA_NVCC_FLAGS}; 
   
    -O0 
	-gencode arch=compute_53,code=sm_53
	-gencode arch=compute_60,code=sm_60
)
find_package(OpenCV REQUIRED)
message("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb:  ${OpenCV_LIBRARIES}")
find_library(NVX visionworks)
message("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx:  ${NVX}")
set( DEBUG_BUILD,1)
set(DEBUG_COUNTER,1)
set(ENABLE_MAV_DEBUG_MSG,1)
set(MAV_DEBUG_MSG_IN,5)
# setup project output paths
#PROJECT_BINARY_DIR=/home/ubuntu/tt/build
#${CMAKE_SYSTEM_PROCESSOR}=aarch64
#PROJECT_OUTPUT_DIR=/home/ubuntu/tt/build/aarch64
#PROJECT_INCLUDE_DIR=/home/ubuntu/tt/build/aarch64/include
set(PROJECT_OUTPUT_DIR  ${PROJECT_BINARY_DIR}/${CMAKE_SYSTEM_PROCESSOR})
set(PROJECT_INCLUDE_DIR ${PROJECT_OUTPUT_DIR}/include)

file(MAKE_DIRECTORY ${PROJECT_INCLUDE_DIR})
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)

message("-- system arch:  ${CMAKE_SYSTEM_PROCESSOR}")
message("-- output path:  ${PROJECT_OUTPUT_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_OUTPUT_DIR}/lib)


# build C/C++ interface
#include_directories(${PROJECT_INCLUDE_DIR} ${GIE_PATH}/include)
include_directories(/usr/include/NVXIO /home/ubuntu/libvisionworks-nvxio-1.5.3.71n/nvxio/src /home/ubuntu/libvisionworks-nvxio-1.5.3.71n/nvxio/include /home/ubuntu/SeriesApp/util /home/ubuntu/SeriesApp/util/cuda /home/ubuntu/SeriesApp/util/display /home/ubuntu/SeriesApp/util/camera /home/ubuntu/SeriesApp/src/algorithm /home/ubuntu/SeriesApp/src/communications/messagee /home/ubuntu/SeriesApp/src/tools /home/ubuntu/SeriesApp/config /usr/local/cuda-8.0/include /usr/include/gstreamer-1.0 /usr/include/c++/4.8 /usr/include/c++/4.8/backward /usr/include/x86_64_gnu /usr/include/x86_64_gnu/c++/4.8 /usr/lib/gcc/x86_64-linux-gnu/4.8/include /usr/lib/gcc/x86_64-linux-gnu/4.8/include-fixed /home/ubuntu/SeriesApp /home/ubuntu/SeriesApp/src /usr/include /usr/include/log4cxx /usr/include/glib-2.0 /usr/include/glibmm-2.4 /usr/include/libxml++-2.6 /usr/lib/aarch64-linux-gnu/glibmm-2.4/include /usr/lib/aarch64-linux-gnu/libxml++-2.6/include /usr/lib/aarch64-linux-gnu/gstreamer-1.0/include /usr/include/glib-2.0 /usr/include/libxml2 /usr/lib/aarch64-linux-gnu/glib-2.0/include/)

file(GLOB inferenceSources *.cpp *.cu util/*.cpp util/camera/*.cpp util/cuda/*.cu src/algorithm/*.cpp src/algorithm/visionWorks/objectTracker/*.cpp src/communications/*.cpp src/communications/messagee/*.cpp src/controller/*.cpp src/detectnet-camera/*.cpp src/pipeline/*.cpp src/tools/*.cpp conf/*.xml config/*.xml *.conf )
file(GLOB inferenceIncludes *.h util/*.h util/camera/*.h util/cuda/*.h src/algorithm/*.h src/algorithm/visionWorks/objectTracker/*.h *.hpp src/communications/*.h src/communications/messagee/*.h src/controller/*.h src/detectnet-camera/*.h src/pipeline/*.h src/tools/*.h config/*.h c_library_tx1/*.h c_library_tx1/common/*.h c_library_tx1/tx1/*.h c_library_tx1_debug/*.h c_library_tx1_debug/tx1_debug/*.h c_library_tx1_debug/common/*.h c_library_tx1_debug/tx1/*.h)

cuda_add_library(jetson-inference SHARED ${inferenceSources})
target_link_libraries(jetson-inference nvcaffe_parser nvinfer Qt4::QtGui GL gstreamer-1.0 gstapp-1.0 log4cxx glib-2.0 gstapp-1.0 xml++-2.6 gobject-2.0 nvxio ${NVX})		# gstreamer-0.10 gstbase-0.10 gstapp-0.10 


# transfer all headers to the include directory
foreach(include ${inferenceIncludes})
	message("-- Copying ${include}")
	configure_file(${include} ${PROJECT_INCLUDE_DIR} COPYONLY)
endforeach()


# create symbolic link for network data
#execute_process( COMMAND "${CMAKE_COMMAND}" "-E" "create_symlink" "${PROJECT_SOURCE_DIR}/data/networks" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/networks" )
  
  
# copy image data
#file(GLOB imageData ${PROJECT_SOURCE_DIR}/data/images/*)

#foreach(image ${imageData})
	#message("-- Copying ${image}")
	#file(COPY ${image} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
	#configure_file(${include} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
#endforeach()


# copy tools
#file(COPY "tools/segnet-batch.sh" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


# build samples & utilities
#add_subdirectory(imagenet-console)
#add_subdirectory(imagenet-camera)

#add_subdirectory(detectnet-console)
add_subdirectory(mm)

#add_subdirectory(segnet-console)

#add_subdirectory(util/camera/gst-camera)
#add_subdirectory(util/camera/v4l2-console)
#add_subdirectory(util/camera/v4l2-display)

#add_subdirectory(docs)


# install
foreach(include ${inferenceIncludes})
    install(FILES "${include}" DESTINATION include/jetson-inference)
endforeach()

# install the shared library
install(TARGETS jetson-inference DESTINATION lib/jetson-inference EXPORT jetson-inferenceConfig)

# install the cmake project, for importing
install(EXPORT jetson-inferenceConfig DESTINATION share/jetson-inference/cmake)

