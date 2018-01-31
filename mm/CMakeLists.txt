
file(GLOB detectcameraSources *.cpp)
file(GLOB detectcameraIncludes *.h )
link_directories("/usr/lib")
cuda_add_executable(tt ${detectcameraSources})
target_link_libraries(tt nvcaffe_parser nvinfer jetson-inference log4cxx visionworks nvxio )
