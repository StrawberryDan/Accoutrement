#include "MainWindow.hpp"



#include "wx/menu.h"
#include "wx/button.h"



namespace Strawberry::Accoutrement
{
	MainWindow::MainWindow()
			: wxFrame(nullptr, wxID_ANY, "Accoutrement")
	{
		wxFrame::SetMenuBar(new wxMenuBar());
		wxFrame::CreateToolBar();
		wxFrame::CreateStatusBar();

		auto sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(new ChannelSelector(this), 0, wxALL | wxGROW, 10);
		auto sizer = new wxFlexGridSizer(2, 1, 5, 5);
		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(1);

		SetSizerAndFit(sizer);
	}
}