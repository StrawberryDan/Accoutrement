//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundEffectsPanel.hpp"
// This Project
#include "../../Discord/Bot.hpp"
#include "../../Model/SoundDatabase.hpp"
// WxWidgets
#include "wx/filedlg.h"
#include "wx/gbsizer.h"
#include "wx/stattext.h"
#include "wx/textdlg.h"

//======================================================================================================================
//  Component IDs
//----------------------------------------------------------------------------------------------------------------------
enum : wxWindowID
{
	PLAY_ONCE_BUTTON = wxID_HIGHEST,
	PLAY_REPEAT_BUTTON,
	ADD_SOUND_BUTTON,
	REMOVE_SOUND_BUTTON,
	RENAME_SOUND_BUTTON,
};

//======================================================================================================================
//  Event Table Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	wxBEGIN_EVENT_TABLE(SoundEffectsPanel, wxPanel) EVT_BUTTON(PLAY_ONCE_BUTTON, SoundEffectsPanel::OnPlayOnceButton)
		EVT_BUTTON(PLAY_REPEAT_BUTTON, SoundEffectsPanel::OnPlayRepeatButton) EVT_BUTTON(ADD_SOUND_BUTTON, SoundEffectsPanel::OnAddSoundButton)
			EVT_BUTTON(REMOVE_SOUND_BUTTON, SoundEffectsPanel::OnRemoveSoundButton) EVT_BUTTON(RENAME_SOUND_BUTTON, SoundEffectsPanel::OnRenameSoundButton)
				wxEND_EVENT_TABLE()
}

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	SoundEffectsPanel::SoundEffectsPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxGridBagSizer();

		sizer->Add(new wxStaticText(this, wxID_ANY, "Sound Effects"), {0, 0}, {1, 2}, wxALL | wxEXPAND, 5);

		mSoundEffectsList = new wxListCtrl(this, wxID_ANY);
		mSoundEffectsList->SetWindowStyle(wxLC_LIST);
		sizer->Add(mSoundEffectsList, {1, 0}, {1, 2}, wxALL | wxEXPAND, 5);

		mPlayOnceButton = new wxButton(this, PLAY_ONCE_BUTTON, "Play Once");
		sizer->Add(mPlayOnceButton, {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		mPlayRepeatButton = new wxButton(this, PLAY_REPEAT_BUTTON, "Play Repeat");
		sizer->Add(mPlayRepeatButton, {2, 1}, {1, 1}, wxALL | wxEXPAND, 5);

		mAddSoundButton = new wxButton(this, ADD_SOUND_BUTTON, "Add Sound");
		sizer->Add(mAddSoundButton, {3, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		mRemoveSoundButton = new wxButton(this, REMOVE_SOUND_BUTTON, "Remove Sound");
		sizer->Add(mRemoveSoundButton, {3, 1}, {1, 1}, wxALL | wxEXPAND, 5);

		mRenameSoundButton = new wxButton(this, RENAME_SOUND_BUTTON, "Rename Sound");
		sizer->Add(mRenameSoundButton, {4, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);
		SetSizerAndFit(sizer);

		auto soundDatabase = SoundDatabase::Get();
		for (int i = 0; i < soundDatabase->Count(); i++)
		{
			auto sound = soundDatabase->GetSound(i);
			if (sound)
			{
				auto item = mSoundEffectsList->InsertItem(mSoundEffectsList->GetItemCount(), sound->GetName());
				mSoundEffectsList->SetItemPtrData(item, i);
			}
		}
	}

	void SoundEffectsPanel::OnPlayOnceButton(wxCommandEvent& event)
	{
		if (Bot::Get())
		{
			auto selectedItem = mSoundEffectsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (selectedItem == -1) return;

			auto soundID = mSoundEffectsList->GetItemData(selectedItem);
			auto sound = SoundDatabase::Get()->GetSound(soundID);
			Core::Assert(sound.HasValue());

			Bot::Get()->GetSoundPlayer().Lock()->PlaySound(soundID, false);
		}
	}

	void SoundEffectsPanel::OnPlayRepeatButton(wxCommandEvent& event)
	{
		if (Bot::Get())
		{
			auto selectedItem = mSoundEffectsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
			if (selectedItem == -1) return;

			auto soundID = mSoundEffectsList->GetItemData(selectedItem);
			auto sound = SoundDatabase::Get()->GetSound(soundID);
			Core::Assert(sound.HasValue());

			Bot::Get()->GetSoundPlayer().Lock()->PlaySound(soundID, true);
		}
	}

	void SoundEffectsPanel::OnAddSoundButton(wxCommandEvent& event)
	{
		wxFileDialog fileDialog(this, "Choose a sound file...");
		fileDialog.SetWindowStyle(wxFD_OPEN | wxFD_MULTIPLE | wxFD_FILE_MUST_EXIST);
		auto dialogResult = fileDialog.ShowModal();
		if (dialogResult == wxID_CANCEL) { return; }
		else if (dialogResult == wxID_OK)
		{
			wxArrayString paths;
			fileDialog.GetPaths(paths);
			for (const auto& path : paths)
			{
				auto fullPath = std::filesystem::absolute(std::string(path));
				auto sound    = Sound::FromFile(fullPath);
				if (!sound) return;

				std::string title      = sound.Value().GetName();
				auto        index      = mSoundEffectsList->InsertItem(mSoundEffectsList->GetItemCount(), title);
				auto        soundIndex = SoundDatabase::Get()->AddSound(sound.Unwrap());
				mSoundEffectsList->SetItemPtrData(index, soundIndex);
			}
		}
	}

	void SoundEffectsPanel::OnRemoveSoundButton(wxCommandEvent& event)
	{
		auto selectedItem = mSoundEffectsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (selectedItem == -1) return;
		auto soundID = mSoundEffectsList->GetItemData(selectedItem);
		mSoundEffectsList->DeleteItem(selectedItem);
		SoundDatabase::Get()->RemoveSound(soundID);
	}

	void SoundEffectsPanel::OnRenameSoundButton(wxCommandEvent& event)
	{
		auto selectedItem = mSoundEffectsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		if (selectedItem == -1) return;
		auto soundID = mSoundEffectsList->GetItemData(selectedItem);
		auto sound   = SoundDatabase::Get()->GetSound(soundID);
		Core::Assert(sound.HasValue());

		auto strDialog = new wxTextEntryDialog(this, "Choose a new name:");
		strDialog->SetValue(sound->GetName());
		if (strDialog->ShowModal() == wxID_OK)
		{
			auto newName = strDialog->GetValue();
			mSoundEffectsList->SetItemText(selectedItem, newName);
			SoundDatabase::Get()->GetSound(soundID).Value()->SetName(static_cast<std::string>(newName));
		}
	}
} // namespace Strawberry::Accoutrement