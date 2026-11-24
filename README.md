# DevLink

DevLink is a device operations platform with a C++/Qt backend, a Qt Widgets desktop client, and a Vue 3 web client.

## Technology

- Backend: C++17, Qt 6, Qt HttpServer, Qt WebSockets
- Desktop: Qt 6 Widgets, Qt Designer UI, Qt Network, Qt WebSockets
- Web: Vue 3, TypeScript, Vite
- Build: CMake + Ninja
- Initial development: Windows / Qt 6.10.3 MinGW 64-bit
- Planned deployment: Ubuntu 22.04+

## First milestone

- Device create, list, rename, and delete from desktop and web.
- REST API as the command interface.
- WebSocket events for cross-client real-time synchronization.
- One backend-owned device state shared by all clients.

## Repository layout

- backend/: C++/Qt API server and in-memory device store.
- desktop/: Qt Widgets client; UI is maintained in Qt Designer .ui files.
- web/: Vue 3 + TypeScript web client.
- shared/: shared API contracts.
- docs/: architecture, API, and development notes.

## Build

See docs/development/windows.md.

    cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.10.3/mingw_64" -DCMAKE_BUILD_TYPE=Debug
    cmake --build build

## Important limitations

This is the first development scaffold. Device data is stored in memory and is lost when the backend exits. Authentication, authorization, audit logs, SQLite persistence, hardware drivers, TLS/WSS, and production deployment hardening are not implemented. Run only on a trusted development LAN; do not expose these development services to the public internet.
