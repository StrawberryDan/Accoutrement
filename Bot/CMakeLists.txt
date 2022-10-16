add_library(Bot STATIC
        include/Bot/Bot.hpp
        src/Bot.cpp
        src/Heartbeater.cpp
        src/Heartbeat.hpp src/Gateway.hpp src/Gateway.cpp)
target_include_directories(Bot PUBLIC include)
target_link_libraries(Bot PUBLIC Codec Util)


add_executable(Bot_Test
        test/Main.cpp)
target_link_libraries(Bot_Test PRIVATE Bot)