#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Discord
#include "Discord/Bot.hpp"
// Codec
#include "Codec/Audio/Playlist.hpp"
#include "Codec/Audio/Mixer.hpp"



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
		Codec::Audio::Playlist                             mPlaylist;
		std::shared_ptr<Codec::Audio::Mixer::InputChannel> mAudioChannel;
		Core::Option<Core::RepeatingTask>                  mAudioSendingThread;
	};
}