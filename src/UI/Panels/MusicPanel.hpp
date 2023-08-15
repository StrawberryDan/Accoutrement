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
	{
	wxDECLARE_EVENT_TABLE();


	public:
		explicit MusicPanel(wxWindow* parent);


	protected:
		void OnUpdate(wxUpdateUIEvent& event);
		void OnAddSong(wxCommandEvent& event);
		void OnEnqueueSong(wxCommandEvent& event);
		void OnRemoveSong(wxCommandEvent& event);
		void OnRenameSong(wxCommandEvent& event);
		void OnRemoveFromDatabase(wxCommandEvent& event);


	protected:
		wxListCtrl* mSongDatabaseList;
		wxListCtrl* mPlaylistView;
		Codec::Audio::Playlist::EventReceiver mEventReceiver;
	};
}
