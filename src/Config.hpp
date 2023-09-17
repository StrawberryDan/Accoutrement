#pragma once

#include "Strawberry/Core/Util/Optional.hpp"
#include <string>


namespace Strawberry::Accoutrement
{
	class Config
	{
	public:
		static void Initialise();

		static Config& Get();

		static Config Read();

		static void Dump();


	public:
		[[nodiscard]] const Core::Optional<std::string>& GetToken() const;


	private:
		Core::Optional<std::string> mToken;
	};
} // namespace Strawberry::Accoutrement