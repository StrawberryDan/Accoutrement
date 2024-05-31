#include "Bot.hpp"


#include <memory>


#include "../Config.hpp"
#include "Strawberry/Core/Assert.hpp"
#include "Strawberry/Core/Timing/Metronome.hpp"
#include <thread>


using Strawberry::Core::Assert;
using Strawberry::Discord::Intent;

namespace Strawberry::Accoutrement
{
    static std::unique_ptr<Bot> gBot = nullptr;
    static std::future<void>    gRun = {};


    bool Bot::Initialise()
    {
        Assert(!gBot);

        if (!Config::Get().GetToken()) return false;
        if (!gBot) gBot = std::unique_ptr<Bot>(new Bot());
        if (!gBot->GetBot()) return false;

        return true;
    }


    void Bot::Run()
    {
        if (gBot && gBot->GetBot())
        {
            gBot->Broadcast(BotStartedRunningEvent());
            gRun = std::async(std::launch::async,
                              []()
                              {
                                  gBot->GetBot()->Run();
                              });
        }
    }


    void Bot::Shutdown()
    {
        if (gBot)
        {
            if (gBot->mBot) gBot->mBot->Shutdown();
            if (gRun.valid()) gRun.wait();
            gBot.reset();
            gBot->Broadcast(BotStoppedRunningEvent());
        }
    }


    Core::Optional<Bot*> Bot::Get()
    {
        return gBot ? Core::Optional(gBot.get()) : Core::NullOpt;
    }


    Core::Optional<Bot*> Bot::TryGet()
    {
        return gBot.get();
    }


    Bot::Bot()
        : mBot(Discord::Bot::Connect(Config::Get().GetToken().Value(), Intent::GUILDS | Intent::GUILD_VOICE_STATES))
        , mPlaylist(Codec::Audio::FrameFormat(48000, AV_SAMPLE_FMT_FLT, AV_CHANNEL_LAYOUT_STEREO), 960)
        , mSoundPlayer(Codec::Audio::FrameFormat(48000, AV_SAMPLE_FMT_FLT, AV_CHANNEL_LAYOUT_STEREO), 960)
    {
        mAudioSendingThread.Emplace(
            [this, musicClock = Core::Metronome(0.00, 0.01), sfxClock = Core::Metronome(0.00, 0.01)](
        Core::RepeatingTask* thread) mutable
            {
                if (musicClock || sfxClock)
                {
                    if (auto connection = Bot::TryGet().AndThen([](auto x)
                    {
                        return x->mBot ? x->mBot->GetVoiceConnection() : Core::NullOpt;
                    }); connection && !mMusicChannel && !mSoundChannel)
                    {
                        mMusicChannel = connection->CreateInputChannel();
                        mSoundChannel = connection->CreateInputChannel();
                    }
                    else if (!connection && mMusicChannel && mSoundChannel)
                    {
                        mMusicChannel.reset();
                        mSoundChannel.reset();
                    }

                    if (musicClock)
                    {
                        if (auto frame = mPlaylist.Lock()->ReadFrame())
                        {
                            musicClock.SetFrequency(frame->GetDuration());
                            musicClock.Tick();
                            if (mMusicChannel) mMusicChannel->EnqueueFrame(frame.Unwrap());
                        }
                        else
                        {
                            musicClock.TickWithoutProgress();
                        }
                    }

                    if (sfxClock)
                    {
                        if (auto frame = mSoundPlayer.Lock()->ReceiveAudio())
                        {
                            sfxClock.SetFrequency(frame->GetDuration());
                            sfxClock.Tick();
                            if (mSoundChannel) mSoundChannel->EnqueueFrame(frame.Unwrap());
                        }
                        else
                        {
                            sfxClock.TickWithoutProgress();
                        }
                    }
                }
                else
                {
                    std::this_thread::yield();
                }
            });
    }
} // namespace Strawberry::Accoutrement
