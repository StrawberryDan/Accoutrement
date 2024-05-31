#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "../Events/BotStartedRunningEvent.hpp"
#include "../Events/BotStoppedRunningEvent.hpp"
// Codec
#include "Codec/Audio/Playlist.hpp"
// Core
#include "Strawberry/Core/IO/ChannelReceiver.hpp"
#include "Strawberry/Core/IO/Receiver.hpp"
// wxWidgets
#include "wx/button.h"
#include "wx/panel.h"
#include "wx/stattext.h"

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
    class NowPlayingPanel
            : public wxPanel, public Codec::Audio::Playlist::EventReceiver, public Core::IO::Receiver<BotStartedRunningEvent, BotStoppedRunningEvent>
    {
            wxDECLARE_EVENT_TABLE();

        public:
            explicit NowPlayingPanel(wxWindow* parent);

        protected:
            void NextSong(wxCommandEvent& event);
            void PrevSong(wxCommandEvent& event);


            void Receive(Codec::Audio::Playlist::SongBeganEvent event) override final;
            void Receive(Codec::Audio::Playlist::PlaybackEndedEvent event) override final;
            void Receive(BotStartedRunningEvent event) override final;
            void Receive(BotStoppedRunningEvent event) override final;
            void Receive(Codec::Audio::Playlist::SongAddedEvent value) override;
            void Receive(Codec::Audio::Playlist::SongRemovedEvent value) override;

        private:
            wxStaticText* mSongTitle;
            wxButton*     mPlayPauseButton;
            wxButton*     mPrevSongButton;
            wxButton*     mNextSongButton;
    };
} // namespace Strawberry::Accoutrement
