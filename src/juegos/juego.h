#pragma once

#include <string>

#include "../dinero.h"

namespace casino {

// Interfaz comun de todos los juegos del casino. Para agregar un juego nuevo
// basta con heredar de esta clase y registrarlo en Casino.
class Juego {
    public:
        virtual ~Juego() = default;

        virtual std::string GetNombre() const = 0;

        // Juega una ronda con la apuesta ya validada. Devuelve el resultado
        // neto: ganancia (positivo), perdida (negativo) o 0 si hay empate.
        virtual Centimos Jugar(Centimos apuesta) = 0;
};

}  // namespace casino
