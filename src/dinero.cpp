#include "dinero.h"

#include <cctype>
#include <iomanip>
#include <sstream>

#include "texto.h"

namespace casino {

std::string FormatearSoles(Centimos monto) {
    std::ostringstream salida;
    if (monto < 0) {
        salida << "-";
        monto = -monto;
    }
    salida << "S/ " << monto / 100 << "." << std::setw(2) << std::setfill('0') << monto % 100;
    return salida.str();
}

bool ParsearSoles(const std::string& texto, Centimos& monto) {
    const std::string limpio = Recortar(texto);
    const int MAX_DIGITOS_ENTEROS = 12;  // evita desbordar long long

    Centimos enteros = 0;
    Centimos decimales = 0;
    int digitosEnteros = 0;
    int digitosDecimales = 0;
    bool enDecimales = false;

    for (char c : limpio) {
        if (c == '.' || c == ',') {
            if (enDecimales) return false;
            enDecimales = true;
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            if (enDecimales) {
                if (++digitosDecimales > 2) return false;
                decimales = decimales * 10 + (c - '0');
            } else {
                if (++digitosEnteros > MAX_DIGITOS_ENTEROS) return false;
                enteros = enteros * 10 + (c - '0');
            }
        } else {
            return false;
        }
    }

    if (digitosEnteros == 0 && digitosDecimales == 0) return false;
    if (digitosDecimales == 1) decimales *= 10;  // "12.5" son 50 centimos

    monto = enteros * 100 + decimales;
    return true;
}

}  // namespace casino
