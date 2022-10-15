#pragma once



#include "Util/Net/Websocket/WebsocketClient.hpp"
#include "Bot/Heartbeater.hpp"
#include "Bot/Intent.hpp"



class Gateway
{
public:
	Gateway(std::string endpoint, std::string token, Intent intents);

	Result<WebsocketMessage, WSSClient::Error> Receive();
	void                                       Send(const WebsocketMessage& msg);
private:
	SharedMutex<WSSClient> mWSS;
	Option<Heartbeater>    mHeartbeat;
};