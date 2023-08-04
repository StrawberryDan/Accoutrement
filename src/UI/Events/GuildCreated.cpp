//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "GuildCreated.hpp"


namespace Strawberry::Accoutrement
{
	GuildCreated::GuildCreated(Discord::Entity::Guild guild)
			: mGuild(std::move(guild)) {}


	const Discord::Entity::Guild& GuildCreated::GetGuild()
	{
		return mGuild;
	}


	wxEvent* GuildCreated::Clone() const
	{
		return new GuildCreated(mGuild);
	}
}