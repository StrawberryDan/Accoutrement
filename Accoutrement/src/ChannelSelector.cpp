#include "ChannelSelector.hpp"



#include "wx/stattext.h"
#include "wx/button.h"



#define ID(v) std::to_underlying(Id::v)



wxBEGIN_EVENT_TABLE(ChannelSelector, wxPanel)
	EVT_BUTTON(ID(CONNECT), ChannelSelector::OnConnect)
wxEND_EVENT_TABLE()



ChannelSelector::ChannelSelector(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	auto sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, "Server:"), 0, wxALL, 10);
	sizer->Add(new wxChoice(this, ID(SERVER)), 1, wxALL, 10);
	sizer->Add(new wxStaticText(this, wxID_ANY, "Channel:"), 0, wxALL, 10);
	sizer->Add(new wxChoice(this, ID(CHANNEL)), 1, wxALL, 10);
	sizer->Add(new wxButton(this, ID(CONNECT), "Connect"), 0, wxALL, 10);
	SetSizerAndFit(sizer);
}



void ChannelSelector::OnConnect(wxCommandEvent& event)
{

}
