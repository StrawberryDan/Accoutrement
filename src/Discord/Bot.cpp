#include "Bot.hpp"


#include <memory>


#include "../Config.hpp"
#include "Strawberry/Core/Util/Assert.hpp"
#include "Strawberry/Core/Util/Metronome.hpp"
#include <thread>


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
		gRun = std::async(std::launch::async, []() { gBot->Strawberry::Discord::Bot::Run(); });
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

	Core::Optional<Bot*> Bot::TryGet()
	{
		return gBot.get();
	}

	Bot::Bot()
		: Strawberry::Discord::Bot(Config::Get().GetToken(), Intent::GUILDS | Intent::GUILD_VOICE_STATES)
		, mPlaylist(Codec::Audio::FrameFormat(48000, AV_SAMPLE_FMT_S32, AV_CHANNEL_LAYOUT_STEREO), 960)
		, mSoundPlayer(Codec::Audio::FrameFormat(48000, AV_SAMPLE_FMT_S32, AV_CHANNEL_LAYOUT_STEREO), 960)
	{
		mAudioSendingThread.Emplace(
			[this, musicClock = Core::Metronome(0.00, 0.01), sfxClock = Core::Metronome(0.00, 0.01)](Core::RepeatingTask* thread) mutable {
				if (musicClock || sfxClock)
				{
					if (auto connection = Bot::TryGet().AndThen([](auto x) { return x->GetVoiceConnection().AsPtr(); });
						connection && !mMusicChannel && !mSoundChannel)
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
						else { musicClock.TickWithoutProgress(); }
					}

					if (sfxClock)
					{
						if (auto frame = mSoundPlayer.Lock()->ReceiveAudio())
						{
							sfxClock.SetFrequency(frame->GetDuration());
							sfxClock.Tick();
							if (mSoundChannel) mSoundChannel->EnqueueFrame(frame.Unwrap());
						}
						else { sfxClock.TickWithoutProgress(); }
					}
				}
				else { std::this_thread::yield(); }
			});
	}
} // namespace Strawberry::Accoutrement