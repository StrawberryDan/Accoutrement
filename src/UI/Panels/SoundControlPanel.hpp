#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "../../Model/SoundPlayer.hpp"
#include "../Events/BotStartedRunningEvent.hpp"
#include "../Events/BotStoppedRunningEvent.hpp"
// Core
#include "Strawberry/Core/IO/ChannelReceiver.hpp"
#include "Strawberry/Core/IO/Receiver.hpp"
// WxWidgets
#include "wx/listctrl.h"
#include "wx/panel.h"

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class SoundControlPanel
		: public wxPanel
		, public Core::IO::ChannelReceiver<SoundPlayerEvent::SoundStarted, SoundPlayerEvent::SoundEnded, SoundPlayerEvent::SoundRepeat>
		, public Core::IO::Receiver<BotStartedRunningEvent, BotStoppedRunningEvent>
	{
		wxDECLARE_EVENT_TABLE();

	public:
		explicit SoundControlPanel(wxWindow* parent);

	protected:
		void Receive(BotStartedRunningEvent value) override;
		void Receive(BotStoppedRunningEvent value) override;
		void Receive(SoundPlayerEvent::SoundStarted value) override;
		void Receive(SoundPlayerEvent::SoundEnded value) override;
		void Receive(SoundPlayerEvent::SoundRepeat value) override;
		void OnRemoveSound(wxCommandEvent& event);
		void OnRepeatSound(wxCommandEvent& event);

	private:
		wxListCtrl* mList         = nullptr;
		wxButton*   mRemoveButton = nullptr;
		wxButton*   mRepeatButton = nullptr;
	};
} // namespace Strawberry::Accoutrement
