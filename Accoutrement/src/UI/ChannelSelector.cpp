#include "ChannelSelector.hpp"



#include "wx/stattext.h"
#include "wx/button.h"
#include "../Discord/Bot.hpp"



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

		wxChoice* serverChoice  = static_cast<wxChoice*>(FindWindowById(ID(SERVER)));
		wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(ID(CHANNEL)));

		serverChoice->Append(guild.GetName());

		for (const auto& channel : channels)
		{
			if (channel.GetType() == Channel::Type::GUILD_VOICE)
			{
				channelChoice->Append(channel.GetName());
			}
		}


		Update();
		Refresh();
	}
}



void ChannelSelector::OnConnect(wxCommandEvent& event)
{

}
