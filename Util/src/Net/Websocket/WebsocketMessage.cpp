#include "Util/Net/Websocket/WebsocketMessage.hpp"



WebsocketMessage::WebsocketMessage(WebsocketMessage::Opcode opcode, Payload payload)
    : mOpcode(opcode)
    , mPayload(payload)
{

}



WebsocketMessage::WebsocketMessage(const std::string& string)
    : mOpcode(Opcode::Text)
    , mPayload(string.data(), string.data() + string.size())
{

}



WebsocketMessage::WebsocketMessage(std::vector<uint8_t> bytes)
    : mOpcode(Opcode::Binary)
    , mPayload(std::move(bytes))
{

}



std::string WebsocketMessage::AsString() const
{
    return std::string(mPayload.data(), mPayload.data() + mPayload.size());
}



Result<nlohmann::json, std::string> WebsocketMessage::AsJSON() const
{
    nlohmann::json json;
    switch (mOpcode)
    {
        case Opcode::Text:
        case Opcode::Binary:
        {
            try
            {
                json = nlohmann::json::parse(mPayload.begin(), mPayload.end());
            }
            catch (std::exception& e)
            {
                return Result<nlohmann::json, std::string>::Err("Parse Error");
            }
            return Result<nlohmann::json, std::string>::Ok(std::move(json));
        }

        default:
            return Result<nlohmann::json, std::string>::Err("Invalid Message Type");
    }
}



void WebsocketMessage::Append(const WebsocketMessage& other)
{
    mPayload.insert(mPayload.end(), other.mPayload.begin(), other.mPayload.end());
}
