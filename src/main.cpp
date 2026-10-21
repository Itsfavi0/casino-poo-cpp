#include <iostream>

#include "casino.h"
#include "consola.h"
#include "entrada.h"

int main() {
    casino::InicializarConsola();

    try {
        casino::Casino miCasino;
        miCasino.Ejecutar();
    } catch (const casino::EntradaCerrada&) {
        std::cout << "\nEntrada cerrada. Hasta pronto!\n";
    }
    return 0;
}
