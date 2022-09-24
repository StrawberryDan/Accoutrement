add_library(Util STATIC
        include/Util/Utilities.hpp
        include/Util/Net/TLSClient.hpp
        src/Net/TLSClient.cpp
        include/Util/Net/Socket.inl
        include/Util/Net/Socket.hpp src/Net/UDPClient.cpp include/Util/Net/UDPClient.hpp src/Net/TCPClient.cpp include/Util/Net/TCPClient.hpp src/Net/HTTPClient.cpp include/Util/Net/HTTPClient.hpp)
target_include_directories(Util PUBLIC include)
target_link_libraries(Util PRIVATE libressl libretls ws2_32)