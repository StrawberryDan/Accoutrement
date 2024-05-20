//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MusicPanel.hpp"
// This Project
#include "../../Discord/Bot.hpp"
#include "../../Model/Song.hpp"
#include "../../Model/SongDatabase.hpp"
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

		ToggleRepeatSongButton,

		SongSearchBar,
	};

	// clang-format off
	wxBEGIN_EVENT_TABLE(MusicPanel, wxPanel)
					EVT_BUTTON(Component::AddSongToDatabaseButton, MusicPanel::OnAddSong)
					EVT_BUTTON(Component::EnqueueSongToPlaylistButton, MusicPanel::OnEnqueueSong)
					EVT_BUTTON(Component::RemoveSongFromDatabaseButton, MusicPanel::OnRemoveFromDatabase)
					EVT_BUTTON(Component::RemoveSongFromPlaylistButton, MusicPanel::OnRemoveSong)
					EVT_BUTTON(Component::RenameSongButton, MusicPanel::OnRenameSong)
					EVT_BUTTON(Component::ToggleRepeatSongButton, MusicPanel::OnToggleRepeatSong)
					EVT_TEXT(Component::SongSearchBar, MusicPanel::OnSongSearchBarText)
	wxEND_EVENT_TABLE();

	// clang-format on


	MusicPanel::MusicPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxGridBagSizer();

		sizer->Add(new wxStaticText(this, wxID_ANY, "Available Songs"), {0, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Playlist"), {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mSongDatabaseList = new wxListCtrl(this, wxID_ANY);
		mSongDatabaseList->SetWindowStyle(wxLC_LIST);
		sizer->Add(mSongDatabaseList, {1, 0}, {1, 1}, wxEXPAND | wxALL, 5);

		wxImage playingIcon("data/playing.png");
		playingIcon = playingIcon.Rescale(10, 10);
		wxImage repeatIcon("data/repeat.png");
		repeatIcon      = repeatIcon.Rescale(10, 10);
		auto* imageList = new wxImageList(10, 10);
		imageList->Add(playingIcon);
		imageList->Add(repeatIcon);

		mPlaylistView = new wxListCtrl(this, wxID_ANY);
		mPlaylistView->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
		mPlaylistView->SetWindowStyleFlag(wxLC_REPORT | wxLC_NO_HEADER);
		mPlaylistView->InsertColumn(0, "");
		mPlaylistView->InsertColumn(1, "");
		mPlaylistView->InsertColumn(2, "");
		mPlaylistView->SetColumnWidth(0, wxLIST_AUTOSIZE);
		mPlaylistView->SetColumnWidth(1, wxLIST_AUTOSIZE);
		sizer->Add(mPlaylistView, {1, 1}, {1, 1}, wxEXPAND | wxALL, 5);

		mSearchBar = new wxTextCtrl(this, Component::SongSearchBar);
		sizer->Add(mSearchBar, {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

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
		playlistButtons->Add(new wxButton(this, ToggleRepeatSongButton, "Repeat"), {1, 2}, {1, 1}, wxEXPAND | wxALL, 5);
		sizer->Add(playlistButtons, {2, 1}, {2, 1}, wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);

		SetSizerAndFit(sizer);

		Bot::Get()->GetPlaylist().Lock()->Register(this);
		mSearchTreeNavigator = SongDatabaseListContentsManager(mSongDatabaseList, mSearchBar);
	}

	void MusicPanel::OnAddSong(wxCommandEvent& event)
	{
		wxFileDialog fileDialog(this, "Choose a song file...");
		fileDialog.SetWindowStyle(wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);
		auto dialogResult = fileDialog.ShowModal();
		if (dialogResult == wxID_CANCEL) { return; }
		else if (dialogResult == wxID_OK)
		{
			wxArrayString paths;
			fileDialog.GetPaths(paths);
			for (const auto& path : paths)
			{
				std::filesystem::path fullPath(path.ToUTF8().data());

				auto song     = Song::FromFile(fullPath);
				if (!song) return;

				auto        songIndex = SongDatabase::Get().AddSong(song.Value());
				mSearchTreeNavigator.AddSong(songIndex);
				mSearchTreeNavigator.Update();
			}
		}
		Refresh();
	}

	void MusicPanel::OnEnqueueSong(wxCommandEvent& event)
	{
		if (Bot::Get())
		{
			size_t selectedSongIndex = mSongDatabaseList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (selectedSongIndex == -1) return;

			wxListItem selectedSongItem;
			selectedSongItem.SetId(selectedSongIndex);
			if (!mSongDatabaseList->GetItem(selectedSongItem)) return;

			selectedSongIndex = selectedSongItem.GetData();

			auto playlist     = Bot::Get()->GetPlaylist().Lock();
			auto song         = SongDatabase::Get().GetSong(selectedSongIndex);
			playlist->EnqueueFile(song.GetPath(), song).Unwrap();
		}
		Refresh();
	}

	void MusicPanel::OnRemoveSong(wxCommandEvent& event)
	{
		if (Bot::Get())
		{
			auto index = mPlaylistView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (index != -1)
			{
				Bot::Get()->GetPlaylist().Lock()->RemoveTrack(index);
			}
		}
		Refresh();
	}

	void MusicPanel::OnRenameSong(wxCommandEvent& event)
	{
		if (Bot::Get())
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

					auto playlist = Bot::Get()->GetPlaylist().Lock();
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
		Refresh();
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
				mSearchTreeNavigator.RemoveSong(index);
				mSearchTreeNavigator.Update();
			}
		}
		Refresh();
	}

	void MusicPanel::OnToggleRepeatSong(wxCommandEvent& event)
	{
		auto selected = mPlaylistView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (selected == -1) return;

		auto       playlist  = Bot::Get()->GetPlaylist().Lock();
		const bool repeating = !playlist->GetTrackRepeating(selected);
		playlist->SetTrackRepeating(selected, repeating);

		mPlaylistView->SetItemColumnImage(selected, 2, repeating ? 1 : -1);
	}


	void MusicPanel::OnSongSearchBarText(wxCommandEvent& event)
	{
		mSearchTreeNavigator.Update();
	}


	void MusicPanel::Receive(Codec::Audio::Playlist::SongBeganEvent event)
	{
		for (int i = 0; i < mPlaylistView->GetItemCount(); i++) { mPlaylistView->SetItemColumnImage(i, 0, i == event.index ? 0 : -1); }

		mPlaylistView->SetColumnWidth(0, wxLIST_AUTOSIZE);
		mPlaylistView->SetColumnWidth(1, wxLIST_AUTOSIZE);
		Refresh();
	}

	void MusicPanel::Receive(Codec::Audio::Playlist::SongAddedEvent event)
	{
		Song song = std::any_cast<Song>(event.associatedData);

		mPlaylistView->InsertItem(mPlaylistView->GetItemCount(), "");\
		mPlaylistView->SetItemColumnImage(mPlaylistView->GetItemCount() - 1, 0, -1);
		mPlaylistView->SetItem(mPlaylistView->GetItemCount() - 1, 1, song.GetTitle());

		mPlaylistView->SetColumnWidth(0, wxLIST_AUTOSIZE);
		mPlaylistView->SetColumnWidth(1, wxLIST_AUTOSIZE);
		Refresh();
	}

	void MusicPanel::Receive(Codec::Audio::Playlist::SongRemovedEvent event)
	{
		Core::Assert(event.index < mPlaylistView->GetItemCount());
		mPlaylistView->DeleteItem(event.index);

		mPlaylistView->SetColumnWidth(0, wxLIST_AUTOSIZE);
		mPlaylistView->SetColumnWidth(1, wxLIST_AUTOSIZE);
		Refresh();
	}

	void MusicPanel::Receive(BotStartedRunningEvent value)
	{
		Bot::Get()->GetPlaylist().Lock()->Register(this);
	}

	void MusicPanel::Receive(BotStoppedRunningEvent value)
	{
		if (Bot::Get()) { Bot::Get()->GetPlaylist().Lock()->Unregister(this); }
	}

	void MusicPanel::Receive(Codec::Audio::Playlist::PlaybackEndedEvent value) {}

} // namespace Strawberry::Accoutrement