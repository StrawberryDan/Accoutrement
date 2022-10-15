add_library(Util STATIC
        include/Util/Base64.hpp
        include/Util/Math/Math.hpp
        include/Util/Math/Math.inl
        include/Util/Net/HTTP/HTTPChunkedPayload.hpp
        include/Util/Net/HTTP/HTTPClient.hpp
        include/Util/Net/HTTP/HTTPClient.inl
        include/Util/Net/HTTP/HTTPConstants.hpp
        include/Util/Net/HTTP/HTTPHeader.hpp
        include/Util/Net/HTTP/HTTPPayload.hpp
        include/Util/Net/HTTP/HTTPRequest.hpp
        include/Util/Net/HTTP/HTTPResponse.hpp
        include/Util/Net/HTTP/HTTPSimplePayload.hpp
        include/Util/Net/Socket/Socket.hpp
        include/Util/Net/Socket/Socket.inl
        include/Util/Net/Socket/TCPClient.hpp
        include/Util/Net/Socket/TLSClient.hpp
        include/Util/Net/Socket/UDPClient.hpp
        include/Util/Net/Websocket/WebsocketClient.hpp
        include/Util/Utilities.hpp
        src/Base64.cpp
        src/Net/HTTP/HTTPChunkedPayload.cpp
        src/Net/HTTP/HTTPConstants.cpp
        src/Net/HTTP/HTTPHeader.cpp
        src/Net/HTTP/HTTPRequest.cpp
        src/Net/HTTP/HTTPResponse.cpp
        src/Net/HTTP/HTTPSimplePayload.cpp
        src/Net/Socket/TCPClient.cpp
        src/Net/Socket/TLSClient.cpp
        src/Net/Socket/UDPClient.cpp
        include/Util/Net/Websocket/WebsocketClient.inl
        include/Util/Mutex.hpp
        src/Net/Websocket/WebsocketMessage.cpp
        include/Util/Net/Websocket/WebsocketMessage.hpp
        include/Util/Result.hpp
        src/Clock.cpp
        include/Util/Clock.hpp
        include/Util/Endian.hpp
        include/Util/Option.hpp
        src/Net/Socket/Socket.cpp
        include/Util/Assert.hpp)
target_include_directories(Util PUBLIC include)
target_link_libraries(Util PRIVATE libressl libretls ws2_32 fmt)

add_executable(Util_Test
        test/Main.cpp)
target_link_libraries(Util_Test PRIVATE Util)