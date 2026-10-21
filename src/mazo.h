#pragma once

#include <vector>

#include "aleatorio.h"
#include "carta.h"

namespace casino {

class Mazo {
    private:
        std::vector<Carta> cartas;
        Aleatorio& azar;

    public:
        explicit Mazo(Aleatorio& azar);  // crea las 52 cartas ya barajadas

        void Llenar();    // vuelve a poner las 52 cartas (ordenadas)
        void Barajar();

        // Si el mazo esta vacio, lo vuelve a llenar y barajar antes de robar.
        Carta Robar();

        size_t Cantidad() const { return cartas.size(); }
};

}  // namespace casino
