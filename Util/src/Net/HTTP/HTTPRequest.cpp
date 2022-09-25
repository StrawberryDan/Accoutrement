#include <utility>

#include "Util/Net/HTTP/HTTPRequest.hpp"



HTTPRequest::HTTPRequest(HTTPVerb verb, std::string uri, HTTPVersion version)
    : mVerb(verb)
    , mURI(std::move(uri))
    , mVersion(version)
{

}
