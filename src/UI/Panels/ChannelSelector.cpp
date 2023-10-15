//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "ChannelSelector.hpp"
// This Project
#include "../../Discord/Bot.hpp"
// Core
#include "Strawberry/Core/Assert.hpp"
// Codec
#include "Codec/MediaFile.hpp"
// Wx Widgets
#include "wx/button.h"
#include "wx/gbsizer.h"
#include "wx/stattext.h"

namespace Strawberry::Accoutrement
{
	enum ComponentId : wxWindowID
	{
		CONNECT = wxID_HIGHEST + 1,
		DISCONNECT,
		SERVER,
		CHANNEL,
	};

	// clang-format off
	wxBEGIN_EVENT_TABLE(ChannelSelector, wxPanel)
	EVT_BUTTON(CONNECT, ChannelSelector::OnConnect)
	EVT_BUTTON(DISCONNECT, ChannelSelector::OnDisconnect)
	EVT_CHOICE(SERVER, ChannelSelector::OnSelectServer) EVT_CHOICE(CHANNEL, ChannelSelector::OnSelectChannel)
	EVT_GUILD_CREATED(wxID_ANY, ChannelSelector::OnGuildCreated)
	wxEND_EVENT_TABLE();

	// clang-format on


	ChannelSelector::ChannelSelector(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxGridBagSizer();
		sizer->Add(new wxStaticText(this, wxID_ANY, "Server:"), {0, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxChoice(this, SERVER), {0, 1}, {1, 1}, wxALL | wxEXPAND, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Channel:"), {0, 2}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxChoice(this, CHANNEL), {0, 3}, {1, 1}, wxALL | wxEXPAND, 5);
		mConnectButton = new wxButton(this, CONNECT, "Connect");
		sizer->Add(mConnectButton, {1, 0}, {1, 2}, wxALL | wxEXPAND, 5);
		mDisconnectButton = new wxButton(this, DISCONNECT, "Disconnect");
		sizer->Add(mDisconnectButton, {1, 2}, {1, 2}, wxALL | wxEXPAND, 5);

		sizer->AddGrowableCol(1, 1);
		sizer->AddGrowableCol(3, 1);
		SetSizerAndFit(sizer);

		if (!Bot::Get() || !Bot::Get()->GetBot())
		{
			mConnectButton->Disable();
			mDisconnectButton->Disable();
		}

		if (Bot::Get() && Bot::Get()->GetBot()) { PopulateGuildsList(); }
	}

	bool ChannelSelector::Destroy()
	{
		if (Bot::Get()) Bot::Get()->GetBot()->DeregisterEventListener(this);

		return wxWindowBase::Destroy();
	}

	void ChannelSelector::ProcessEvent(const Strawberry::Discord::Event::EventBase& event)
	{
		using Strawberry::Discord::Entity::Channel;
		using Strawberry::Discord::Event::GuildCreate;


		if (auto guildCreate = event.Cast<GuildCreate>()) { QueueEvent(new GuildCreated(guildCreate->GetGuild())); }
	}

	Core::Optional<std::pair<Discord::Snowflake, Discord::Snowflake>> ChannelSelector::GetSelectedChannel() const
	{
		auto* serverChoice        = static_cast<wxChoice*>(FindWindowById(SERVER));
		auto* channelChoice       = static_cast<wxChoice*>(FindWindowById(CHANNEL));

		int serverSelectionIndex  = serverChoice->GetSelection();
		int channelSelectionIndex = channelChoice->GetSelection();
		if (serverSelectionIndex == wxNOT_FOUND || channelSelectionIndex == wxNOT_FOUND) { return Core::NullOpt; }

		auto guildId   = static_cast<SnowflakeClientData*>(serverChoice->GetClientObject(serverSelectionIndex))->Get();
		auto channelId = static_cast<SnowflakeClientData*>(channelChoice->GetClientObject(channelSelectionIndex))->Get();

		return std::make_pair(guildId, channelId);
	}

	void ChannelSelector::AddGuild(const Strawberry::Discord::Entity::Guild& guild)
	{
		wxChoice* serverChoice    = static_cast<wxChoice*>(FindWindowById(SERVER));
		// Check if the server is already in the list
		bool      alreadyContains = false;
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
		if (!alreadyContains) { serverChoice->Append(guild.GetName(), new SnowflakeClientData(guild.GetId())); }

		// Select this guild now if it's the first in.
		if (serverChoice->GetCount() == 1) { OnSelectServer(guild.GetId()); }
	}

	void ChannelSelector::AddChannel(const Strawberry::Discord::Entity::Channel& channel)
	{
		wxChoice* channelChoice   = static_cast<wxChoice*>(FindWindowById(CHANNEL));
		// Check if the server is already in the list
		bool      alreadyContains = false;
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
		if (!alreadyContains) { channelChoice->Append(channel.GetName(), new SnowflakeClientData(channel.GetId())); }
	}

	void ChannelSelector::OnConnect(wxCommandEvent& event)
	{
		const bool alreadyConnected = IsConnectedToSelectedChannel();

		if (Bot::Get() && !alreadyConnected)
		{
			auto selection = GetSelectedChannel();
			if (!selection) return;
			auto [guildId, channelId] = selection.Unwrap();

			Bot::Get()->GetBot()->ConnectToVoice(guildId, channelId);

			mConnectButton->Disable();
			QueueEvent(new ConnectedToVoice(*Bot::Get()->GetBot()->GetGuild(guildId), *Bot::Get()->GetBot()->GetChannel(channelId)));
		}
	}

	void ChannelSelector::OnDisconnect(wxCommandEvent& event)
	{
		mConnectButton->Enable();
		Bot::Get()->GetBot()->DisconnectFromVoice();
		QueueEvent(new DisconnectedFromVoice());
	}

	void ChannelSelector::OnSelectServer(wxCommandEvent& event)
	{
		auto guildId = static_cast<SnowflakeClientData*>(event.GetClientObject())->Get();
		OnSelectServer(guildId);
	}

	void ChannelSelector::OnSelectServer(const Discord::Snowflake& guildId)
	{
		using namespace Strawberry::Discord::Entity;

		if (Bot::Get())
		{
			wxBusyCursor busyCursor;

			const auto* guild         = Bot::Get()->GetBot()->FetchGuild(guildId);
			wxChoice*   channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));
			channelChoice->Clear();


			std::vector<Channel> channels;
			for (auto channelId : Bot::Get()->GetBot()->FetchChannels(guild->GetId()))
			{
				auto* channel = Bot::Get()->GetBot()->GetChannel(channelId);
				if (channel->GetType() == Channel::Type::GUILD_VOICE) { channels.push_back(*channel); }
			}


			std::sort(channels.begin(), channels.end(), [](const auto& a, const auto& b) { return a.GetPosition() < b.GetPosition(); });
			for (const auto& channel : channels) { AddChannel(channel); }

			UpdateConnectButton();
		}
	}

