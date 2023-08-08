#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include <Strawberry/Core/IO/ChannelReceiver.hpp>
#include <Codec/Audio/Playlist.hpp>
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
		void Update(wxUpdateUIEvent& event);
		void NextSong(wxCommandEvent& event);
		void PrevSong(wxCommandEvent& event);


	private:
		wxStaticText* mSongTitle;
		wxButton*     mPlayPauseButton;
		wxButton*     mPrevSongButton;
		wxButton*     mNextSongButton;


		std::shared_ptr<Core::IO::ChannelReceiver<Codec::Audio::Playlist::Event>> mEventReceiver;
	};
}