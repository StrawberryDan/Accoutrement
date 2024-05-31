#pragma once

//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// Codec
#include "Codec/Packet.hpp"
#include "Codec/Audio/Frame.hpp"
#include "Codec/MediaFile.hpp"
#include "Codec/Audio/Decoder.hpp"
// Core
#include "Strawberry/Core/Types/Optional.hpp"
// JSON
#include "nlohmann/json.hpp"
// Standard Library
#include <filesystem>
#include <deque>

//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
    class Sound
    {
        public:
            /// Loads a sound from a media file.
            static Core::Optional<Sound> FromFile(const std::filesystem::path& path);
            /// Loads a sound from a json entry
            static Core::Optional<Sound> FromJSON(const nlohmann::json& json);
            /// Serialised a sound into json
            [[nodiscard]] nlohmann::json AsJSON() const;


            /// Returns a constant reference to the index'th audio frame in the sound.
            [[nodiscard]] Core::Optional<Codec::Audio::Frame> Read();

            /// Returns the name of the sound.
            [[nodiscard]] const std::string& GetName() const
            {
                return mName;
            }


            /// Returns the path to the sound file.
            [[nodiscard]] const std::filesystem::path& GetPath() const
            {
                return mFile.GetPath();
            }


            /// Sets the name of the sound
            void SetName(const std::string& name)
            {
                mName = name;
            }


            void Seek(Core::Seconds time);

        protected:
            Sound(Codec::MediaFile file);

        private:
            std::string                                mName;
            Codec::MediaFile                           mFile;
            Core::ReflexivePointer<Codec::MediaStream> mStream;
            Codec::Audio::Decoder                      mDecoder;

            std::vector<Codec::Packet>      mBufferedPackets;
            std::deque<Codec::Audio::Frame> mBufferedFrames;
            Core::Seconds                   mPosition;
            Core::Seconds                   mDecodePosition;
    };
} // namespace Strawberry::Accoutrement
