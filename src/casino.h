#pragma once

#include <memory>
#include <string>
#include <vector>

#include "aleatorio.h"
#include "dinero.h"
#include "juegos/juego.h"
#include "jugador.h"

namespace casino {

const Centimos MONTO_MINIMO = Soles(10);
const Centimos RECARGA_MAXIMA = Soles(100000);

// Maneja el menu principal, los jugadores y la ronda de apuestas comun a
// todos los juegos. La logica de cada juego vive en su propia clase.
class Casino {
    private:
        Aleatorio azar;
        std::vector<Jugador> jugadores;
        std::vector<std::unique_ptr<Juego>> juegos;

        void MostrarLogo() const;
        void MostrarListaJugadores(const std::vector<Jugador>& lista) const;
        bool HayJugadores() const;
        bool ExisteJugador(const std::string& nombre) const;

        // Devuelve el indice del jugador elegido o -1 si el usuario vuelve al menu.
        int SeleccionarJugador() const;

        void RegistrarJugador();
        void RecargarSaldo();
        void JugarJuego(Juego& juego);
        void MostrarRanking() const;
        void BuscarJugador() const;

    public:
        Casino();

        // Los juegos guardan una referencia a "azar", asi que el casino no se copia.
        Casino(const Casino&) = delete;
        Casino& operator=(const Casino&) = delete;

        void Ejecutar();
};

}  // namespace casino
