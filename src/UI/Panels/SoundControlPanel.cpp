//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "SoundControlPanel.hpp"
// WxWidgets
#include "wx/button.h"
#include "wx/gbsizer.h"
#include "wx/listctrl.h"


//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	SoundControlPanel::SoundControlPanel(wxWindow* parent)
		: wxPanel(parent)
	{
		auto sizer = new wxGridBagSizer();

		auto list  = new wxListCtrl(this, wxID_ANY);
		sizer->Add(list, {0, 0}, {2, 1}, wxALL | wxEXPAND, 5);

		auto removeButton = new wxButton(this, wxID_ANY, "Remove");
		sizer->Add(removeButton, {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		auto repeatButton = new wxButton(this, wxID_ANY, "Repeat");
		sizer->Add(repeatButton, {1, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

		sizer->AddGrowableRow(0);
		sizer->AddGrowableRow(1);
		sizer->AddGrowableCol(0);
		SetSizerAndFit(sizer);
	}
} // namespace Strawberry::Accoutrement