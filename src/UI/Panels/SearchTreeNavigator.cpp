#include "SongDatabaseListContentsManager.hpp"

#include <ranges>
#include "../../Model/SongDatabase.hpp"


namespace Strawberry::Accoutrement
{
    SongDatabaseListContentsManager::SongDatabaseListContentsManager(wxListCtrl* songList, wxTextCtrl* searchBar)
        : mSongList(songList)
        , mSearchBar(searchBar)
        , mLastStringValue("")
    {
        mSearchBuffer.emplace_back("", SongDatabase::Get().GetSongIndices());
        Update();
    }


    void SongDatabaseListContentsManager::Update()
    {
        StringType newString  = mSearchBar->GetValue();
        auto       difference = GetStringDifference(mLastStringValue, newString);

        std::set<size_t> filteredSongs;
        if (newString.empty())
        {
            filteredSongs = SongDatabase::Get().GetSongIndices();
            mSearchBuffer.resize(1);
        }
        else if (difference > 0)
        {
            auto suffix = newString.substr(difference - 1);
            Extend(suffix);
            filteredSongs = mSearchBuffer.rbegin()->second;
        }
        else if (difference < 0)
        {
            mSearchBuffer.resize(newString.length() + 1);
            filteredSongs = mSearchBuffer.rbegin()->second;
        }
        else
        {
            filteredSongs = mSearchBuffer.rbegin()->second;
        }


        std::set<size_t> toAdd = filteredSongs;
        std::set<size_t> toRemove;
        for (int i = 0; i < mSongList->GetItemCount(); i++)
        {
            auto songId = mSongList->GetItemData(i);
            if (filteredSongs.contains(songId))
            {
                toAdd.erase(songId);
            }
            else
            {
                toRemove.emplace(i);
            }
        }


        for (auto x: std::ranges::reverse_view(toRemove)) mSongList->DeleteItem(x);
        for (auto x: toAdd)
        {
            auto index = mSongList->InsertItem(mSongList->GetItemCount(), SongDatabase::Get().GetSong(x).GetTitle());
            mSongList->SetItemData(index, x);
        }

        mLastStringValue = newString;
        mSongList->Refresh();
    }


    void SongDatabaseListContentsManager::AddSong(size_t songIndex)
    {
        auto& song = SongDatabase::Get().GetSong(songIndex);
        mSearchBuffer[0].second.emplace(songIndex);

        for (int i = 1; i < mSearchBuffer.size(); i++)
        {
            const bool matchesTitle  = song.GetTitle().find(mSearchBuffer[i].first) != StringType::npos;
            const bool matchesArtist = song.GetArtist().HasValue() && song.GetArtist().Value().find(
                mSearchBuffer[i].first) != StringType::npos;
            if (matchesTitle || matchesArtist)
            {
                mSearchBuffer[i].second.emplace(songIndex);
            }
            else
            {
                break;
            }
        }
    }


    void SongDatabaseListContentsManager::RemoveSong(size_t songIndex)
    {
        for (auto& [searchTerm, candidates]: mSearchBuffer)
        {
            bool found = candidates.contains(songIndex);
            if (!found)
            {
                return;
            }

            candidates.erase(songIndex);
        }
    }


    int32_t SongDatabaseListContentsManager::GetStringDifference(SongDatabaseListContentsManager::StringType& a,
                                                                 SongDatabaseListContentsManager::StringType& b)
    {
        StringType* larger;
        StringType* smaller;
        if (a.Len() > b.Len())
        {
            larger  = &a;
            smaller = &b;
        }
        else
        {
            larger  = &b;
            smaller = &b;
        }


        auto prefixLength = GetLongestPrefixLength(*larger, *smaller);


        if (larger == &a)
        {
            return prefixLength - a.size();
        }
        else
        {
            return prefixLength;
        }
    }


    int32_t SongDatabaseListContentsManager::GetLongestPrefixLength(SongDatabaseListContentsManager::StringType& a,
                                                                    SongDatabaseListContentsManager::StringType& b)
    {
        uint32_t prefixLength = 0;
        for (int i = 0; i < std::min(a.Len(), b.Len()); i++)
        {
            if (a[i] == b[i])
            {
                prefixLength += 1;
            }
            else
            {
                return prefixLength;
            }
        }
        return prefixLength;
    }


    void SongDatabaseListContentsManager::Extend(SongDatabaseListContentsManager::StringType suffix)
    {
        auto searchTerm = mSearchBuffer.rbegin()->first;
        auto candidates = mSearchBuffer.rbegin()->second;


        while (!suffix.empty())
        {
            searchTerm.Append(suffix[0]);
            suffix.Remove(0);

            std::set<size_t> toRemove;

            for (auto candidate: candidates)
            {
                Song song   = SongDatabase::Get().GetSong(candidate);
                auto title  = song.GetTitle();
                auto artist = song.GetArtist();

                const bool matchesTitle  = title.find(searchTerm) != StringType::npos;
                const bool matchesArtist = artist.HasValue() && artist.Value().find(searchTerm) != StringType::npos;
                if (!matchesTitle && !matchesArtist)
                {
                    toRemove.emplace(candidate);
                }
            }

            for (auto x: toRemove) candidates.erase(x);

            mSearchBuffer.emplace_back(searchTerm, candidates);
        }
    }
}
