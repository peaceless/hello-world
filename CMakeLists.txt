# set cmke version
cmake_minimum_required(VERSION 3.10)

# set project name controled by cmake
project(hello-world)

# compiler version
set(CMAKE_CXX_STANDARD 11)

# working mode

set(SEV Server.cpp)
set(CLIENT Client.cpp)

add_executable(Server ${SEV})
add_executable(Client ${CLIENT})

# lazy
add_executable(sServer select_server.cpp)
add_executable(pServer p_server.cpp)
