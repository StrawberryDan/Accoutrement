add_executable(
        Accoutrement
        src/Main.cpp src/UI/MainWindow.cpp src/UI/MainWindow.hpp src/UI/ChannelSelector.cpp src/UI/ChannelSelector.hpp)
target_link_libraries(Accoutrement PRIVATE Bot Codec wxmono Util)