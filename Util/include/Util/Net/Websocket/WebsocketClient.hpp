#pragma once



#include <cstdint>
#include <string>
#include <optional>
#include <thread>
#include <future>



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

    ~WebsocketClientImpl();

    void SendFrame(const WebsocketMessage& message);


    [[nodiscard]] inline bool IsValid() const { return mSocket.HasValue(); }
    inline S TakeSocket() { return Take(mSocket); }

    Result<WebsocketMessage, Error> ReadMessage();

private:
    using Fragment = std::pair<bool, WebsocketMessage>;


    [[nodiscard]] Result<WebsocketMessage, Error>   ReceiveFrame();
    [[nodiscard]] Result<Fragment,         Error>   ReceiveFragment();
    [[nodiscard]] Result<size_t,           Error>   TransmitFrame(const WebsocketMessage& frame);


    [[nodiscard]] static std::string GenerateNonce();
    [[nodiscard]] static uint8_t GetOpcodeMask(WebsocketMessage::Opcode opcode);
    [[nodiscard]] static Option<WebsocketMessage::Opcode> GetOpcodeFromByte(uint8_t byte);
    [[nodiscard]] static uint32_t GenerateMaskingKey();
    [[nodiscard]] static Error ErrorFromSocketError(Socket::Error err);


private:
    using MessageBuffer = Mutex<std::vector<WebsocketMessage>>;

    Option<S>        mSocket;
    Option<Error>    mError;

    std::future<void> mReceiver;
    std::future<void> mTransmitter;

    Mutex<bool>                   mRunning;
    MessageBuffer                 mRecvMessageBuffer;
    MessageBuffer                 mSendMessageBuffer;
};



template<SocketImpl S, uint16_t PORT>
enum class WebsocketClientImpl<S, PORT>::Error
{
    NoMessage,
    Closed,
};



using WSClient  = WebsocketClientImpl<TCPClient,  80>;
using WSSClient = WebsocketClientImpl<TLSClient, 443>;



#include "WebsocketClient.inl"