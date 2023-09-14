//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundPlayer.hpp"
// Standard Library
#include <thread>

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	SoundPlayer::SoundPlayer(Codec::Audio::FrameFormat format, unsigned int size)
		: mAudioMixer(format, size)
		, mMixingThread([this]() { Mix(); })
	{}

	Core::Optional<Codec::Audio::Frame> SoundPlayer::ReceiveAudio()
	{
		if (!mAudioMixer.IsEmpty()) { return mAudioMixer.ReadFrame(); }
		else { return Core::NullOpt; }
	}

	unsigned int SoundPlayer::PlaySound(Sound sound, bool repeat)
	{
		auto id = mIdGenerator.Generate();
		mCurrentSounds.emplace(id, std::make_tuple(std::move(sound), 0, repeat));
		return id;
	}

	void SoundPlayer::RemoveSound(unsigned int id)
	{
		mCurrentSounds.erase(id);
		mMixerChannels.erase(id);
		mMixingMetronomes.erase(id);
		mIdGenerator.Free(id);
	}

	void SoundPlayer::Mix()
	{
		// List of sounds to remove once we're done iterating.
		std::set<unsigned int> soundsToRemove;
		// Iterate over our current sounds
		for (auto& [id, soundEntry] : mCurrentSounds)
		{
			// Unpack our sound data
			auto& [sound, progress, repeating] = soundEntry;
			// Make sure we have the input channel for this sound
			if (!mMixerChannels.contains(id)) { mMixerChannels.emplace(id, mAudioMixer.CreateInputChannel()); }
			// Get our input channel
			auto& inputChannel = mMixerChannels.at(id);
			// Get our metronome
			if (!mMixingMetronomes.contains(id)) { mMixingMetronomes.emplace(id, Core::Metronome(0.0, 0.01)); }
			auto& metronome = mMixingMetronomes.at(id);

			if (metronome)
			{
				// Update our progress counter
				if (progress >= sound.Size())
				{
					if (repeating) { progress = 0; }
					else
					{
						// Remove this later
						soundsToRemove.emplace(id);
						continue;
					}
				}

				// Get the frame to mix
				auto frame = sound[progress++];
				// Send our frame
				inputChannel->EnqueueFrame(frame);
				// Update metronome
				metronome.SetFrequency(frame.GetDuration());
				metronome.Tick();
			}
		}

		// Remove songs where appropriate
		for (auto id : soundsToRemove) { RemoveSound(id); }

		std::this_thread::yield();
	}
} // namespace Strawberry::Accoutrement