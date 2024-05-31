//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Config.hpp"
#include "Discord/Bot.hpp"
#include "UI/MainWindow.hpp"
// Strawberry Core
#include "Strawberry/Core/IO/Logging.hpp"
// WxWidgets
#include "wx/sizer.h"
#include "wx/wx.h"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
    class Application : public wxApp
    {
        public:
            bool OnInit() override
            {
                Config::Initialise();
                if (Bot::Initialise())
                {
                    Bot::Run();
                }

                wxInitAllImageHandlers();
                mMainWindow = new MainWindow();
                SetTopWindow(mMainWindow);
                mMainWindow->Show();

                return true;
            }


            int OnExit() override
            {
                Bot::Shutdown();
                Config::Dump();
                return wxAppBase::OnExit();
            }

        private:
            MainWindow* mMainWindow;
    };
} // namespace Strawberry::Accoutrement


wxIMPLEMENT_APP(Strawberry::Accoutrement::Application);
