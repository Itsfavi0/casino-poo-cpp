#pragma once

#include <stdexcept>
#include <string>

#include "dinero.h"

// Lectura de datos del usuario. Todo se lee linea por linea con getline, asi
// nunca quedan restos en el buffer ni hace falta cin.ignore().
namespace casino {

// Se lanza cuando la entrada se cierra (Ctrl+Z / Ctrl+D o fin de archivo).
class EntradaCerrada : public std::runtime_error {
    public:
        EntradaCerrada() : std::runtime_error("La entrada estandar se cerro") {}
};

std::string LeerLinea(const std::string& mensaje);

std::string LeerTextoNoVacio(const std::string& mensaje);

// Repite la pregunta hasta recibir un entero dentro de [minimo, maximo].
int LeerEntero(const std::string& mensaje, int minimo, int maximo);

// Repite hasta recibir un monto en [minimo, maximo]. Si permitirCancelar es
// true, el usuario puede escribir 0 y la funcion devuelve 0.
Centimos LeerMonto(const std::string& mensaje, Centimos minimo, Centimos maximo,
                   bool permitirCancelar = false);

// Pregunta S/N. Devuelve true si la respuesta es S.
bool Confirmar(const std::string& mensaje);

void EsperarEnter();

}  // namespace casino
