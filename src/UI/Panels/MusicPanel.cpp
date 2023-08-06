//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MusicPanel.hpp"
// This Project
#include "../Model/Song.hpp"
#include "../Model/SongDatabase.hpp"
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
	class SongClientData
		: public wxClientData
	{
	public:
		SongClientData(Song song)
			: mSong(std::move(song))
		{}

		~SongClientData()
		{
			Core::DebugBreak();
		}


		Song GetSong() const { return mSong; }


	private:
		const Song mSong;


	};


	wxBEGIN_EVENT_TABLE(MusicPanel, wxPanel)
		EVT_BUTTON(Component::AddSongButton, MusicPanel::OnAddSong)
	wxEND_EVENT_TABLE()


	MusicPanel::MusicPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer(5, 5);

		sizer->Add(new wxStaticText(this, wxID_ANY, "Available Songs"), {0, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Playlist"), {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mSongDatabaseList = new wxListCtrl(this, wxID_ANY);
		for (int i = 0; i < SongDatabase::Get().GetNumSongs(); i++)
		{
			mSongDatabaseList->InsertItem(mSongDatabaseList->GetItemCount(), SongDatabase::Get().GetSong(i).GetTitle());
		}
		sizer->Add(mSongDatabaseList, {1, 0}, {1, 1}, wxEXPAND | wxALL, 5);

		sizer->Add(new wxListCtrl(this, wxID_ANY), {1, 1}, {1, 1}, wxEXPAND | wxALL, 5);

		sizer->Add(new wxTextCtrl(this, wxID_ANY), {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		auto songListButtons = new wxBoxSizer(wxHORIZONTAL);
		songListButtons->Add(new wxButton(this, Component::AddSongButton, "Add Song"), 0, wxALL, 5);
		songListButtons->Add(new wxButton(this, wxID_ANY, "Enqueue"), 0,  wxALL, 5);
		sizer->Add(songListButtons, {3, 0}, {1, 1}, wxALIGN_CENTER_HORIZONTAL, 5);

		auto playlistButtons = new wxGridBagSizer(5, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Remove"), {0, 0}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Up"), {0, 1}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Down"), {1, 1}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Shuffle"), {0, 2}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Repeat"), {1, 2}, {1, 1},  wxEXPAND | wxALL, 5);
		sizer->Add(playlistButtons, {2, 1}, {2, 1}, wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);

		SetSizerAndFit(sizer);
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
				auto song = Song::FromFile(fullPath);
				if (!song) return;

				SongDatabase::Get().AddSong(song.Value());
				std::string title = song.Value().GetTitle();
				auto index = mSongDatabaseList->InsertItem(mSongDatabaseList->GetItemCount(), title);
			}
		}
	}
}