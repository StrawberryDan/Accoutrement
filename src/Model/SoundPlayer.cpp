//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundPlayer.hpp"
#include "SoundDatabase.hpp"
// Standard Library
#include <thread>

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement::SoundPlayer
{
	SoundPlayer::SoundPlayer(Codec::Audio::FrameFormat format, unsigned int size)
		: mAudioMixer(format, size)
		, mMixingThread([this]() { Mix(); })
	{}

	Core::Optional<Codec::Audio::Frame> SoundPlayer::ReceiveAudio()
	{
		if (!mAudioMixer.IsEmpty()) { return AdjustVolume(mAudioMixer.ReadFrame()); }
		else { return Core::NullOpt; }
	}

	unsigned int SoundPlayer::PlaySound(size_t sound, bool repeat)
	{
		auto currentSounds = mCurrentSounds.Lock();

		auto id = mIdGenerator.Allocate();
		currentSounds->emplace(id, std::make_tuple(sound, 0, repeat, 1.0f));
		Broadcast(SoundStartedEvent{.soundID = static_cast<unsigned int>(id), .repeating = repeat});
		return id;
	}

	bool SoundPlayer::GetRepeat(unsigned int id) const
	{
		auto currentSounds = mCurrentSounds.Lock();

		const auto& [song, position, repeating, volume] = currentSounds->at(id);
		return repeating;
	}

	void SoundPlayer::SetRepeat(unsigned int id, bool repeat)
	{
		auto currentSounds = mCurrentSounds.Lock();

		auto& [sound, position, repeating, volume] = currentSounds->at(id);
		repeating                         = repeat;
		Broadcast(SoundRepeatEvent{.soundID = static_cast<unsigned int>(id), .repeating = repeat});
	}

	void SoundPlayer::RemoveSound(unsigned int id)
	{
		auto currentSounds = mCurrentSounds.Lock();

		const auto& [sound, progress, repeating, volume] = currentSounds->at(id);
		Broadcast(SoundEndedEvent{.songID = id, .soundID = sound});
		currentSounds->erase(id);
		mMixerChannels.erase(id);
		mMixingMetronomes.erase(id);
		mIdGenerator.Free(id);
	}


	size_t SoundPlayer::GetSoundID(unsigned int ticket)
	{
		return std::get<0>(mCurrentSounds.Lock()->at(ticket));
	}


	void SoundPlayer::Mix()
	{
		auto currentSounds = mCurrentSounds.Lock();
		// List of sounds to remove once we're done iterating.
		std::set<unsigned int> soundsToRemove;
		// Iterate over our current sounds
		for (auto& [id, soundEntry] : *currentSounds)
		{
			// Unpack our sound data
			auto& [sound, progress, repeating, volume] = soundEntry;
			// Make sure we have the input channel for this sound
			if (!mMixerChannels.contains(id)) { mMixerChannels.emplace(id, mAudioMixer.CreateInputChannel()); }
			// Get our input channel
			auto& inputChannel = mMixerChannels.at(id);
			// Get our metronome
			if (!mMixingMetronomes.contains(id)) { mMixingMetronomes.emplace(id, Core::Metronome(0.0, 0.01)); }
			auto& metronome = mMixingMetronomes.at(id);

			if (metronome)
			{
				auto soundData = SoundDatabase::Get()->GetSound(sound).Unwrap();
				// Get the frame to mix
				auto frame = soundData->GetFrame(progress++);
				// Update our progress counter
				if (!frame)
				{
					if (repeating) { progress = 0; }
					else
					{
						// Remove this later
						soundsToRemove.emplace(id);
						continue;
					}
				}

				// Update metronome
				metronome.SetFrequency(frame->GetDuration());
				metronome.Tick();
				// Send our frame
				frame->Multiply(volume);
				inputChannel->EnqueueFrame(std::move(frame.Unwrap()));
			}
		}

		// Remove songs where appropriate
		for (auto id : soundsToRemove) { RemoveSound(id); }

		std::this_thread::yield();
	}


	Codec::Audio::Frame SoundPlayer::AdjustVolume(Codec::Audio::Frame input)
	{
		input.Multiply(mVolume);
		return input;
	}


	float SoundPlayer::GetVolume() const
	{
		return mVolume;
	}


	void SoundPlayer::SetVolume(float volume)
	{
		mVolume = volume;
	}
} // namespace Strawberry::Accoutrement::SoundPlayer