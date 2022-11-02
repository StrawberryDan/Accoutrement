add_executable(
        Accoutrement
        src/Main.cpp src/MainWindow.cpp src/MainWindow.hpp src/ChannelSelector.cpp src/ChannelSelector.hpp)
target_link_libraries(Accoutrement PRIVATE Bot Codec wxmono Util)