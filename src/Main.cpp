//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Config.hpp"
#include "Discord/Bot.hpp"
#include "UI/MainWindow.hpp"
// WxWidgets
#include "wx/wx.h"
#include "wx/sizer.h"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class Application
		: public wxApp
	{
	wxDECLARE_EVENT_TABLE();


	public:
		bool OnInit() override
		{
			Config::Initialise();
			Bot::Initialise();
			Bot::Run();

			mMainWindow = new MainWindow();
			SetTopWindow(mMainWindow);
			mMainWindow->Show();

			return true;
		}


		int OnExit() override
		{
			Bot::Stop();
			Config::Dump();
			return wxAppBase::OnExit();
		}


		void OnVoiceConnect(ConnectToVoice& event)
		{
			Bot::Get().ConnectToVoice(event.GetGuild().GetId(), event.GetChannel().GetId());
		}


		void OnVoiceDisconnect(DisconnectFromVoice& event)
		{
			Bot::Get().DisconnectFromVoice();
		}


	private:
		MainWindow* mMainWindow;
	};


	wxBEGIN_EVENT_TABLE(Application, wxApp)
		EVT_CONNECT_TO_VOICE(wxID_ANY, Application::OnVoiceConnect)
		EVT_DISCONNECTED_FROM_VOICE(wxID_ANY, Application::OnVoiceDisconnect)
	wxEND_EVENT_TABLE()
}


wxIMPLEMENT_APP(Strawberry::Accoutrement::Application);