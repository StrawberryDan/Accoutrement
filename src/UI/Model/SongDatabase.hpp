#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Song.hpp"
// Standard Library
#include <cstdint>
#include <map>
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
		~SongDatabase();


		[[nodiscard]] const Song& GetSong(size_t index) const;
		Song&                     GetSong(size_t index);
		[[nodiscard]] size_t      GetNumSongs() const;

		size_t               AddSong(Song song);
		Core::Option<size_t> GetSongIndex(const Song& song);
		void                 RemoveSong(size_t index);


	private:
		size_t                 mNextSongId = 0;
		std::map<size_t, Song> mSongs;


	private:
		SongDatabase();
	};
}// namespace Strawberry::Accoutrement