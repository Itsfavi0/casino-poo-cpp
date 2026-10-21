#pragma once

#include "../aleatorio.h"
#include "../carta.h"
#include "../mazo.h"
#include "juego.h"

namespace casino {

// El jugador y el croupier sacan una carta cada uno; gana la mas alta.
// Paga 1 a 1. Si empatan se devuelve la apuesta.
class CartaMayor : public Juego {
    private:
        Mazo mazo;

    public:
        explicit CartaMayor(Aleatorio& azar);

        std::string GetNombre() const override { return "Carta Mayor"; }
        Centimos Jugar(Centimos apuesta) override;

        static Centimos CalcularResultado(const Carta& croupier, const Carta& jugador, Centimos apuesta);
};

}  // namespace casino
