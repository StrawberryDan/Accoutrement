#include "Config.hpp"



#include <memory>
#include <filesystem>
#include <fstream>



#include "Standard/Assert.hpp"
#include "nlohmann/json.hpp"



using Strawberry::Standard::Assert;



constexpr const char* kConfigFile = "config.json";
std::unique_ptr<Config> gConfig = nullptr;



void Config::Initialise()
{
	Assert(gConfig == nullptr);

	if (std::filesystem::exists(kConfigFile))
	{
		gConfig = std::make_unique<Config>(Read());
	}
	else
	{
		gConfig = std::make_unique<Config>();
		Config::Dump();
	}
}



Config& Config::Get()
{
	Assert(gConfig != nullptr);
	return *gConfig;
}



Config Config::Read()
{
	using nlohmann::json;

	auto data = json::parse(std::ifstream(kConfigFile));

	Config config;

	config.mToken = data["token"];

	return config;
}



void Config::Dump()
{
	using nlohmann::json;

	json data;
	data["token"] = gConfig->mToken;

	std::ofstream file(kConfigFile, std::ofstream::ate);
	file << data.dump(1, '\t', false);
}



const std::string& Config::GetToken() const
{
	return mToken;
}
