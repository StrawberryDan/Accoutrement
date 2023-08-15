#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Discord
#include "Discord/Bot.hpp"
// Codec
#include "Codec/Audio/Mixer.hpp"
#include "Codec/Audio/Playlist.hpp"
// Core
#include "Strawberry/Core/Sync/Mutex.hpp"


namespace Strawberry::Accoutrement
{
	class Bot
		: public Strawberry::Discord::Bot
	{
	public:
		static void Initialise();

		static void Run();

		static void Stop();

		static Bot& Get();

		static Core::Option<Bot*> TryGet();


		auto& GetPlaylist() { return mPlaylist; }


	private:
		Bot();


	private:
		Core::Mutex<Codec::Audio::Playlist>                mPlaylist;
		std::shared_ptr<Codec::Audio::Mixer::InputChannel> mAudioChannel;
		Core::Option<Core::RepeatingTask>                  mAudioSendingThread;
	};
} // namespace Strawberry::Accoutrement