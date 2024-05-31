#pragma once


//======================================================================================================================
//  Include
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Events/BotStartedRunningEvent.hpp"
#include "Events/BotStoppedRunningEvent.hpp"
#include "Events/ConnectedToVoice.hpp"
#include "Events/DisconnectedFromVoice.hpp"
// Strawberry Core
#include "Strawberry/Core/IO/Receiver.hpp"
// wxWidgets
#include "wx/frame.h"

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
    class MainWindow
            : public wxFrame, public Core::IO::Receiver<BotStartedRunningEvent, BotStoppedRunningEvent>
    {
            wxDECLARE_EVENT_TABLE();

        public:
            MainWindow();

            void UpdateStatusBar();

        protected:
            void OnVoiceConnect(ConnectedToVoice& event);
            void OnVoiceDisconnect(DisconnectedFromVoice& event);
            void OnSetToken(wxCommandEvent& event);

            void Receive(BotStartedRunningEvent event);
            void Receive(BotStoppedRunningEvent event);

            static wxMenuBar* CreateMenuBar();
    };
} // namespace Strawberry::Accoutrement
