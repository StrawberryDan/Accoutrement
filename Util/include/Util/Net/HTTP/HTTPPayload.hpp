#pragma once


#include <variant>
#include <optional>



#include "HTTPSimplePayload.hpp"
#include "HTTPChunkedPayload.hpp"



using HTTPPayload = Option<std::variant
<
    HTTPSimplePayload,
    HTTPChunkedPayload
>>;
