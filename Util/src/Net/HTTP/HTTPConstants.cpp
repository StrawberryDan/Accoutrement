#include "Util/Net/HTTP/HTTPConstants.hpp"



#include "Util/Utilities.hpp"


#include <map>



std::string HTTPVerbToString(HTTPVerb verb)
{
    switch (verb)
    {

        case HTTPVerb::POST:
            return "POST";
        case HTTPVerb::GET:
            return "GET";
        case HTTPVerb::PUT:
            return "PUT";
        case HTTPVerb::PATCH:
            return "PATCH";
        case HTTPVerb::DELETE:
            return "DELETE";
        default:
            std::abort();
    }
}



Option<HTTPVerb> HTTPVerbFromString(const std::string& string)
{
    static const std::map<std::string, HTTPVerb> mapping =
    {
        {"POST",   HTTPVerb::POST},
        {"GET",    HTTPVerb::GET},
        {"PUT",    HTTPVerb::PUT},
        {"PATCH",  HTTPVerb::PATCH},
        {"DELETE", HTTPVerb::DELETE}
    };

    std::string upper = ToUppercase(string);
    if (mapping.contains(upper))
    {
        return mapping.at(upper);
    }
    else
    {
        return {};
    }
}



Option<HTTPVersion> HTTPVersionFromString(const std::string& string)
{
    static const std::map<std::string, HTTPVersion> mapping =
    {
    {"1.0", HTTPVersion::VERSION_1_0},
    {"1.1", HTTPVersion::VERSION_1_1},
    {"2",   HTTPVersion::VERSION_2},
    {"3",   HTTPVersion::VERSION_3},
    };

    if (mapping.contains(string))
    {
        return mapping.at(string);
    }
    else
    {
        return {};
    }
}



std::string HTTPVersionToString(HTTPVersion version)
{
    switch (version)
    {
        case HTTPVersion::VERSION_1_0:
            return "1.0";
        case HTTPVersion::VERSION_1_1:
            return "1.1";
        case HTTPVersion::VERSION_2:
            return "2";
        case HTTPVersion::VERSION_3:
            return "3";
        default:
            std::abort();
    }
}
