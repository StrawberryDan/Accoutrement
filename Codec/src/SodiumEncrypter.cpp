#include "Codec/SodiumEncrypter.hpp"



#include <cassert>



SodiumEncrypter::SodiumEncrypter(Key key)
    : mKey(key)
{
    auto result = sodium_init();
    assert(result >= 0);
}



EncryptedPacket SodiumEncrypter::Encrypt(const Packet& packet)
{
    Encrypted ciphertext;
    Nonce nonce;
    randombytes_buf(nonce.data(), nonce.size());
    ciphertext.resize(crypto_secretbox_MACBYTES + packet->size);
    auto result = crypto_secretbox_easy(ciphertext.data(), packet->data, packet->size, nonce.data(), mKey.data());
    assert(result >= 0);
    return {nonce, ciphertext};
}
