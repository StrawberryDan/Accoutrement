#include "Util/Net/HTTP/HTTPSimplePayload.hpp"



#include <iostream>
#include <utility>



HTTPSimplePayload::HTTPSimplePayload(std::vector<uint8_t>  bytes)
    : mData(std::move(bytes))
{

}



void HTTPSimplePayload::Write(const uint8_t* data, size_t len)
{
    mData.insert(mData.end(), data, data + len);
}



std::optional<nlohmann::json> HTTPSimplePayload::AsJSON()
{
    std::string asString(mData.begin(), mData.end());

    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(asString.begin(), asString.end());
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return {};
    }

    if (json.is_discarded())
    {
        return {};
    }
    else
    {
        return json;
    }
}
