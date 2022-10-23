add_library(Bot STATIC
        include/Bot/Bot.hpp
        src/Bot.cpp
        src/Heartbeater.cpp
        src/Heartbeat.hpp src/Gateway.hpp src/Gateway.cpp include/Bot/Snowflake.hpp src/Snowflake.cpp include/Bot/Behaviour.hpp include/Bot/Events/GuildCreateEvent.hpp include/Bot/Events/ReadyEvent.hpp)
target_include_directories(Bot PUBLIC include)
target_link_libraries(Bot PUBLIC Codec Util)


add_executable(Bot_Test
        test/Main.cpp)
target_link_libraries(Bot_Test PRIVATE Bot)