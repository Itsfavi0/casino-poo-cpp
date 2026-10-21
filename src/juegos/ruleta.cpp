#include "ruleta.h"

#include <iostream>

#include "../consola.h"
#include "../entrada.h"

namespace casino {

Centimos Ruleta::CalcularResultado(TipoApuestaRuleta tipo, int numeroApostado, int numeroSalido,
                                   Centimos apuesta) {
    bool gana = false;
    switch (tipo) {
        case TipoApuestaRuleta::Par:    gana = numeroSalido != 0 && numeroSalido % 2 == 0; break;
        case TipoApuestaRuleta::Impar:  gana = numeroSalido % 2 != 0; break;
        case TipoApuestaRuleta::Numero: gana = numeroSalido == numeroApostado; break;
    }
    if (!gana) return -apuesta;
    return (tipo == TipoApuestaRuleta::Numero) ? apuesta * PAGO_NUMERO : apuesta;
}

Centimos Ruleta::Jugar(Centimos apuesta) {
    std::cout << "Tipo de apuesta:\n";
    std::cout << "  1. Par (paga 1 a 1)\n";
    std::cout << "  2. Impar (paga 1 a 1)\n";
    std::cout << "  3. Numero exacto (paga " << PAGO_NUMERO << " a 1)\n";
    const int opcion = LeerEntero("Opcion: ", 1, 3);

    const TipoApuestaRuleta tipo = (opcion == 1)   ? TipoApuestaRuleta::Par
                                   : (opcion == 2) ? TipoApuestaRuleta::Impar
                                                   : TipoApuestaRuleta::Numero;
    int numeroApostado = -1;
    if (tipo == TipoApuestaRuleta::Numero) {
        numeroApostado = LeerEntero("Numero (0-36): ", 0, 36);
    }

    std::cout << "Girando...\n";
    Esperar(2);
    const int numeroSalido = azar.Entero(0, 36);
    ImprimirLinea("Salio el: " + std::to_string(numeroSalido), Color::Amarillo);

    const Centimos resultado = CalcularResultado(tipo, numeroApostado, numeroSalido, apuesta);
    if (resultado > 0) {
        ImprimirLinea("GANASTE " + FormatearSoles(resultado) + "!", Color::Verde);
    } else {
        ImprimirLinea("PIERDES.", Color::Rojo);
    }
    return resultado;
}

}  // namespace casino
