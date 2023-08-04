#pragma once



#include <map>
#include <mutex>
#include <vector>

#include "Discord/Entity/Channel.hpp"
#include "Discord/Entity/Guild.hpp"
#include "Discord/EventListener.hpp"
#include "Discord/Snowflake.hpp"

#include "wx/choice.h"
#include "wx/panel.h"
#include "wx/sizer.h"
#include "Events/GuildCreated.hpp"


namespace Strawberry::Accoutrement
{
	class ChannelSelector
			: public wxPanel, public Strawberry::Discord::EventListener
	{
		wxDECLARE_EVENT_TABLE();


	public:
		ChannelSelector(wxWindow* parent);


		bool Destroy() override;


	private:
		wxButton* mConnectButton = nullptr;


	private:
		void ProcessEvent(const Strawberry::Discord::Event::EventBase& event) override;
		void OnGuildCreated(wxEvent& event) { AddGuild(static_cast<GuildCreated*>(&event)->GetGuild()); }
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