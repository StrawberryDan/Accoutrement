#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include "wx/button.h"
#include "wx/panel.h"
#include "wx/stattext.h"
#include <Codec/Audio/Playlist.hpp>
#include <Strawberry/Core/IO/ChannelReceiver.hpp>


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
		explicit NowPlayingPanel(wxWindow* parent);


	protected:
		void Update(wxUpdateUIEvent& event);
		void NextSong(wxCommandEvent& event);
		void PrevSong(wxCommandEvent& event);


	private:
		wxStaticText* mSongTitle;
		wxButton*     mPlayPauseButton;
		wxButton*     mPrevSongButton;
		wxButton*     mNextSongButton;


		Codec::Audio::Playlist::EventReceiver mEventReceiver;
	};
} // namespace Strawberry::Accoutrement