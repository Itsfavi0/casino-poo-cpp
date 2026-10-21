#include "mazo.h"

namespace casino {

Mazo::Mazo(Aleatorio& azar) : azar(azar) {
    Llenar();
    Barajar();
}

void Mazo::Llenar() {
    cartas.clear();
    const Palo palos[] = {Palo::Espadas, Palo::Corazones, Palo::Diamantes, Palo::Treboles};
    for (Palo palo : palos) {
        for (int valor = 1; valor <= 13; valor++) {
            cartas.emplace_back(valor, palo);
        }
    }
}

void Mazo::Barajar() {
    azar.Barajar(cartas);
}

Carta Mazo::Robar() {
    if (cartas.empty()) {
        Llenar();
        Barajar();
    }
    Carta carta = cartas.back();
    cartas.pop_back();
    return carta;
}

}  // namespace casino
