#include "texto.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <climits>
#include <cstdlib>

namespace casino {

std::string Recortar(const std::string& texto) {
    const char* espacios = " \t\r\n";
    const auto inicio = texto.find_first_not_of(espacios);
    if (inicio == std::string::npos) return "";
    const auto fin = texto.find_last_not_of(espacios);
    return texto.substr(inicio, fin - inicio + 1);
}

std::string AMayusculas(std::string texto) {
    // El cast a unsigned char evita comportamiento indefinido con letras como la ñ.
    std::transform(texto.begin(), texto.end(), texto.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return texto;
}

bool IgualesSinMayusculas(const std::string& a, const std::string& b) {
    return AMayusculas(a) == AMayusculas(b);
}

bool ContieneSinMayusculas(const std::string& texto, const std::string& buscado) {
    return AMayusculas(texto).find(AMayusculas(buscado)) != std::string::npos;
}

bool ParsearEntero(const std::string& texto, int& valor) {
    const std::string limpio = Recortar(texto);
    if (limpio.empty()) return false;

    char* fin = nullptr;
    errno = 0;
    const long resultado = std::strtol(limpio.c_str(), &fin, 10);
    if (*fin != '\0' || errno == ERANGE || resultado < INT_MIN || resultado > INT_MAX) {
        return false;
    }
    valor = static_cast<int>(resultado);
    return true;
}

}  // namespace casino
