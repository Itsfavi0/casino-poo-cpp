#pragma once

#include <algorithm>
#include <random>
#include <vector>

namespace casino {

// Generador de numeros aleatorios compartido por todo el casino.
// Reemplaza a rand() % n, que no reparte los valores de forma uniforme.
class Aleatorio {
    private:
        std::mt19937 motor;

    public:
        Aleatorio();                        // semilla basada en el reloj
        explicit Aleatorio(unsigned semilla);  // semilla fija: util para pruebas

        // Entero uniforme en el rango [minimo, maximo].
        int Entero(int minimo, int maximo);

        template <typename T>
        void Barajar(std::vector<T>& elementos) {
            std::shuffle(elementos.begin(), elementos.end(), motor);
        }
};

}  // namespace casino
