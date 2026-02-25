cmake_minimum_required(VERSION 3.10)

project(OnlineChat)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

# Создаем исполняемый файл
add_executable(OnlineChat ${SOURCES})
target_include_directories(OnlineChat PRIVATE include)
# Важнейшая часть для Windows: подключаем библиотеку WinSock2
if(WIN32)
    target_link_libraries(OnlineChat ws2_32)
endif()