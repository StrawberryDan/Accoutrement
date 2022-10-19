#pragma once



#include "Util/Net/Websocket/WebsocketClient.hpp"
#include "Heartbeat.hpp"
#include "Bot/Intent.hpp"



class Gateway
{
public:
	Gateway(std::string endpoint, std::string token, Intent intents);

	Result<WebsocketMessage, WSSClient::Error> Read();
	Result<WebsocketMessage, WSSClient::Error> Receive();
	Result<WebsocketMessage, WSSClient::Error> Await(bool (* pred)(const WebsocketMessage&));
	void                                       Send(const WebsocketMessage& msg);
private:
	SharedMutex<WSSClient>        mWSS;
	std::vector<WebsocketMessage> mMessageBuffer;
	Option<Heartbeat>             mHeartbeat;
};