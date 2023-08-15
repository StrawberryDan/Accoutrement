#pragma once


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
		[[nodiscard]] const std::string& GetToken() const;


	private:
		std::string mToken;
	};
}