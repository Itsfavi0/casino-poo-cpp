#pragma once

#include <string>
#include <vector>

#include "consola.h"

namespace casino {

enum class Palo { Espadas, Corazones, Diamantes, Treboles };

class Carta {
    private:
        int valor;  // 1 (As) a 13 (K)
        Palo palo;

    public:
        Carta(int valor, Palo palo);

        int GetValor() const { return valor; }
        Palo GetPalo() const { return palo; }

        // En Carta Mayor el As es la carta mas fuerte.
        int GetFuerza() const { return (valor == 1) ? 14 : valor; }

        // "A", "2", ..., "10", "J", "Q", "K"
        std::string TextoValor() const;
        std::string Simbolo() const;
        Color GetColor() const;

        // Las lineas del dibujo ASCII de la carta (todas del mismo ancho).
        std::vector<std::string> Lineas() const;
};

// Dibuja varias cartas lado a lado, cada una con un titulo encima.
void DibujarCartas(const std::vector<Carta>& cartas, const std::vector<std::string>& titulos);

}  // namespace casino
