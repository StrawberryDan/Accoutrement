//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MainWindow.hpp"
// This Project
#include "../Config.hpp"
#include "../Discord/Bot.hpp"
#include "Events/BotInitialisedEvent.hpp"
#include "Panels/ChannelSelector.hpp"
#include "Panels/MusicPanel.hpp"
#include "Panels/NowPlayingPanel.hpp"
#include "Panels/SoundControlPanel.hpp"
#include "Panels/SoundEffectsPanel.hpp"
// Strawberry Core
#include "Strawberry/Core/Util/Logging.hpp"
// wxWidgets
#include "wx/gbsizer.h"
#include "wx/menu.h"
#include "wx/textdlg.h"
// Format
#include "fmt/format.h"

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	enum : wxWindowID
	{
		SET_TOKEN_MENU_ITEM = wxID_HIGHEST,
	};

	// clang-format off
	wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
		EVT_CONNECT_TO_VOICE(wxID_ANY, MainWindow::OnVoiceConnect)
		EVT_DISCONNECTED_FROM_VOICE(wxID_ANY, MainWindow::OnVoiceDisconnect)
		EVT_MENU(SET_TOKEN_MENU_ITEM, MainWindow::OnSetToken)
	wxEND_EVENT_TABLE();

	// clang-format on


	MainWindow::MainWindow()
		: wxFrame(nullptr, wxID_ANY, "Accoutrement")
	{
		wxFrame::SetMenuBar(CreateMenuBar());
		wxFrame::CreateToolBar();
		auto statusBar = wxFrame::CreateStatusBar();
		statusBar->SetStatusText("Not Connected");

		auto sizer           = new wxFlexGridSizer(3, 1, 5, 5);

		auto channelSelector = new ChannelSelector(this);
		sizer->Add(channelSelector, 0, wxALL | wxEXPAND | wxALIGN_TOP, 10);

		auto musicSizer = new wxGridBagSizer(5, 5);

		musicSizer->Add(new MusicPanel(this), {0, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		musicSizer->Add(new SoundEffectsPanel(this), {0, 1}, {1, 1}, wxALL | wxEXPAND, 5);

		auto nowPlayer = new NowPlayingPanel(this);
		musicSizer->Add(nowPlayer, {1, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		auto soundControlPanel = new SoundControlPanel(this);
		musicSizer->Add(soundControlPanel, {1, 1}, {1, 2}, wxALL | wxEXPAND, 5);

		musicSizer->AddGrowableRow(0, 6);
		musicSizer->AddGrowableRow(1, 1);
		musicSizer->AddGrowableCol(0, 4);
		musicSizer->AddGrowableCol(1, 3);
		sizer->Add(musicSizer, 0, wxALL | wxEXPAND, 5);

		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(1);
		SetSizerAndFit(sizer);
	}

	void MainWindow::OnVoiceConnect(ConnectToVoice& event)
	{
		GetStatusBar()->SetStatusText(fmt::format("Connected to [{}] --> [{}]", event.GetGuild().GetName(), event.GetChannel().GetName()));
		event.Skip();
	}

	void MainWindow::OnVoiceDisconnect(DisconnectFromVoice& event)
	{
		GetStatusBar()->SetStatusText("Not Connected");
		event.Skip();
	}

	void MainWindow::OnSetToken(wxCommandEvent& event)
	{
		auto dialog = new wxTextEntryDialog(this, "Enter your bot token...");
		if (dialog->ShowModal())
		{
			std::string token(dialog->GetValue());
			if (Bot::Get()) { Bot::Shutdown(); }
			Config::Get().SetToken(token);
			if (Bot::Initialise()) { Bot::Run(); }
		}
	}

	wxMenuBar* MainWindow::CreateMenuBar()
	{
		auto menu    = new wxMenuBar();

		auto botMenu = new wxMenu();
		botMenu->Append(SET_TOKEN_MENU_ITEM, "Set Token");
		menu->Append(botMenu, "Bot");

		return menu;
	}
} // namespace Strawberry::Accoutrement