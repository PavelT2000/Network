# Network Project

A collection of Windows-based network programming examples demonstrating **UDP broadcast communication** and **TCP client-server architecture**. Built with C++17 and the WinSock2 API.

---

## 📁 Project Structure

```text
Network/
├── OnlineChat/                 # UDP Broadcast Chat Application
│   ├── CMakeLists.txt          # CMake build configuration
│   ├── config.txt              # Listener port configuration
│   ├── include/
│   │   └── Listener.h          # Thread & socket declarations
│   └── src/
│       ├── Listener.cpp        # UDP background listener implementation
│       └── main.cpp            # CLI entry point & broadcast sender
└── Test Work/                  # TCP Client/Server Examples
    ├── Client/
    │   └── main.cpp            # TCP client implementation
    └── Server/
        ├── config.txt          # Server port configuration
        └── main.cpp            # TCP server implementation
```

---

## 🛠 Prerequisites

| Requirement          | Details                                                                 |
|----------------------|-------------------------------------------------------------------------|
| **Operating System** | Windows (WinSock2 dependency)                                           |
| **Compiler**         | MSVC (Visual Studio) or MinGW-w64 with C++17 support                    |
| **Build System**     | CMake `3.10+` (required for `OnlineChat`)                               |
| **IDE**              | Visual Studio Code, Visual Studio, or any C++ IDE with terminal support |

> ⚠️ **Note:** This project relies on Windows-specific APIs (`ws2_32.lib`, `#pragma comment`, `GetConsoleMode`). It is not cross-platform out of the box.

---

## 🚀 Building & Running

### 1. OnlineChat (UDP Broadcast)
A console-based chat that broadcasts messages to all devices on the local network and listens for incoming UDP packets in a background thread.

**Build:**
```bash
cd OnlineChat
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

**Run:**
```bash
./OnlineChat.exe
```
- Type messages and press `Enter` to broadcast.
- Type `exit` to terminate.
- Incoming messages appear dynamically above the input prompt.

### 2. Test Work (TCP Client/Server)
A synchronous TCP server that accepts a single client connection and logs received messages to the console.

**Compile (MSVC Command Line):**
```cmd
cl /EHsc /std:c++17 "Test Work/Server/main.cpp" /Fe:server.exe /link ws2_32.lib
cl /EHsc /std:c++17 "Test Work/Client/main.cpp" /Fe:client.exe /link ws2_32.lib
```

**Run:**
1. Start the server:
   ```cmd
   server.exe
   ```
2. In a new terminal, start the client:
   ```cmd
   client.exe
   ```
3. Follow the prompts to enter the server IP and port. Type messages to send, or `exit` to disconnect.

---

## ⚙️ Configuration

| File                          | Purpose                                  | Default Value |
|-------------------------------|------------------------------------------|---------------|
| `OnlineChat/config.txt`       | Port for the UDP listener                | `8080`        |
| `Test Work/Server/config.txt` | Port for the TCP server                  | `8080`        |

> 🔍 **Implementation Note:** `OnlineChat` currently hardcodes the port (`8080`) in `Listener.cpp`. To make it fully dynamic, modify `ThreadArgs args = { 8080, &isRunning };` to read from `config.txt` at runtime.

---

## 📐 Technical Overview

### OnlineChat Architecture
- **UDP Broadcast:** Messages are sent to `255.255.255.255` with `SO_BROADCAST` enabled.
- **Background Listener:** Uses `CreateThread` wrapped in a `std::thread` to run `UDPListenerThread` asynchronously.
- **Console UI:** Leverages ANSI escape sequences (`\r\033[K`) and `ENABLE_VIRTUAL_TERMINAL_PROCESSING` for clean, non-blocking input/output.
- **WinSock Lifecycle:** `WSAStartup`/`WSACleanup` handled in `main.cpp`.

### Test Work Architecture
- **TCP Server:** Synchronous, single-client design using `socket() → bind() → listen() → accept() → recv()`.
- **TCP Client:** Interactive CLI that resolves user input via `inet_pton()` and streams data using `send()`.
- **Graceful Handling:** Detects client disconnects (`recv() == 0`) and socket errors (`SOCKET_ERROR`).

---

## ⚠️ Limitations & Known Issues

| Area                | Limitation                                                                 |
|---------------------|----------------------------------------------------------------------------|
| **Platform**        | Windows-only due to WinSock2 and MSVC pragmas.                             |
| **Firewall**        | UDP broadcast (`255.255.255.255`) is often blocked by default on Windows.  |
| **Concurrency**     | TCP server handles only one client at a time (blocking `accept`/`recv`).   |
| **Config Sync**     | `OnlineChat` does not dynamically parse `config.txt` (hardcoded fallback). |
| **Error Recovery**  | No automatic reconnection or retry logic on network drops.                 |

---

## 🔮 Future Improvements

- [ ] Cross-platform socket abstraction (`#ifdef _WIN32` / POSIX fallback)
- [ ] Dynamic config parsing for `OnlineChat`
- [ ] Multi-client TCP support using `std::thread` or I/O multiplexing (`select`/`WSAPoll`)
- [ ] Graceful signal handling (`Ctrl+C` cleanup)
- [ ] CMake integration for `Test Work`
- [ ] Input validation & message length limits

---

## 📜 License

This project is provided for educational and demonstration purposes. Use at your own discretion in production environments.