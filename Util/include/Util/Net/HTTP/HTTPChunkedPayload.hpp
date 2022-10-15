#pragma once



#include <vector>
#include <cstdint>



#include "HTTPSimplePayload.hpp"



class HTTPChunkedPayload
{
public:
    using Chunk  = HTTPSimplePayload;
    using Chunks = std::vector<Chunk>;

    void AddChunk(const Chunk& chunk);
    [[nodiscard]] const Chunk& Get(size_t index) const;
    Chunk& Get(size_t index);
    [[nodiscard]] size_t Size() const;

    inline const Chunk& operator[](size_t index) const { return Get(index); }
    inline       Chunk& operator[](size_t index)       { return Get(index); }

    const Chunks& operator*() const { return mChunks; }
          Chunks& operator*()       { return mChunks; }

private:
    Chunks mChunks;
};