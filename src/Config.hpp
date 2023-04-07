#pragma once



#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/usr/include/c++/v1/string"



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
