qt_add_executable(devlink-desktop
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
    src/MainWindow.ui
)
target_link_libraries(devlink-desktop PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::WebSockets)
