#pragma once



#include <string>
#include <optional>



#include "Util/Mutex.hpp"
#include "Util/Net/HTTP/HTTPClient.hpp"
#include "Util/Net/Websocket/WebsocketClient.hpp"



class Bot
{
public:
    using Token = std::string;

public:
    explicit Bot(Token token);

    void Run();

private:
    Token                           mToken;
    Mutex<HTTPSClient>              mHTTPS;
    std::optional<Mutex<WSSClient>> mWSS;
};