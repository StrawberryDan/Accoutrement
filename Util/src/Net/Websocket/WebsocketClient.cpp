#include "Util/Net/Websocket/WebsocketClient.hpp"



#include "Util/Net/HTTP/HTTPClient.hpp"



template<SocketImpl S, uint16_t PORT>
WebsocketClientImpl<S, PORT>::WebsocketClientImpl(const std::string& hostname)
{
    HTTPClientImpl<S, PORT> handshaker(hostname);

    HTTPRequest upgradeRequest(HTTPVerb::GET, hostname);
    upgradeRequest.Header().Add("Upgrade", "websocket");
    upgradeRequest.Header().Add("Connection", "Upgrade");
}
