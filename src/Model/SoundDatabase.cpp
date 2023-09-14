//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundDatabase.hpp"
// JSON
#include "nlohmann/json.hpp"
// Standard Library
#include <fstream>

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	static Core::Mutex<Core::Optional<Core::Mutex<SoundDatabase>>> sInstance;

	Core::MutexGuard<SoundDatabase> SoundDatabase::Get()
	{
		auto lock = sInstance.Lock();
		if (!lock->HasValue()) { (*lock) = Core::Mutex(SoundDatabase()); }
		return lock->Value().Lock();
	}

	SoundDatabase::~SoundDatabase()
	{
		nlohmann::json json;
		for (const auto& [index, sound] : mSounds) { json["sounds"].push_back(sound.AsJSON()); }


		std::ofstream file("./sound_database.json");
		file << json.dump('\t');
	}

	SoundDatabase::Id SoundDatabase::AddSound(Sound sound)
	{
		auto id = mNextId++;
		mSounds.emplace(id, std::move(sound));
		return id;
	}

	void SoundDatabase::RemoveSound(SoundDatabase::Id id)
	{
		mSounds.erase(id);
	}

	Core::Optional<Sound*> SoundDatabase::GetSound(SoundDatabase::Id id)
	{
		return mSounds.contains(id) ? Core::Optional(&mSounds.at(id)) : Core::NullOpt;
	}

	size_t SoundDatabase::Count() const
	{
		return mSounds.size();
	}

	SoundDatabase::SoundDatabase()
	{
		std::ifstream file("./sound_database.json");

		if (file.is_open())
		{
			nlohmann::json json;
			file >> json;

			for (auto songData : json["sounds"])
			{
				Core::Optional<Sound> sound = Sound::FromJSON(songData);
				if (!sound) continue;
				else
					AddSound(sound.Unwrap());
			}
		}
	}
} // namespace Strawberry::Accoutrement