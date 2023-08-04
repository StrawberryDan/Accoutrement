//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "ChannelSelector.hpp"
// This Project
#include "../Discord/Bot.hpp"
#include "Events/GuildCreated.hpp"
// Core
#include "Strawberry/Core/Assert.hpp"
// Codec
#include "Codec/MediaFile.hpp"
// Wx Widgets
#include "wx/button.h"
#include "wx/stattext.h"
#include "wx/gbsizer.h"


namespace Strawberry::Accoutrement
{
	enum CommandEvents
		: wxEventType
	{
		AddGuild,
	};


	enum ComponentId
		: wxWindowID
	{
		CONNECT = wxID_HIGHEST + 1,
		DISCONNECT,
		SERVER,
		CHANNEL,
	};


	wxBEGIN_EVENT_TABLE(ChannelSelector, wxPanel)
			EVT_BUTTON(CONNECT, ChannelSelector::OnConnect)
			EVT_BUTTON(DISCONNECT, ChannelSelector::OnDisconnect)
			EVT_CHOICE(SERVER, ChannelSelector::OnSelectServer)
			EVT_CHOICE(CHANNEL, ChannelSelector::OnSelectChannel)
			EVT_GUILD_CREATED(wxID_ANY, ChannelSelector::OnGuildCreated)
	wxEND_EVENT_TABLE()


	ChannelSelector::ChannelSelector(wxWindow* parent)
		: wxPanel(parent)
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer(5, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Server:"), {0, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxChoice(this, SERVER), {0, 1}, {1, 1}, wxALL | wxEXPAND, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Channel:"), {0, 2}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxChoice(this, CHANNEL), {0, 3}, {1, 1}, wxALL | wxEXPAND, 5);
		mConnectButton = new wxButton(this, CONNECT, "Connect");
		sizer->Add(mConnectButton, {1, 0}, {1, 2}, wxALL | wxEXPAND, 5);
		sizer->Add(new wxButton(this, DISCONNECT, "Disconnect"), {1, 2}, {1, 2}, wxALL | wxEXPAND, 5);

		sizer->AddGrowableCol(1, 1);
		sizer->AddGrowableCol(3, 1);
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


	Core::Option<std::pair<Discord::Snowflake, Discord::Snowflake>> ChannelSelector::GetSelectedChannel() const
	{
		auto* serverChoice = static_cast<wxChoice*>(FindWindowById(SERVER));
		auto* channelChoice = static_cast<wxChoice*>(FindWindowById(CHANNEL));

		int serverSelectionIndex = serverChoice->GetSelection();
		int channelSelectionIndex = channelChoice->GetSelection();
		if (serverSelectionIndex == wxNOT_FOUND || channelSelectionIndex == wxNOT_FOUND)
		{
			return Core::NullOpt;
		}

		auto guildId = static_cast<SnowflakeClientData*>(serverChoice->GetClientObject(
			serverSelectionIndex))->Get();
		auto channelId = static_cast<SnowflakeClientData*>(channelChoice->GetClientObject(
			channelSelectionIndex))->Get();

		return std::make_pair(guildId, channelId);
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
		const bool alreadyConnected = IsConnectedToSelectedChannel();

		if (!alreadyConnected)
		{
			auto selection = GetSelectedChannel();
			if (!selection) return;
			auto [guildId, channelId] = selection.Unwrap();

			Bot::Get().ConnectToVoice(guildId, channelId);
			mConnectButton->Disable();


			std::vector<Codec::MediaFile> files;
			files.push_back(Codec::MediaFile::Open("data/cotn.flac").Unwrap());
//			files.push_back(Codec::MediaFile::Open("data/pd.wav").Unwrap());
//			files.push_back(Codec::MediaFile::Open("data/girigiri.mp3").Unwrap());
//			files.push_back(Codec::MediaFile::Open("data/dcl.wav").Unwrap());


			for (auto& file: files)
			{
				auto stream = file.GetBestStream(Codec::MediaType::Audio).Unwrap();
				std::vector<Codec::Packet> packets;
				while (auto packet = stream->Read())
				{
					packets.push_back(packet.Unwrap());
				}


				std::vector<Codec::Audio::Frame> frames;
				Codec::Audio::Decoder decoder = stream->GetDecoder();
				for (const auto& packet: packets)
				{
					for (auto& frame: decoder.DecodePacket(packet))
					{
						frames.push_back(std::move(frame));
					}
				}

				auto channel = Bot::Get().GetVoiceConnection()->CreateInputChannel();
				for (auto& frame: frames)
				{
					channel->EnqueueFrame(std::move(frame));
				}
			}
		}
		else if (alreadyConnected && Bot::Get().GetVoiceConnection())
		{
			Bot::Get().DisconnectFromVoice();
			mConnectButton->Enable();
		}
		else
		{
			Core::Unreachable();
		}
	}


	void ChannelSelector::OnDisconnect(wxCommandEvent& event)
	{
		Bot::Get().DisconnectFromVoice();
		mConnectButton->Enable();
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


		std::sort(channels.begin(), channels.end(),
				  [](const auto& a, const auto& b) { return a.GetPosition() < b.GetPosition(); });
		for (const auto& channel: channels)
		{
			AddChannel(channel);
		}

		UpdateConnectButton();
	}


	void ChannelSelector::OnSelectChannel(wxCommandEvent& event)
	{
		UpdateConnectButton();
	}


	void ChannelSelector::UpdateConnectButton()
	{
		if (auto selection = GetSelectedChannel())
		{
			auto [guildId, channelId] = selection.Unwrap();
			auto connectedGuild = Bot::Get().GetVoiceConnection()
				.Map([](const auto& x) { return x.GetGuild(); });
			auto connectedChannel = Bot::Get().GetVoiceConnection()
				.Map([](const auto& x) { return x.GetChannel(); });

			if (guildId == connectedGuild && channelId == connectedChannel)
			{
				mConnectButton->Disable();
			}
			else
			{
				mConnectButton->Enable();
			}
		}
	}


	bool ChannelSelector::IsConnectedToSelectedChannel() const
	{
		if (auto selection = GetSelectedChannel())
		{
			auto [guildId, channelId] = selection.Unwrap();
			auto connectedGuild = Bot::Get().GetVoiceConnection()
				.Map([](const auto& x) { return x.GetGuild(); });
			auto connectedChannel = Bot::Get().GetVoiceConnection()
				.Map([](const auto& x) { return x.GetChannel(); });
			return guildId == connectedGuild && channelId == connectedChannel;
		}

		return false;
	}
}