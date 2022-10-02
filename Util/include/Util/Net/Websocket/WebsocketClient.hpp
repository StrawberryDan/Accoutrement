#pragma once



#include <cstdint>
#include <string>
#include <optional>



#include "Util/Result.hpp"
#include "Util/Net/Socket/Socket.hpp"
#include "Util/Net/Socket/TCPClient.hpp"
#include "Util/Net/Socket/TLSClient.hpp"
#include "Util/Net/Websocket/WebsocketMessage.hpp"




template<SocketImpl S, uint16_t PORT>
class WebsocketClientImpl
{
public:
    enum class Error;


public:
    WebsocketClientImpl(const std::string& hostname, const std::string& subresource);

    void Send(const WebsocketMessage& message) const;
    Result<WebsocketMessage, Error> Receive();


    [[nodiscard]] inline bool IsValid() const { return mSocket.has_value(); }
    inline S TakeSocket() { return Take(mSocket); }

private:
    [[nodiscard]] std::pair<bool, WebsocketMessage> ReceiveFragment() const;


    [[nodiscard]] static std::string GenerateNonce();
    [[nodiscard]] static uint8_t GetOpcodeMask(WebsocketMessage::Opcode opcode);
    [[nodiscard]] static std::optional<WebsocketMessage::Opcode> GetOpcodeFromByte(uint8_t byte);
    [[nodiscard]] static uint32_t GenerateMaskingKey();


private:
    std::optional<S>     mSocket;
    std::optional<Error> mError;
};



template<SocketImpl S, uint16_t PORT>
enum class WebsocketClientImpl<S, PORT>::Error
{
    Closed,
};



using WSClient  = WebsocketClientImpl<TCPClient,  80>;
using WSSClient = WebsocketClientImpl<TLSClient, 443>;



#include "WebsocketClient.inl"