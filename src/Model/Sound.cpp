//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Sound.hpp"
// Codec
#include "Codec/MediaFile.hpp"

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	Core::Optional<Sound> Sound::FromFile(const std::filesystem::path& path)
	{
		// Load our file
		auto file = Codec::MediaFile::Open(path);
		if (!file) return Core::NullOpt;
		// Get the best audio stream in the file.
		auto audioStream = file->GetBestStream(Codec::MediaType::Audio);
		if (!audioStream) return Core::NullOpt;
		// Get a decoder for the stream.
		Codec::Audio::Decoder            decoder = audioStream->GetDecoder();
		// Extract and decoder our frames.
		std::vector<Codec::Audio::Frame> frames;
		while (auto packet = audioStream->Read())
		{
			decoder.Send(packet.Unwrap());
			auto decodedFrames = decoder.Receive();
			for (auto& frame : decodedFrames) { frames.emplace_back(std::move(frame)); }
		}
		// Compress and return as a Sound.
		frames.shrink_to_fit();
		Sound sound(frames);
		sound.mName = path;
		return sound;
	}

	Sound::Sound(std::vector<Codec::Audio::Frame> frames)
		: mFrames(std::move(frames))
	{}

	const Codec::Audio::Frame& Sound::GetFrame(size_t index) const
	{
		Core::Assert(index < mFrames.size());
		return mFrames[index];
	}

	const Codec::Audio::Frame& Sound::operator[](size_t index) const
	{
		return GetFrame(index);
	}

	size_t Sound::Size() const
	{
		return mFrames.size();
	}
} // namespace Strawberry::Accoutrement