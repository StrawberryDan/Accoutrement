cmake_minimum_required(VERSION 3.20)
project(Accoutrement)

find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG libavcodec libavformat libavutil)

if(NOT ${FFMPEG_FOUND})
        message(FATAL_ERROR "Could not find FFMPEG")
endif()

set(wxBUILD_SHARED False)
set(wxBUILD_MONOLITHIC True)
add_subdirectory($ENV{wxWidgets_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/3rdParty/wxWidgets)

add_executable(
        Accoutrement
        src/Main.cpp)


target_include_directories(Accoutrement PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(Accoutrement PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(Accoutrement PRIVATE wxmono ${FFMPEG_LIBRARIES})