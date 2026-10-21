#pragma once

#include <string>
#include <vector>

#include "../aleatorio.h"
#include "juego.h"

namespace casino {

struct Partido {
    std::string local;
    std::string visitante;
    std::string ganador;
};

// Torneo de eliminacion directa: el jugador elige un equipo y, si sale
// campeon, cobra PAGO_CAMPEON a 1.
class Mundialito : public Juego {
    private:
        Aleatorio& azar;

    public:
        static const int PAGO_CAMPEON = 2;

        explicit Mundialito(Aleatorio& azar) : azar(azar) {}

        std::string GetNombre() const override { return "Mundial de Clubes"; }
        Centimos Jugar(Centimos apuesta) override;

        static const std::vector<std::string>& EquiposBase();

        // Simula el torneo completo. Devuelve los partidos de cada ronda; el
        // campeon es el ganador del ultimo partido. La cantidad de equipos
        // debe ser una potencia de 2 (2, 4, 8, 16...).
        static std::vector<std::vector<Partido>> SimularTorneo(const std::vector<std::string>& equipos,
                                                              Aleatorio& azar);
};

}  // namespace casino
