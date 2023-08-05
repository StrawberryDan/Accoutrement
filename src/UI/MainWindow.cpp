#include "MainWindow.hpp"


#include "wx/menu.h"
#include "ChannelSelector.hpp"
#include "MusicPanel.hpp"
#include "fmt/format.h"


namespace Strawberry::Accoutrement
{
	wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
		EVT_CONNECTED_TO_VOICE(wxID_ANY, MainWindow::OnVoiceConnect)
		EVT_DISCONNECTED_FROM_VOICE(wxID_ANY, MainWindow::OnVoiceDisconnect)
	wxEND_EVENT_TABLE()


	MainWindow::MainWindow()
		: wxFrame(nullptr, wxID_ANY, "Accoutrement")
	{
		wxFrame::SetMenuBar(new wxMenuBar());
		wxFrame::CreateToolBar();
		auto statusBar = wxFrame::CreateStatusBar();
		statusBar->SetStatusText("Not Connected");

		auto sizer = new wxFlexGridSizer(2, 1, 5, 5);
		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(1);

		auto channelSelector = new ChannelSelector(this);
		sizer->Add(channelSelector, 0, wxALL | wxEXPAND | wxALIGN_TOP, 10);
		auto musicSizer = new wxBoxSizer(wxHORIZONTAL);
		musicSizer->Add(new MusicPanel(this), 1, wxEXPAND);
		sizer->Add(musicSizer, 0, wxALL | wxEXPAND, 10);
		SetSizerAndFit(sizer);
	}


	void MainWindow::OnVoiceConnect(ConnectedToVoice& event)
	{
		GetStatusBar()->SetStatusText(
			fmt::format("Connected to [{}] --> [{}]",
						event.GetGuild().GetName(),
						event.GetChannel().GetName()));
	}


	void MainWindow::OnVoiceDisconnect(DisconnectedFromVoice& event)
	{
		GetStatusBar()->SetStatusText("Not Connected");
	}
}