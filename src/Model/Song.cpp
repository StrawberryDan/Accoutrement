//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
#include "Song.hpp"
// Codec
#include "Codec/MediaFile.hpp"


//======================================================================================================================
//  Class Definitions
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
    Core::Optional<Song> Song::FromFile(const std::filesystem::path& filePath)
    {
        Song song;

        std::filesystem::path absolutePath(filePath);
        if (!std::filesystem::exists(absolutePath)) return Core::NullOpt;
        absolutePath = std::filesystem::absolute(absolutePath);

        auto file = Codec::MediaFile::Open(filePath);
        if (!file) return Core::NullOpt;

        auto stream = file->GetBestStream(Codec::MediaType::Audio);
        if (!stream) return Core::NullOpt;

        song.mTitle  = stream->GetTitle();
        song.mAlbum  = stream->GetAlbum();
        song.mArtist = stream->GetArtist();
        song.mPath   = absolutePath;

        return song;
    }


    std::string Song::GetTitle() const
    {
        std::string fileName = GetPath().filename().string();
        return mTitle.ValueOr(fileName);
    }


    const Core::Optional<std::string>& Song::GetAlbum() const
    {
        return mAlbum;
    }


    const Core::Optional<std::string>& Song::GetArtist() const
    {
        return mArtist;
    }


    const std::filesystem::path& Song::GetPath() const
    {
        return mPath;
    }


    void Song::SetTitle(const std::string& title)
    {
        mTitle = title;
    }


    nlohmann::json Song::ToJSON() const
    {
        nlohmann::json json;
        json["path"]  = GetPath();
        json["title"] = GetTitle();
        return json;
    }
} // namespace Strawberry::Accoutrement
