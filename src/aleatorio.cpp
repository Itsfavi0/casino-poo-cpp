#include "aleatorio.h"

#include <chrono>

namespace casino {

// No se usa std::random_device porque en algunas versiones de MinGW
// devuelve siempre la misma secuencia.
Aleatorio::Aleatorio()
    : motor(static_cast<unsigned>(
          std::chrono::high_resolution_clock::now().time_since_epoch().count())) {
}

Aleatorio::Aleatorio(unsigned semilla) : motor(semilla) {
}

int Aleatorio::Entero(int minimo, int maximo) {
    std::uniform_int_distribution<int> distribucion(minimo, maximo);
    return distribucion(motor);
}

}  // namespace casino
