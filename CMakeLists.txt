cmake_minimum_required(VERSION 3.20)
project(Accoutrement)

set(wxBUILD_SHARED False)
set(wxBUILD_MONOLITHIC True)
add_subdirectory(wxWidgets/3.2.0)

add_executable(
        Accoutrement
        src/Main.cpp)


target_link_libraries(Accoutrement PRIVATE wxmono)