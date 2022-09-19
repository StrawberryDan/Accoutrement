add_library(Bot STATIC
        include/Bot/Bot.hpp
        src/Bot.cpp)
target_include_directories(Bot PRIVATE include Codec)
target_link_libraries(Bot PRIVATE Codec libretls)