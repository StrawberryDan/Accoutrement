#pragma once



#include <cstdint>
#include <string>
#include <optional>
#include <thread>
#include <future>



#include "Util/Option.hpp"
#include "Util/Result.hpp"
#include "Util/Mutex.hpp"
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
	static Result<WebsocketClientImpl, Error> Connect(const std::string& host, const std::string& resource);

	WebsocketClientImpl(const WebsocketClientImpl&)                =  delete;
	WebsocketClientImpl& operator=(const WebsocketClientImpl&)     =  delete;
	WebsocketClientImpl(WebsocketClientImpl&& rhs) noexcept;
	WebsocketClientImpl& operator=(WebsocketClientImpl&& rhs) noexcept;

    ~WebsocketClientImpl();



    void SendMessage(const WebsocketMessage& message);

    Result<WebsocketMessage, Error> ReadMessage();

    Result<WebsocketMessage, Error> WaitMessage();



    [[nodiscard]] inline bool IsValid() const { return mSocket.HasValue(); }
    inline S TakeSocket() { return Take(mSocket); }



private:
    using Fragment = std::pair<bool, WebsocketMessage>;


private:
	WebsocketClientImpl() = default;



    [[nodiscard]] Result<WebsocketMessage, Error>   ReceiveFrame();
    [[nodiscard]] Result<Fragment,         Error>   ReceiveFragment();
    [[nodiscard]] Result<size_t,           Error>   TransmitFrame(const WebsocketMessage& frame);


    [[nodiscard]] static std::string GenerateNonce();
    [[nodiscard]] static uint8_t GetOpcodeMask(WebsocketMessage::Opcode opcode);
    [[nodiscard]] static Option<WebsocketMessage::Opcode> GetOpcodeFromByte(uint8_t byte);
    [[nodiscard]] static uint32_t GenerateMaskingKey();
    [[nodiscard]] static Error ErrorFromSocketError(Socket::Error err);



	void ReceiverFunction();
	void TransmitterFunction();


private:
    using MessageBuffer = Mutex<std::vector<WebsocketMessage>>;

    Option<S>         mSocket;
    Option<Error>     mError;

    std::future<void> mReceiver;
    std::future<void> mTransmitter;

    Mutex<bool>       mRunning;
    MessageBuffer     mRecvMessageBuffer;
    MessageBuffer     mSendMessageBuffer;
};



template<SocketImpl S, uint16_t PORT>
enum class WebsocketClientImpl<S, PORT>::Error
{
	Unknown,
    NoMessage,
    Closed,
	Refused,
};



using WSClient  = WebsocketClientImpl<TCPClient,  80>;
using WSSClient = WebsocketClientImpl<TLSClient, 443>;



#include "WebsocketClient.inl"