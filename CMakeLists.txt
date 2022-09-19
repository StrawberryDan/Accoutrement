cmake_minimum_required(VERSION 3.0)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 20)



set(wxBUILD_SHARED False)
set(wxBUILD_MONOLITHIC True)
add_subdirectory($ENV{wxWidgets_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/3rdParty/wxWidgets)



add_library(FFMPEG INTERFACE)
target_include_directories(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/include)
target_link_libraries(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/lib/libavformat.a $ENV{FFMPEG_ROOT}/lib/libavcodec.a $ENV{FFMPEG_ROOT}/lib/libswresample.a $ENV{FFMPEG_ROOT}/lib/libavutil.a lzma bz2 z bcrypt atomic m user32 opus iconv mfuuid ole32 strmiids secur32 ws2_32)



add_library(Codec STATIC
        Codec/include/Codec/AudioFile.hpp
        Codec/src/AudioFile.cpp
        Codec/src/Packet.cpp
        Codec/include/Codec/Packet.hpp
        Codec/src/Decoder.cpp
        Codec/include/Codec/Decoder.hpp
        Codec/src/Frame.cpp
        Codec/include/Codec/Frame.hpp
        Codec/src/Resampler.cpp
        Codec/include/Codec/Resampler.hpp
        Codec/src/OpusEncoder.cpp
        Codec/include/Codec/OpusEncoder.hpp
        Codec/include/Codec/Sample.hpp
        Codec/src/Sample.cpp
        Codec/src/Muxer.cpp
        Codec/include/Codec/Muxer.hpp
        Codec/src/Utilities.hpp)
target_include_directories(Codec PUBLIC Codec/include FFMPEG)
target_link_directories(Codec PUBLIC FFMPEG)
target_link_libraries(Codec PUBLIC FFMPEG sodium)

add_executable(Codec_Test Codec/test/Main.cpp)
target_include_directories(Codec_Test PRIVATE Codec)
target_link_libraries(Codec_Test PRIVATE Codec)



add_executable(
        Accoutrement
        Accoutrement/src/Main.cpp)
target_include_directories(Accoutrement PRIVATE Codec)
target_link_directories(Accoutrement PRIVATE Codec)
target_link_libraries(Accoutrement PRIVATE Codec wxmono libretls libressl)