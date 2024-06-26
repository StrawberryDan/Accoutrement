#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include "wx/event.h"
// Discord
#include "Discord/Entities/Channel.hpp"
#include "Discord/Entities/Guild.hpp"


namespace Strawberry::Accoutrement
{
    class ConnectedToVoice : public wxCommandEvent
    {
        public:
            ConnectedToVoice(Discord::Entity::Guild guild, Discord::Entity::Channel channel);


            [[nodiscard]] wxEvent* Clone() const override;


            [[nodiscard]] const Discord::Entity::Guild& GetGuild() const
            {
                return mGuild;
            }


            [[nodiscard]] const Discord::Entity::Channel& GetChannel() const
            {
                return mChannel;
            }

        private:
            const Discord::Entity::Guild   mGuild;
            const Discord::Entity::Channel mChannel;
    };


    wxDECLARE_EVENT(EVT_TYPE_CONNECT_TO_VOICE, ConnectedToVoice);

#define EVT_CONNECT_TO_VOICE(id, fn) DECLARE_EVENT_TABLE_ENTRY(EVT_TYPE_CONNECT_TO_VOICE, id, wxID_ANY, (&fn), nullptr),
} // namespace Strawberry::Accoutrement
