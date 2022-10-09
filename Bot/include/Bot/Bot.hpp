#pragma once



#include <string>
#include <optional>



#include "Util/Mutex.hpp"
#include "Util/Net/HTTP/HTTPClient.hpp"
#include "Util/Net/Websocket/WebsocketClient.hpp"
#include "Heartbeater.hpp"



class Bot
{
public:
    using Token = std::string;

public:
    explicit Bot(Token token);

    void Run();

private:
    std::string GetGatewayEndpoint();

private:
    Token                                 mToken;
    SharedMutex<HTTPSClient>              mHTTPS;
    Option<SharedMutex<WSSClient>> mWSS;
    Option<Heartbeater>            mHeartbeat;
};