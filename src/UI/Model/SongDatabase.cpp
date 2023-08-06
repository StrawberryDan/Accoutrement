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


	size_t SongDatabase::AddSong(Song song)
	{
		mSongs.emplace_back(std::move(song));
		return mSongs.size() - 1;
	}


	Core::Option<size_t> SongDatabase::GetSongIndex(const Song& song)
	{
		for (int i = 0; i < mSongs.size(); i++)
		{
			if (song == mSongs[i]) return i;
		}

		return Core::NullOpt;
	}


	SongDatabase::SongDatabase()
	{

	}
}