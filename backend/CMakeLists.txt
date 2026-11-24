add_executable(devlink-server
    app/main.cpp
    api/ApiServer.cpp api/ApiServer.h
    core/DeviceStore.cpp core/DeviceStore.h
)
target_include_directories(devlink-server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(devlink-server PRIVATE Qt6::Core Qt6::Network Qt6::HttpServer Qt6::WebSockets)
