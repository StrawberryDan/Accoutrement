cmake_minimum_required(VERSION 3.0)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 23)


# MODULES
add_subdirectory(../Core    ${CMAKE_CURRENT_BINARY_DIR}/Core)
add_subdirectory(../Codec   ${CMAKE_CURRENT_BINARY_DIR}/Codec)
add_subdirectory(../Discord ${CMAKE_CURRENT_BINARY_DIR}/Discord)


find_package(wxWidgets REQUIRED
        COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(
        Accoutrement
        src/Main.cpp
        src/UI/MainWindow.cpp
        src/UI/MainWindow.hpp
		src/UI/Panels/ChannelSelector.cpp
		src/UI/Panels/ChannelSelector.hpp
        src/Discord/Bot.hpp
        src/Discord/Bot.cpp
        src/Config.cpp
        src/Config.hpp
		src/UI/Events/GuildCreated.cpp
		src/UI/Events/GuildCreated.hpp
		src/UI/Panels/MusicPanel.cpp
		src/UI/Panels/MusicPanel.hpp
		src/UI/Events/ConnectToVoice.cpp
		src/UI/Events/ConnectToVoice.hpp
		src/UI/Events/DisconnectFromVoice.cpp
		src/UI/Events/DisconnectFromVoice.hpp
		src/UI/Panels/NowPlayingPanel.cpp
		src/UI/Panels/NowPlayingPanel.hpp
)
target_include_directories(Accoutrement PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(Accoutrement PRIVATE StrawberryDiscord StrawberryCodec StrawberryCore ${wxWidgets_LIBRARIES})