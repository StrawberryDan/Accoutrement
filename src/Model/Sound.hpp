#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Codec
#include "Codec/Audio/Frame.hpp"
// Core
#include "Strawberry/Core/Util/Optional.hpp"
// Standard Library
#include <filesystem>
#include <vector>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class Sound
	{
	public:
		/// Loads a sound from a media file.
		static Core::Optional<Sound> FromFile(const std::filesystem::path& path);


		/// Constructs a sound from a list of frames.
		Sound(std::vector<Codec::Audio::Frame> frames);


		/// Returns a constant reference to the index'th audio frame in the sound.
		const Codec::Audio::Frame& GetFrame(size_t index) const;
		const Codec::Audio::Frame& operator[](size_t index) const;

		/// Returns the name of the sound.
		const std::string& GetName() const { return mName; }

		/// Sets the name of the sound
		void SetName(const std::string& name) { mName = name; }

		/// Returns the number of audio frames in the sound.
		size_t Size() const;


	private:
		std::string                      mName;
		std::vector<Codec::Audio::Frame> mFrames;
	};
} // namespace Strawberry::Accoutrement
