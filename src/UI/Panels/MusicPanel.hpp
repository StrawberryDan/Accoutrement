#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// WxWidgets
#include "wx/listctrl.h"
#include "wx/panel.h"
// Codec
#include "Codec/Audio/Playlist.hpp"

namespace Strawberry::Accoutrement

{
	class MusicPanel
		: public wxPanel
		, public Codec::Audio::Playlist::EventReceiver
	{
		wxDECLARE_EVENT_TABLE();


	public:
		explicit MusicPanel(wxWindow* parent);


	protected:
		void OnAddSong(wxCommandEvent& event);
		void OnEnqueueSong(wxCommandEvent& event);
		void OnRemoveSong(wxCommandEvent& event);
		void OnRenameSong(wxCommandEvent& event);
		void OnRemoveFromDatabase(wxCommandEvent& event);

		virtual void Receive(Codec::Audio::Playlist::SongBeganEvent event) override;
		virtual void Receive(Codec::Audio::Playlist::SongAddedEvent event) override;
		virtual void Receive(Codec::Audio::Playlist::SongRemovedEvent event) override;


	protected:
		wxListCtrl* mSongDatabaseList;
		wxListCtrl* mPlaylistView;
	};
} // namespace Strawberry::Accoutrement
