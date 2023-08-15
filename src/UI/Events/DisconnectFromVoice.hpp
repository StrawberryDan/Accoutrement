#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include "wx/event.h"


namespace Strawberry::Accoutrement
{
	class DisconnectFromVoice
		: public wxCommandEvent
	{
	public:
		DisconnectFromVoice();


	private:
		wxEvent* Clone() const override;
	};


	wxDECLARE_EVENT(EVT_TYPE_DISCONNECT_FROM_VOICE, DisconnectFromVoice);


#define EVT_DISCONNECTED_FROM_VOICE(id, fn) \
	DECLARE_EVENT_TABLE_ENTRY(EVT_TYPE_DISCONNECT_FROM_VOICE, id, wxID_ANY, (&fn), nullptr),
} // namespace Strawberry::Accoutrement