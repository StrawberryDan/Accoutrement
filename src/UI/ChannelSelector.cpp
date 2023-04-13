#include "ChannelSelector.hpp"



#include "../Discord/Bot.hpp"
#include "Core/Assert.hpp"
#include "wx/button.h"
#include "wx/stattext.h"




#define ID(v) std::to_underlying(Id::v)



wxBEGIN_EVENT_TABLE(ChannelSelector, wxPanel)
	EVT_BUTTON(ID(CONNECT), ChannelSelector::OnConnect)
	EVT_CHOICE(ID(SERVER),  ChannelSelector::OnSelectServer)
wxEND_EVENT_TABLE()



ChannelSelector::ChannelSelector(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
{
	std::unique_lock lk(mMutex);

	auto sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(new wxStaticText(this, wxID_ANY, "Server:"), 0, wxALL, 10);
	sizer->Add(new wxChoice(this, ID(SERVER)), 1, wxALL, 10);
	sizer->Add(new wxStaticText(this, wxID_ANY, "Channel:"), 0, wxALL, 10);
	sizer->Add(new wxChoice(this, ID(CHANNEL)), 1, wxALL, 10);
	sizer->Add(new wxButton(this, ID(CONNECT), "Connect"), 0, wxALL, 10);
	SetSizerAndFit(sizer);

	Bot::Get().RegisterEventListener(this);

	wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(ID(SERVER)));
	for (auto snowflake : Bot::Get().FetchGuilds())
	{
		auto guild = Bot::Get().FetchGuild(snowflake);
		AddGuild(*guild);
	}
}



bool ChannelSelector::Destroy()
{
	std::unique_lock lk(mMutex);
	Bot::Get().DeregisterEventListener(this);
	return wxWindowBase::Destroy();
}



void ChannelSelector::ProcessEvent(const Strawberry::Discord::Event::EventBase& event)
{
	using Strawberry::Discord::Entity::Channel;
	using Strawberry::Discord::Event::GuildCreate;

	std::unique_lock lk(mMutex);
	if (auto guildCreate = event.Cast<GuildCreate>())
	{
		auto guild = (*guildCreate)->GetGuild();
		AddGuild(guild);
	}
}



void ChannelSelector::AddGuild(const Strawberry::Discord::Entity::Guild& guild)
{
	wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(ID(SERVER)));
	// Check if the server is already in the list
	bool alreadyContains = false;
	for (int i = 0; i < serverChoice->GetCount(); i++)
	{
		auto clientData = dynamic_cast<SnowflakeClientData*>(serverChoice->GetClientObject(i));
		Strawberry::Core::Assert(clientData);

		if (clientData->Get() == guild.GetId())
		{
			alreadyContains = true;
			// Update name just in case it's changed.
			serverChoice->SetString(i, guild.GetName());
			break;
		}
	}

	// Add new servers to the list
	if (!alreadyContains)
	{
		serverChoice->Append(guild.GetName(), new SnowflakeClientData(guild.GetId()));
	}
}



void ChannelSelector::OnConnect(wxCommandEvent& event)
{
	std::unique_lock lk(mMutex);

	auto* serverChoice  = static_cast<wxChoice*>(FindWindowById(ID(SERVER)));
	auto* channelChoice = static_cast<wxChoice*>(FindWindowById(ID(CHANNEL)));


	int serverSelectionIndex  = serverChoice->GetSelection();
	int channelSelectionIndex = channelChoice->GetSelection();
	if (serverSelectionIndex == wxNOT_FOUND || channelSelectionIndex == wxNOT_FOUND)
	{
		return;
	}

	auto guildId   = static_cast<SnowflakeClientData*>(serverChoice->GetClientObject(serverSelectionIndex))->Get();
	auto channelId = static_cast<SnowflakeClientData*>(channelChoice->GetClientObject(channelSelectionIndex))->Get();

	Bot::Get().ConnectToVoice(guildId, channelId);
}



void ChannelSelector::OnSelectServer(wxCommandEvent& event)
{
	std::unique_lock lk(mMutex);

	auto guildId = static_cast<SnowflakeClientData*>(event.GetClientObject())->Get();
	wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(ID(CHANNEL)));
	channelChoice->Clear();
}
