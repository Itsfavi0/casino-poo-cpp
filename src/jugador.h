#pragma once

#include <string>
#include <vector>

#include "dinero.h"

namespace casino {

class Jugador {
    private:
        std::string nombre;
        Centimos saldo;

    public:
        Jugador(std::string nombre, Centimos saldo);

        const std::string& GetNombre() const { return nombre; }
        Centimos GetSaldo() const { return saldo; }

        void Recargar(Centimos monto);

        // Suma una ganancia (positivo) o resta una perdida (negativo).
        // Lanza std::invalid_argument si la perdida es mayor que el saldo.
        void AplicarResultado(Centimos neto);
};

// Copia de los jugadores ordenada de mayor a menor saldo.
std::vector<Jugador> OrdenarPorSaldo(std::vector<Jugador> jugadores);

// Jugadores cuyo nombre contiene el texto buscado (sin distinguir mayusculas).
std::vector<Jugador> BuscarPorNombre(const std::vector<Jugador>& jugadores, const std::string& texto);

}  // namespace casino
