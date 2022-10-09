#pragma once



#include <string>
#include "Util/Option.hpp"



enum class HTTPVerb
{
    POST,
    GET,
    PUT,
    PATCH,
    DELETE
};



std::string HTTPVerbToString(HTTPVerb verb);
Option<HTTPVerb> HTTPVerbFromString(const std::string& string);



enum class HTTPVersion
{
    VERSION_1_0,
    VERSION_1_1,
    VERSION_2,
    VERSION_3
};


Option<HTTPVersion> HTTPVersionFromString(const std::string& string);
std::string                HTTPVersionToString(HTTPVersion version);