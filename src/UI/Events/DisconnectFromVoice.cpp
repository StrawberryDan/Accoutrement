//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "DisconnectedFromVoice.hpp"

namespace Strawberry::Accoutrement
{
    wxDEFINE_EVENT(EVT_TYPE_DISCONNECT_FROM_VOICE, DisconnectedFromVoice);


    DisconnectedFromVoice::DisconnectedFromVoice()
        : wxCommandEvent(EVT_TYPE_DISCONNECT_FROM_VOICE, GetId()) {}


    wxEvent* DisconnectedFromVoice::Clone() const
    {
        return new DisconnectedFromVoice();
    }
} // namespace Strawberry::Accoutrement
