//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MainWindow.hpp"
// This Project
#include "../Config.hpp"
#include "../Discord/Bot.hpp"
#include "Events/BotStartedRunningEvent.hpp"
#include "Panels/ChannelSelector.hpp"
#include "Panels/MusicPanel.hpp"
#include "Panels/NowPlayingPanel.hpp"
#include "Panels/SoundControlPanel.hpp"
#include "Panels/SoundEffectsPanel.hpp"
// Strawberry Core
#include "Strawberry/Core/IO/Logging.hpp"
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

		auto statusBar       = wxFrame::CreateStatusBar();

		auto sizer           = new wxFlexGridSizer(3, 1, 0, 0);

		auto channelSelector = new ChannelSelector(this);
		sizer->Add(channelSelector, 0, wxALL | wxEXPAND | wxALIGN_TOP, 0);

		auto musicSizer = new wxGridBagSizer();

		musicSizer->Add(new MusicPanel(this), {0, 0}, {1, 1}, wxALL | wxEXPAND, 0);

		musicSizer->Add(new SoundEffectsPanel(this), {0, 1}, {1, 1}, wxALL | wxEXPAND, 0);

		auto nowPlayer = new NowPlayingPanel(this);
		musicSizer->Add(nowPlayer, {1, 0}, {1, 1}, wxALL | wxEXPAND, 0);

		auto soundControlPanel = new SoundControlPanel(this);
		musicSizer->Add(soundControlPanel, {1, 1}, {1, 2}, wxALL | wxEXPAND, 0);

		musicSizer->AddGrowableRow(0, 6);
		musicSizer->AddGrowableRow(1, 1);
		musicSizer->AddGrowableCol(0, 4);
		musicSizer->AddGrowableCol(1, 3);
		sizer->Add(musicSizer, 0, wxALL | wxEXPAND, 0);

		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(1);
		SetSizerAndFit(sizer);

		UpdateStatusBar();
	}

	void MainWindow::UpdateStatusBar()
	{
		const bool botExists        = Bot::Get() && Bot::Get()->GetBot();
		const bool connectedToVoice = botExists && Bot::Get()->GetBot()->GetVoiceConnection();

		if (connectedToVoice)
		{
			auto voice   = Bot::Get()->GetBot()->GetVoiceConnection().Unwrap();
			auto guild   = Bot::Get()->GetBot()->GetGuild(voice->GetGuild())->GetName();
			auto channel = Bot::Get()->GetBot()->GetChannel(voice->GetChannel())->GetName();
			GetStatusBar()->SetStatusText(fmt::format("Connected to \'{}\' -> \'{}\'", guild, channel));
		}
		else if (botExists) { GetStatusBar()->SetStatusText("Not connected to a voice channel"); }
		else if (!botExists) { GetStatusBar()->SetStatusText("Bot not initialised"); }
		else { Core::Unreachable(); }
	}

	void MainWindow::OnVoiceConnect(ConnectedToVoice& event)
	{
		UpdateStatusBar();
		event.Skip();
	}

	void MainWindow::OnVoiceDisconnect(DisconnectedFromVoice& event)
	{
		UpdateStatusBar();
		event.Skip();
	}

	void MainWindow::OnSetToken(wxCommandEvent& event)
	{
		auto dialog = new wxTextEntryDialog(this, "Enter your bot token...");
		if (dialog->ShowModal() == wxID_OK)
		{
			std::string token(dialog->GetValue());
			if (Bot::Get()) { Bot::Shutdown(); }
			Config::Get().SetToken(token);
			if (Bot::Initialise()) { Bot::Run(); }
		}
	}

	void MainWindow::Receive(BotStartedRunningEvent event)
	{
		UpdateStatusBar();
	}

	void MainWindow::Receive(BotStoppedRunningEvent event)
	{
		UpdateStatusBar();
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