#pragma once


//======================================================================================================================
//  Include
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Events/ConnectToVoice.hpp"
#include "Events/DisconnectFromVoice.hpp"
// wxWidgets
#include "wx/frame.h"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class MainWindow
		: public wxFrame
	{
		wxDECLARE_EVENT_TABLE();

	public:
		MainWindow();


		void OnVoiceConnect(ConnectToVoice& event);
		void OnVoiceDisconnect(DisconnectFromVoice& event);
	};
} // namespace Strawberry::Accoutrement