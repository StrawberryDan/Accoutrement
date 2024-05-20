#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Song.hpp"
#include "Strawberry/Core/Util/IDPool.hpp"
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


		[[nodiscard]] const Song&      GetSong(size_t index) const;
		Song&                          GetSong(size_t index);
		[[nodiscard]] size_t           GetNumSongs() const;
		[[nodiscard]] std::set<size_t> GetSongIndices() const;

		size_t                 AddSong(Song song);
		Core::Optional<size_t> GetSongIndex(const Song& song);
		void                   RemoveSong(size_t index);


	private:
		Core::IDPool<size_t>   mSongIDGenerator;
		std::map<size_t, Song> mSongs;


	private:
		SongDatabase();
	};
} // namespace Strawberry::Accoutrement