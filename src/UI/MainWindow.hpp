#pragma once


#include "wx/frame.h"
#include "Events/ConnectedToVoice.hpp"


namespace Strawberry::Accoutrement
{
	class MainWindow
		: public wxFrame
	{
	wxDECLARE_EVENT_TABLE();
	public:
		MainWindow();


		void OnVoiceConnect(ConnectedToVoice& event);
	};
}