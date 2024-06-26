#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "../Model/SoundPlayer.hpp"
#include "../UI/Events/BotStartedRunningEvent.hpp"
#include "../UI/Events/BotStoppedRunningEvent.hpp"
// Discord
#include "Discord/Bot.hpp"
// Codec
#include "Codec/Audio/Mixer.hpp"
#include "Codec/Audio/Playlist.hpp"
// Core
#include "Strawberry/Core/IO/Broadcaster.hpp"
#include "Strawberry/Core/Sync/Mutex.hpp"

namespace Strawberry::Accoutrement
{
    class Bot : public Core::IO::Broadcaster<BotStartedRunningEvent, BotStoppedRunningEvent>
    {
        public:
            [[nodiscard]] static bool Initialise();

            static void Run();

            static void Shutdown();

            static Core::Optional<Bot*> Get();

            static Core::Optional<Bot*> TryGet();


            auto& GetBot()
            {
                return mBot;
            }


            auto& GetPlaylist()
            {
                return mPlaylist;
            }


            auto& GetSoundPlayer()
            {
                return mSoundPlayer;
            }

        private:
            Bot();

        private:
            Core::Optional<Discord::Bot>                       mBot;
            Core::Mutex<Codec::Audio::Playlist::Playlist>      mPlaylist;
            std::shared_ptr<Codec::Audio::Mixer::InputChannel> mMusicChannel;
            std::shared_ptr<Codec::Audio::Mixer::InputChannel> mSoundChannel;
            Core::Mutex<SoundPlayer::SoundPlayer>              mSoundPlayer;

            // Must be deleted first
            Core::Optional<Core::RepeatingTask> mAudioSendingThread;
    };
} // namespace Strawberry::Accoutrement
