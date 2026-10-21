#include "carta.h"

#include <iostream>
#include <stdexcept>

namespace casino {

namespace {

const size_t ANCHO_CARTA = 9;
const size_t SEPARACION = 4;

std::string Rellenar(const std::string& texto, size_t ancho) {
    if (texto.size() >= ancho) return texto;
    return texto + std::string(ancho - texto.size(), ' ');
}

}  // namespace

Carta::Carta(int valor, Palo palo) : valor(valor), palo(palo) {
    if (valor < 1 || valor > 13) throw std::invalid_argument("Valor de carta fuera de rango");
}

std::string Carta::TextoValor() const {
    switch (valor) {
        case 1:  return "A";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: return std::to_string(valor);
    }
}

std::string Carta::Simbolo() const {
    switch (palo) {
        case Palo::Espadas:   return "♠";  // ♠
        case Palo::Corazones: return "♥";  // ♥
        case Palo::Diamantes: return "♦";  // ♦
        case Palo::Treboles:  return "♣";  // ♣
    }
    return "?";
}

Color Carta::GetColor() const {
    switch (palo) {
        case Palo::Corazones:
        case Palo::Diamantes: return Color::Rojo;
        case Palo::Espadas:   return Color::Cian;
        case Palo::Treboles:  return Color::Verde;
    }
    return Color::Normal;
}

std::vector<std::string> Carta::Lineas() const {
    const std::string v = TextoValor();
    const std::string huecoArriba(7 - v.size(), ' ');
    return {
        " _______ ",
        "|" + v + huecoArriba + "|",
        "|       |",
        "|   " + Simbolo() + "   |",
        "|       |",
        "|" + huecoArriba + v + "|",
        "|_______|",
    };
}

void DibujarCartas(const std::vector<Carta>& cartas, const std::vector<std::string>& titulos) {
    const std::string separacion(SEPARACION, ' ');

    for (size_t i = 0; i < titulos.size(); i++) {
        std::cout << Rellenar(titulos[i], ANCHO_CARTA) << separacion;
    }
    std::cout << "\n";

    std::vector<std::vector<std::string>> dibujos;
    for (const Carta& carta : cartas) dibujos.push_back(carta.Lineas());

    const size_t altura = dibujos.empty() ? 0 : dibujos[0].size();
    for (size_t fila = 0; fila < altura; fila++) {
        for (size_t i = 0; i < cartas.size(); i++) {
            Imprimir(dibujos[i][fila], cartas[i].GetColor());
            std::cout << separacion;
        }
        std::cout << "\n";
    }
}

}  // namespace casino
