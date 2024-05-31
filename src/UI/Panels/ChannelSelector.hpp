#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "../Events/BotStartedRunningEvent.hpp"
#include "../Events/BotStoppedRunningEvent.hpp"
#include "../Events/ConnectedToVoice.hpp"
#include "../Events/DisconnectedFromVoice.hpp"
#include "../Events/GuildCreated.hpp"
// wxWidgets
#include "wx/choice.h"
#include "wx/panel.h"
#include "wx/sizer.h"
// Discord
#include "Discord/Entities/Channel.hpp"
#include "Discord/Entities/Guild.hpp"
#include "Discord/EventListener.hpp"
#include "Discord/Snowflake.hpp"
// Strawberry Core
#include "Strawberry/Core/IO/Receiver.hpp"
// Standard Library
#include <map>
#include <mutex>
#include <vector>

namespace Strawberry::Accoutrement
{
    class ChannelSelector
            : public wxPanel, public Discord::EventListener, public Core::IO::Receiver<BotStartedRunningEvent, BotStoppedRunningEvent>
    {
            wxDECLARE_EVENT_TABLE();

        public:
            explicit ChannelSelector(wxWindow* parent);


            bool Destroy() override;

        private:
            wxButton* mConnectButton    = nullptr;
            wxButton* mDisconnectButton = nullptr;

        private:
            void ProcessEvent(const Strawberry::Discord::Event::EventBase& event) override;


            void OnGuildCreated(GuildCreated& event)
            {
                event.Skip();
                AddGuild(static_cast<GuildCreated&>(event).GetGuild());
            }


            Core::Optional<std::pair<Discord::Snowflake, Discord::Snowflake> > GetSelectedChannel() const;
            void                                                               UpdateConnectButton();
            bool                                                               IsConnectedToSelectedChannel() const;

        private:
            void AddGuild(const Strawberry::Discord::Entity::Guild& guild);
            void AddChannel(const Strawberry::Discord::Entity::Channel& channel);
            void OnConnect(wxCommandEvent& event);
            void OnDisconnect(wxCommandEvent& event);
            void OnSelectServer(wxCommandEvent& event);
            void OnSelectServer(const Discord::Snowflake& guildId);
            void OnSelectChannel(wxCommandEvent& event);

            virtual void Receive(BotStartedRunningEvent event) override;
            virtual void Receive(BotStoppedRunningEvent event) override;

            void PopulateGuildsList();

        private:
            class SnowflakeClientData;
    };


    class ChannelSelector::SnowflakeClientData : public wxClientData
    {
        public:
            SnowflakeClientData(Strawberry::Discord::Snowflake data)
                : mData(data) {}


            [[nodiscard]] Strawberry::Discord::Snowflake Get() const
            {
                return mData;
            }

        private:
            Strawberry::Discord::Snowflake mData;
    };
} // namespace Strawberry::Accoutrement