	void ChannelSelector::OnSelectChannel(wxCommandEvent& event)
	{
		UpdateConnectButton();
	}

	void ChannelSelector::UpdateConnectButton()
	{
		Core::Assert(Bot::Get().HasValue());
		if (auto selection = GetSelectedChannel())
		{
			auto [guildId, channelId] = selection.Unwrap();
			auto connectedGuild       = Bot::Get()->GetBot()->GetVoiceConnection().Map([](const auto& x) { return x->GetGuild(); });
			auto connectedChannel     = Bot::Get()->GetBot()->GetVoiceConnection().Map([](const auto& x) { return x->GetChannel(); });

			if (guildId == connectedGuild && channelId == connectedChannel) { mConnectButton->Disable(); }
			else { mConnectButton->Enable(); }
		}
	}

	bool ChannelSelector::IsConnectedToSelectedChannel() const
	{
		Core::Assert(Bot::Get().HasValue());
		if (auto selection = GetSelectedChannel())
		{
			auto [guildId, channelId] = selection.Unwrap();
			auto connectedGuild       = Bot::Get()->GetBot()->GetVoiceConnection().Map([](const auto& x) { return x->GetGuild(); });
			auto connectedChannel     = Bot::Get()->GetBot()->GetVoiceConnection().Map([](const auto& x) { return x->GetChannel(); });
			return guildId == connectedGuild && channelId == connectedChannel;
		}

		return false;
	}

	void ChannelSelector::Receive(BotStartedRunningEvent event)
	{
		Bot::Get()->GetBot()->RegisterEventListener(this);
		PopulateGuildsList();
		UpdateConnectButton();
		mDisconnectButton->Enable();
	}

	void ChannelSelector::Receive(BotStoppedRunningEvent event)
	{
		wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(SERVER));
		serverChoice->Clear();
		wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));
		channelChoice->Clear();
		mConnectButton->Disable();
		mDisconnectButton->Disable();
	}

	void ChannelSelector::PopulateGuildsList()
	{
		Core::Assert(Bot::Get().HasValue());

		wxChoice* serverChoice = static_cast<wxChoice*>(FindWindowById(SERVER));
		serverChoice->Clear();
		wxChoice* channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));
		channelChoice->Clear();
		for (auto snowflake : Bot::Get()->GetBot()->FetchGuilds())
		{
			auto guild = Bot::Get()->GetBot()->FetchGuild(snowflake);
			wxPanel::QueueEvent(new GuildCreated(*guild));
		}
	}
} // namespace Strawberry::Accoutrement