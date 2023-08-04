#include "Bot.hpp"



#include <memory>



#include <thread>
#include "Strawberry/Core/Assert.hpp"
#include "../Config.hpp"



using Strawberry::Core::Assert;
using Strawberry::Discord::Intent;



namespace Strawberry::Accoutrement
{
	static std::unique_ptr<Bot> gBot = nullptr;
	static std::future<void>    gRun = {};



	void Bot::Initialise()
	{
		Assert(!gBot);
		gBot = std::unique_ptr<Bot>(new Bot());
	}



	void Bot::Run()
	{
		gRun = std::async(std::launch::async, []()
		{
			gBot->Strawberry::Discord::Bot::Run();
		});
	}



	void Bot::Stop()
	{
		gBot->Strawberry::Discord::Bot::Stop();
		gRun.wait();
		gRun = {};
		gBot.reset();
	}



	Bot& Bot::Get()
	{
		Assert(gBot != nullptr);
		return *gBot;
	}



	Bot::Bot()
		: Strawberry::Discord::Bot(Config::Get().GetToken(), Intent::GUILDS | Intent::GUILD_VOICE_STATES)
		, mPlaylist({48000, AV_SAMPLE_FMT_S32, AV_CHANNEL_LAYOUT_STEREO}, 960)
	{}
}