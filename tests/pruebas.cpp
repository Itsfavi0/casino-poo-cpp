// Pruebas unitarias de la logica del casino (sin entrada/salida de consola).
// Mini framework propio para no depender de bibliotecas externas.

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include "aleatorio.h"
#include "carta.h"
#include "dinero.h"
#include "juegos/carta_mayor.h"
#include "juegos/mundialito.h"
#include "juegos/ruleta.h"
#include "jugador.h"
#include "mazo.h"
#include "texto.h"

using namespace casino;

namespace {

int fallos = 0;

#define VERIFICAR(condicion)                                                              \
    do {                                                                                  \
        if (!(condicion)) {                                                               \
            std::cerr << "  FALLO " << __FILE__ << ":" << __LINE__ << ": " #condicion "\n"; \
            fallos++;                                                                     \
        }                                                                                 \
    } while (0)

#define VERIFICAR_LANZA(expresion)                 \
    do {                                           \
        bool lanzo = false;                        \
        try {                                      \
            expresion;                             \
        } catch (const std::exception&) {          \
            lanzo = true;                          \
        }                                          \
        VERIFICAR(lanzo && "deberia lanzar: " #expresion); \
    } while (0)

// --- Dinero y texto ---

void PruebaFormatearSoles() {
    VERIFICAR(FormatearSoles(0) == "S/ 0.00");
    VERIFICAR(FormatearSoles(1250) == "S/ 12.50");
    VERIFICAR(FormatearSoles(5) == "S/ 0.05");
    VERIFICAR(FormatearSoles(-1000) == "-S/ 10.00");
}

void PruebaParsearSoles() {
    Centimos monto = -1;
    VERIFICAR(ParsearSoles("12", monto) && monto == 1200);
    VERIFICAR(ParsearSoles("12.5", monto) && monto == 1250);
    VERIFICAR(ParsearSoles("12,05", monto) && monto == 1205);
    VERIFICAR(ParsearSoles("  7.10 ", monto) && monto == 710);
    VERIFICAR(ParsearSoles(".5", monto) && monto == 50);
    VERIFICAR(!ParsearSoles("", monto));
    VERIFICAR(!ParsearSoles("abc", monto));
    VERIFICAR(!ParsearSoles("12abc", monto));
    VERIFICAR(!ParsearSoles("-5", monto));
    VERIFICAR(!ParsearSoles("1.234", monto));
    VERIFICAR(!ParsearSoles("1.2.3", monto));
    VERIFICAR(!ParsearSoles("9999999999999", monto));
}

void PruebaParsearEntero() {
    int valor = 0;
    VERIFICAR(ParsearEntero("42", valor) && valor == 42);
    VERIFICAR(ParsearEntero(" -3 ", valor) && valor == -3);
    VERIFICAR(!ParsearEntero("", valor));
    VERIFICAR(!ParsearEntero("12abc", valor));
    VERIFICAR(!ParsearEntero("1.5", valor));
    VERIFICAR(!ParsearEntero("99999999999999", valor));
}

void PruebaTexto() {
    VERIFICAR(Recortar("  hola  ") == "hola");
    VERIFICAR(Recortar("   ") == "");
    VERIFICAR(AMayusculas("Ana") == "ANA");
    VERIFICAR(IgualesSinMayusculas("Ana Maria", "ANA maria"));
    VERIFICAR(!IgualesSinMayusculas("Ana", "Ana Maria"));
    VERIFICAR(ContieneSinMayusculas("Ana Maria", "mar"));
    // No debe fallar con caracteres fuera de ASCII (ñ en UTF-8).
    VERIFICAR(AMayusculas("pe\xc3\xb1" "a") == "PE\xc3\xb1" "A");
}

// --- Cartas y mazo ---

void PruebaCarta() {
    VERIFICAR(Carta(1, Palo::Espadas).GetFuerza() == 14);
    VERIFICAR(Carta(13, Palo::Espadas).GetFuerza() == 13);
    VERIFICAR(Carta(1, Palo::Corazones).TextoValor() == "A");
    VERIFICAR(Carta(10, Palo::Corazones).TextoValor() == "10");
    VERIFICAR(Carta(12, Palo::Corazones).TextoValor() == "Q");
    VERIFICAR(Carta(5, Palo::Diamantes).Lineas().size() == 7);
    VERIFICAR_LANZA(Carta(0, Palo::Espadas));
    VERIFICAR_LANZA(Carta(14, Palo::Espadas));
}

void PruebaMazo() {
    Aleatorio azar(123);
    Mazo mazo(azar);
    VERIFICAR(mazo.Cantidad() == 52);

    std::set<std::pair<int, int>> vistas;
    for (int i = 0; i < 52; i++) {
        const Carta carta = mazo.Robar();
        vistas.insert({carta.GetValor(), static_cast<int>(carta.GetPalo())});
    }
    VERIFICAR(vistas.size() == 52);  // ninguna carta repetida
    VERIFICAR(mazo.Cantidad() == 0);

    mazo.Robar();  // un mazo vacio se vuelve a llenar
    VERIFICAR(mazo.Cantidad() == 51);
}

// --- Jugador ---

void PruebaJugador() {
    Jugador jugador("Ana", Soles(50));
    jugador.Recargar(Soles(10));
    VERIFICAR(jugador.GetSaldo() == Soles(60));
    jugador.AplicarResultado(-Soles(60));
    VERIFICAR(jugador.GetSaldo() == 0);
    jugador.AplicarResultado(Soles(25));
    VERIFICAR(jugador.GetSaldo() == Soles(25));

    VERIFICAR_LANZA(jugador.Recargar(0));
    VERIFICAR_LANZA(jugador.AplicarResultado(-Soles(26)));
    VERIFICAR_LANZA(Jugador("X", -1));
}

void PruebaRankingYBusqueda() {
    const std::vector<Jugador> jugadores = {
        Jugador("Ana", Soles(10)), Jugador("Beto", Soles(30)), Jugador("Carla", Soles(30)),
        Jugador("Mariana", Soles(20))};

    const std::vector<Jugador> ranking = OrdenarPorSaldo(jugadores);
    VERIFICAR(ranking[0].GetNombre() == "Beto");   // empate: se respeta el orden de registro
    VERIFICAR(ranking[1].GetNombre() == "Carla");
    VERIFICAR(ranking[3].GetNombre() == "Ana");

    VERIFICAR(BuscarPorNombre(jugadores, "ana").size() == 2);  // Ana y Mariana
    VERIFICAR(BuscarPorNombre(jugadores, "zzz").empty());
}

// --- Juegos ---

void PruebaCartaMayor() {
    const Centimos apuesta = Soles(10);
    const Carta as(1, Palo::Espadas), rey(13, Palo::Corazones), otroRey(13, Palo::Treboles);
    VERIFICAR(CartaMayor::CalcularResultado(rey, as, apuesta) == apuesta);
    VERIFICAR(CartaMayor::CalcularResultado(as, rey, apuesta) == -apuesta);
    VERIFICAR(CartaMayor::CalcularResultado(rey, otroRey, apuesta) == 0);
}

void PruebaRuleta() {
    const Centimos apuesta = Soles(10);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Par, -1, 4, apuesta) == apuesta);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Par, -1, 5, apuesta) == -apuesta);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Par, -1, 0, apuesta) == -apuesta);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Impar, -1, 7, apuesta) == apuesta);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Impar, -1, 0, apuesta) == -apuesta);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Numero, 17, 17, apuesta) ==
              apuesta * Ruleta::PAGO_NUMERO);
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Numero, 17, 18, apuesta) == -apuesta);
    // Bug del original: apostar al 0 y que salga 0 ahora si gana.
    VERIFICAR(Ruleta::CalcularResultado(TipoApuestaRuleta::Numero, 0, 0, apuesta) > 0);
}

