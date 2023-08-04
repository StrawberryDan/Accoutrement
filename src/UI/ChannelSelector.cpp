//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "ChannelSelector.hpp"
// This Project
#include "../Discord/Bot.hpp"
#include "Events/GuildCreated.hpp"
// Core
#include "Strawberry/Core/Assert.hpp"
#include "wx/button.h"
#include "wx/stattext.h"


namespace Strawberry::Accoutrement
{
	enum CommandEvents : wxEventType
	{
		AddGuild,
	};


	enum ComponentId : wxWindowID
	{
		CONNECT = wxID_HIGHEST + 1,
		DISCONNECT,
		SERVER,
		CHANNEL,
	};



	wxBEGIN_EVENT_TABLE(ChannelSelector, wxPanel)
					EVT_BUTTON(CONNECT,    ChannelSelector::OnConnect)
					EVT_BUTTON(DISCONNECT, ChannelSelector::OnDisconnect)
					EVT_CHOICE(SERVER,     ChannelSelector::OnSelectServer)
					EVT_GUILD_CREATED(wxID_ANY, ChannelSelector::OnGuildCreated)
	wxEND_EVENT_TABLE()



	ChannelSelector::ChannelSelector(wxWindow* parent)
			: wxPanel(parent)
	{
		auto buttons = new wxBoxSizer(wxVERTICAL);
		buttons->Add(new wxButton(this, CONNECT, "Connect"), 1, wxALL | wxEXPAND, 5);
		buttons->Add(new wxButton(this, DISCONNECT, "Disconnect"), 1, wxALL | wxEXPAND, 5);

		auto sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Server:"), 0, wxALL | wxALIGN_CENTER, 10);
		sizer->Add(new wxChoice(this, SERVER), 1, wxALL | wxALIGN_CENTER, 10);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Channel:"), 0, wxALL | wxALIGN_CENTER, 10);
		sizer->Add(new wxChoice(this, CHANNEL), 1, wxALL | wxALIGN_CENTER, 10);
		sizer->Add(buttons, 0, wxALL | wxALIGN_CENTER, 10);
		SetSizerAndFit(sizer);

		Bot::Get().RegisterEventListener(this);

		wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(SERVER));
		for (auto snowflake: Bot::Get().FetchGuilds())
		{
			auto guild = Bot::Get().FetchGuild(snowflake);
			AddGuild(*guild);
		}
	}



	bool ChannelSelector::Destroy()
	{
		Bot::Get().DeregisterEventListener(this);
		return wxWindowBase::Destroy();
	}



	void ChannelSelector::ProcessEvent(const Strawberry::Discord::Event::EventBase& event)
	{
		using Strawberry::Discord::Entity::Channel;
		using Strawberry::Discord::Event::GuildCreate;


		if (auto guildCreate = event.Cast<GuildCreate>())
		{
			QueueEvent(new GuildCreated(guildCreate->GetGuild()));
		}
	}



	void ChannelSelector::AddGuild(const Strawberry::Discord::Entity::Guild& guild)
	{
		wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(SERVER));
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



	void ChannelSelector::AddChannel(const Strawberry::Discord::Entity::Channel& channel)
	{
		wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));
		// Check if the server is already in the list
		bool alreadyContains = false;
		for (int i = 0; i < channelChoice->GetCount(); i++)
		{
			auto clientData = dynamic_cast<SnowflakeClientData*>(channelChoice->GetClientObject(i));
			Strawberry::Core::Assert(clientData);

			if (clientData->Get() == channel.GetId())
			{
				alreadyContains = true;
				// Update name just in case it's changed.
				channelChoice->SetString(i, channel.GetName());
				break;
			}
		}

		// Add new servers to the list
		if (!alreadyContains)
		{
			channelChoice->Append(channel.GetName(), new SnowflakeClientData(channel.GetId()));
		}
	}



	void ChannelSelector::OnConnect(wxCommandEvent& event)
	{
		auto* serverChoice = static_cast<wxChoice*>(FindWindowById(SERVER));
		auto* channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));


		int serverSelectionIndex = serverChoice->GetSelection();
		int channelSelectionIndex = channelChoice->GetSelection();
		if (serverSelectionIndex == wxNOT_FOUND || channelSelectionIndex == wxNOT_FOUND)
		{
			return;
		}

		auto guildId = static_cast<SnowflakeClientData*>(serverChoice->GetClientObject(serverSelectionIndex))->Get();
		auto channelId = static_cast<SnowflakeClientData*>(channelChoice->GetClientObject(
				channelSelectionIndex))->Get();

		Bot::Get().ConnectToVoice(guildId, channelId);
	}



	void ChannelSelector::OnDisconnect(wxCommandEvent& event)
	{
		Bot::Get().DisconnectFromVoice();
	}



	void ChannelSelector::OnSelectServer(wxCommandEvent& event)
	{
		using namespace Strawberry::Discord::Entity;

		wxBusyCursor busyCursor;


		auto guildId = static_cast<SnowflakeClientData*>(event.GetClientObject())->Get();
		const auto* guild = Bot::Get().FetchGuild(guildId);
		wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));

		channelChoice->Clear();

		std::vector<Channel> channels;
		for (auto channelId: Bot::Get().FetchChannels(guild->GetId()))
		{
			auto* channel = Bot::Get().GetChannel(channelId);
			if (channel->GetType() == Channel::Type::GUILD_VOICE)
			{
				channels.push_back(*channel);
			}
		}

		std::sort(channels.begin(), channels.end(), [](const auto& a, const auto& b)
		{ return a.GetPosition() < b.GetPosition(); });
		for (const auto& channel: channels)
		{
			AddChannel(channel);
		}
	}
}