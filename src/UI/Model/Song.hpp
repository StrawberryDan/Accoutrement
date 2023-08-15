#pragma once


//======================================================================================================================
//  Includes
//----------------------------------------------------------------------------------------------------------------------
// JSON
#include "nlohmann/json.hpp"
// Core
#include "Strawberry/Core/Util/Option.hpp"
// Standard Library
#include <cstdint>
#include <filesystem>
#include <string>


//======================================================================================================================
//  Class Declaration
//----------------------------------------------------------------------------------------------------------------------
namespace Strawberry::Accoutrement
{
	class Song
	{
	public:
		static Core::Option<Song> FromFile(const std::string& filePath);


		[[nodiscard]] std::string                      GetTitle() const;
		[[nodiscard]] const Core::Option<std::string>& GetAlbum() const;
		[[nodiscard]] const Core::Option<std::string>& GetArtist() const;
		[[nodiscard]] const std::filesystem::path&     GetPath() const;


		void SetTitle(const std::string& title);


		bool operator==(const Song& rhs) const { return mPath == rhs.mPath; }


		bool operator!=(const Song& rhs) const { return mPath != rhs.mPath; }


		[[nodiscard]] nlohmann::json ToJSON() const;


	private:
		Song() = default;


	private:
		Core::Option<std::string> mTitle;
		Core::Option<std::string> mAlbum;
		Core::Option<std::string> mArtist;
		std::filesystem::path     mPath;
	};
}// namespace Strawberry::Accoutrement