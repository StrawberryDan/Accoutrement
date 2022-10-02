#include <random>



#include "Util/Net/HTTP/HTTPClient.hpp"
#include "Util/Base64.hpp"



template<SocketImpl S, uint16_t PORT>
WebsocketClientImpl<S, PORT>::WebsocketClientImpl(const std::string& hostname, const std::string& subresource)
{
    HTTPClientImpl<S, PORT> handshaker(hostname);
    HTTPRequest upgradeRequest(HTTPVerb::GET, subresource);
    upgradeRequest.Header().Add("Host", hostname);
    upgradeRequest.Header().Add("Origin", hostname);
    upgradeRequest.Header().Add("Upgrade", "websocket");
    upgradeRequest.Header().Add("Connection", "Upgrade");
    upgradeRequest.Header().Add("Sec-WebSocket-Key", GenerateNonce());
    upgradeRequest.Header().Add("Sec-WebSocket-Version", "13");
    handshaker.Request(upgradeRequest);
    auto response = handshaker.Receive();
    if (response.Status() == 101)
    {
        mSocket = handshaker.TakeSocket();
    }
}



template<SocketImpl S, uint16_t PORT>
std::string WebsocketClientImpl<S, PORT>::GenerateNonce() const
{
    std::random_device randomDevice;
    std::vector<uint8_t> nonce;
    nonce.reserve(16);
    while (nonce.size() < 16)
    {
        auto val = randomDevice();
        for (int i = 0; i < sizeof(val) && nonce.size() < 16; i++)
        {
            nonce.push_back(reinterpret_cast<uint8_t*>(&val)[i]);
        }
    }

    return Base64::Encode(nonce);
}
