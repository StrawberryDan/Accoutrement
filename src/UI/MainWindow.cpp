#include "MainWindow.hpp"



#include "wx/menu.h"
#include "ChannelSelector.hpp"
#include "MusicPanel.hpp"



namespace Strawberry::Accoutrement
{
	MainWindow::MainWindow()
			: wxFrame(nullptr, wxID_ANY, "Accoutrement")
	{
		wxFrame::SetMenuBar(new wxMenuBar());
		wxFrame::CreateToolBar();
		wxFrame::CreateStatusBar();

		auto sizer = new wxFlexGridSizer(2, 1, 5, 5);
		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(1);

		sizer->Add(new ChannelSelector(this), 0, wxALL | wxEXPAND | wxALIGN_TOP, 10);
		auto musicSizer = new wxBoxSizer(wxHORIZONTAL);
		musicSizer->Add(new MusicPanel(this), 1, wxEXPAND);
		sizer->Add(musicSizer, 0, wxALL | wxEXPAND, 10);
		SetSizerAndFit(sizer);
	}
}