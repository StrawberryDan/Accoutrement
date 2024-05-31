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
        REPEAT_SOUND,
        CHANGE_MASTER_VOLUME,
        CHANGE_SOUND_VOLUME,
        LIST_CTRL,
    };


    wxBEGIN_EVENT_TABLE(SoundControlPanel, wxPanel)
        EVT_BUTTON(REMOVE_SOUND, SoundControlPanel::OnRemoveSound)
        EVT_BUTTON(REPEAT_SOUND, SoundControlPanel::OnRepeatSound)
        EVT_COMMAND_SCROLL(CHANGE_MASTER_VOLUME, SoundControlPanel::OnChangeMasterVolume)
        EVT_LIST_ITEM_SELECTED(LIST_CTRL, SoundControlPanel::OnSoundClick)
        EVT_COMMAND_SCROLL(CHANGE_SOUND_VOLUME, SoundControlPanel::OnChangeSoundVolume)
    wxEND_EVENT_TABLE()


    SoundControlPanel::SoundControlPanel(wxWindow* parent)
        : wxPanel(parent)
    {
        auto sizer = new wxGridBagSizer();

        wxImage playingIcon("data/repeat.png");
        playingIcon.Rescale(15, 15);
        auto* imageList = new wxImageList(15, 15);
        imageList->Add(playingIcon);

        mList = new wxListCtrl(this, LIST_CTRL);
        mList->AssignImageList(imageList, wxIMAGE_LIST_SMALL);
        mList->SetWindowStyleFlag(wxLC_LIST);
        sizer->Add(mList, {0, 0}, {2, 1}, wxALL | wxEXPAND, 5);

        mRemoveButton = new wxButton(this, REMOVE_SOUND, "Remove");
        sizer->Add(mRemoveButton, {0, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

        mRepeatButton = new wxButton(this, REPEAT_SOUND, "Repeat");
        sizer->Add(mRepeatButton, {1, 1}, {1, 1}, wxALL | wxALIGN_CENTER, 5);

        auto volumeSizer    = new wxBoxSizer(wxHORIZONTAL);
        mMasterVolumeSlider = new wxSlider(this, CHANGE_MASTER_VOLUME, 100.0, 0.0, 200.0);
        volumeSizer->Add(mMasterVolumeSlider, wxALL | wxEXPAND, 5);

        mSoundVolumeSlider = new wxSlider(this, CHANGE_SOUND_VOLUME, 100.0, 0.0, 200.0);
        volumeSizer->Add(mSoundVolumeSlider, wxALL | wxEXPAND, 5);
        sizer->Add(volumeSizer, {2, 0}, {1, 2}, wxALL | wxEXPAND, 5);

        sizer->AddGrowableRow(0);
        sizer->AddGrowableRow(1);
        sizer->AddGrowableRow(2);
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


    void SoundControlPanel::Receive(SoundPlayer::SoundStartedEvent value)
    {
        auto name = SoundDatabase::Get()->GetSound(Bot::Get()->GetSoundPlayer().Lock()->GetSoundID(value.soundTicket))->
                                          GetName();
        auto itemID = mList->InsertItem(mList->GetItemCount(), name);
        mList->SetItemImage(itemID, value.repeating ? 0 : -1);
        mList->SetItemPtrData(itemID, value.soundTicket);
        mList->Refresh();
    }


    void SoundControlPanel::Receive(SoundPlayer::SoundEndedEvent value)
    {
        for (int i = 0; i < mList->GetItemCount(); i++)
        {
            if (value.soundTicket == mList->GetItemData(i))
            {
                mList->DeleteItem(i);
            }
        }
    }


    void SoundControlPanel::Receive(SoundPlayer::SoundRepeatEvent value)
    {
        for (int i = 0; i < mList->GetItemCount(); i++)
        {
            mList->SetItemImage(i, value.soundTicket == mList->GetItemData(i) && value.repeating ? 0 : -1);
        }
        Layout();
    }


    void SoundControlPanel::OnRemoveSound(wxCommandEvent& event)
    {
        auto selected = mList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (selected == -1) return;

        auto soundID = mList->GetItemData(selected);
        Bot::Get()->GetSoundPlayer().Lock()->RemoveSound(soundID);
    }


    void SoundControlPanel::OnRepeatSound(wxCommandEvent& event)
    {
        auto selected = mList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (selected == -1) return;

        auto soundID     = mList->GetItemData(selected);
        auto soundPlayer = Bot::Get()->GetSoundPlayer().Lock();
        soundPlayer->SetRepeat(soundID, !soundPlayer->GetRepeat(soundID));
    }


    void SoundControlPanel::OnChangeMasterVolume(wxScrollEvent& event)
    {
        Bot::Get()->GetSoundPlayer().Lock()->SetVolume(mMasterVolumeSlider->GetValue());
    }


    void SoundControlPanel::OnChangeSoundVolume(wxScrollEvent& event)
    {
        auto selected = mList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (selected == -1) return;

        Bot::Get()->GetSoundPlayer().Lock()->SetTrackVolume(mList->GetItemData(selected),
                                                            mSoundVolumeSlider->GetValue());
    }


    void SoundControlPanel::OnSoundClick(wxListEvent& event)
    {
        auto selected = mList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (selected == -1) return;

        auto soundID = mList->GetItemData(selected);
        int  volume  = Bot::Get()->GetSoundPlayer().Lock()->GetTrackVolume(soundID);
        mSoundVolumeSlider->SetValue(volume);
    }
} // namespace Strawberry::Accoutrement
