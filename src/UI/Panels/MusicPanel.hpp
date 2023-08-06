#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// WxWidgets
#include "wx/listctrl.h"
#include "wx/panel.h"


namespace Strawberry::Accoutrement
{
	class MusicPanel
			: public wxPanel
	{
		wxDECLARE_EVENT_TABLE();


	public:
		enum Component
		{
			AddSongButton = wxID_HIGHEST + 1,
			EnqueueSongButton,
		};


	public:
		MusicPanel(wxWindow* parent);


	protected:
		void OnAddSong(wxCommandEvent& event);
		void OnEnqueueSong(wxCommandEvent& event);


	protected:
		wxListCtrl* mSongDatabaseList;


	};
}
