//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MainWindow.hpp"
// This Project
#include "Panels/ChannelSelector.hpp"
#include "Panels/MusicPanel.hpp"
#include "Panels/NowPlayingPanel.hpp"
#include "Panels/SoundControlPanel.hpp"
#include "Panels/SoundEffectsPanel.hpp"
// wxWidgets
#include "wx/gbsizer.h"
#include "wx/menu.h"
// Format
#include "fmt/format.h"


//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
		EVT_CONNECT_TO_VOICE(wxID_ANY, MainWindow::OnVoiceConnect)
			EVT_DISCONNECTED_FROM_VOICE(wxID_ANY, MainWindow::OnVoiceDisconnect)
				wxEND_EVENT_TABLE()


					MainWindow::MainWindow()
		: wxFrame(nullptr, wxID_ANY, "Accoutrement")
	{
		wxFrame::SetMenuBar(new wxMenuBar());
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
		GetStatusBar()->SetStatusText(
			fmt::format("Connected to [{}] --> [{}]", event.GetGuild().GetName(), event.GetChannel().GetName()));
		event.Skip();
	}


	void MainWindow::OnVoiceDisconnect(DisconnectFromVoice& event)
	{
		GetStatusBar()->SetStatusText("Not Connected");
		event.Skip();
	}
}// namespace Strawberry::Accoutrement