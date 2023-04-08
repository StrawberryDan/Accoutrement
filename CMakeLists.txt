cmake_minimum_required(VERSION 3.0)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 23)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-fno-omit-frame-pointer -fsanitize=thread -fsanitize=undefined)
    add_link_options(-fno-omit-frame-pointer -fsanitize=thread -fsanitize=undefined)
endif()


# MODULES
add_subdirectory(../Core    ${CMAKE_CURRENT_BINARY_DIR}/Core)
add_subdirectory(../Codec   ${CMAKE_CURRENT_BINARY_DIR}/Codec)
add_subdirectory(../Discord ${CMAKE_CURRENT_BINARY_DIR}/Discord)


find_package(wxWidgets REQUIRED
        COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(
        Accoutrement
        src/Main.cpp
        src/UI/MainWindow.cpp
        src/UI/MainWindow.hpp
        src/UI/ChannelSelector.cpp
        src/UI/ChannelSelector.hpp
        src/Discord/Bot.hpp
        src/Discord/Bot.cpp
        src/Config.cpp
        src/Config.hpp
)
target_include_directories(Accoutrement PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(Accoutrement PRIVATE Strawberry_Discord Strawberry_Codec Strawberry_Core ${wxWidgets_LIBRARIES})