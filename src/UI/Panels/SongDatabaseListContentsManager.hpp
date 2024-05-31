#pragma once


//======================================================================================================================
//	Includes
//======================================================================================================================
#include "wx/listctrl.h"
#include "wx/textctrl.h"
#include <cstdint>
#include <set>


//======================================================================================================================
//	Class Declaration
//======================================================================================================================
namespace Strawberry::Accoutrement
{
    class SongDatabaseListContentsManager
    {
        public:
            using StringType = wxString;


            SongDatabaseListContentsManager() = default;
            SongDatabaseListContentsManager(wxListCtrl* songList, wxTextCtrl* searchBar);


            void Update();


            void AddSong(size_t songIndex);
            void RemoveSong(size_t songIndex);

        protected:
            static int32_t GetStringDifference(StringType& a, StringType& b);
            static int32_t GetLongestPrefixLength(StringType& a, StringType& b);


            void Extend(StringType suffix);

        private:
            wxListCtrl* mSongList;
            wxTextCtrl* mSearchBar;


            wxString mLastStringValue;


            std::vector<std::pair<wxString, std::set<size_t> > > mSearchBuffer;
    };
}
