//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundEffectsPanel.hpp"
// This Project
#include "../../Discord/Bot.hpp"
// WxWidgets
#include "wx/gbsizer.h"
#include "wx/stattext.h"

//======================================================================================================================
//  Component IDs
//----------------------------------------------------------------------------------------------------------------------
enum : wxWindowID
{
	PLAY_ONCE_BUTTON = wxID_HIGHEST,
	PLAY_REPEAT_BUTTON,
	ADD_SOUND_BUTTON,
	REMOVE_SOUND_BUTTON,
};

//======================================================================================================================
//  Event Table Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	wxBEGIN_EVENT_TABLE(SoundEffectsPanel, wxPanel) EVT_BUTTON(PLAY_ONCE_BUTTON, SoundEffectsPanel::OnPlayOnceButton)
		EVT_BUTTON(PLAY_REPEAT_BUTTON, SoundEffectsPanel::OnPlayRepeatButton) EVT_BUTTON(ADD_SOUND_BUTTON, SoundEffectsPanel::OnAddSoundButton)
			EVT_BUTTON(REMOVE_SOUND_BUTTON, SoundEffectsPanel::OnRemoveSoundButton) wxEND_EVENT_TABLE()
}

//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	SoundEffectsPanel::SoundEffectsPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		SetWindowStyle(wxSUNKEN_BORDER);

		auto sizer = new wxGridBagSizer(5, 5);

		sizer->Add(new wxStaticText(this, wxID_ANY, "Sound Effects"), {0, 0}, {1, 2}, wxALL | wxEXPAND, 5);

		mSoundEffectsList = new wxListCtrl(this, wxID_ANY);
		sizer->Add(mSoundEffectsList, {1, 0}, {1, 2}, wxALL | wxEXPAND, 5);

		mPlayOnceButton = new wxButton(this, wxID_ANY, "Play Once");
		sizer->Add(mPlayOnceButton, {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		mPlayRepeatButton = new wxButton(this, wxID_ANY, "Play Repeat");
		sizer->Add(mPlayRepeatButton, {2, 1}, {1, 1}, wxALL | wxEXPAND, 5);

		mAddSoundButton = new wxButton(this, ADD_SOUND_BUTTON, "Add Sound");
		sizer->Add(mAddSoundButton, {3, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		mRemoveSoundButton = new wxButton(this, REMOVE_SOUND_BUTTON, "Remove Sound");
		sizer->Add(mRemoveSoundButton, {3, 1}, {1, 1}, wxALL | wxEXPAND, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);
		SetSizerAndFit(sizer);
	}

	void SoundEffectsPanel::OnPlayOnceButton(wxCommandEvent& event) {}

	void SoundEffectsPanel::OnPlayRepeatButton(wxCommandEvent& event) {}

	void SoundEffectsPanel::OnAddSoundButton(wxCommandEvent& event) {}

	void SoundEffectsPanel::OnRemoveSoundButton(wxCommandEvent& event) {}
} // namespace Strawberry::Accoutrement