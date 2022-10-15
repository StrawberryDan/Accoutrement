#include <random>



#include <future>



#include "Util/Net/HTTP/HTTPClient.hpp"
#include "Util/Base64.hpp"
#include "Util/Endian.hpp"



template<SocketImpl S, uint16_t PORT>
WebsocketClientImpl<S, PORT>::WebsocketClientImpl(const std::string& hostname, const std::string& subresource)
{
    HTTPClientImpl<S, PORT> handshaker(hostname);
    HTTPRequest upgradeRequest(HTTPVerb::GET, subresource);
    upgradeRequest.Header().Add("Host", "gateway.discord.gg");
    upgradeRequest.Header().Add("Upgrade", "websocket");
    upgradeRequest.Header().Add("Connection", "Upgrade");
    upgradeRequest.Header().Add("Sec-WebSocket-Key", GenerateNonce());
    upgradeRequest.Header().Add("Sec-WebSocket-Version", "13");
    handshaker.Request(upgradeRequest);


    auto response = handshaker.Receive();
    Assert(response.Status() == 101);
    Assert(response.Header().Get("Upgrade") == "websocket");
    Assert(response.Header().Get("Connection") == "upgrade");
    if (response.Status() == 101)
    {
        mSocket = handshaker.TakeSocket();
        *mRunning.Lock() = true;

        mReceiver = std::async(std::launch::async, [this]()
        {
            while (*mRunning.Lock())
            {
                auto msg = ReceiveFrame();
                if (msg)
                {
                    mRecvMessageBuffer.Lock()->push_back(msg.Unwrap());
                }
            }
        });

        mTransmitter = std::async(std::launch::async, [this]()
        {
            while (*mRunning.Lock())
            {
                {
                    auto buffer = mSendMessageBuffer.Lock();
                    if (!buffer->empty())
                    {

                        auto msg = std::move(*buffer->rbegin());
                        buffer->pop_back();
                        TransmitFrame(msg).Unwrap();
                    }
                }
                std::this_thread::yield();
            }
        });
    }
}



template<SocketImpl S, uint16_t PORT>
WebsocketClientImpl<S, PORT>::~WebsocketClientImpl()
{
	SendMessage(WebsocketMessage(WebsocketMessage::Opcode::Close));
    while (true)
    {
        auto msg = ReadMessage();
        if (msg && msg.Unwrap().GetOpcode() == WebsocketMessage::Opcode::Close)
        {
            break;
        }
    }

    *mRunning.Lock() = false;
    mReceiver.wait();
    mTransmitter.wait();
    mSocket.Reset();
}



template<SocketImpl S, uint16_t PORT>
void WebsocketClientImpl<S, PORT>::SendMessage(const WebsocketMessage& message)
{
	auto buffer = mSendMessageBuffer.Lock();
	buffer->push_back(message);
}



template<SocketImpl S, uint16_t PORT>
Result<WebsocketMessage, typename WebsocketClientImpl<S, PORT>::Error> WebsocketClientImpl<S, PORT>::ReadMessage()
{
	auto buffer = mRecvMessageBuffer.Lock();
	if (buffer->empty())
	{
		return Result<WebsocketMessage, Error>::Err(Error::NoMessage);
	}
	else
	{
		auto message = std::move((*buffer)[0]);
		buffer->erase(buffer->begin());
		return Result<WebsocketMessage, Error>::Ok(message);
	}
}



template<SocketImpl S, uint16_t PORT>
Result<WebsocketMessage, typename WebsocketClientImpl<S, PORT>::Error> WebsocketClientImpl<S, PORT>::WaitMessage()
{
	while (true)
	{
		auto msg = ReadMessage();
		if (msg)
		{
			return Result<WebsocketMessage, Error>::Ok(msg.Unwrap());
		}
		else if (msg.Err() == Error::NoMessage)
		{
			continue;
		}
		else
		{
			return Result<WebsocketMessage, Error>::Err(msg.Err());
		}
	}
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
    Assert(nonce.size() == 16);
    auto base64 = Base64::Encode(nonce);
    Assert(base64.size() == 24);
    return base64;
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
        default:
            std::abort();
    }
}



template<SocketImpl S, uint16_t PORT>
Option<WebsocketMessage::Opcode> WebsocketClientImpl<S, PORT>::GetOpcodeFromByte(uint8_t byte)
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



template<SocketImpl S, uint16_t PORT>
uint32_t WebsocketClientImpl<S, PORT>::GenerateMaskingKey()
{
    std::random_device rd;
    uint32_t key;
    for (int i = 0; i < sizeof(key); i++)
    {
        reinterpret_cast<uint8_t*>(&key)[i] = static_cast<uint8_t>(rd());
    }
    return key;
}



