//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "DisconnectFromVoice.hpp"


namespace Strawberry::Accoutrement
{
	wxDEFINE_EVENT(EVT_TYPE_DISCONNECT_FROM_VOICE, DisconnectFromVoice);


	DisconnectFromVoice::DisconnectFromVoice()
		: wxCommandEvent(EVT_TYPE_DISCONNECT_FROM_VOICE, GetId())
	{

	}


	wxEvent* DisconnectFromVoice::Clone() const
	{
		return new DisconnectFromVoice();
	}
}