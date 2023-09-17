#include "Config.hpp"


#include <filesystem>
#include <fstream>
#include <memory>


#include "Strawberry/Core/Util/Assert.hpp"
#include "nlohmann/json.hpp"


using Strawberry::Core::Assert;


namespace Strawberry::Accoutrement
{
	constexpr const char* kConfigFile = "config.json";
	std::unique_ptr<Config> gConfig = nullptr;


	void Config::Initialise()
	{
		Assert(gConfig == nullptr);

		if (std::filesystem::exists(kConfigFile)) gConfig = std::make_unique<Config>(Read());
		else gConfig = std::make_unique<Config>();
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

		if (data.contains("token"))
		{
			std::string tokenString = data["token"];
			config.mToken = tokenString.empty() ? Core::NullOpt : Core::Optional(tokenString);
		}

		return config;
	}


	void Config::Dump()
	{
		using nlohmann::json;

		json data;
		data["token"] = gConfig->mToken.ValueOr("");

		std::ofstream file(kConfigFile, std::ofstream::ate);
		file << data.dump(1, '\t', false);
	}


	const Core::Optional<std::string>& Config::GetToken() const
	{
		return mToken;
	}
} // namespace Strawberry::Accoutrement