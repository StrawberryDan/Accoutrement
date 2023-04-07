cmake_minimum_required(VERSION 3.0)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 23)



# MODULES
add_subdirectory(../Core    ${CMAKE_CURRENT_BINARY_DIR}/Core)
add_subdirectory(../Codec   ${CMAKE_CURRENT_BINARY_DIR}/Codec)
add_subdirectory(../Discord ${CMAKE_CURRENT_BINARY_DIR}/Discord)
add_subdirectory(Accoutrement)