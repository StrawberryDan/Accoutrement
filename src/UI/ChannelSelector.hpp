#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Events/GuildCreated.hpp"
#include "Events/ConnectToVoice.hpp"
#include "Events/DisconnectFromVoice.hpp"
// wxWidgets
#include "wx/choice.h"
#include "wx/panel.h"
#include "wx/sizer.h"
// Discord
#include "Discord/Entity/Channel.hpp"
#include "Discord/Entity/Guild.hpp"
#include "Discord/EventListener.hpp"
#include "Discord/Snowflake.hpp"
// Standard Library
#include <map>
#include <mutex>
#include <vector>


namespace Strawberry::Accoutrement
{
	class ChannelSelector
		: public wxPanel
		, public Strawberry::Discord::EventListener
	{
		wxDECLARE_EVENT_TABLE();


	public:
		ChannelSelector(wxWindow* parent);


		bool Destroy() override;


	private:
		wxButton* mConnectButton = nullptr;


	private:
		void ProcessEvent(const Strawberry::Discord::Event::EventBase& event) override;
		void OnGuildCreated(GuildCreated& event) { event.Skip(); 	AddGuild(static_cast<GuildCreated&>(event).GetGuild()); }
		Core::Option<std::pair<Discord::Snowflake, Discord::Snowflake>>
			GetSelectedChannel() const;
		void UpdateConnectButton();
		bool IsConnectedToSelectedChannel() const;



	private:
		void AddGuild(const Strawberry::Discord::Entity::Guild& guild);
		void AddChannel(const Strawberry::Discord::Entity::Channel& channel);
		void OnConnect(wxCommandEvent& event);
		void OnDisconnect(wxCommandEvent& event);
		void OnSelectServer(wxCommandEvent& event);
		void OnSelectChannel(wxCommandEvent& event);


	private:
		class SnowflakeClientData;
	};



	class ChannelSelector::SnowflakeClientData : public wxClientData
	{
	public:
		SnowflakeClientData(Strawberry::Discord::Snowflake data) : mData(data)
		{}


		Strawberry::Discord::Snowflake Get() const { return mData; }


	private:
		Strawberry::Discord::Snowflake mData;
	};
}