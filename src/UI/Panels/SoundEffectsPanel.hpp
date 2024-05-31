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
    class SoundEffectsPanel : public wxPanel
    {
            wxDECLARE_EVENT_TABLE();

        public:
            SoundEffectsPanel(wxWindow* parent);

        protected:
            void OnPlayOnceButton(wxCommandEvent& event);
            void OnPlayRepeatButton(wxCommandEvent& event);
            void OnAddSoundButton(wxCommandEvent& event);
            void OnRemoveSoundButton(wxCommandEvent& event);
            void OnRenameSoundButton(wxCommandEvent& event);

        private:
            wxListCtrl* mSoundEffectsList  = nullptr;
            wxButton*   mPlayOnceButton    = nullptr;
            wxButton*   mPlayRepeatButton  = nullptr;
            wxButton*   mAddSoundButton    = nullptr;
            wxButton*   mRemoveSoundButton = nullptr;
            wxButton*   mRenameSoundButton = nullptr;
    };
} // namespace Strawberry::Accoutrement
