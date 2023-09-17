#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "../Events/BotStartedRunningEvent.hpp"
#include "../Events/BotStoppedRunningEvent.hpp"
// WxWidgets
#include "wx/listctrl.h"
#include "wx/panel.h"
// Codec
#include "Codec/Audio/Playlist.hpp"
// Core
#include "Strawberry/Core/IO/Receiver.hpp"

namespace Strawberry::Accoutrement

{
	class MusicPanel
		: public wxPanel
		, public Codec::Audio::Playlist::EventReceiver
		, public Core::IO::Receiver<BotStartedRunningEvent, BotStoppedRunningEvent>
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

		void Receive(Codec::Audio::Playlist::SongBeganEvent event) override;
		void Receive(Codec::Audio::Playlist::SongAddedEvent event) override;
		void Receive(Codec::Audio::Playlist::SongRemovedEvent event) override;
		void Receive(BotStartedRunningEvent value) override;
		void Receive(BotStoppedRunningEvent value) override;


	protected:
		wxListCtrl* mSongDatabaseList;
		wxListCtrl* mPlaylistView;
	};
} // namespace Strawberry::Accoutrement
