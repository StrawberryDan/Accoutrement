#pragma once



#include <string>
#include <optional>



#include "Util/Mutex.hpp"
#include "Util/Net/HTTP/HTTPClient.hpp"
#include "../../src/Gateway.hpp"
#include "Intent.hpp"



class Bot
{
public:
    using Token = std::string;

public:
    explicit Bot(Token token, Intent intents);

    void Run();

private:
    std::string GetGatewayEndpoint();

private:
    Token                          mToken;
    Intent                         mIntents;
    SharedMutex<HTTPSClient>       mHTTPS;

    Option<Gateway>                mGateway;
};



