#pragma once



#include <string>



class Config
{
public:
	static void Initialise();

	static Config& Get();

	static Config Read();

	static void Dump();



public:
	const std::string& GetToken() const;



private:
	std::string mToken;
};