cmake_minimum_required(VERSION 3.0)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

find_path(TLS_H_DIR tls.h)
find_path(TLS_LIB_DIR liblibretls.a)

message(STATUS ${TLS_LIB_DIR})

set(wxBUILD_SHARED False)
set(wxBUILD_MONOLITHIC True)
add_subdirectory($ENV{wxWidgets_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/3rdParty/wxWidgets)



add_library(FFMPEG INTERFACE)
target_include_directories(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/include)
target_link_directories(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/lib)
target_link_libraries(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/lib/libavformat.a $ENV{FFMPEG_ROOT}/lib/libavcodec.a $ENV{FFMPEG_ROOT}/lib/libswresample.a $ENV{FFMPEG_ROOT}/lib/libavutil.a lzma bz2 z bcrypt atomic m user32 opus iconv mfuuid ole32 strmiids secur32 ws2_32)



add_library(Codec STATIC
        Codec/include/AudioFile.hpp
        Codec/src/AudioFile.cpp
        Codec/src/Packet.cpp
        Codec/include/Packet.hpp
        Codec/src/Decoder.cpp
        Codec/include/Decoder.hpp
        Codec/src/Frame.cpp
        Codec/include/Frame.hpp
        Codec/src/Resampler.cpp
        Codec/include/Resampler.hpp
        Codec/src/OpusEncoder.cpp
        Codec/include/OpusEncoder.hpp
        Codec/include/Sample.hpp
        Codec/src/Muxer.cpp
        Codec/include/Muxer.hpp
        Codec/src/Utilities.hpp)
target_include_directories(Codec PUBLIC Codec/include PRIVATE FFMPEG)
target_link_directories(Codec PRIVATE FFMPEG)
target_link_libraries(Codec PRIVATE FFMPEG sodium)



add_executable(
        Accoutrement
        Accoutrement/src/Main.cpp)
target_include_directories(Accoutrement PRIVATE Codec ${TLS_H_DIR})
target_link_directories(Accoutrement PRIVATE Codec ${TLS_LIB_DIR}/lib)
target_link_libraries(Accoutrement PRIVATE Codec wxmono libretls libressl)