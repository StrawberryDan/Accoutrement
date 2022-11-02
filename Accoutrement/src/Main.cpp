#include "wx/wx.h"
#include "wx/sizer.h"
#include "UI/MainWindow.hpp"



class Accoutrement : public wxApp
{
public:
    bool OnInit() override
    {
		mMainWindow = new MainWindow();
	    SetTopWindow(mMainWindow);
		mMainWindow->Show();
		return true;
    }



private:
	MainWindow* mMainWindow;

};



wxIMPLEMENT_APP(Accoutrement);