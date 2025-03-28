cmake_minimum_required(VERSION 3.28)
project(Korlang)

set(CMAKE_CXX_STANDARD 17)

add_executable(Korlang main.cpp
        Kind.cpp
        Kind.h
        Token.h
        Scanner.cpp
        Node.h
        Parser.cpp
        Printer.cpp 
        Code.h
        Generator.cpp
        Machine.cpp
        DataType.cpp
        DataType.h
        BuildinFunctionTable.cpp
)
