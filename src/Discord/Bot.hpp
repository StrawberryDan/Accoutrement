#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Discord
#include "Discord/Bot.hpp"
// Codec
#include "Codec/Audio/Playlist.hpp"



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



	private:
		Bot();


	private:
		Codec::Audio::Playlist mPlaylist;
	};
}