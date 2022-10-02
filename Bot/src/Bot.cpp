#include <utility>

#include "Bot/Bot.hpp"



Bot::Bot(Token token)
    : mToken(std::move(token))
    , mHTTPS("discord.com")
    , mWSS()
{
    HTTPRequest request(HTTPVerb::GET, "/api/v10/gateway/bot");
    request.Header().Add("Authorization", fmt::format("Bot {}", mToken));
    request.Header().Add("Host", "discord.com");
    mHTTPS.Lock()->Request(request);
    auto response = mHTTPS.Lock()->Receive();
    auto payload = std::get<HTTPChunkedPayload>(response.Payload());
    auto json = *payload[0].AsJSON();
    auto url = (std::string) json["url"];
    url.erase(0, 6);
    mWSS = WSSClient(url, "/");
    assert(mWSS->Lock()->IsValid());
}



void Bot::Run()
{

}
