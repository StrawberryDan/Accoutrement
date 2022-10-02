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
    mWSS = WSSClient(url, "/?v=10&encoding=json");
    assert(mWSS->Lock()->IsValid());

    while (true)
    {
        auto hello = mWSS->Lock()->Receive();

        if (hello.IsErr())
        {
            break;
        }

        if (hello->GetOpcode() == WebsocketMessage::Opcode::Close)
        {
            auto bytes = hello->AsBytes();
            uint16_t closeCode = static_cast<uint16_t>(bytes[0]) << 8 | static_cast<uint16_t>(bytes[1]) << 0;
            std::cout << "Connection closed. Reason: " << closeCode << std::endl;
        }

        auto helloJSON = hello->AsJSON();
        if (helloJSON)
        {
            std::cout << std::setw(4) << *helloJSON << std::endl;
        }
        else
        {
            std::cerr << helloJSON.Err() << std::endl;
        }
    }
}



void Bot::Run()
{

}
