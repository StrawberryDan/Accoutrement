//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "ConnectedToVoice.hpp"



namespace Strawberry::Accoutrement
{
	ConnectedToVoice::ConnectedToVoice(Discord::Entity::Guild guild, Discord::Entity::Channel channel)
		: mGuild(std::move(guild))
		, mChannel(std::move(channel))
	{}


	wxEvent* ConnectedToVoice::Clone() const
	{
		return new ConnectedToVoice(mGuild, mChannel);
	}
}