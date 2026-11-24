# Windows 开发环境

## 依赖

- Windows 10/11 64-bit
- Qt 6.10.3 MinGW 64-bit
- Qt 模块：Core、Network、Widgets、WebSockets、HttpServer
- CMake 3.21+
- Ninja
- MinGW 64-bit compiler
- Node.js 20+

Qt 安装目录：C:/Qt/6.10.3/mingw_64

请在 Qt Maintenance Tool 中确认已安装 Qt HttpServer 和 Qt WebSockets。若 CMake 报告缺少 Qt6HttpServerConfig.cmake，说明当前 Qt Kit 未安装 HttpServer 模块。

## 配置和编译

在仓库根目录 PowerShell 执行：

    cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.10.3/mingw_64" -DCMAKE_BUILD_TYPE=Debug
    cmake --build build

## 启动后端

    .\build\backend\devlink-server.exe

验证接口：

    Invoke-RestMethod http://127.0.0.1:8080/api/v1/health

## 启动桌面端

另开一个 PowerShell：

    .\build\desktop\devlink-desktop.exe

## 启动 Web

另开一个 PowerShell：

    cd web
    npm install
    npm run dev

浏览器打开 Vite 输出的地址，通常为 http://localhost:5173。

## 局域网访问

如果 Web 页面从另一台电脑访问，Vite 页面可通过开发机局域网 IP 访问；WebSocket 地址也会使用浏览器当前主机名。Windows 防火墙需要按需放行 5173、8080、8081。当前服务监听所有 IPv4 网卡，务必仅在可信开发网络测试。
