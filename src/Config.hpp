#pragma once


#include "Strawberry/Core/Types/Optional.hpp"
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
            [[nodiscard]] const Core::Optional<std::string>& GetToken() const;
            void                                             SetToken(std::string token);

        private:
            Core::Optional<std::string> mToken;
    };
} // namespace Strawberry::Accoutrement
