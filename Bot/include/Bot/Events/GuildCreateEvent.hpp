#pragma once



#include <nlohmann/json.hpp>
#include "Bot/Snowflake.hpp"
#include "Bot/Error.hpp"
#include "Util/Result.hpp"
#include "Bot/Entities/Guild.hpp"



class GuildCreateEvent
{
public:
	// Constructors
	static Result<GuildCreateEvent, Error> Parse(const nlohmann::json& json);



private:
	GuildCreateEvent() = default;



private:
	Guild mGuild;
};
