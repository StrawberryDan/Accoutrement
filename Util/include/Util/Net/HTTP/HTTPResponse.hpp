#pragma once



#include "HTTPConstants.hpp"
#include "HTTPHeader.hpp"
#include "HTTPPayload.hpp"



class HTTPResponse
{
public:
    HTTPResponse(HTTPVersion mVersion, unsigned int mStatus, std::string  mStatusText);

    inline const HTTPVersion&     Version() const { return    mVersion; }
    inline const unsigned int&     Status() const { return     mStatus; }
    inline const std::string&  StatusText() const { return mStatusText; }

    inline const HTTPHeader& Header() const { return mHeader; }
    inline       HTTPHeader& Header()       { return mHeader; }

    inline const HTTPPayload& Payload() const { return mPayload; }
    inline void SetPayload(const HTTPPayload& payload) { mPayload = payload; }
private:
    HTTPVersion  mVersion;
    unsigned int mStatus;
    std::string  mStatusText;
    HTTPHeader   mHeader;
    HTTPPayload  mPayload;
};
