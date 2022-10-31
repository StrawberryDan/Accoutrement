#include <utility>

#include "Bot/Bot.hpp"
#include "Bot/Events/GuildCreateEvent.hpp"



Bot::Bot(Token token, Intent intents)
	: mRunning(true)
    , mToken(std::move(token))
    , mIntents(intents)
    , mHTTPS("discord.com")
    , mGateway(GetGatewayEndpoint(), mToken, mIntents)
{

}



void Bot::Run()
{
	mRunning = true;
    while (mRunning)
    {
		auto gatewayMessage = mGateway->Receive();
		if (gatewayMessage)
		{
			OnGatewayMessage(gatewayMessage.Unwrap());
		}
		else
		{
			continue;
			UNREACHABLE;
		}
	}
}



void Bot::SetBehaviour(std::unique_ptr<Behaviour> behaviour)
{
	mBehaviour = std::move(behaviour);
}



void Bot::OnGatewayMessage(WebsocketMessage message)
{
	auto json = message.AsJSON().UnwrapOr({});
	if (json.is_null())
	{
		return;
	}

	switch (static_cast<int>(json["op"]))
	{
		case 0: // Update Event
		{
			const std::string type = json["t"];
			if (type == "READY")
			{
				ReadyEvent event;
				if (mBehaviour) mBehaviour->OnReady(event);
			}
			else if (json["t"] == "GUILD_CREATE")
			{
				auto event = GuildCreateEvent::Parse(json).Unwrap();
				if (mBehaviour) mBehaviour->OnGuildCreate(event);
			}

			break;
		}

		case 11: // Heartbeat Acknowledge
			break;

		default:
			std::cout << std::setw(4) << json << std::endl;
			break;
	}
}



std::string Bot::GetGatewayEndpoint()
{
    HTTPRequest request(HTTPVerb::GET, "/api/v10/gateway/bot");
    request.Header().Add("Authorization", fmt::format("Bot {}", mToken));
    request.Header().Add("Host", "discord.com");
    mHTTPS.Lock()->Request(request);
    auto response = mHTTPS.Lock()->Receive();
    Assert(response.Status() == 200);
    auto payload = std::get<HTTPChunkedPayload>(*response.Payload());
    auto json = *payload[0].AsJSON();
    auto url = (std::string) json["url"];
    url.erase(0, 6);
    return url;
}
