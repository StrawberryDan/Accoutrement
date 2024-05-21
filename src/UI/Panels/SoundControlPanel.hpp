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
#include "wx/slider.h"

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class SoundControlPanel
		: public wxPanel
		, public Core::IO::ChannelReceiver<SoundPlayer::SoundStartedEvent, SoundPlayer::SoundEndedEvent, SoundPlayer::SoundRepeatEvent>
		, public Core::IO::Receiver<BotStartedRunningEvent, BotStoppedRunningEvent>
	{
		wxDECLARE_EVENT_TABLE();

	public:
		explicit SoundControlPanel(wxWindow* parent);

	protected:
		void Receive(BotStartedRunningEvent value) override;
		void Receive(BotStoppedRunningEvent value) override;
		void Receive(SoundPlayer::SoundStartedEvent value) override;
		void Receive(SoundPlayer::SoundEndedEvent value) override;
		void Receive(SoundPlayer::SoundRepeatEvent value) override;
		void OnRemoveSound(wxCommandEvent& event);
		void OnRepeatSound(wxCommandEvent& event);
		void OnChangeMasterVolume(wxScrollEvent& event);
		void OnChangeSoundVolume(wxScrollEvent& event);
		void OnSoundClick(wxListEvent& event);

	private:
		wxListCtrl* mList               = nullptr;
		wxButton*   mRemoveButton       = nullptr;
		wxButton*   mRepeatButton       = nullptr;
		wxSlider*   mMasterVolumeSlider = nullptr;
		wxSlider*   mSoundVolumeSlider  = nullptr;
	};
} // namespace Strawberry::Accoutrement
