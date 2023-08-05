#pragma once


//======================================================================================================================
//  Include
//----------------------------------------------------------------------------------------------------------------------
#include "wx/frame.h"
#include "Events/ConnectToVoice.hpp"
#include "Events/DisconnectFromVoice.hpp"


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
}