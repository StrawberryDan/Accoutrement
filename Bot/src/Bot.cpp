#include <utility>

#include "Bot/Bot.hpp"



Bot::Bot(Token token)
    : mToken(std::move(token))
    , mHTTPS("discord.com")
    , mWSS()
{
    auto gateway = GetGatewayEndpoint();
    mWSS.Emplace(gateway, "/?v=10&encoding=json");
    assert(mWSS->Lock()->IsValid());

    auto hello = mWSS->Lock()->ReadMessage();
    while (!hello)
    {
        hello = mWSS->Lock()->ReadMessage();
    }

    auto helloJSON = hello->AsJSON().Unwrap();
    std::cout << std::setw(8) << helloJSON << std::endl;
    nlohmann::json op;
    double interval;
    if (helloJSON.is_array())
    {
        op = helloJSON[0]["op"];
        interval = static_cast<double>(helloJSON[0]["d"]["heartbeat_interval"]) / 1000.0;
    }
    else
    {
        op = helloJSON["op"];
        interval = static_cast<double>(helloJSON["d"]["heartbeat_interval"]) / 1000.0;
    }

    assert(op == 10);
    mHeartbeat.Emplace(*mWSS, interval);
}



void Bot::Run()
{
    int i = 0;
    while (i < 100)
    {
        auto message = mWSS->Lock()->ReadMessage();

        if (message)
        {
            switch (message->GetOpcode())
            {
                case WebsocketMessage::Opcode::Close:
                    std::cout << "Closing. Reason: " << message->GetCloseStatusCode() << std::endl;
                    break;
                case WebsocketMessage::Opcode::Text:
                    std::cout << message->AsString() << std::endl;
                    break;
                default:
                    break;
            }

            std::cout << i + 1 << "/100" << std::endl;
            i++;
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
    assert(response.Status() == 200);
    auto payload = std::get<HTTPChunkedPayload>(*response.Payload());
    auto json = *payload[0].AsJSON();
    auto url = (std::string) json["url"];
    url.erase(0, 6);
    return url;
}
