#include "casino.h"

#include <iomanip>
#include <iostream>

#include "consola.h"
#include "entrada.h"
#include "juegos/carta_mayor.h"
#include "juegos/mundialito.h"
#include "juegos/ruleta.h"
#include "texto.h"

namespace casino {

namespace {

const std::string LINEA = "------------------------------------------";

void Titulo(const std::string& texto) {
    ImprimirLinea("------ " + texto + " ------", Color::Cian);
}

}  // namespace

Casino::Casino() {
    juegos.push_back(std::unique_ptr<Juego>(new CartaMayor(azar)));
    juegos.push_back(std::unique_ptr<Juego>(new Ruleta(azar)));
    juegos.push_back(std::unique_ptr<Juego>(new Mundialito(azar)));
}

// -------------------------------------------------------------------------
// --- UTILIDADES ---
// -------------------------------------------------------------------------

void Casino::MostrarLogo() const {
    CambiarColor(Color::Amarillo);
    std::cout << "   _____          _               \n";
    std::cout << "  / ____|        (_)              \n";
    std::cout << " | |     __ _ ___ _ _ __   ___    \n";
    std::cout << " | |    / _` / __| | '_ \\ / _ \\   \n";
    std::cout << " | |___| (_| \\__ \\ | | | | (_) |  \n";
    std::cout << "  \\_____\\__,_|___/_|_| |_|\\___/   \n";
    std::cout << "           POO EDITION      \n\n";
    CambiarColor(Color::Normal);
}

void Casino::MostrarListaJugadores(const std::vector<Jugador>& lista) const {
    ImprimirLinea(LINEA, Color::Cian);
    std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "NOMBRE" << "SALDO DISPONIBLE\n";
    ImprimirLinea(LINEA, Color::Cian);

    for (size_t i = 0; i < lista.size(); i++) {
        std::cout << std::left << std::setw(5) << (i + 1) << std::setw(20) << lista[i].GetNombre();
        ImprimirLinea(FormatearSoles(lista[i].GetSaldo()), Color::Verde);
    }
    ImprimirLinea(LINEA, Color::Cian);
}

bool Casino::HayJugadores() const {
    if (!jugadores.empty()) return true;
    ImprimirLinea("No hay jugadores registrados. Registre uno desde el menu.", Color::Rojo);
    EsperarEnter();
    return false;
}

bool Casino::ExisteJugador(const std::string& nombre) const {
    for (const Jugador& jugador : jugadores) {
        if (IgualesSinMayusculas(jugador.GetNombre(), nombre)) return true;
    }
    return false;
}

int Casino::SeleccionarJugador() const {
    MostrarListaJugadores(jugadores);
    const int id = LeerEntero("ID del jugador (0 para volver): ", 0, static_cast<int>(jugadores.size()));
    return id - 1;
}

// -------------------------------------------------------------------------
// --- GESTION DE JUGADORES ---
// -------------------------------------------------------------------------

void Casino::RegistrarJugador() {
    do {
        LimpiarPantalla();
        Titulo("Registro de jugadores");
        const std::string nombre = LeerTextoNoVacio("Nombre del jugador: ");

        if (ExisteJugador(nombre)) {
            ImprimirLinea("El nombre '" + nombre + "' ya existe.", Color::Rojo);
            continue;
        }

        const Centimos saldo = LeerMonto("Saldo inicial (min " + FormatearSoles(MONTO_MINIMO) + "): ",
                                         MONTO_MINIMO, RECARGA_MAXIMA);
        jugadores.emplace_back(nombre, saldo);
        ImprimirLinea("-> Jugador '" + nombre + "' registrado con exito!", Color::Verde);
    } while (Confirmar("Desea registrar a otro jugador?"));
}

void Casino::RecargarSaldo() {
    Titulo("Recargar saldo");
    if (!HayJugadores()) return;

    do {
        const int idx = SeleccionarJugador();
        if (idx < 0) return;

        Jugador& jugador = jugadores[idx];
        std::cout << "Saldo actual: ";
        ImprimirLinea(FormatearSoles(jugador.GetSaldo()), Color::Verde);

        const Centimos monto = LeerMonto("Monto a recargar (min " + FormatearSoles(MONTO_MINIMO) + "): ",
                                         MONTO_MINIMO, RECARGA_MAXIMA);
        jugador.Recargar(monto);
        ImprimirLinea("Nuevo saldo de '" + jugador.GetNombre() + "': " + FormatearSoles(jugador.GetSaldo()),
                      Color::Verde);
    } while (Confirmar("Desea recargar saldo a otro jugador?"));
}

void Casino::MostrarRanking() const {
    Titulo("Ranking");
    if (!HayJugadores()) return;

    MostrarListaJugadores(OrdenarPorSaldo(jugadores));
    EsperarEnter();
}

void Casino::BuscarJugador() const {
    Titulo("Buscador");
    if (!HayJugadores()) return;

    const std::string buscado = LeerTextoNoVacio("Nombre (o parte del nombre) a buscar: ");
    const std::vector<Jugador> encontrados = BuscarPorNombre(jugadores, buscado);

    if (encontrados.empty()) {
        ImprimirLinea("No se encontro ningun jugador.", Color::Rojo);
    } else {
        MostrarListaJugadores(encontrados);
    }
    EsperarEnter();
}

// -------------------------------------------------------------------------
// --- JUEGOS ---
// -------------------------------------------------------------------------

// Ronda comun a todos los juegos: elegir jugador, apostar, jugar y actualizar saldo.
void Casino::JugarJuego(Juego& juego) {
    do {
        LimpiarPantalla();
        Titulo("Bienvenido a " + juego.GetNombre());
        if (!HayJugadores()) return;

        const int idx = SeleccionarJugador();
        if (idx < 0) return;
        Jugador& jugador = jugadores[idx];

        if (jugador.GetSaldo() < MONTO_MINIMO) {
            ImprimirLinea("Saldo insuficiente (min " + FormatearSoles(MONTO_MINIMO) + "). Recargue saldo.",
                          Color::Rojo);
            continue;
        }

        const Centimos apuesta =
            LeerMonto("Apuesta (" + FormatearSoles(MONTO_MINIMO) + " - " + FormatearSoles(jugador.GetSaldo()) +
                          ", 0 para cancelar): ",
                      MONTO_MINIMO, jugador.GetSaldo(), true);
        if (apuesta == 0) continue;

        jugador.AplicarResultado(juego.Jugar(apuesta));

        std::cout << "Nuevo saldo de " << jugador.GetNombre() << ": ";
        ImprimirLinea(FormatearSoles(jugador.GetSaldo()), Color::Verde);
    } while (Confirmar("Jugar de nuevo?"));
}

// -------------------------------------------------------------------------
// --- MENU PRINCIPAL ---
// -------------------------------------------------------------------------

void Casino::Ejecutar() {
    // Opciones: 1 y 2 fijas, luego un numero por juego y al final ranking, buscar y salir.
    const int primerJuego = 3;
    const int opcionRanking = primerJuego + static_cast<int>(juegos.size());
    const int opcionBuscar = opcionRanking + 1;
    const int opcionSalir = opcionBuscar + 1;

    while (true) {
        LimpiarPantalla();
        MostrarLogo();
        std::cout << "1. Registrar jugador\n";
        std::cout << "2. Recargar saldo\n";
        for (size_t i = 0; i < juegos.size(); i++) {
            std::cout << (primerJuego + static_cast<int>(i)) << ". Jugar " << juegos[i]->GetNombre() << "\n";
        }
        std::cout << opcionRanking << ". Ranking\n";
        std::cout << opcionBuscar << ". Buscar jugador\n";
        std::cout << opcionSalir << ". Salir\n";

        const int opcion = LeerEntero("Opcion: ", 1, opcionSalir);
        LimpiarPantalla();

        if (opcion == 1) {
            RegistrarJugador();
        } else if (opcion == 2) {
            RecargarSaldo();
        } else if (opcion < opcionRanking) {
            JugarJuego(*juegos[opcion - primerJuego]);
        } else if (opcion == opcionRanking) {
            MostrarRanking();
        } else if (opcion == opcionBuscar) {
            BuscarJugador();
        } else {
            ImprimirLinea("MUCHAS GRACIAS POR JUGAR! VUELVA PRONTO :D", Color::Amarillo);
            return;
        }
    }
}

}  // namespace casino
