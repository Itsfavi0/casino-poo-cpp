#include "carta_mayor.h"

#include <iostream>

#include "../consola.h"

namespace casino {

CartaMayor::CartaMayor(Aleatorio& azar) : mazo(azar) {
}

Centimos CartaMayor::CalcularResultado(const Carta& croupier, const Carta& jugador, Centimos apuesta) {
    if (jugador.GetFuerza() > croupier.GetFuerza()) return apuesta;
    if (jugador.GetFuerza() < croupier.GetFuerza()) return -apuesta;
    return 0;
}

Centimos CartaMayor::Jugar(Centimos apuesta) {
    const Carta cartaCroupier = mazo.Robar();
    const Carta cartaJugador = mazo.Robar();

    std::cout << "\nSe estan repartiendo las cartas...\n";
    Esperar(1.5);
    DibujarCartas({cartaCroupier, cartaJugador}, {"Croupier:", "Tu carta:"});
    std::cout << "\n";

    const Centimos resultado = CalcularResultado(cartaCroupier, cartaJugador, apuesta);
    if (resultado > 0) {
        ImprimirLinea("GANAS " + FormatearSoles(resultado) + "!!!", Color::Verde);
    } else if (resultado < 0) {
        ImprimirLinea("PIERDES " + FormatearSoles(-resultado) + " :c", Color::Rojo);
    } else {
        ImprimirLinea("EMPATE. Se devuelve la apuesta.", Color::Amarillo);
    }
    return resultado;
}

}  // namespace casino
