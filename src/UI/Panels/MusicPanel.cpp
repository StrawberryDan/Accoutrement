//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MusicPanel.hpp"
// This Project
#include "../Model/Song.hpp"
#include "../Model/SongDatabase.hpp"
#include "../../Discord/Bot.hpp"
// Wx Widgets
#include "wx/button.h"
#include "wx/listbox.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/gbsizer.h"
#include "wx/filedlg.h"
// Standard Library
#include <filesystem>


namespace Strawberry::Accoutrement
{
	wxBEGIN_EVENT_TABLE(MusicPanel, wxPanel)
			EVT_UPDATE_UI(wxID_ANY, MusicPanel::OnUpdate)
			EVT_BUTTON(Component::AddSongButton, MusicPanel::OnAddSong)
			EVT_BUTTON(Component::EnqueueSongButton, MusicPanel::OnEnqueueSong)
			EVT_BUTTON(Component::RemoveSongButton, MusicPanel::OnRemoveSong)
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
			auto index = mSongDatabaseList->InsertItem(mSongDatabaseList->GetItemCount(),
													   SongDatabase::Get().GetSong(i).GetTitle());
			mSongDatabaseList->SetItemPtrData(index, i);
		}
		sizer->Add(mSongDatabaseList, {1, 0}, {1, 1}, wxEXPAND | wxALL, 5);

		mPlaylistView = new wxListCtrl(this, wxID_ANY);
		mPlaylistView->SetWindowStyleFlag(wxLC_LIST);
		sizer->Add(mPlaylistView, {1, 1}, {1, 1}, wxEXPAND | wxALL, 5);

		sizer->Add(new wxTextCtrl(this, wxID_ANY), {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		auto songListButtons = new wxBoxSizer(wxHORIZONTAL);
		songListButtons->Add(new wxButton(this, Component::AddSongButton, "Add Song"), 0, wxALL, 5);
		songListButtons->Add(new wxButton(this, Component::EnqueueSongButton, "Enqueue"), 0, wxALL, 5);
		sizer->Add(songListButtons, {3, 0}, {1, 1}, wxALIGN_CENTER_HORIZONTAL, 5);

		auto playlistButtons = new wxGridBagSizer(5, 5);
		playlistButtons->Add(new wxButton(this, Component::RemoveSongButton, "Remove"), {0, 0}, {1, 1},
							 wxEXPAND | wxALL, 5);
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
			mPlaylistView->InsertItem(songAdded->index, songAdded->title.ValueOr(songAdded->path));
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
			for (const auto& path: paths)
			{
				auto fullPath = std::filesystem::absolute(std::string(path));
				auto song = Song::FromFile(fullPath);
				if (!song) return;

				auto songIndex = SongDatabase::Get().AddSong(song.Value());
				std::string title = song.Value().GetTitle();
				auto index = mSongDatabaseList->InsertItem(mSongDatabaseList->GetItemCount(), title);
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

		auto& playlist = Bot::Get().GetPlaylist();
		auto song = SongDatabase::Get().GetSong(selectedSongIndex);
		playlist.Lock()->EnqueueFile(song.GetPath());
	}


	void MusicPanel::OnRemoveSong(wxCommandEvent& event)
	{
		auto index = mPlaylistView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (index != -1)
		{
			Bot::Get().GetPlaylist().Lock()->RemoveTrack(index);
		}
	}
}