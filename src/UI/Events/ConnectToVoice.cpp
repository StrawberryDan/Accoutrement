//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "ConnectToVoice.hpp"



namespace Strawberry::Accoutrement
{
	wxDEFINE_EVENT(EVT_TYPE_CONNECT_TO_VOICE, ConnectToVoice);


	ConnectToVoice::ConnectToVoice(Discord::Entity::Guild guild, Discord::Entity::Channel channel)
		: wxCommandEvent(EVT_TYPE_CONNECT_TO_VOICE, GetId())
		, mGuild(std::move(guild))
		, mChannel(std::move(channel))
	{}


	wxEvent* ConnectToVoice::Clone() const
	{
		return new ConnectToVoice(mGuild, mChannel);
	}
}