//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "GuildCreated.hpp"


namespace Strawberry::Accoutrement
{
	wxDEFINE_EVENT(EVT_TYPE_GUILD_CREATED, GuildCreated);


	GuildCreated::GuildCreated(Discord::Entity::Guild guild)
		: wxCommandEvent(EVT_TYPE_GUILD_CREATED, GetId())
		, mGuild(std::move(guild))
	{
	}


	const Discord::Entity::Guild& GuildCreated::GetGuild()
	{
		return mGuild;
	}


	wxEvent* GuildCreated::Clone() const
	{
		return new GuildCreated(mGuild);
	}
}// namespace Strawberry::Accoutrement