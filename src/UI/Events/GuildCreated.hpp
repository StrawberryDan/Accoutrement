#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Discord
#include "Discord/Entity/Guild.hpp"
// WxWidgets
#include "wx/event.h"


namespace Strawberry::Accoutrement
{
	class GuildCreated : public wxCommandEvent
	{
	public:
		GuildCreated(Discord::Entity::Guild guild);


		const Discord::Entity::Guild& GetGuild();


		wxEvent* Clone() const override;


	private:
		Discord::Entity::Guild mGuild;
	};


	wxDECLARE_EVENT(EVT_TYPE_GUILD_CREATED, GuildCreated);


#define EVT_GUILD_CREATED(id, fn) DECLARE_EVENT_TABLE_ENTRY(EVT_TYPE_GUILD_CREATED, id, wxID_ANY, (&fn), nullptr),
} // namespace Strawberry::Accoutrement