//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundControlPanel.hpp"
#include "../../Discord/Bot.hpp"
// WxWidgets
#include "../../Model/SoundDatabase.hpp"
#include "wx/button.h"
#include "wx/gbsizer.h"
#include "wx/listctrl.h"

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	enum : wxWindowID
	{
		REMOVE_SOUND = wxID_HIGHEST,
		REPEAT_SOUND
	};

	wxBEGIN_EVENT_TABLE(SoundControlPanel, wxPanel) EVT_BUTTON(REMOVE_SOUND, SoundControlPanel::OnRemoveSound)
		EVT_BUTTON(REPEAT_SOUND, SoundControlPanel::OnRepeatSound) wxEND_EVENT_TABLE()

			SoundControlPanel::SoundControlPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxGridBagSizer();

		mList      = new wxListCtrl(this, wxID_ANY);
		mList->SetWindowStyleFlag(wxLC_LIST);
		sizer->Add(mList, {0, 0}, {2, 1}, wxALL | wxEXPAND, 5);

		mRemoveButton = new wxButton(this, REMOVE_SOUND, "Remove");
		sizer->Add(mRemoveButton, {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mRepeatButton = new wxButton(this, REPEAT_SOUND, "Repeat");
		sizer->Add(mRepeatButton, {1, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(0);
		sizer->AddGrowableRow(1);
		sizer->AddGrowableCol(0);
		SetSizerAndFit(sizer);

		if (Bot::Get()) Bot::Get()->GetSoundPlayer().Lock()->Register(this);
	}

	void SoundControlPanel::Receive(BotStartedRunningEvent value)
	{
		Bot::Get()->GetSoundPlayer().Lock()->Register(this);
	}

	void SoundControlPanel::Receive(BotStoppedRunningEvent value)
	{
		Bot::Get()->GetSoundPlayer().Lock()->Unregister(this);
	}

	void SoundControlPanel::Receive(SoundPlayerEvent::SoundStarted value)
	{
		auto itemID = mList->InsertItem(mList->GetItemCount(), value.sound->GetName());
		mList->SetItemPtrData(itemID, value.soundID);

		mList->SetItemTextColour(itemID, value.repeating ? *wxBLACK : mList->GetTextColour());
		mList->SetItemBackgroundColour(itemID, value.repeating ? *wxGREEN : mList->GetBackgroundColour());
	}

	void SoundControlPanel::Receive(SoundPlayerEvent::SoundEnded value)
	{
		for (int i = 0; i < mList->GetItemCount(); i++)
		{
			if (value.songID == mList->GetItemData(i)) { mList->DeleteItem(i); }
		}
	}

	void SoundControlPanel::Receive(SoundPlayerEvent::SoundRepeat value)
	{
		for (int i = 0; i < mList->GetItemCount(); i++)
		{
			if (value.songID == mList->GetItemData(i)) { mList->SetItemTextColour(i, value.repeating ? *wxBLACK : mList->GetTextColour()); }
			if (value.songID == mList->GetItemData(i)) { mList->SetItemBackgroundColour(i, value.repeating ? *wxGREEN : mList->GetBackgroundColour()); }
		}
	}

	void SoundControlPanel::OnRemoveSound(wxCommandEvent& event)
	{
		auto selected = mList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (selected == -1) return;

		auto soundID = mList->GetItemData(selected);
		mList->DeleteItem(selected);
		Bot::Get()->GetSoundPlayer().Lock()->RemoveSound(soundID);
	}

	void SoundControlPanel::OnRepeatSound(wxCommandEvent& event)
	{
		auto selected = mList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (selected == -1) return;

		auto soundID = mList->GetItemData(selected);
		Bot::Get()->GetSoundPlayer().Lock()->SetRepeat(soundID, !Bot::Get()->GetSoundPlayer().Lock()->GetRepeat(soundID));
	}
} // namespace Strawberry::Accoutrement