#include "Gateway.hpp"



Gateway::Gateway(std::string endpoint, std::string token, Intent intent)
	: mWSS(WSSClient::Connect(endpoint, "/?v=10&encoding=json").Unwrap())
	, mHeartbeat()
{
	auto hello = Receive().Unwrap().AsJSON().Unwrap();
	Assert(hello["op"] == 10);
	mHeartbeat.Emplace(mWSS, static_cast<double>(hello["d"]["heartbeat_interval"]) / 1000.0);

	nlohmann::json identifier;
	identifier["op"]                         = 2;
	identifier["d"]["token"]                 = token;
	identifier["d"]["intents"]               = std::to_underlying(intent);
	identifier["d"]["properties"]["os"]      = "windows";
	identifier["d"]["properties"]["browser"] = "strawberry";
	identifier["d"]["properties"]["device"]  = "strawberry";

	Send(WebsocketMessage(to_string(identifier)));
	auto ready = mWSS.Lock()->WaitMessage().Unwrap().AsJSON().Unwrap();
	std::cout << std::setw(4) << ready << std::endl;
}



Result<WebsocketMessage, WSSClient::Error> Gateway::Receive()
{
	while (true)
	{
		auto msg = mWSS.Lock()->ReadMessage();

		if (msg)
		{
			auto json = msg->AsJSON();
			if (mHeartbeat && json && json->at("s").is_number())
			{
				std::cout << "Sequence Number: " << static_cast<int>(json->at("s")) << std::endl;
				mHeartbeat->UpdateSequenceNumber(json.Unwrap()["s"]);
			}

			return msg;
		}
		else
		{
			if (msg.Err() == WSSClient::Error::NoMessage)
			{
				continue;
			}
			else
			{
				return msg;
			}
		}
	}
}



void Gateway::Send(const WebsocketMessage& msg)
{
	mWSS.Lock()->SendMessage(msg);
}
