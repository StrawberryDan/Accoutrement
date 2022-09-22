add_library(Bot STATIC
        include/Bot/Bot.hpp
        src/Bot.cpp
        src/TLSSocket.hpp
        src/TLSSocket.cpp)
target_include_directories(Bot PUBLIC include)
target_link_libraries(Bot PRIVATE Codec Util libretls)


add_executable(Bot_Test
        test/Main.cpp)
target_link_libraries(Bot_Test PRIVATE Bot)