void PruebaMundialito() {
    Aleatorio azar(7);
    std::vector<std::string> equipos(Mundialito::EquiposBase().begin(), Mundialito::EquiposBase().begin() + 8);

    const auto rondas = Mundialito::SimularTorneo(equipos, azar);
    VERIFICAR(rondas.size() == 3);  // 8 -> 4 -> 2 -> 1
    VERIFICAR(rondas[0].size() == 4);
    VERIFICAR(rondas[2].size() == 1);

    const std::string campeon = rondas.back().front().ganador;
    VERIFICAR(std::find(equipos.begin(), equipos.end(), campeon) != equipos.end());

    // Cada ganador debe ser uno de los dos equipos de su partido.
    for (const auto& ronda : rondas) {
        for (const Partido& p : ronda) {
            VERIFICAR(p.ganador == p.local || p.ganador == p.visitante);
        }
    }

    VERIFICAR(Mundialito::EquiposBase().size() == 16);
    VERIFICAR_LANZA(Mundialito::SimularTorneo({"A", "B", "C"}, azar));
    VERIFICAR_LANZA(Mundialito::SimularTorneo({"A"}, azar));
}

void PruebaAleatorio() {
    Aleatorio a(42), b(42);
    for (int i = 0; i < 100; i++) {
        const int x = a.Entero(0, 36);
        VERIFICAR(x >= 0 && x <= 36);
        VERIFICAR(x == b.Entero(0, 36));  // misma semilla, misma secuencia
    }
}

}  // namespace

int main() {
    const std::vector<std::pair<std::string, std::function<void()>>> pruebas = {
        {"FormatearSoles", PruebaFormatearSoles},
        {"ParsearSoles", PruebaParsearSoles},
        {"ParsearEntero", PruebaParsearEntero},
        {"Texto", PruebaTexto},
        {"Carta", PruebaCarta},
        {"Mazo", PruebaMazo},
        {"Jugador", PruebaJugador},
        {"RankingYBusqueda", PruebaRankingYBusqueda},
        {"CartaMayor", PruebaCartaMayor},
        {"Ruleta", PruebaRuleta},
        {"Mundialito", PruebaMundialito},
        {"Aleatorio", PruebaAleatorio},
    };

    for (const auto& prueba : pruebas) {
        const int antes = fallos;
        prueba.second();
        std::cout << (fallos == antes ? "[ OK ] " : "[FALLA] ") << prueba.first << "\n";
    }

    std::cout << "\n" << pruebas.size() << " pruebas, " << fallos << " verificaciones fallidas.\n";
    return fallos == 0 ? 0 : 1;
}
