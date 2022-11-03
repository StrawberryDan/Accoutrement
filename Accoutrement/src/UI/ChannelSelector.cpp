#include "ChannelSelector.hpp"



#include "wx/stattext.h"
#include "wx/button.h"
#include "../Discord/Bot.hpp"



#define ID(v) std::to_underlying(Id::v)



wxBEGIN_EVENT_TABLE(ChannelSelector, wxPanel)
	EVT_BUTTON(ID(CONNECT), ChannelSelector::OnConnect)
	EVT_CHOICE(ID(SERVER),  ChannelSelector::OnSelectServer)
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

	Bot::Get().RegisterEventListener(this);
}



bool ChannelSelector::Destroy()
{
	Bot::Get().DeregisterEventListener(this);
	return wxWindowBase::Destroy();
}



void ChannelSelector::ProcessEvent(const Strawberry::Discord::Event::Base& event)
{
	using Strawberry::Discord::Entity::Channel;
	using Strawberry::Discord::Event::GuildCreate;

	if (auto guildCreate = event.Cast<GuildCreate>())
	{
		auto guild = (*guildCreate)->GetGuild();
		auto& channels = guild.GetChannels();

		wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(ID(SERVER)));

		mChannelMap.insert({guild.GetId(), {}});

		for (const auto& channel : channels)
		{
			if (channel.GetType() == Channel::Type::GUILD_VOICE)
			{
				mChannelMap.at(guild.GetId()).push_back(channel.GetId());
			}
		}



		serverChoice->Append(guild.GetName(), new SnowflakeClientData(guild.GetId()));



		Update();
		Refresh();
	}
}



void ChannelSelector::OnConnect(wxCommandEvent& event)
{

}



void ChannelSelector::OnSelectServer(wxCommandEvent& event)
{
	auto guildId = static_cast<SnowflakeClientData*>(event.GetClientObject())->Get();
	wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(ID(CHANNEL)));

	channelChoice->Clear();
	for (const auto& channel : mChannelMap.at(guildId))
	{
		channelChoice->Append(Bot::Get().GetChannelById(channel)->GetName(), new SnowflakeClientData(channel));
	}
}
