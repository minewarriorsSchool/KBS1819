cmake_minimum_required(VERSION 3.12)
project(CodeIRGame)

set(CMAKE_CXX_STANDARD 14)

add_executable(CodeIRGame Core/main.cpp Data/IRSendClass.cpp Data/IRSendClass.h Data/IRProtocol.cpp Data/IRProtocol.h Data/Decoding.cpp Data/Decoding.h Data/Receive.cpp Data/Receive.h Data/IRfrequency.cpp Data/IRfrequency.h)