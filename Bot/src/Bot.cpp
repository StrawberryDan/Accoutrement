#include "Bot/Bot.hpp"



#include <iostream>



Bot::Bot()
    : mSocket("discord.com", 443)
    , mUDPSocket("discord.com", 443)
{

}
