cmake_minimum_required(VERSION 3.14)
project(CasinoPOO VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Toda la logica va en una biblioteca para que la usen el juego y los tests.
add_library(casino_core STATIC
    src/aleatorio.cpp
    src/carta.cpp
    src/casino.cpp
    src/consola.cpp
    src/dinero.cpp
    src/entrada.cpp
    src/jugador.cpp
    src/mazo.cpp
    src/texto.cpp
    src/juegos/carta_mayor.cpp
    src/juegos/mundialito.cpp
    src/juegos/ruleta.cpp
)
target_include_directories(casino_core PUBLIC src)

if(MSVC)
    target_compile_options(casino_core PUBLIC /W4 /utf-8)
else()
    target_compile_options(casino_core PUBLIC -Wall -Wextra -Wpedantic)
endif()

add_executable(casino src/main.cpp)
target_link_libraries(casino PRIVATE casino_core)

option(CASINO_BUILD_TESTS "Compilar las pruebas unitarias" ON)
if(CASINO_BUILD_TESTS)
    enable_testing()
    add_executable(pruebas tests/pruebas.cpp)
    target_link_libraries(pruebas PRIVATE casino_core)
    add_test(NAME pruebas COMMAND pruebas)
endif()
