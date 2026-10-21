#pragma once

#include <string>

namespace casino {

// El dinero se guarda en centimos (enteros) para evitar los errores de
// redondeo que aparecen al sumar y restar montos con double.
using Centimos = long long;

constexpr Centimos Soles(long long soles) {
    return soles * 100;
}

// 1250 -> "S/ 12.50"
std::string FormatearSoles(Centimos monto);

// "12", "12.5", "12,50" -> 1250. Devuelve false si el texto no es un monto valido.
bool ParsearSoles(const std::string& texto, Centimos& monto);

}  // namespace casino
