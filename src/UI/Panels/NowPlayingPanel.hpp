#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include "wx/panel.h"
#include "wx/button.h"
#include "wx/stattext.h"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class NowPlayingPanel
		: public wxPanel
	{
		wxDECLARE_EVENT_TABLE();
	public:
		NowPlayingPanel(wxWindow* parent);



	protected:
		void NextSong(wxCommandEvent& event);
		void PrevSong(wxCommandEvent& event);


	private:
		wxStaticText* mSongTitle;
		wxButton*     mPlayPauseButton;
		wxButton*     mPrevSongButton;
		wxButton*     mNextSongButton;
	};
}