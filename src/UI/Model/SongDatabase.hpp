#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Song.hpp"
// Standard Library
#include <vector>
#include <cstdint>
#include <memory>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class SongDatabase
	{
	public:
		static SongDatabase& Get();


	private:
		static std::unique_ptr<SongDatabase> sGlobalInstance;


	public:
		const Song& GetSong(size_t index) const;
		size_t GetNumSongs() const;

		size_t               AddSong(Song song);
		Core::Option<size_t> GetSongIndex(const Song& song);


	private:
		std::vector<Song> mSongs;


	private:
		SongDatabase();
	};
}