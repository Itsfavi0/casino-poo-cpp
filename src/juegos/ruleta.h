#pragma once

#include "../aleatorio.h"
#include "juego.h"

namespace casino {

enum class TipoApuestaRuleta { Par, Impar, Numero };

// Ruleta europea (0 a 36). Par/Impar paga 1 a 1 y el 0 no cuenta como par.
// Numero exacto paga PAGO_NUMERO a 1.
class Ruleta : public Juego {
    private:
        Aleatorio& azar;

    public:
        static const int PAGO_NUMERO = 10;

        explicit Ruleta(Aleatorio& azar) : azar(azar) {}

        std::string GetNombre() const override { return "La Ruleta"; }
        Centimos Jugar(Centimos apuesta) override;

        // numeroApostado solo se usa cuando tipo == Numero.
        static Centimos CalcularResultado(TipoApuestaRuleta tipo, int numeroApostado, int numeroSalido,
                                          Centimos apuesta);
};

}  // namespace casino
