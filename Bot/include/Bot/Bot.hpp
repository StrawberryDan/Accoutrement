#pragma once



#include <string>
#include <optional>



#include "Util/Mutex.hpp"
#include "Util/Net/HTTP/HTTPClient.hpp"
#include "../../src/Gateway.hpp"
#include "Intent.hpp"
#include "Behaviour.hpp"



class Bot
{
public:
    using Token = std::string;

public:
    explicit Bot(Token token, Intent intents);

    void Run();



	void SetBehaviour(std::unique_ptr<Behaviour> behaviour);



private:
	void OnGatewayMessage(WebsocketMessage message);



private:
    std::string GetGatewayEndpoint();


private:
	bool                           mRunning;
    Token                          mToken;
    Intent                         mIntents;
    SharedMutex<HTTPSClient>       mHTTPS;
    Option<Gateway>                mGateway;
	std::unique_ptr<Behaviour>     mBehaviour;
};



