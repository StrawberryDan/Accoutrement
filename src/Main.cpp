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


	private:
		MainWindow* mMainWindow;
	};


	wxBEGIN_EVENT_TABLE(Application, wxApp)

	wxEND_EVENT_TABLE()
}


wxIMPLEMENT_APP(Strawberry::Accoutrement::Application);