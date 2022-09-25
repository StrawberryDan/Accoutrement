add_library(Util STATIC
        include/Util/Utilities.hpp
        include/Util/Net/Socket/TLSClient.hpp
        src/Net/Socket/TLSClient.cpp
        include/Util/Net/Socket/Socket.inl
        include/Util/Net/Socket/Socket.hpp
        src/Net/Socket/UDPClient.cpp
        include/Util/Net/Socket/UDPClient.hpp
        src/Net/Socket/TCPClient.cpp
        include/Util/Net/Socket/TCPClient.hpp
        include/Util/Net/HTTP/HTTPClient.inl
        include/Util/Net/HTTP/HTTPClient.hpp
        src/Net/HTTP/HTTPRequest.cpp
        include/Util/Net/HTTP/HTTPRequest.hpp
        src/Net/HTTP/HTTPResponse.cpp
        include/Util/Net/HTTP/HTTPResponse.hpp
        src/Net/HTTP/HTTPConstants.cpp
        include/Util/Net/HTTP/HTTPConstants.hpp
        src/Net/HTTP/HTTPHeader.cpp
        include/Util/Net/HTTP/HTTPHeader.hpp
        src/Net/HTTP/HTTPSimplePayload.cpp
        include/Util/Net/HTTP/HTTPSimplePayload.hpp
        src/Net/HTTP/HTTPChunkedPayload.cpp
        include/Util/Net/HTTP/HTTPChunkedPayload.hpp
        include/Util/Net/HTTP/HTTPPayload.hpp)
target_include_directories(Util PUBLIC include)
target_link_libraries(Util PRIVATE libressl libretls ws2_32 fmt)