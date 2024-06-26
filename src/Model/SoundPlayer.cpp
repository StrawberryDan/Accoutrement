//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundPlayer.hpp"
#include "SoundDatabase.hpp"
// Standard Library
#include <thread>

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement::SoundPlayer
{
    SoundPlayer::SoundPlayer(Codec::Audio::FrameFormat format, unsigned int size)
        : mAudioMixer(format, size)
        , mMixingThread([this]()
        {
            Mix();
        })
        , mVolume(100, 100) {}


    Core::Optional<Codec::Audio::Frame> SoundPlayer::ReceiveAudio()
    {
        if (!mAudioMixer.IsEmpty())
        {
            auto frame = mAudioMixer.ReadFrame();
            frame.Multiply(std::pow(2.0, mVolume.Evaluate()) - 1.0);
            return frame;
        }
        else
        {
            return Core::NullOpt;
        }
    }


    unsigned int SoundPlayer::PlaySound(size_t sound, bool repeat)
    {
        auto currentSounds = mCurrentSounds.Lock();

        auto id = mIdGenerator.Allocate();
        currentSounds->emplace(id, SoundEntry{sound, 0.0, repeat, {100, 100}});
        SoundDatabase::Get()->GetSound(sound)->Seek(0.0);
        Broadcast(SoundStartedEvent{.soundTicket = static_cast<unsigned int>(id), .repeating = repeat});
        return id;
    }


    bool SoundPlayer::GetRepeat(unsigned int id) const
    {
        auto currentSounds = mCurrentSounds.Lock();

        const auto& [song, progress, repeating, volume] = currentSounds->at(id);
        return repeating;
    }


    void SoundPlayer::SetRepeat(unsigned int id, bool repeat)
    {
        auto currentSounds = mCurrentSounds.Lock();

        auto& [sound, progress, repeating, volume] = currentSounds->at(id);
        repeating                                  = repeat;
        Broadcast(SoundRepeatEvent{.soundTicket = static_cast<unsigned int>(id), .repeating = repeat});
    }


    void SoundPlayer::RemoveSound(unsigned int id)
    {
        auto currentSounds = mCurrentSounds.Lock();

        const auto& [sound, progress, repeating, volume] = currentSounds->at(id);
        Broadcast(SoundEndedEvent{.soundTicket = id, .soundID = sound});
        currentSounds->erase(id);
        mMixerChannels.erase(id);
        mMixingMetronomes.erase(id);
        mIdGenerator.Free(id);
    }


    size_t SoundPlayer::GetSoundID(unsigned int ticket)
    {
        return mCurrentSounds.Lock()->at(ticket).soundID;
    }


    void SoundPlayer::Mix()
    {
        auto currentSounds = mCurrentSounds.Lock();
        // List of sounds to remove once we're done iterating.
        std::set<unsigned int> soundsToRemove;
        // Iterate over our current sounds
        for (auto& [id, soundEntry]: *currentSounds)
        {
            // Unpack our sound data
            auto& [sound, progress, repeating, volume] = soundEntry;
            // Make sure we have the input channel for this sound
            if (!mMixerChannels.contains(id))
            {
                mMixerChannels.emplace(id, mAudioMixer.CreateInputChannel());
            }
            // Get our input channel
            auto& inputChannel = mMixerChannels.at(id);
            // Get our metronome
            if (!mMixingMetronomes.contains(id))
            {
                mMixingMetronomes.emplace(id, Core::Metronome(0.0, 0.01));
            }
            auto& metronome = mMixingMetronomes.at(id);

            if (metronome)
            {
                auto soundData = SoundDatabase::Get()->GetSound(sound);
                if (!soundData)
                    continue;

                // Get the frame to mix
                auto frame = soundData->Read();
                // Update our progress counter
                if (!frame)
                {
                    if (repeating)
                    {
                        progress = 0.0;
                        soundData->Seek(0.0);
                        continue;
                    }
                    else
                    {
                        // Remove this later
                        soundsToRemove.emplace(id);
                        continue;
                    }
                }

                // Update metronome
                progress += frame->GetDuration();
                metronome.SetFrequency(frame->GetDuration());
                metronome.Tick();
                // Send our frame
                frame->Multiply(std::pow(2.0, volume.Evaluate()) - 1.0);
                inputChannel->EnqueueFrame(std::move(frame.Unwrap()));
            }
        }

        // Remove songs where appropriate
        for (auto id: soundsToRemove)
        {
            RemoveSound(id);
        }

        std::this_thread::yield();
    }


    float SoundPlayer::GetVolume() const
    {
        return mVolume.Numerator();
    }


    void SoundPlayer::SetVolume(int volume)
    {
        mVolume.Numerator() = volume;
    }


    int SoundPlayer::GetTrackVolume(unsigned int ticket) const
    {
        return mCurrentSounds.Lock()->at(ticket).volume.Numerator();
    }


    void SoundPlayer::SetTrackVolume(unsigned int ticket, int volume)
    {
        mCurrentSounds.Lock()->at(ticket).volume.Numerator() = volume;
    }
} // namespace Strawberry::Accoutrement::SoundPlayer
