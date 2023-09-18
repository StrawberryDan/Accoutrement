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
#include "Strawberry/Core/Util/Metronome.hpp"
// Standard Library
#include <Strawberry/Core/IO/ChannelReceiver.hpp>
#include <map>
#include <tuple>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	namespace SoundPlayerEvent
	{
		struct SoundStarted {
			unsigned int soundID;
			const Sound* sound;
			bool         repeating;
		};

		struct SoundEnded {
			unsigned int songID;
			const Sound* sound;
		};

		struct SoundRepeat {
			unsigned int songID;
			bool         repeating;
		};
	} // namespace SoundPlayerEvent

	class SoundPlayer : public Core::IO::ChannelBroadcaster<SoundPlayerEvent::SoundStarted, SoundPlayerEvent::SoundEnded, SoundPlayerEvent::SoundRepeat>
	{
	public:
		SoundPlayer(Codec::Audio::FrameFormat format, unsigned int size);
		/// Receive a frame of audio from the player.
		Core::Optional<Codec::Audio::Frame> ReceiveAudio();
		/// Add a sound to the set of currently playing sounds.
		unsigned int                        PlaySound(Sound sound, bool repeat = false);
		/// Returns whether the given sound is repeating
		bool                                GetRepeat(unsigned int id) const;
		/// Sets whether the given sound should repeat
		void                                SetRepeat(unsigned int id, bool repeat);
		/// Remove a sound from the list of currently playing sounds.
		void                                RemoveSound(unsigned int id);

	protected:
		void Mix();

	private:
		// A Sound Entry is a sound, how far into the sound we've mixed, and whether it should repeat.
		using SoundEntry = std::tuple<Sound, unsigned int, bool>;

		// A map of our currently active sounds
		std::map<unsigned int, SoundEntry>                                         mCurrentSounds;
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
	};
} // namespace Strawberry::Accoutrement