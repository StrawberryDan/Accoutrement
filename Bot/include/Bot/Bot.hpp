#pragma once



#include <string>
#include <optional>



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
    Token                    mToken;
    HTTPSClient              mHTTPS;
    std::optional<WSSClient> mWSS;
};