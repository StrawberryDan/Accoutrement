#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "../Model/SoundPlayer.hpp"
// Discord
#include "Discord/Bot.hpp"
// Codec
#include "Codec/Audio/Mixer.hpp"
#include "Codec/Audio/Playlist.hpp"
// Core
#include "Strawberry/Core/Sync/Mutex.hpp"

namespace Strawberry::Accoutrement
{
	class Bot : public Strawberry::Discord::Bot
	{
	public:
		static void Initialise();

		static void Run();

		static void Stop();

		static Core::Optional<Bot*> Get();

		static Core::Optional<Bot*> TryGet();

		auto& GetPlaylist() { return mPlaylist; }

		auto& GetSoundPlayer() { return mSoundPlayer; }


	private:
		Bot();


	private:
		Core::Mutex<Codec::Audio::Playlist>                mPlaylist;
		std::shared_ptr<Codec::Audio::Mixer::InputChannel> mMusicChannel;
		std::shared_ptr<Codec::Audio::Mixer::InputChannel> mSoundChannel;
		Core::Optional<Core::RepeatingTask>                mAudioSendingThread;
		Core::Mutex<SoundPlayer>                           mSoundPlayer;
	};
} // namespace Strawberry::Accoutrement