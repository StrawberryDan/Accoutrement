#include "MainWindow.hpp"


#include "wx/menu.h"
#include "ChannelSelector.hpp"
#include "MusicPanel.hpp"
#include "fmt/format.h"
#include "Events/ConnectedToVoice.hpp"


namespace Strawberry::Accoutrement
{
	wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
			EVT_COMMAND(wxID_ANY, EVT_TYPE_CONNECTED_TO_VOICE, MainWindow::OnVoiceConnect)
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


	void MainWindow::OnVoiceConnect(wxCommandEvent& event)
	{
		auto& e = static_cast<ConnectedToVoice&>(event);
		GetStatusBar()->SetStatusText(
			fmt::format("Connected to [{}] --> [{}]",
						e.GetGuild().GetName(),
						e.GetChannel().GetName()));
	}
}