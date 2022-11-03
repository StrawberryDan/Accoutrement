#pragma once



#include <vector>
#include <map>



#include "wx/panel.h"
#include "wx/choice.h"
#include "wx/sizer.h"
#include "Discord/EventListener.hpp"
#include "Discord/Snowflake.hpp"
#include "Discord/Entity/Channel.hpp"



namespace
{
	using Strawberry::Discord::Snowflake;
	using Strawberry::Discord::Entity::Channel;
}



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
	void OnSelectServer(wxCommandEvent& event);



private:
	wxDECLARE_EVENT_TABLE();



private:
	std::unordered_map<Snowflake, std::vector<Snowflake>> mChannelMap;



private:
	enum class Id;
	class SnowflakeClientData;



};



class ChannelSelector::SnowflakeClientData : public wxClientData
{
public:
	SnowflakeClientData(Snowflake data) : mData(data) {}

	Snowflake Get() const { return mData; }
private:
	Snowflake mData;
};



enum class ChannelSelector::Id : wxWindowID
{
	CONNECT = wxID_HIGHEST + 1,
	SERVER,
	CHANNEL,
};


