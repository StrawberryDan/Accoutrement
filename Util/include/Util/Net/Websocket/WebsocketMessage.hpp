#pragma once



#include <vector>
#include <cstdint>
#include <string>
#include <variant>
#include <optional>



#include "Util/Result.hpp"
#include "nlohmann/json.hpp"



class WebsocketMessage
{
public:
    enum class Opcode;
    using Payload = std::vector<uint8_t>;

public:
    WebsocketMessage(Opcode opcode, Payload payload = {});
    WebsocketMessage(const std::string& string);
    WebsocketMessage(std::vector<uint8_t> bytes);


    inline Opcode GetOpcode() const { return mOpcode; }
    inline std::vector<uint8_t>         AsBytes() const { return mPayload; }
    std::string                         AsString() const;
    Result<nlohmann::json, std::string> AsJSON() const;
    uint16_t GetCloseStatusCode() const;


    void Append(const WebsocketMessage& other);


private:
    Opcode mOpcode;
    Payload mPayload;
};



enum class WebsocketMessage::Opcode
{
    Continuation,
    Text,
    Binary,
    Close,
    Ping,
    Pong,
};
