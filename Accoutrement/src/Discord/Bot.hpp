#pragma once



#include "Discord/Bot.hpp"



class Bot
	: public Strawberry::Discord::Bot
{
public:
	static void Initialise();

	static void Run();

	static void Stop();

	static Bot& Get();



public:
	Bot();
};