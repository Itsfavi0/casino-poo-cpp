#pragma once

#include <string>

namespace casino {

// Quita espacios al inicio y al final.
std::string Recortar(const std::string& texto);

std::string AMayusculas(std::string texto);

// Compara sin distinguir mayusculas de minusculas.
bool IgualesSinMayusculas(const std::string& a, const std::string& b);

// true si "texto" contiene "buscado", sin distinguir mayusculas.
bool ContieneSinMayusculas(const std::string& texto, const std::string& buscado);

// Convierte todo el texto a entero. Rechaza "12abc", "" o valores fuera de int.
bool ParsearEntero(const std::string& texto, int& valor);

}  // namespace casino
