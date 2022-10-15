#include <utility>

#include "Bot/Bot.hpp"



Bot::Bot(Token token, Intent intents)
    : mToken(std::move(token))
    , mIntents(intents)
    , mHTTPS("discord.com")
    , mGateway(GetGatewayEndpoint(), mToken, mIntents)
{

}



void Bot::Run()
{
    while (true)
    {
        auto msg = mGateway->Receive();
        if (msg)
        {
            auto json = msg->AsJSON();
            if (json)
            {
                std::cout << std::setw(8) << json.Unwrap() << std::endl;
            }
            else if (msg->GetOpcode() == WebsocketMessage::Opcode::Close)
            {
                std::cout << "Closing: " << msg->GetCloseStatusCode() << std::endl;
            }
            else
            {
                std::cout << reinterpret_cast<const char*>(msg->AsString().c_str()) << std::endl;
            }
        }
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
