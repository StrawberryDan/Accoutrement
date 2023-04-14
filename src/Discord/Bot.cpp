#include "Bot.hpp"



#include <memory>



#include <thread>
#include "Core/Assert.hpp"
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
		gBot = std::make_unique<Bot>();
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
	}



	Bot& Bot::Get()
	{
		Assert(gBot != nullptr);
		return *gBot;
	}



	Bot::Bot()
			: Strawberry::Discord::Bot(Config::Get().GetToken(), Intent::GUILDS | Intent::GUILD_VOICE_STATES)
	{}
}