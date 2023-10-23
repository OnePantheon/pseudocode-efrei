cmake_minimum_required(VERSION 3.26)
project(efreipseudocode C)

set(CMAKE_C_STANDARD 11)

add_executable(efreipseudocode src/main.c
        src/Lexer/Token.h
        src/Lexer/Token.c
        src/Lexer/Dictionary.h
        src/Lexer/Dictionary.h
        src/Lexer/Lexer.h
        src/Lexer/Lexer.h
        src/Lexer/Lexer.c
        src/Utils/Utils.h
        src/Utils/Utils.c
        src/Lexer/Dictionary.c
        src/Lexer/Dictionary.c)