template<SocketImpl S, uint16_t PORT>
typename WebsocketClientImpl<S, PORT>::Error WebsocketClientImpl<S, PORT>::ErrorFromSocketError(Socket::Error err)
{
    switch (err)
    {
        case Socket::Error::Closed:
            return Error::Closed;
        default:
            std::unreachable();
    }

}



template<SocketImpl S, uint16_t PORT>
Result<size_t, typename WebsocketClientImpl<S, PORT>::Error> WebsocketClientImpl<S, PORT>::TransmitFrame(const WebsocketMessage& message)
{
    size_t bytesTransmitted = 0;

    uint8_t byte = 0b10000000;
    byte |= GetOpcodeMask(message.GetOpcode());
    bytesTransmitted += mSocket->WriteType(byte).Unwrap();

    byte = 0b10000000;
    auto bytes = message.AsBytes();
    if (bytes.size() <= 125)
    {
        byte |= static_cast<uint8_t>(bytes.size());
        bytesTransmitted += mSocket->WriteType(byte).Unwrap();
    } else if (bytes.size() <= std::numeric_limits<uint16_t>::max())
    {
        byte |= 126;
        bytesTransmitted += mSocket->WriteType(byte).Unwrap();
        bytesTransmitted += mSocket->WriteType(ToBigEndian(static_cast<uint16_t>(bytes.size()))).Unwrap();
    } else if (bytes.size() <= std::numeric_limits<uint64_t>::max())
    {
        byte |= 127;
        bytesTransmitted += mSocket->WriteType(byte).Unwrap();
        bytesTransmitted += mSocket->WriteType(ToBigEndian(static_cast<uint64_t>(bytes.size()))).Unwrap();
    }

    uint32_t maskingKey = ToBigEndian(GenerateMaskingKey());
    static_assert(sizeof(maskingKey) == 4);
    bytesTransmitted += mSocket->WriteType(maskingKey).Unwrap();

    for (int i = 0; i < bytes.size(); i++)
    {
        auto mask = reinterpret_cast<uint8_t*>(&maskingKey)[i % sizeof(maskingKey)];
        bytesTransmitted += mSocket->template WriteType<uint8_t>(bytes[i] ^ mask).Unwrap();
    }

    return Result<size_t, Error>::Ok(bytesTransmitted);
}



template<SocketImpl S, uint16_t PORT>
Result<WebsocketMessage, typename WebsocketClientImpl<S, PORT>::Error> WebsocketClientImpl<S, PORT>::ReceiveFrame()
{
    if (mError == Error::Closed)
    {
        return Result<WebsocketMessage, Error>::Err(*mError);
    }

    auto fragResult = ReceiveFragment();

    if (fragResult)
    {
        auto [final, message] = fragResult.Unwrap();

        while (!final)
        {
            auto fragResultB = ReceiveFragment();
            if (fragResultB)
            {
                auto [finalB, messageB] = fragResultB.Unwrap();
                message.Append(messageB);
                final = finalB;
            }
            else
            {
                return Result<WebsocketMessage, Error>::Err(fragResultB.Err());
            }
        }

        if (message.GetOpcode() == WebsocketMessage::Opcode::Close)
        {
            mSocket = {};
            mError = Error::Closed;
        }

        return Result<WebsocketMessage, Error>::Ok(std::move(message));
    }
    else
    {
        return Result<WebsocketMessage, Error>::Err(fragResult.Err());
    }
}



template<SocketImpl S, uint16_t PORT>
Result<typename WebsocketClientImpl<S, PORT>::Fragment, typename WebsocketClientImpl<S, PORT>::Error>
WebsocketClientImpl<S, PORT>::ReceiveFragment()
{
    using Opcode = WebsocketMessage::Opcode;

    bool final;
    Opcode opcode;
    if (auto byte = mSocket->template ReadType<uint8_t>())
    {
        final = *byte & 0b10000000;
        opcode = *GetOpcodeFromByte(*byte & 0b00001111);
    }
    else
    {
        return Result<Fragment, Error>::Err(ErrorFromSocketError(byte.Err()));
    }


    bool   masked;
    size_t size;
    if (auto byte = mSocket->template ReadType<uint8_t>())
    {
        masked = *byte & 0b10000000;
        Assert(!masked);
        uint8_t sizeByte = *byte & 0b01111111;
        if (sizeByte == 126)
        {
            size = FromBigEndian(mSocket->template ReadType<uint16_t>().Unwrap());
        }
        else if (sizeByte == 127)
        {
            size = FromBigEndian(mSocket->template ReadType<uint64_t>().Unwrap());
        }
        else
        {
            size = sizeByte;
        }
    }
    else
    {
        return Result<Fragment, Error>::Err(ErrorFromSocketError(byte.Err()));
    }


    auto payload = mSocket->template ReadVector<uint8_t>(size).Unwrap();
    return Result<Fragment, Error>::Ok(final, WebsocketMessage(opcode, payload));
}