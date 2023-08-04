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
	class GuildCreated
			: public wxEvent
	{
	public:
		GuildCreated(Discord::Entity::Guild guild);


		const Discord::Entity::Guild& GetGuild();


		wxEvent* Clone() const override;


	private:
		Discord::Entity::Guild mGuild;
	};


	wxDEFINE_EVENT(EVT_GUILD_CREATED, GuildCreated);

#define EVT_GUILD_CREATED(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(EVT_GUILD_CREATED, id, wxID_ANY, wxEventHandler(fn), nullptr),
}