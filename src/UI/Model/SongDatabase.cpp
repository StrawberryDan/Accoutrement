//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SongDatabase.hpp"


//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	std::unique_ptr<SongDatabase> SongDatabase::sGlobalInstance = nullptr;


	SongDatabase& SongDatabase::Get()
	{
		if (!sGlobalInstance)
		{
			sGlobalInstance = std::unique_ptr<SongDatabase>(new SongDatabase());
		}

		return *sGlobalInstance;
	}


	const Song& SongDatabase::GetSong(size_t index) const
	{
		return mSongs[index];
	}


	size_t SongDatabase::GetNumSongs() const
	{
		return mSongs.size();
	}


	void SongDatabase::AddSong(Song song)
	{
		mSongs.emplace_back(std::move(song));
	}


	SongDatabase::SongDatabase()
	{

	}
}