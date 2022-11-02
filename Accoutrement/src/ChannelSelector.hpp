#pragma once



#include "wx/panel.h"
#include "wx/choice.h"
#include "wx/sizer.h"



class ChannelSelector : public wxPanel
{
public:
	ChannelSelector(wxWindow* parent);



private:
	void OnConnect(wxCommandEvent& event);



private:
	wxDECLARE_EVENT_TABLE();



private:
	enum class Id;
};



enum class ChannelSelector::Id : wxWindowID
{
	CONNECT = wxID_HIGHEST + 1,
	SERVER,
	CHANNEL,
};


