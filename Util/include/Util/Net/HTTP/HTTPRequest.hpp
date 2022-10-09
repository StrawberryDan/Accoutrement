#pragma once



#include "HTTPConstants.hpp"
#include "HTTPHeader.hpp"
#include "HTTPPayload.hpp"
#include <string>



class HTTPRequest
{
public:
    HTTPRequest(HTTPVerb verb, std::string  uri, HTTPVersion version = HTTPVersion::VERSION_1_1);

    [[nodiscard]] inline const HTTPVerb&       Verb() const { return    mVerb; }
    [[nodiscard]] inline const std::string&     URI() const { return     mURI; }
    [[nodiscard]] inline const HTTPVersion& Version() const { return mVersion; }

    [[nodiscard]] inline const HTTPHeader& Header() const { return mHeader; }
    inline       HTTPHeader& Header()       { return mHeader; }

    [[nodiscard]] inline const HTTPPayload& Payload() const { return mPayload; }
    inline void SetPayload(const HTTPPayload& payload) { mPayload = payload; }

private:
    HTTPVerb    mVerb;
    std::string mURI;
    HTTPVersion mVersion;
    HTTPHeader  mHeader;
    HTTPPayload mPayload;
};
