#include "Util/Net/HTTP/HTTPSimplePayload.hpp"



void HTTPSimplePayload::Write(const uint8_t* data, size_t len)
{
    mData.insert(mData.end(), data, data + len);
}
