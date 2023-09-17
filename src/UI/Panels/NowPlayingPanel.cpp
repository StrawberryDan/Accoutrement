//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "NowPlayingPanel.hpp"
// Accoutrement
#include "../../Discord/Bot.hpp"
#include "../../Model/Song.hpp"
// wxWidgets
#include "wx/gbsizer.h"

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	enum Component
	{
		NextSongButton = wxID_HIGHEST + 1,
		PrevSongButton,
	};

	// clang-format off
	wxBEGIN_EVENT_TABLE(NowPlayingPanel, wxPanel)
	EVT_BUTTON(Component::NextSongButton, NowPlayingPanel::NextSong)
	EVT_BUTTON(Component::PrevSongButton, NowPlayingPanel::PrevSong)
	wxEND_EVENT_TABLE();

	// clang-format on


	NowPlayingPanel::NowPlayingPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer();

		mSongTitle = new wxStaticText(this, wxID_ANY, "No Song Playing");
		mSongTitle->SetWindowStyle(wxALIGN_CENTRE_HORIZONTAL);
		sizer->Add(mSongTitle, {0, 0}, {1, 3}, wxALL | wxALIGN_CENTER, 5);

		mPrevSongButton = new wxButton(this, Component::PrevSongButton, "Previous Song");
		sizer->Add(mPrevSongButton, {1, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		mPlayPauseButton = new wxButton(this, wxID_ANY, "Play");
		sizer->Add(mPlayPauseButton, {1, 1}, {1, 1}, wxALL | wxEXPAND, 5);

		mNextSongButton = new wxButton(this, Component::NextSongButton, "Next Song");
		sizer->Add(mNextSongButton, {1, 2}, {1, 1}, wxALL | wxEXPAND, 5);

		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);
		sizer->AddGrowableCol(2, 1);
		sizer->AddGrowableRow(0, 1);
		SetSizerAndFit(sizer);

		Bot::Get()->GetPlaylist().Lock()->Register(this);
	}

	void NowPlayingPanel::NextSong(wxCommandEvent& event)
	{
		if (Bot::Get()) Bot::Get()->GetPlaylist().Lock()->GotoNextTrack();
	}

	void NowPlayingPanel::PrevSong(wxCommandEvent& event)
	{
		if (Bot::Get()) Bot::Get()->GetPlaylist().Lock()->GotoPrevTrack();
	}

	void NowPlayingPanel::Receive(Codec::Audio::Playlist::SongBeganEvent event)
	{
		Song song = std::any_cast<Song>(event.associatedData);
		mSongTitle->SetLabelText(song.GetTitle());
		Layout();
	}

	void NowPlayingPanel::Receive(Codec::Audio::Playlist::PlaybackEndedEvent event)
	{
		mSongTitle->SetLabelText("No Song Playing");
		Layout();
	}

	void NowPlayingPanel::Receive(BotStartedRunningEvent event)
	{
		Bot::Get()->GetPlaylist().Lock()->Register(this);
	}

	void NowPlayingPanel::Receive(BotStoppedRunningEvent event)
	{
		Bot::Get()->GetPlaylist().Lock()->Unregister(this);
	}
} // namespace Strawberry::Accoutrement