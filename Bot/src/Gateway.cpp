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



Result<WebsocketMessage, WSSClient::Error> Gateway::Read()
{
	if (mMessageBuffer.empty())
	{
		return Receive();
	}
	else
	{
		auto msg = std::move(mMessageBuffer[0]);
		mMessageBuffer.erase(mMessageBuffer.begin());
		return msg;
	}
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



Result<WebsocketMessage, WSSClient::Error> Gateway::Await(bool (* pred)(const WebsocketMessage&))
{
	auto bufferCheck = std::find_if(mMessageBuffer.begin(), mMessageBuffer.end(), pred);
	if (bufferCheck != mMessageBuffer.end())
	{
		return *bufferCheck;
	}

	while (true)
	{
		auto msg = Receive();

		if (msg)
		{
			if (pred(*msg))
			{
				return msg.Unwrap();
			}
			else
			{
				mMessageBuffer.push_back(msg.Unwrap());
			}
		}
	}
}



void Gateway::Send(const WebsocketMessage& msg)
{
	mWSS.Lock()->SendMessage(msg);
}
