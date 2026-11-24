# REST API（v1）

Base URL: http://<server-host>:8080/api/v1

| Method | Path | Purpose |
|---|---|---|
| GET | /health | 服务健康检查 |
| GET | /devices | 获取设备快照 |
| POST | /devices | 新增设备 |
| PUT | /devices/{id} | 修改设备名称或类型 |
| DELETE | /devices/{id} | 删除设备 |

## Create device

请求示例：

    { "name": "CAN采集箱 A", "type": "generic", "source": "virtual" }

## List devices

响应示例：

    { "devices": [] }

当前原型的错误响应使用 { "error": "message" }。后续版本应补充标准 HTTP 状态码、字段校验错误码和分页。
