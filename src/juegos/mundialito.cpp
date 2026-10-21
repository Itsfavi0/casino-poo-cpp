#include "mundialito.h"

#include <iostream>
#include <stdexcept>

#include "../consola.h"
#include "../entrada.h"

namespace casino {

const std::vector<std::string>& Mundialito::EquiposBase() {
    static const std::vector<std::string> equipos = {
        "Real Madrid", "Barcelona", "Bayern", "Liverpool", "City", "PSG",
        "Boca Juniors", "River Plate", "Flamengo", "Alianza Lima", "Universitario",
        "Cristal", "Juventus", "Milan", "Inter", "Chelsea"
    };
    return equipos;
}

std::vector<std::vector<Partido>> Mundialito::SimularTorneo(const std::vector<std::string>& equipos,
                                                           Aleatorio& azar) {
    const size_t n = equipos.size();
    if (n < 2 || (n & (n - 1)) != 0) {
        throw std::invalid_argument("La cantidad de equipos debe ser una potencia de 2");
    }

    std::vector<std::vector<Partido>> rondas;
    std::vector<std::string> enCompetencia = equipos;
    while (enCompetencia.size() > 1) {
        std::vector<Partido> ronda;
        std::vector<std::string> clasificados;
        for (size_t i = 0; i < enCompetencia.size(); i += 2) {
            const std::string& local = enCompetencia[i];
            const std::string& visitante = enCompetencia[i + 1];
            const std::string& ganador = (azar.Entero(0, 1) == 0) ? local : visitante;
            ronda.push_back({local, visitante, ganador});
            clasificados.push_back(ganador);
        }
        rondas.push_back(ronda);
        enCompetencia = clasificados;
    }
    return rondas;
}

Centimos Mundialito::Jugar(Centimos apuesta) {
    const int modo = LeerEntero("Modo: 1. Rapido (8 equipos) | 2. Completo (16 equipos): ", 1, 2);
    const size_t cantidadEquipos = (modo == 1) ? 8 : 16;

    std::vector<std::string> equipos = EquiposBase();
    azar.Barajar(equipos);
    equipos.resize(cantidadEquipos);

    std::cout << "\nEquipos:\n";
    for (size_t i = 0; i < equipos.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << equipos[i] << "\n";
    }
    const int eleccion = LeerEntero("Tu equipo (numero): ", 1, static_cast<int>(equipos.size()));
    const std::string miEquipo = equipos[eleccion - 1];

    const auto rondas = SimularTorneo(equipos, azar);
    for (const auto& ronda : rondas) {
        std::cout << "\n--- Ronda de " << ronda.size() * 2 << " ---\n";
        for (const Partido& partido : ronda) {
            std::cout << partido.local << " vs " << partido.visitante << " -> ";
            ImprimirLinea(partido.ganador, partido.ganador == miEquipo ? Color::Verde : Color::Normal);
        }
        Esperar(2);
    }

    const std::string& campeon = rondas.back().front().ganador;
    std::cout << "\nCAMPEON: ";
    ImprimirLinea(campeon, Color::Amarillo);

    if (campeon == miEquipo) {
        const Centimos ganancia = apuesta * PAGO_CAMPEON;
        ImprimirLinea("CAMPEONES!! Ganas " + FormatearSoles(ganancia), Color::Verde);
        return ganancia;
    }
    ImprimirLinea("Tu equipo '" + miEquipo + "' fue eliminado.", Color::Rojo);
    return -apuesta;
}

}  // namespace casino
