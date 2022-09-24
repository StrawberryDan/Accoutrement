add_library(Util STATIC
        include/Util/Utilities.hpp
        include/Util/Net/TLSSocket.hpp
        src/Net/TLSSocket.cpp
        include/Util/Net/Socket.cpp
        include/Util/Net/Socket.inl
        include/Util/Net/Socket.hpp)
target_include_directories(Util PUBLIC include)
target_link_libraries(Util PRIVATE libressl libretls)