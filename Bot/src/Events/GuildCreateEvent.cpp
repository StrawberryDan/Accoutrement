#include "Bot/Events/GuildCreateEvent.hpp"



#include "Util/Assert.hpp"



Result<GuildCreateEvent, Error> GuildCreateEvent::Parse(const nlohmann::json& json)
{
	if(json["op"] != 0 || json["t"] != "GUILD_CREATE")
	{
		return Error(Error::Type::InvalidJSON);
	}

	auto info = json["d"];

	GuildCreateEvent event;
	event.mGuild = Guild::Parse(info).Unwrap();

	return event;
}


