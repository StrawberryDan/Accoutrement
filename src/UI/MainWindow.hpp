#pragma once



#include "wx/frame.h"


namespace Strawberry::Accoutrement
{
	class MainWindow
			: public wxFrame
	{
		wxDECLARE_EVENT_TABLE();
	public:
		MainWindow();


		void OnVoiceConnect(wxCommandEvent& event);
	};
}