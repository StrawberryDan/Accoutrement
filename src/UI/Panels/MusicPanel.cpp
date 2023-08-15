//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MusicPanel.hpp"
// This Project
#include "../../Discord/Bot.hpp"
#include "../Model/Song.hpp"
#include "../Model/SongDatabase.hpp"
// Wx Widgets
#include "wx/button.h"
#include "wx/filedlg.h"
#include "wx/gbsizer.h"
#include "wx/msgdlg.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/textdlg.h"
// Standard Library
#include <filesystem>


namespace Strawberry::Accoutrement
{
	enum Component
	{
		AddSongToDatabaseButton = wxID_HIGHEST + 1,
		RenameSongButton,
		RemoveSongFromDatabaseButton,

		EnqueueSongToPlaylistButton,
		RemoveSongFromPlaylistButton,
	};


	wxBEGIN_EVENT_TABLE(MusicPanel, wxPanel)
		EVT_BUTTON(Component::AddSongToDatabaseButton, MusicPanel::OnAddSong)
			EVT_BUTTON(Component::EnqueueSongToPlaylistButton, MusicPanel::OnEnqueueSong)
				EVT_BUTTON(Component::RemoveSongFromDatabaseButton, MusicPanel::OnRemoveFromDatabase)
					EVT_BUTTON(Component::RemoveSongFromPlaylistButton, MusicPanel::OnRemoveSong)
						EVT_BUTTON(Component::RenameSongButton, MusicPanel::OnRenameSong)
							EVT_UPDATE_UI(wxID_ANY, MusicPanel::OnUpdate)
								wxEND_EVENT_TABLE()


									MusicPanel::MusicPanel(wxWindow* parent)
		: wxPanel(parent)
		, mEventReceiver(Bot::Get().GetPlaylist().Lock()->CreateEventReceiver())
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer(5, 5);

