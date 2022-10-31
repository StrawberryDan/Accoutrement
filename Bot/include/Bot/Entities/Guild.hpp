#pragma once



#include "Bot/Snowflake.hpp"
#include "Channel.hpp"
#include "Bot/Error.hpp"
#include "nlohmann/json.hpp"
#include "Util/Result.hpp"
#include <string>



class Guild
{
public:
	static Result<Guild, Error> Parse(const nlohmann::json& json);



public:
	const Snowflake& Id() const { return mGuildId; }
	const std::string& Name() const { return mGuildName; }



private:
	Snowflake            mGuildId;
	std::string          mGuildName;
	std::vector<Channel> mChannels;
};