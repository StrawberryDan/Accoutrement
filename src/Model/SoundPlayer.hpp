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
#include "Strawberry/Core/Thread/RepeatingTask.hpp"
#include "Strawberry/Core/Util/IDGenerator.hpp"
#include "Strawberry/Core/Util/Metronome.hpp"
// Standard Library
#include <map>
#include <tuple>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class SoundPlayer
	{
	public:
		SoundPlayer(Codec::Audio::FrameFormat format, unsigned int size);
		// Receive a frame of audio from the player.
		Core::Optional<Codec::Audio::Frame> ReceiveAudio();
		// Add a sound to the set of currently playing sounds.
		unsigned int                        PlaySound(Sound sound, bool repeat = false);
		// Remove a sound from the list of currently playing sounds.
		void                                RemoveSound(unsigned int id);

	protected:
		void Mix();

	private:
		// A Sound Entry is a sound, how far into the sound we've mixes, and whether it should repeat.
		using SoundEntry = std::tuple<Sound, unsigned int, bool>;

		// A map of our currently active sounds
		std::map<unsigned int, SoundEntry>                                         mCurrentSounds;
		// Our audio mixer.
		Codec::Audio::Mixer                                                        mAudioMixer;
		// Our audio mixer channels
		std::map<unsigned int, std::shared_ptr<Codec::Audio::Mixer::InputChannel>> mMixerChannels;
		// The generator for our sound IDs.
		Core::IDGenerator<unsigned int>                                            mIdGenerator;
		// The metronomes to time the mixing of frames by.
		std::map<unsigned int, Core::Metronome>                                    mMixingMetronomes;
		// The thread which does the mixing.
		Core::RepeatingTask                                                        mMixingThread;
	};
} // namespace Strawberry::Accoutrement