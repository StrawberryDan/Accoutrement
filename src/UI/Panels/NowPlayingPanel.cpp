//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "NowPlayingPanel.hpp"
// wxWidgets
#include "wx/gbsizer.h"
#include "../../Discord/Bot.hpp"


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


	wxBEGIN_EVENT_TABLE(NowPlayingPanel, wxPanel)
		EVT_UPDATE_UI(wxID_ANY, NowPlayingPanel::Update)
		EVT_BUTTON(Component::NextSongButton, NowPlayingPanel::NextSong)
		EVT_BUTTON(Component::PrevSongButton, NowPlayingPanel::PrevSong)
	wxEND_EVENT_TABLE()


	NowPlayingPanel::NowPlayingPanel(wxWindow* parent)
		: wxPanel(parent)
		, mEventReceiver(Bot::Get().GetPlaylist().CreateEventReceiver())
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer(5, 5);

		mSongTitle = new wxStaticText(this, wxID_ANY, "No Song Playing");
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
	}


	void NowPlayingPanel::Update(wxUpdateUIEvent& event)
	{
		while (true)
		{
			auto mMessage = mEventReceiver->Read();
			if (!mMessage) return;

			if (auto songChanged = mMessage->Value<Codec::Audio::Playlist::SongChangedEvent>())
			{
				mSongTitle->SetLabelText(songChanged->newSongTitle.ValueOr(songChanged->newSongPath));
			}
		}
	}


	void NowPlayingPanel::NextSong(wxCommandEvent& event)
	{
		Bot::Get().GetPlaylist().GotoNextTrack();
	}


	void NowPlayingPanel::PrevSong(wxCommandEvent& event)
	{
		Bot::Get().GetPlaylist().GotoPrevTrack();
	}
}