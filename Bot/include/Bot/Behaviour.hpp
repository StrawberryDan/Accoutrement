#pragma once



#include "Bot/Events/GuildCreateEvent.hpp"
#include "Bot/Events/ReadyEvent.hpp"



class Behaviour
{
public:
	virtual ~Behaviour() = default;



public:
	virtual void OnReady(const ReadyEvent& event) {}
	virtual void OnGuildCreate(const GuildCreateEvent& event) {}
};
