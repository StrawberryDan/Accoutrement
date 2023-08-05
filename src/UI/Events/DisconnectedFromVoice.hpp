#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include "wx/event.h"


namespace Strawberry::Accoutrement
{
	class DisconnectedFromVoice
		: public wxCommandEvent
	{
	public:
		DisconnectedFromVoice();


	private:
		wxEvent* Clone() const override;
	};


	wxDECLARE_EVENT(EVT_TYPE_DISCONNECTED_FROM_VOICE, DisconnectedFromVoice);


#define EVT_DISCONNECTED_FROM_VOICE(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY(EVT_TYPE_DISCONNECTED_FROM_VOICE, id, wxID_ANY, (&fn), nullptr),
}