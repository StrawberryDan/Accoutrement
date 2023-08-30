//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundDatabase.hpp"

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

	Core::Optional<const Sound*> SoundDatabase::GetSound(SoundDatabase::Id id) const
	{
		return mSounds.contains(id) ? Core::Optional(&mSounds.at(id)) : Core::NullOpt;
	}

	size_t SoundDatabase::Count() const
	{
		return mSounds.size();
	}
} // namespace Strawberry::Accoutrement