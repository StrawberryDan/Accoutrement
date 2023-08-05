//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "ConnectedToVoice.hpp"



namespace Strawberry::Accoutrement
{
	wxDEFINE_EVENT(EVT_TYPE_CONNECTED_TO_VOICE, ConnectedToVoice);


	ConnectedToVoice::ConnectedToVoice(Discord::Entity::Guild guild, Discord::Entity::Channel channel)
		: wxCommandEvent(EVT_TYPE_CONNECTED_TO_VOICE, GetId())
		, mGuild(std::move(guild))
		, mChannel(std::move(channel))
	{}


	wxEvent* ConnectedToVoice::Clone() const
	{
		return new ConnectedToVoice(mGuild, mChannel);
	}
}