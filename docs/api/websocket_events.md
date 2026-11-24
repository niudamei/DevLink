# WebSocket 事件

Endpoint: ws://<server-host>:8081/ws

连接成功后服务端依次发送 connection.ready 和 devices.snapshot。

设备变化事件：

- device.created
- device.updated
- device.deleted

统一事件结构：

    {
      "eventId": "uuid",
      "event": "device.updated",
      "timestamp": "2026-11-24T10:30:00Z",
      "data": { "device": {} }
    }

第一版客户端收到任何事件后重新请求 GET /api/v1/devices。后续增加单调递增序号、心跳、断线重连补偿和事件回放。
