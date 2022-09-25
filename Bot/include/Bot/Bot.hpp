#pragma once



#include <string>
#include "Util/Net/HTTP/HTTPClient.hpp"



class Bot
{
public:
    using Token = std::string;

public:
    Bot(Token  token);

    void Run();

private:
    Token       mToken;
    HTTPSClient mHTTPS;
};