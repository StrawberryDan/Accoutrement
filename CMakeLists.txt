cmake_minimum_required(VERSION 3.0)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 23)


include_directories(/usr/local/include)
link_directories(/usr/local/lib)



# WXWIDGETS
set(wxBUILD_SHARED False)
set(wxBUILD_MONOLITHIC True)
set(wxBUILD_CXX_STANDARD 20)
set(wxBUILD_PRECOMP ON)
set(wxUSE_LIBTIFF OFF)
add_subdirectory($ENV{wxWidgets_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/3rdParty/wxWidgets)



#FFMPEG
add_library(FFMPEG INTERFACE)
target_include_directories(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/include)

if (WIN32)
    target_link_libraries(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/lib/libavformat.a $ENV{FFMPEG_ROOT}/lib/libavcodec.a $ENV{FFMPEG_ROOT}/lib/libswresample.a $ENV{FFMPEG_ROOT}/lib/libavutil.a lzma bz2 z bcrypt atomic m user32 opus iconv mfuuid ole32 strmiids secur32 ws2_32)
elseif(APPLE)
    target_link_libraries(FFMPEG INTERFACE $ENV{FFMPEG_ROOT}/lib/libavformat.a $ENV{FFMPEG_ROOT}/lib/libavcodec.a $ENV{FFMPEG_ROOT}/lib/libswresample.a $ENV{FFMPEG_ROOT}/lib/libavutil.a lzma bz2 z m opus iconv "-framework CoreVideo" "-framework AudioToolbox" "-framework VideoToolbox" "-framework CoreFoundation" "-framework CoreMedia" "-framework CoreServices" "-framework Security")
endif()



# MODULES
add_subdirectory(Util)
add_subdirectory(Codec)
add_subdirectory(Bot)
add_subdirectory(Accoutrement)