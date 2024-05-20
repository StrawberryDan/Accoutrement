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


	Core::Optional<Codec::Audio::Frame> Sound::GetFrame(size_t index)
	{
		Seek(index);

		if (mBufferedPackets.empty())
		{
			for (int i = 0; i < 1024; i++)
			{
				auto packet = mStream->Read();
				if (!packet) break;
				mBufferedPackets.emplace_back(packet.Unwrap());
			}
			mBufferedPackets.shrink_to_fit();
		}

		if (mBufferedFrames.empty())
		{
			for (int i = 0; i < 1024 && mCurrentDTS + i < mBufferedPackets.size(); i++)
			{
				mDecoder.Send(mBufferedPackets[mCurrentDTS + i]);
				auto frames = mDecoder.Receive();

				for (auto& frame: frames)
				{
					mBufferedFrames.push_back(std::move(frame));
				}
			}
		}

		if (mBufferedFrames.empty())
		{
			return Core::NullOpt;
		}

		auto result = std::move(mBufferedFrames.front());
		mBufferedFrames.pop_front();
		return result;
	}

	Codec::Audio::Frame Sound::operator[](size_t index)
	{
		return GetFrame(index).Unwrap();
	}


	Sound::Sound(Codec::MediaFile file)
		: mName(file.GetPath().string())
		, mFile(std::move(file))
		, mStream(mFile.GetBestStream(Codec::MediaType::Audio))
		, mDecoder(mStream->GetDecoder())
		, mBufferedPackets()
	{}


	void Sound::Seek(int dts)
	{
		if (dts != 0 && mCurrentDTS != dts - 1)
		{
			mCurrentDTS = dts;
			while (!(mBufferedPackets[mCurrentDTS]->flags & AV_FRAME_FLAG_KEY) && mCurrentDTS > 0)
			{
				mCurrentDTS--;
			}

			mBufferedFrames.clear();
		}
		else
		{
			mCurrentDTS = dts;
		}
	}
} // namespace Strawberry::Accoutrement