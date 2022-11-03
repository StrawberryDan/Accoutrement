#include "wx/wx.h"
#include "wx/sizer.h"
#include "UI/MainWindow.hpp"
#include "Config.hpp"
#include "Discord/Bot.hpp"



class Accoutrement : public wxApp
{
public:
    bool OnInit() override
    {
		Config::Initialise();
		Bot::Initialise();

		mMainWindow = new MainWindow();
	    SetTopWindow(mMainWindow);
		mMainWindow->Show();
		Bot::Run();
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



wxIMPLEMENT_APP(Accoutrement);