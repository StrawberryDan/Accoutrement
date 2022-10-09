#include <utility>

#include "Util/Net/HTTP/HTTPResponse.hpp"



HTTPResponse::HTTPResponse(HTTPVersion mVersion, unsigned int mStatus, std::string mStatusText)
    : mVersion(mVersion)
    , mStatus(mStatus)
    , mStatusText(std::move(mStatusText))
{

}
