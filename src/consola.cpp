#include "consola.h"

#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <chrono>
#include <thread>
#endif

namespace casino {

#ifdef _WIN32

void InicializarConsola() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void CambiarColor(Color color) {
    WORD atributo = 7;
    switch (color) {
        case Color::Normal:   atributo = 7; break;
        case Color::Rojo:     atributo = 12; break;
        case Color::Verde:    atributo = 10; break;
        case Color::Amarillo: atributo = 14; break;
        case Color::Cian:     atributo = 11; break;
    }
    std::cout.flush();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), atributo);
}

void LimpiarPantalla() {
    std::cout.flush();
    std::system("cls");
}

void Esperar(double segundos) {
    std::cout.flush();
    Sleep(static_cast<DWORD>(segundos * 1000));
}

#else

void InicializarConsola() {
}

void CambiarColor(Color color) {
    switch (color) {
        case Color::Normal:   std::cout << "\033[0m"; break;
        case Color::Rojo:     std::cout << "\033[91m"; break;
        case Color::Verde:    std::cout << "\033[92m"; break;
        case Color::Amarillo: std::cout << "\033[93m"; break;
        case Color::Cian:     std::cout << "\033[96m"; break;
    }
}

void LimpiarPantalla() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void Esperar(double segundos) {
    std::cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(segundos * 1000)));
}

#endif

void Imprimir(const std::string& texto, Color color) {
    CambiarColor(color);
    std::cout << texto;
    CambiarColor(Color::Normal);
}

void ImprimirLinea(const std::string& texto, Color color) {
    Imprimir(texto, color);
    std::cout << "\n";
}

}  // namespace casino
