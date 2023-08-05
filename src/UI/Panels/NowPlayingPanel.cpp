//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "NowPlayingPanel.hpp"
// wxWidgets
#include "wx/gbsizer.h"


//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	NowPlayingPanel::NowPlayingPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer(5, 5);

		mSongTitle = new wxStaticText(this, wxID_ANY, "No Song Playing");
		sizer->Add(mSongTitle, {0, 0}, {1, 3}, wxALL | wxALIGN_CENTER, 5);

		mPrevSongButton = new wxButton(this, wxID_ANY, "Previous Song");
		sizer->Add(mPrevSongButton, {1, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mPlayPauseButton = new wxButton(this, wxID_ANY, "Play");
		sizer->Add(mPlayPauseButton, {1, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mNextSongButton = new wxButton(this, wxID_ANY, "Next Song");
		sizer->Add(mNextSongButton, {1, 2}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		SetSizerAndFit(sizer);
	}
}