cmake_minimum_required(VERSION 3.20)
project(Accoutrement)
set(CMAKE_CXX_STANDARD 23)
include(FetchContent)


# Get wxWidgets
message(STATUS "Configuring wxWidgets")
set(wxBUILD_CXX_STANDARD 20)
set(wxBUILD_SHARED OFF)
set(wxBUILD_MONOLITHIC ON)
FetchContent_Declare(wxWidgets
	GIT_REPOSITORY "https://github.com/wxWidgets/wxWidgets.git")
FetchContent_MakeAvailable(wxWidgets)


# MODULES
add_subdirectory(../Core ${CMAKE_CURRENT_BINARY_DIR}/Core)
add_subdirectory(../Codec ${CMAKE_CURRENT_BINARY_DIR}/Codec)
add_subdirectory(../Discord ${CMAKE_CURRENT_BINARY_DIR}/Discord)


add_executable(Accoutrement
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
		src/UI/Panels/SoundEffectsPanel.cpp
		src/UI/Panels/SoundEffectsPanel.hpp
		src/UI/Panels/SoundControlPanel.cpp
		src/UI/Panels/SoundControlPanel.hpp
		src/Model/SongDatabase.cpp
		src/Model/SongDatabase.hpp
		src/Model/Song.cpp
		src/Model/Song.hpp
		src/Model/SoundDatabase.cpp
		src/Model/SoundDatabase.hpp
		src/Model/Sound.cpp
		src/Model/Sound.hpp
		src/Model/SoundPlayer.cpp
		src/Model/SoundPlayer.hpp
)


target_link_libraries(Accoutrement PRIVATE StrawberryDiscord StrawberryCodec StrawberryCore wx::mono)