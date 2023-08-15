#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// wxWidgets
#include "wx/button.h"
#include "wx/listctrl.h"
#include "wx/panel.h"


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class SoundEffectsPanel
		: public wxPanel
	{
	public:
		SoundEffectsPanel(wxWindow* parent);


	private:
		wxListCtrl* mSoundEffectsList = nullptr;
		wxButton*   mPlayButton       = nullptr;
		wxButton*   mStopButton       = nullptr;
	};
}// namespace Strawberry::Accoutrement
