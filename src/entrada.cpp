#include "entrada.h"

#include <iostream>

#include "consola.h"
#include "texto.h"

namespace casino {

std::string LeerLinea(const std::string& mensaje) {
    std::cout << mensaje;
    std::string linea;
    if (!std::getline(std::cin, linea)) throw EntradaCerrada();
    return Recortar(linea);
}

std::string LeerTextoNoVacio(const std::string& mensaje) {
    std::string texto = LeerLinea(mensaje);
    while (texto.empty()) {
        ImprimirLinea("No puede estar vacio.", Color::Rojo);
        texto = LeerLinea(mensaje);
    }
    return texto;
}

int LeerEntero(const std::string& mensaje, int minimo, int maximo) {
    while (true) {
        const std::string linea = LeerLinea(mensaje);
        int valor = 0;
        if (ParsearEntero(linea, valor) && valor >= minimo && valor <= maximo) {
            return valor;
        }
        ImprimirLinea("Entrada invalida. Ingrese un numero entre " + std::to_string(minimo) +
                          " y " + std::to_string(maximo) + ".",
                      Color::Rojo);
    }
}

Centimos LeerMonto(const std::string& mensaje, Centimos minimo, Centimos maximo,
                   bool permitirCancelar) {
    while (true) {
        const std::string linea = LeerLinea(mensaje);
        Centimos monto = 0;
        if (ParsearSoles(linea, monto)) {
            if (permitirCancelar && monto == 0) return 0;
            if (monto >= minimo && monto <= maximo) return monto;
        }
        ImprimirLinea("Monto invalido. Debe estar entre " + FormatearSoles(minimo) + " y " +
                          FormatearSoles(maximo) + ".",
                      Color::Rojo);
    }
}

bool Confirmar(const std::string& mensaje) {
    while (true) {
        const std::string respuesta = AMayusculas(LeerLinea(mensaje + " (S/N): "));
        if (respuesta == "S") return true;
        if (respuesta == "N") return false;
        ImprimirLinea("Opcion '" + respuesta + "' invalida.", Color::Rojo);
    }
}

void EsperarEnter() {
    LeerLinea("\nPresione Enter para continuar...");
}

}  // namespace casino
