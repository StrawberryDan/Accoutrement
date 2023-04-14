#include "wx/wx.h"
#include "wx/sizer.h"
#include "UI/MainWindow.hpp"
#include "Config.hpp"
#include "Discord/Bot.hpp"



namespace Strawberry::Accoutrement
{
	class Application : public wxApp
	{
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
}



wxIMPLEMENT_APP(Strawberry::Accoutrement::Application);