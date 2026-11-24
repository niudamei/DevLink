# DevLink

DevLink is a cross-platform device management and communication platform with a Qt desktop client, a Vue web client, and a centralized C++/Qt backend.

## Technology

- Backend: C++17, Qt 6, Qt HttpServer, Qt WebSockets
- Desktop: Qt 6 Widgets, Qt Designer UI forms, Qt Network, Qt WebSockets
- Web: Vue 3, TypeScript, Vite
- Build: CMake
- Initial development platform: Windows
- Planned deployment platform: Ubuntu 22.04+

## First milestone

The first milestone implements bidirectional device-management synchronization:

1. Add, edit, and delete devices from the Qt desktop client.
2. Add, edit, and delete devices from the Vue web client.
3. Publish device changes through WebSocket events.
4. Refresh client snapshots after receiving server events.

## Initial API

Base path: `/api/v1`

- `GET /health`
- `GET /devices`
- `POST /devices`
- `PUT /devices/{id}`
- `DELETE /devices/{id}`

WebSocket endpoint: `/ws` (initial development port: 8081).

## Development environment

The initial Windows build targets Qt 6.10.3 with the MinGW 64-bit kit installed at:

`C:/Qt/6.10.3/mingw_64`

## Current limitations

The first prototype is intended for a trusted development LAN only. Before production deployment, add persistent storage, authentication and authorization, audit logging, and TLS/WSS.