#pragma once


#include <variant>
#include "HTTPSimplePayload.hpp"
#include "HTTPChunkedPayload.hpp"



using HTTPPayload = std::variant
<
    HTTPSimplePayload,
    HTTPChunkedPayload
>;
