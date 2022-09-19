add_executable(
        Accoutrement
        src/Main.cpp)
target_include_directories(Accoutrement PRIVATE Codec)
target_link_directories(Accoutrement PRIVATE Codec)
target_link_libraries(Accoutrement PRIVATE Codec wxmono libretls libressl)