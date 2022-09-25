#include <utility>

#include "Bot/Bot.hpp"



Bot::Bot(Token token)
    : mToken(std::move(token))
    , mHTTPS("discord.com")
{

}



void Bot::Run()
{
    HTTPRequest request(HTTPVerb::GET, "/api/v10/gateway/bot");
    request.Header().Add("Authorization", fmt::format("Bot {}", mToken));
    request.Header().Add("Host", "discord.com");
    mHTTPS.Request(request);

    auto response = mHTTPS.Receive();
}
