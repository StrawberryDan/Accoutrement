#include <iostream>
#include "Codec/AudioFile.hpp"
#include "Codec/OpusEncoder.hpp"
#include "Codec/Muxer.hpp"



int main()
{
    av_log_set_level(AV_LOG_DEBUG);

    AudioFile file("data/music.mp3");
    OpusEncoder encoder;
    Samples samples;
    while (!file.IsEof())
    {
        auto frame = file.ReadFrame();
        if (frame)
        {
            auto someSamples = frame->GetSamples();
            samples.Append(std::move(someSamples));
        }
    }
    samples.Multiply(10.0);

    std::vector<Packet> packets = encoder.Encode(samples);
    {
        auto final = encoder.Finish();
        packets.insert(packets.end(), final.begin(), final.end());
    }

    Muxer muxer("output.opus");
    muxer.OpenStream(encoder.Parameters());
    muxer.WriteHeader();
    for (auto& packet : packets)
    {
        muxer.WritePacket(packet);
    }
    muxer.WriteTrailer();
    muxer.Flush();

    return 0;
}