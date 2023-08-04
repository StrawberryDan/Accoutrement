//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MusicPanel.hpp"
// Wx Widgets
#include "wx/button.h"
#include "wx/listbox.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/gbsizer.h"




namespace Strawberry::Accoutrement
{
	MusicPanel::MusicPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxGridBagSizer(5, 5);

		sizer->Add(new wxStaticText(this, wxID_ANY, "Available Songs"), {0, 0}, {1, 1}, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Playlist"), {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		sizer->Add(new wxListBox(this, wxID_ANY), {1, 0}, {1, 1}, wxEXPAND | wxALL, 5);
		sizer->Add(new wxListBox(this, wxID_ANY), {1, 1}, {1, 1}, wxEXPAND | wxALL, 5);

		sizer->Add(new wxTextCtrl(this, wxID_ANY), {2, 0}, {1, 1}, wxALL | wxEXPAND, 5);

		auto songListButtons = new wxBoxSizer(wxHORIZONTAL);
		songListButtons->Add(new wxButton(this, wxID_ANY, "Enqueue"), 0,  wxALL, 5);
		sizer->Add(songListButtons, {3, 0}, {1, 1}, wxALIGN_CENTER_HORIZONTAL, 5);

		auto playlistButtons = new wxGridBagSizer(5, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Remove"), {0, 0}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Up"), {0, 1}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Down"), {1, 1}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Shuffle"), {0, 2}, {1, 1},  wxEXPAND | wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Repeat"), {1, 2}, {1, 1},  wxEXPAND | wxALL, 5);
		sizer->Add(playlistButtons, {2, 1}, {2, 1}, wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);

		SetSizerAndFit(sizer);
	}
}