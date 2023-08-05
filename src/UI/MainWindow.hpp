#pragma once


//======================================================================================================================
//  Include
//----------------------------------------------------------------------------------------------------------------------
#include "wx/frame.h"
#include "Events/ConnectedToVoice.hpp"
#include "Events/DisconnectedFromVoice.hpp"


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


		void OnVoiceConnect(ConnectedToVoice& event);
		void OnVoiceDisconnect(DisconnectedFromVoice& event);
	};
}