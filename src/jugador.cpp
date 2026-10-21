#include "jugador.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include "texto.h"

namespace casino {

Jugador::Jugador(std::string nombre, Centimos saldo) : nombre(std::move(nombre)), saldo(saldo) {
    if (saldo < 0) throw std::invalid_argument("El saldo inicial no puede ser negativo");
}

void Jugador::Recargar(Centimos monto) {
    if (monto <= 0) throw std::invalid_argument("El monto a recargar debe ser positivo");
    saldo += monto;
}

void Jugador::AplicarResultado(Centimos neto) {
    if (saldo + neto < 0) throw std::invalid_argument("La perdida supera el saldo del jugador");
    saldo += neto;
}

std::vector<Jugador> OrdenarPorSaldo(std::vector<Jugador> jugadores) {
    // stable_sort mantiene el orden de registro entre jugadores con el mismo saldo.
    std::stable_sort(jugadores.begin(), jugadores.end(),
                     [](const Jugador& a, const Jugador& b) { return a.GetSaldo() > b.GetSaldo(); });
    return jugadores;
}

std::vector<Jugador> BuscarPorNombre(const std::vector<Jugador>& jugadores, const std::string& texto) {
    std::vector<Jugador> encontrados;
    for (const Jugador& jugador : jugadores) {
        if (ContieneSinMayusculas(jugador.GetNombre(), texto)) encontrados.push_back(jugador);
    }
    return encontrados;
}

}  // namespace casino
