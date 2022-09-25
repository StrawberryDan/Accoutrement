#include "Util/Net/HTTP/HTTPResponse.hpp"



HTTPResponse::HTTPResponse(HTTPVersion mVersion, unsigned int mStatus, const std::string& mStatusText)
    : mVersion(mVersion)
    , mStatus(mStatus)
    , mStatusText(mStatusText)
{

}
