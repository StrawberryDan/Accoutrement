#include <random>



#include "Util/Net/HTTP/HTTPClient.hpp"
#include "Util/Base64.hpp"



template<SocketImpl S, uint16_t PORT>
WebsocketClientImpl<S, PORT>::WebsocketClientImpl(const std::string& hostname, const std::string& subresource)
{
    HTTPClientImpl<S, PORT> handshaker(hostname);
    HTTPRequest upgradeRequest(HTTPVerb::GET, subresource);
    upgradeRequest.Header().Add("Host", hostname);
    upgradeRequest.Header().Add("Origin", hostname);
    upgradeRequest.Header().Add("Upgrade", "websocket");
    upgradeRequest.Header().Add("Connection", "Upgrade");
    upgradeRequest.Header().Add("Sec-WebSocket-Key", GenerateNonce());
    upgradeRequest.Header().Add("Sec-WebSocket-Version", "13");
    handshaker.Request(upgradeRequest);
    auto response = handshaker.Receive();
    if (response.Status() == 101)
    {
        mSocket = handshaker.TakeSocket();
    }
}



template<SocketImpl S, uint16_t PORT>
void WebsocketClientImpl<S, PORT>::Send(const WebsocketMessage& message) const
{
    uint8_t byte = 0b10000000;
    byte |= GetOpcodeMask(message.GetOpcode());
    mSocket->WriteType(byte);

    byte = 0b10000000;
    auto bytes = message.AsBytes();
    if (bytes.size() <= 125)
    {
        byte |= static_cast<uint8_t>(bytes.size());
        mSocket.WriteType(byte);
    }
    else if (bytes.size() <= std::numeric_limits<uint16_t>::max())
    {
        byte |= 126;
        mSocket.WriteType(byte);
        mSocket.WriteType(static_cast<uint16_t>(bytes.size()));
    }
    else if (bytes.size() <= std::numeric_limits<uint64_t>::max())
    {
        byte |= 127;
        mSocket.WriteType(byte);
        mSocket.WriteType(static_cast<uint64_t>(bytes.size()));
    }

    auto maskingKey = GenerateMaskingKey();
    mSocket.WriteType(maskingKey);

    for (int i = 0; i < bytes.size(); i++)
    {
        auto mask = reinterpret_cast<uint8_t*>(&maskingKey)[i % sizeof(maskingKey)];
        mSocket.WriteType(bytes[i] ^ mask);
    }
}



template<SocketImpl S, uint16_t PORT>
Result<WebsocketMessage, typename WebsocketClientImpl<S, PORT>::Error> WebsocketClientImpl<S, PORT>::Receive()
{
    if (mError == Error::Closed)
    {
        return Result<WebsocketMessage, Error>::Err(*mError);
    }

    auto [final, message] = ReceiveFragment();

    while (!final)
    {
        auto [finalB, messageB] = ReceiveFragment();
        message.Append(messageB);
        final = finalB;
    }

    if (message.GetOpcode() == WebsocketMessage::Opcode::Close)
    {
        mSocket = {};
        mError = Error::Closed;
    }

    return Result<WebsocketMessage, Error>::Ok(std::move(message));
}



template<SocketImpl S, uint16_t PORT>
std::pair<bool, WebsocketMessage> WebsocketClientImpl<S, PORT>::ReceiveFragment() const
{
    using Opcode = WebsocketMessage::Opcode;

    uint8_t byte = mSocket->template ReadType<uint8_t>();
    bool     final =  byte & 0b10000000;
    Opcode opcode  = *GetOpcodeFromByte(byte & 0b00001111);

    byte = mSocket->template ReadType<uint8_t>();
    bool      masked = byte & 0b10000000;
    assert(!masked);
    uint8_t sizeByte = byte & 0b01111111;
    size_t  size;
    if (sizeByte == 126)
    {
        size = mSocket->template ReadType<uint16_t>();
    }
    else if (sizeByte == 127)
    {
        size = mSocket->template ReadType<uint64_t>();
    }
    else
    {
        size = sizeByte;
    }

    auto payload = mSocket->template ReadVector<uint8_t>(size);
    return {final, WebsocketMessage(opcode, payload)};
}



template<SocketImpl S, uint16_t PORT>
std::string WebsocketClientImpl<S, PORT>::GenerateNonce()
{
    std::random_device randomDevice;
    std::vector<uint8_t> nonce;
    nonce.reserve(16);
    while (nonce.size() < 16)
    {
        auto val = randomDevice();
        for (int i = 0; i < sizeof(val) && nonce.size() < 16; i++)
        {
            nonce.push_back(reinterpret_cast<uint8_t*>(&val)[i]);
        }
    }

    return Base64::Encode(nonce);
}



template<SocketImpl S, uint16_t PORT>
uint8_t WebsocketClientImpl<S, PORT>::GetOpcodeMask(WebsocketMessage::Opcode opcode)
{
    switch (opcode)
    {
        case WebsocketMessage::Opcode::Continuation:
            return 0x0;
        case WebsocketMessage::Opcode::Text:
            return 0x1;
        case WebsocketMessage::Opcode::Binary:
            return 0x2;
        case WebsocketMessage::Opcode::Close:
            return 0x8;
        case WebsocketMessage::Opcode::Ping:
            return 0x9;
        case WebsocketMessage::Opcode::Pong:
            return 0xA;
    }
}



template<SocketImpl S, uint16_t PORT>
std::optional<WebsocketMessage::Opcode> WebsocketClientImpl<S, PORT>::GetOpcodeFromByte(uint8_t byte)
{
    using Opcode = WebsocketMessage::Opcode;

    switch (byte)
    {
        case 0x0: return Opcode::Continuation;
        case 0x1: return Opcode::Text;
        case 0x2: return Opcode::Binary;
        case 0x8: return Opcode::Close;
        case 0x9: return Opcode::Ping;
        case 0xA: return Opcode::Pong;
        default:  return {};
    }
}