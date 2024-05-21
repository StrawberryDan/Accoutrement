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
		return Sound(file.Unwrap());
	}

	Core::Optional<Sound> Sound::FromJSON(const nlohmann::json& json)
	{
		std::filesystem::path file(json["file"]);
		if (!std::filesystem::exists(file)) return Core::NullOpt;

		Core::Optional<Sound> sound = Sound::FromFile(file);
		if (!sound) return Core::NullOpt;

		if (json.contains("name")) { sound->SetName(json["name"]); }

		return sound;
	}

	nlohmann::json Sound::AsJSON() const
	{
		return {
			{"file", mFile.GetPath().string()},
			{"name", mName},
		};
	}


	Core::Optional<Codec::Audio::Frame> Sound::Read()
	{
		if (mBufferedPackets.empty())
		{
			mStream->Seek(mDecodePosition);
			for (int i = 0; i < 64; i++)
			{
				auto packet = mStream->Read();
				if (!packet)
				{
					break;
				}

				mDecodePosition += packet.Value()->duration * mStream->GetTimeBase().Evaluate();
				mBufferedPackets.emplace_back(packet.Unwrap());
			}
			mBufferedPackets.shrink_to_fit();
		}

		if (mBufferedFrames.empty())
		{
			for (int i = 0; i < mBufferedPackets.size(); i++)
			{
				mDecoder.Send(std::move(mBufferedPackets[i]));
				auto frames = mDecoder.Receive();

				for (auto& frame: frames)
				{
					mBufferedFrames.push_back(std::move(frame));
				}
			}
			mBufferedPackets.clear();
		}

		if (mBufferedFrames.empty() && mBufferedPackets.empty())
		{
			return Core::NullOpt;
		}

		auto result = std::move(mBufferedFrames.front());
		mBufferedFrames.pop_front();
		mPosition += result.GetDuration();
		return result;
	}


	Sound::Sound(Codec::MediaFile file)
		: mName(file.GetPath().string())
		, mFile(std::move(file))
		, mStream(mFile.GetBestStream(Codec::MediaType::Audio))
		, mDecoder(mStream->GetDecoder())
	{
		Seek(0.0);
	}


	void Sound::Seek(Core::Seconds time)
	{
		mStream->Seek(time);
		mPosition = time;
		mDecodePosition = time;
		mBufferedPackets.clear();
		mBufferedFrames.clear();
	}
} // namespace Strawberry::Accoutrement