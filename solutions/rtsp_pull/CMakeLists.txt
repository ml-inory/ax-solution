cmake_minimum_required(VERSION 3.0)

set(THIRDPARTY ../../third-party-install)
set(BSP_DIR ${THIRDPARTY}/ax_bsp)
set(JSONCPP ${THIRDPARTY}/jsoncpp)
set(OPENCV ${THIRDPARTY}/opencv-arm-linux)
set(RTSP ${THIRDPARTY}/RTSP)
set(RTSPSERVER ${THIRDPARTY}/RTSPServer)

link_directories(${BSP_DIR}/lib)
link_directories(${JSONCPP}/lib)
link_directories(${OPENCV}/lib)
link_directories(${RTSP}/lib)
link_directories(${RTSPSERVER}/lib)

include_directories(../../inc)
include_directories(${BSP_DIR}/include)
include_directories(${JSONCPP}/include)
include_directories(${OPENCV}/include/opencv4)
include_directories(${RTSP}/include)
include_directories(${RTSPSERVER}/include)

add_executable(rtsp_pull main.cpp)
target_link_libraries(rtsp_pull
    ax_sys
    ax_vdec
    libjsoncpp.a
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    rtspclisvr
    RtspServer
    pthread
    dl)

set(CMAKE_INSTALL_PREFIX install)
install(TARGETS rtsp_pull
    RUNTIME DESTINATION bin)