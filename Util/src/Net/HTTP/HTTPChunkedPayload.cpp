#include "Util/Net/HTTP/HTTPChunkedPayload.hpp"



void HTTPChunkedPayload::AddChunk(const HTTPChunkedPayload::Chunk& chunk)
{
    mChunks.push_back(chunk);
}



const HTTPChunkedPayload::Chunk& HTTPChunkedPayload::Get(size_t index) const
{
    return mChunks[index];
}



HTTPChunkedPayload::Chunk& HTTPChunkedPayload::Get(size_t index)
{
    return mChunks[index];
}



size_t HTTPChunkedPayload::Size() const
{
    return mChunks.size();
}
