#include "Bot/Bot.hpp"



#include <iostream>



Bot::Bot()
    : mSocket("discord.com", 443)
{
    std::string buf = "GET /api/v10/gateway/bot HTTP/1.1\r\nHost: discord.com\r\nUser-Agent: DiscordBot (justnoise.net, 0.1)\r\nAuthorization: Bot OTg0MjE4MDkwNjM4Mjk5MTc2.Gv7WBL.90WctjqyhKx-zce1oqg0EdGHQvWr2ZO-OfSngY\r\nAccept:application/json\r\nAccept-Language: en-GB\r\n\r\n";
    mSocket.Write(buf.data(), buf.size());


    while (true)
    {
        char c = mSocket.ReadType<char>();
        std::cout << c;
        if (c == '\0') break;
    }
}
