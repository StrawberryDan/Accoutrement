#include "MainWindow.hpp"



#include "wx/menu.h"
#include "wx/button.h"



MainWindow::MainWindow()
	: wxFrame(nullptr, wxID_ANY, "Accoutrement")
{
	wxFrame::SetMenuBar(new wxMenuBar());
	wxFrame::CreateToolBar();
	wxFrame::CreateStatusBar();

	auto sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new ChannelSelector(this), 0, wxALL | wxGROW, 10);
	SetSizerAndFit(sizer);
}
