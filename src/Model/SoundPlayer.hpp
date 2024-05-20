#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Sound.hpp"
// Codec
#include "Codec/Audio/Frame.hpp"
#include "Codec/Audio/Mixer.hpp"
// Core
#include "Strawberry/Core/IO/ChannelBroadcaster.hpp"
#include "Strawberry/Core/Thread/RepeatingTask.hpp"
#include "Strawberry/Core/Util/IDPool.hpp"
#include "Strawberry/Core/Timing/Metronome.hpp"
// Standard Library
#include <Strawberry/Core/IO/ChannelReceiver.hpp>
#include <map>
#include <tuple>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement::SoundPlayer
{
	struct SoundStartedEvent {
		unsigned int soundID;
		bool         repeating;
	};

	struct SoundEndedEvent {
		unsigned int songID;
		size_t       soundID;
	};

	struct SoundRepeatEvent {
		unsigned int soundID;
		bool         repeating;
	};

	class SoundPlayer : public Core::IO::ChannelBroadcaster<SoundStartedEvent, SoundEndedEvent, SoundRepeatEvent>
	{
	public:
		SoundPlayer(Codec::Audio::FrameFormat format, unsigned int size);
		/// Receive a frame of audio from the player.
		Core::Optional<Codec::Audio::Frame> ReceiveAudio();
		/// Add a sound to the set of currently playing sounds.
		unsigned int                        PlaySound(size_t sound, bool repeat = false);
		/// Returns whether the given sound is repeating
		bool                                GetRepeat(unsigned int id) const;
		/// Sets whether the given sound should repeat
		void                                SetRepeat(unsigned int id, bool repeat);
		/// Remove a sound from the list of currently playing sounds.
		void                                RemoveSound(unsigned int id);

		size_t                              GetSoundID(unsigned int ticket);

	protected:
		void Mix();

		Codec::Audio::Frame AdjustVolume(Codec::Audio::Frame input);

	private:
		// A Sound Entry is a sound, how far into the sound we've mixed, and whether it should repeat.
		using SoundEntry = std::tuple<size_t, unsigned int, bool>;

		// A map of our currently active sounds
		Core::Mutex<std::map<unsigned int, SoundEntry>>                            mCurrentSounds;
		// Our audio mixer.
		Codec::Audio::Mixer                                                        mAudioMixer;
		// Our audio mixer channels
		std::map<unsigned int, std::shared_ptr<Codec::Audio::Mixer::InputChannel>> mMixerChannels;
		// The generator for our sound IDs.
		Core::IDPool<unsigned int>                                                 mIdGenerator;
		// The metronomes to time the mixing of frames by.
		std::map<unsigned int, Core::Metronome>                                    mMixingMetronomes;
		// The thread which does the mixing.
		Core::RepeatingTask                                                        mMixingThread;

		float                                                                      mVolume = 1.0f;
	};
} // namespace Strawberry::Accoutrement::SoundPlayer