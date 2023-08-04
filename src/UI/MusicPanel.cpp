//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "MusicPanel.hpp"
// Wx Widgets
	#include "wx/sizer.h"
#include "wx/listbox.h"
#include "wx/stattext.h"
#include "wx/button.h"



namespace Strawberry::Accoutrement
{
	MusicPanel::MusicPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxFlexGridSizer(3, 2, 5, 5);
		sizer->AddGrowableRow(1, 1);
		sizer->AddGrowableCol(0, 1);
		sizer->AddGrowableCol(1, 1);

		sizer->Add(new wxStaticText(this, wxID_ANY, "Available Songs"), 0, wxALL | wxALIGN_CENTER, 5);
		sizer->Add(new wxStaticText(this, wxID_ANY, "Playlist"), 0, wxALL | wxALIGN_CENTER, 5);

		sizer->Add(new wxListBox(this, wxID_ANY), 1, wxEXPAND | wxALL, 5);
		sizer->Add(new wxListBox(this, wxID_ANY), 1, wxEXPAND | wxALL, 5);

		auto songListButtons = new wxBoxSizer(wxHORIZONTAL);
		songListButtons->Add(new wxButton(this, wxID_ANY, "Enqueue"), 0,  wxALL, 5);
		sizer->Add(songListButtons, 1, wxALIGN_CENTER_HORIZONTAL, 5);

		auto playlistButtons = new wxBoxSizer(wxHORIZONTAL);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Remove"), 0,  wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Up"), 0,  wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Move Down"), 0,  wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Shuffle"), 0,  wxALL, 5);
		playlistButtons->Add(new wxButton(this, wxID_ANY, "Repeat"), 0,  wxALL, 5);
		sizer->Add(playlistButtons, 1, wxALIGN_CENTER_HORIZONTAL, 5);

		SetSizerAndFit(sizer);
	}
}