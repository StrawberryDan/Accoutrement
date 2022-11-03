#pragma once



#include "wx/panel.h"
#include "wx/choice.h"
#include "wx/sizer.h"
#include "Discord/EventListener.hpp"






class ChannelSelector
	: public wxPanel
	, public Strawberry::Discord::EventListener
{
public:
	ChannelSelector(wxWindow* parent);


	bool Destroy() override;



private:
	void ProcessEvent(const Strawberry::Discord::Event::Base& event) override;



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


