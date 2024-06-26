#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// This Project
#include "Sound.hpp"
// Core
#include "Strawberry/Core/Sync/Mutex.hpp"
#include "Strawberry/Core/Types/Optional.hpp"
// Standard Library
#include <cstddef>
#include <map>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
    class SoundDatabase
    {
        public:
            friend class Core::Mutex<SoundDatabase>;


            static Core::MutexGuard<SoundDatabase> Get();

        public:
            using Id = size_t;

        public:
            /// Writes contents to file on shutdown
            ~SoundDatabase();
            /// Adds a new sound to the database. Returns the id of the added sound.
            Id AddSound(Sound sound);
            /// Removes the sound associated with the given Id.
            void RemoveSound(Id id);
            /// Returns an pointer to the sound with the given id if it exists.
            [[nodiscard]] Core::Optional<Sound*> GetSound(Id id);
            /// Returns the number of sounds in the database.
            size_t Count() const;

        public:
            /// Loads database from file
            SoundDatabase();

        private:
            Id                  mNextId = 0;
            std::map<Id, Sound> mSounds;
    };
} // namespace Strawberry::Accoutrement
