#include <regex>
#include <cassert>
#include <iostream>
#include "fmt/core.h"



template<SocketImpl S, uint16_t PORT>
HTTPClientImpl<S, PORT>::HTTPClientImpl(const std::string& hostname)
    : mSocket(hostname, PORT)
{

}



template<SocketImpl S, uint16_t PORT>
void HTTPClientImpl<S, PORT>::Request(const HTTPRequest& request)
{
    std::string headerLine = fmt::format(
        "{} {} HTTP/{}\r\n",
        HTTPVerbToString(request.Verb()), request.URI(), HTTPVersionToString(request.Version()));
    mSocket.WriteArray(headerLine.data(), headerLine.length());
    for (const auto& [key, values] : *request.Header())
    {
        for (const auto& value : values)
        {
            std::string formatted = fmt::format("{}: {}\r\n", key, value);
            mSocket.WriteArray(formatted.data(), formatted.length());
        }
    }

    std::vector<char> blankLine = {'\r', '\n'};
    mSocket.WriteVector(blankLine);

    if (std::holds_alternative<HTTPSimplePayload>(request.Payload()))
    {
        const auto& payload = std::get<HTTPSimplePayload>(request.Payload());
        if (payload.Size() > 0)
        {
            mSocket.WriteVector(*payload);
        }
    }
    else if (std::holds_alternative<HTTPChunkedPayload>(request.Payload()))
    {
        const auto& payload = std::get<HTTPChunkedPayload>(request.Payload());
        for (const auto& chunk : *payload)
        {
            mSocket.WriteVector(chunk);
        }
    }

    mSocket.WriteVector(blankLine);
}



template<SocketImpl S, uint16_t PORT>
HTTPResponse HTTPClientImpl<S, PORT>::Receive()
{
    static const auto statusLinePattern = std::regex(R"(HTTP\/([^\s]+)\s+(\d{3})\s+([^\r]*)\r\n)");
    static const auto headerLinePattern = std::regex(R"(([^:]+)\s*:\s*([^\r]+)\r\n)");

    auto currentLine = ReadLine();
    std::smatch matchResults;
    auto matched = std::regex_match(currentLine, matchResults, statusLinePattern);
    assert(matched);

    std::string version    = matchResults[1],
                status     = matchResults[2],
                statusText = matchResults[3];

    HTTPResponse response(*HTTPVersionFromString(version), std::stoul(status), statusText);

    while (true)
    {
        currentLine = ReadLine();

        if (currentLine == "\r\n")
        {
            break;
        }
        else if (std::regex_match(currentLine, matchResults, headerLinePattern))
        {
            response.Header().Add(matchResults[1], matchResults[2]);
        }
    }

    HTTPPayload payload;
    if (response.Header().Contains("Transfer-Encoding"))
    {
        auto transferEncoding = response.Header().Get("Transfer-Encoding");

        if (transferEncoding == "chunked")
        {
            payload = ReadChunkedPayload();
        }
        else
        {
            fmt::print(stderr, "Unsupported value for Transfer-Encoding: {}\n", transferEncoding);
        }
    }
    else if (response.Header().Contains("Content-Length"))
    {
        HTTPSimplePayload simplePayload;
        std::vector<uint8_t> data = mSocket.template ReadVector<uint8_t>(std::stoul(response.Header().Get("Content-Length")));
        simplePayload.Write(data.data(), data.size());
        payload = simplePayload;
    }
    response.SetPayload(payload);

    return response;
}



template<SocketImpl S, uint16_t PORT>
HTTPChunkedPayload HTTPClientImpl<S, PORT>::ReadChunkedPayload()
{
    std::string line;
    std::smatch matchResults;
    static const auto chunkSizeLine = std::regex(R"((\d+)\r\n)");

    HTTPChunkedPayload payload;
    while (true)
    {
        line = this->ReadLine();
        if (line == "\r\n" || !std::regex_match(line, matchResults, chunkSizeLine))
        {
            break;
        }

        auto bytesToRead = std::__cxx11::stoul(matchResults[1]);
        if (bytesToRead > 0)
        {
            HTTPChunkedPayload::Chunk chunk = this->mSocket.template ReadVector<uint8_t>(bytesToRead);
            payload.AddChunk(chunk);
        }
    }

    return payload;
}



template<SocketImpl S, uint16_t PORT>
std::string HTTPClientImpl<S, PORT>::ReadLine()
{
    std::string line;
    while (!line.ends_with("\r\n"))
    {
        line += mSocket.template ReadType<char>();
    }
    return line;
}