#include "Bot.hpp"



#include <memory>



#include <thread>
#include "Strawberry/Core/Util/Assert.hpp"
#include "../Config.hpp"
#include "Strawberry/Core/Util/Metronome.hpp"



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


	Core::Option<Bot*> Bot::TryGet()
	{
		return gBot.get();
	}


	Bot::Bot()
		: Strawberry::Discord::Bot(Config::Get().GetToken(), Intent::GUILDS | Intent::GUILD_VOICE_STATES)
		, mPlaylist(Codec::Audio::FrameFormat(48000, AV_SAMPLE_FMT_S32, AV_CHANNEL_LAYOUT_STEREO), 960)
	{
		mAudioSendingThread.Emplace([
			this,
			clock = Core::Metronome(0.00, 0.01)
		] (Core::RepeatingTask* thread) mutable	{
			if (clock)
			{
				if (auto connection = Bot::TryGet().AndThen([](auto x) { return x->GetVoiceConnection().AsPtr(); });
					connection && !mAudioChannel)
				{
					mAudioChannel = connection->CreateInputChannel();
				}
				else if (!connection && mAudioChannel)
				{
					mAudioChannel.reset();
				}

				if (auto frame = mPlaylist.Lock()->ReadFrame())
				{
					clock.SetFrequency(frame->GetDuration());
					clock.Tick();
					if (mAudioChannel)
						mAudioChannel->EnqueueFrame(frame.Unwrap());
				}
				else
				{
					clock.Restart();
				}
			}
			else
			{
				std::this_thread::yield();
			}
		});
	}
}