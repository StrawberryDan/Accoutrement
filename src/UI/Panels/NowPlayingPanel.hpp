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
		, public Codec::Audio::Playlist::EventReceiver
	{
		wxDECLARE_EVENT_TABLE();

	public:
		explicit NowPlayingPanel(wxWindow* parent);


	protected:
		void NextSong(wxCommandEvent& event);
		void PrevSong(wxCommandEvent& event);


		virtual void Receive(Codec::Audio::Playlist::SongBeganEvent event) override;
		virtual void Receive(Codec::Audio::Playlist::PlaybackEndedEvent event) override;


	private:
		wxStaticText* mSongTitle;
		wxButton*     mPlayPauseButton;
		wxButton*     mPrevSongButton;
		wxButton*     mNextSongButton;
	};
} // namespace Strawberry::Accoutrement