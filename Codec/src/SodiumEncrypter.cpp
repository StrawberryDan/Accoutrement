#include "Codec/SodiumEncrypter.hpp"



#include "Util/Assert.hpp"



SodiumEncrypter::SodiumEncrypter(Key key)
    : mKey(key)
{
    auto result = sodium_init();
    Assert(result >= 0);
}



EncryptedPacket SodiumEncrypter::Encrypt(Nonce nonce, const Packet& packet)
{
    Encrypted ciphertext;
    ciphertext.resize(crypto_secretbox_MACBYTES + packet->size);
    auto result = crypto_secretbox_easy(ciphertext.data(), packet->data, packet->size, nonce.data(), mKey.data());
    Assert(result >= 0);
    return {nonce, ciphertext};
}