		sizer->Add(new wxStaticText(this, wxID_ANY, "Available Songs"), {0, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Playlist"), {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mSongDatabaseList = new wxListCtrl(this, wxID_ANY);
		mSongDatabaseList->SetWindowStyle(wxLC_LIST);
		for (int i = 0; i < SongDatabase::Get().GetNumSongs(); i++)
		{
			auto index = mSongDatabaseList->InsertItem(mSongDatabaseList->GetItemCount(), SongDatabase::Get().GetSong(i).GetTitle());
			mSongDatabaseList->SetItemPtrData(index, i);
		}
		sizer->Add(mSongDatabaseList, {1, 0}, {1, 1}, wxEXPAND | wxALL, 5);

		mPlaylistView = new wxListCtrl(this, wxID_ANY);
		mPlaylistView->SetWindowStyleFlag(wxLC_LIST);
		sizer->Add(mPlaylistView, {1, 1}, {1, 1}, wxEXPAND | wxALL, 5);

		sizer->Add(new wxTextCtrl(this, wxID_ANY), {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		auto songListButtons = new wxBoxSizer(wxHORIZONTAL);
		songListButtons->Add(new wxButton(this, Component::AddSongToDatabaseButton, "Add Song"), 0, wxALL, 5);
		songListButtons->Add(new wxButton(this, Component::EnqueueSongToPlaylistButton, "Enqueue"), 0, wxALL, 5);
		songListButtons->Add(new wxButton(this, Component::RenameSongButton, "Rename"), 0, wxALL, 5);
		songListButtons->Add(new wxButton(this, Component::RemoveSongFromDatabaseButton, "Remove"), 0, wxALL, 5);
		sizer->Add(songListButtons, {3, 0}, {1, 1}, wxALIGN_CENTER_HORIZONTAL, 5);

		auto playlistButtons = new wxGridBagSizer(5, 5);
		playlistButtons->Add(new wxButton(this, Component::RemoveSongFromPlaylistButton, "Remove"), {0, 0}, {1, 1}, wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Up"), {0, 1}, {1, 1}, wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Down"), {1, 1}, {1, 1}, wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Shuffle"), {0, 2}, {1, 1}, wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Repeat"), {1, 2}, {1, 1}, wxEXPAND | wxALL, 5);
		sizer->Add(playlistButtons, {2, 1}, {2, 1}, wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);

		SetSizerAndFit(sizer);
	}


	void MusicPanel::OnUpdate(wxUpdateUIEvent& event)
	{
		auto playlistMessage = mEventReceiver->Read();
		if (!playlistMessage) return;

		if (auto songAdded = playlistMessage->Value<Codec::Audio::Playlist::SongAddedEvent>())
		{
			Song song = std::any_cast<Song>(songAdded->associatedData);
			mPlaylistView->InsertItem(songAdded->index, song.GetTitle());
		}
		else if (auto songRemoved = playlistMessage->Value<Codec::Audio::Playlist::SongRemovedEvent>())
		{
			Core::Assert(songRemoved->index < mPlaylistView->GetItemCount());
			mPlaylistView->DeleteItem(songRemoved->index);
		}
		else if (auto songBegan = playlistMessage->Value<Codec::Audio::Playlist::SongBeganEvent>())
		{
			for (int i = 0; i < mPlaylistView->GetItemCount(); i++)
			{
				mPlaylistView->SetItemBackgroundColour(i, i == songBegan->index ? wxColor(32, 128, 32, 255) : mPlaylistView->GetBackgroundColour());
			}
		}
	}


	void MusicPanel::OnAddSong(wxCommandEvent& event)
	{
		wxFileDialog fileDialog(this, "Choose a song file...");
		fileDialog.SetWindowStyle(wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);
		auto dialogResult = fileDialog.ShowModal();
		if (dialogResult == wxID_CANCEL)
		{
			return;
		}
		else if (dialogResult == wxID_OK)
		{
			wxArrayString paths;
			fileDialog.GetPaths(paths);
			for (const auto& path : paths)
			{
				auto fullPath = std::filesystem::absolute(std::string(path));
				auto song     = Song::FromFile(fullPath);
				if (!song) return;

				auto        songIndex = SongDatabase::Get().AddSong(song.Value());
				std::string title     = song.Value().GetTitle();
				auto        index     = mSongDatabaseList->InsertItem(mSongDatabaseList->GetItemCount(), title);
				mSongDatabaseList->SetItemPtrData(index, songIndex);
			}
		}
	}


	void MusicPanel::OnEnqueueSong(wxCommandEvent& event)
	{
		size_t selectedSongIndex = mSongDatabaseList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (selectedSongIndex == -1)
			return;

		wxListItem selectedSongItem;
		selectedSongItem.SetId(selectedSongIndex);
		if (!mSongDatabaseList->GetItem(selectedSongItem))
			return;

		selectedSongIndex = selectedSongItem.GetData();

		auto playlist     = Bot::Get().GetPlaylist().Lock();
		auto song         = SongDatabase::Get().GetSong(selectedSongIndex);
		playlist->EnqueueFile(song.GetPath(), song).Unwrap();
	}


	void MusicPanel::OnRemoveSong(wxCommandEvent& event)
	{
		auto index = mPlaylistView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (index != -1)
		{
			Bot::Get().GetPlaylist().Lock()->RemoveTrack(index);
		}
	}


	void MusicPanel::OnRenameSong(wxCommandEvent& event)
	{
		long item = mSongDatabaseList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (item != -1)
		{
			auto  songID    = mSongDatabaseList->GetItemData(item);
			auto& song      = SongDatabase::Get().GetSong(songID);
			auto  strDialog = new wxTextEntryDialog(this, "Choose a new name:");
			strDialog->SetValue(song.GetTitle());
			if (strDialog->ShowModal() == wxID_OK)
			{
				std::string title(strDialog->GetValue());

				song.SetTitle(title);
				mSongDatabaseList->SetItem(item, 0, title);

				auto playlist = Bot::Get().GetPlaylist().Lock();
				for (int i = 0; i < playlist->Length(); i++)
				{
					Song playlistSong = playlist->GetTrackAssociatedData<Song>(i);
					if (song.GetPath() == playlistSong.GetPath())
					{
						playlistSong.SetTitle(title);
						playlist->SetTrackAssociatedData(i, playlistSong);
						mPlaylistView->SetItem(i, 0, title);
					}
				}

				Layout();
			}
		}
	}


	void MusicPanel::OnRemoveFromDatabase(wxCommandEvent& event)
	{
		wxMessageDialog* mCheckDialog = new wxMessageDialog(this, "Are you Sure?");
		mCheckDialog->SetMessageDialogStyle(wxOK | wxCANCEL);

		auto result = mCheckDialog->ShowModal();
		if (result == wxID_OK)
		{
			auto index = mSongDatabaseList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (index != -1)
			{
				SongDatabase::Get().RemoveSong(index);
				mSongDatabaseList->DeleteItem(index);
			}
		}
	}
}// namespace Strawberry::Accoutrement