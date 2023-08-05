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
	public:
		NowPlayingPanel(wxWindow* parent);


	private:
		wxStaticText* mSongTitle;
		wxButton*     mPlayPauseButton;
		wxButton*     mPrevSongButton;
		wxButton*     mNextSongButton;
	};
}