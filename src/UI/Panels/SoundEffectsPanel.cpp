//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundEffectsPanel.hpp"
// WxWidgets
#include "wx/gbsizer.h"
#include "wx/stattext.h"


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

		sizer->Add(new wxStaticText(this, wxID_ANY, "Sound Effects"), {0, 0}, {1, 2}, wxALL | wxALIGN_CENTER, 5);

		mSoundEffectsList = new wxDataViewTreeCtrl(this, wxID_ANY);
		sizer->Add(mSoundEffectsList, {1, 0}, {1, 2}, wxALL | wxEXPAND, 5);

		mPlayButton = new wxButton(this, wxID_ANY, "Play");
		sizer->Add(mPlayButton, {2, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		mStopButton = new wxButton(this, wxID_ANY, "Stop");
		sizer->Add(mStopButton, {2, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);
		SetSizerAndFit(sizer);
	}
}