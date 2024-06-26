//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SongDatabase.hpp"
// Standard Library
#include <fstream>
#include <string>


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


    SongDatabase::~SongDatabase()
    {
        nlohmann::json json;
        for (const auto& [index, song]: mSongs)
        {
            json["songs"].push_back(song.ToJSON());
        }


        std::ofstream file("./song_database.json");
        file << json.dump('\t');
    }


    const Song& SongDatabase::GetSong(size_t index) const
    {
        return mSongs.at(index);
    }


    Song& SongDatabase::GetSong(size_t index)
    {
        return mSongs.at(index);
    }


    size_t SongDatabase::GetNumSongs() const
    {
        return mSongs.size();
    }


    std::set<size_t> SongDatabase::GetSongIndices() const
    {
        std::set<size_t> indices;

        for (auto& [key, val]: mSongs)
        {
            indices.emplace(key);
        }

        return indices;
    }


    size_t SongDatabase::AddSong(Song song)
    {
        auto id = mSongIDGenerator.Allocate();
        mSongs.emplace(id, std::move(song));
        return id;
    }


    Core::Optional<size_t> SongDatabase::GetSongIndex(const Song& song)
    {
        for (int i = 0; i < mSongs.size(); i++)
        {
            if (song == mSongs.at(i)) return i;
        }

        return Core::NullOpt;
    }


    void SongDatabase::RemoveSong(size_t index)
    {
        mSongs.erase(index);
        mSongIDGenerator.Free(index);
    }


    SongDatabase::SongDatabase()
    {
        std::ifstream file("./song_database.json");

        if (file.is_open())
        {
            nlohmann::json json;
            file >> json;

            for (auto songData: json["songs"])
            {
                std::string title = songData["title"];
                auto        song  = Song::FromFile(songData["path"]);
                if (song)
                {
                    song->SetTitle(title);
                    AddSong(song.Value());
                }
            }
        }
    }
} // namespace Strawberry::Accoutrement
