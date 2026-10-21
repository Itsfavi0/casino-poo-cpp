#include <iostream>
#include <string>
#include <cstdlib>      
#include <ctime>        
#include <algorithm>    
#include <cctype>       
#include <limits>       
#include <windows.h>
#include <vector>
#include <iomanip>

using namespace std;

// -------------------------------------------------------------------------
// --- UTILIDADES GLOBALES ---
// -------------------------------------------------------------------------
void Color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Esperar(double segundos) {
    Sleep(static_cast<int>(segundos * 1000));
}

void LimpiarPantalla() {
    system("cls");
}

string aMayusculas(string s) { 
    transform(s.begin(), s.end(), s.begin(), ::toupper); return s; 
}

// Template para validar entradas numéricas
template <typename T>
void VerificarEntrada(T &entrada) {
    while (cin.fail()) {
        Color(12);
        cout << "Entrada invalida. Vuelva a intentar: ";
        Color(7);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> entrada;
    }
}

// -------------------------------------------------------------------------
// --- CLASE CARTA ---
// -------------------------------------------------------------------------
class Carta {
    private:
        int valor;
        string palo;

    public:
        Carta(int v, string p) : valor(v), palo(p) {
        }

        int GetValor() const {
            return valor; 
        }
        
        int GetFuerza() const {
            return (valor == 1) ? 14 : valor;
        }

        string GetPalo() const {
            return palo; 
        }

        void Dibujar() const {
            string simbolo; 
            int colorCarta;

            if (palo == "C") { simbolo = "(C)"; colorCarta = 12; }      
            else if (palo == "D") { simbolo = "<D>"; colorCarta = 12; } 
            else if (palo == "E") { simbolo = "{E}"; colorCarta = 11; } 
            else if (palo == "T") { simbolo = "(T)"; colorCarta = 10; } 

            string vTexto;
            if (valor == 1) vTexto = "A";
            else if (valor == 11) vTexto = "J";
            else if (valor == 12) vTexto = "Q";
            else if (valor == 13) vTexto = "K";
            else vTexto = to_string(valor);

            Color(colorCarta);

            cout << " _______ " << endl;
            if (vTexto.length() == 2) cout << "|" << vTexto << "     |" << endl; 
            else cout << "|" << vTexto << "      |" << endl; 

            cout << "|       |" << endl;
            cout << "|  " << simbolo << "  |" << endl; 
            cout << "|       |" << endl;

            if (vTexto.length() == 2) cout << "|     " << vTexto << "|" << endl;
            else cout << "|      " << vTexto << "|" << endl;

            cout << " _______ " << endl;
            Color(7); 
        }
};

// -------------------------------------------------------------------------
// --- CLASE MAZO ---
// -------------------------------------------------------------------------
class Mazo {
    private:
        vector<Carta> cartas;
    public:
        Mazo() {
            Llenar();
        }

        void Llenar() {
            cartas.clear();
            string palos[] = {"E", "C", "D", "T"}; 
            for (int j = 0; j < 4; j++) {
                for (int k = 1; k <= 13; k++) {
                    cartas.emplace_back(k, palos[j]);
                }
            }
        }

        void Barajar() {
            for (size_t i = 0; i < cartas.size(); i++) {
                int j = rand() % cartas.size();
                swap(cartas[i], cartas[j]);
            }
        }

        Carta Robar() {
            if (cartas.empty()) Llenar();
            Carta c = cartas.back();
            cartas.pop_back();
            return c;
        }

        int Cantidad() const {
            return cartas.size(); 
        }
};

// -------------------------------------------------------------------------
// --- CLASE JUGADOR ---
// -------------------------------------------------------------------------
class Jugador {
    private:
        string nombre;
        double saldo;
    public:
        Jugador(string n, double s) : nombre(n), saldo(s) {
        }

        string GetNombre() const {
            return nombre; 
        }
        
        double GetSaldo() const {
            return saldo; 
        }

        void Recargar(double monto) {
            if (monto > 0) saldo += monto;
        }

        void Ganar(double monto) {
            saldo += monto;
        }

        void Perder(double monto) {
            saldo -= monto;
        }
};

// -------------------------------------------------------------------------
// --- CLASE CASINO ---
// -------------------------------------------------------------------------
class Casino {
    private:
        vector<Jugador> jugadores;
        Mazo mazo;

        const vector<string> EQUIPOS_BASE = {
            "Real Madrid", "Barcelona", "Bayern", "Liverpool", "City", "PSG", 
            "Boca Juniors", "River Plate", "Flamengo", "Alianza Lima", "Universitario", 
            "Cristal", "Juventus", "Milan", "Inter", "Chelsea"
        };

        // --- MÉTODOS PRIVADOS ---

        string PedirConfirmacion(string mensaje) {
            string opc;
            cout << mensaje << " (S/N): ";
            cin >> opc;
            while (aMayusculas(opc) != "S" && aMayusculas(opc) != "N") {
                Color(12);
                cout << "Opcion '" << opc << "' invalida. Vuelva a intentar (S/N): ";
                Color(7);
                cin >> opc;
            }
            return aMayusculas(opc);
        }

        void MostrarLogo() {
            Color(14); // Amarillo
            cout << "   _____          _               " << endl;
            cout << "  / ____|        (_)              " << endl;
            cout << " | |     __ _ ___ _ _ __   ___    " << endl;
            cout << " | |    / _` / __| | '_ \\ / _ \\   " << endl;
            cout << " | |___| (_| \\__ \\ | | | | (_) |  " << endl;
            cout << "  \\_____\\__,_|___/_|_| |_|\\___/   " << endl;
            cout << "           POO EDITION      " << endl;
            cout << endl;
            Color(7);
        }

        void MostrarListaJugadores(const vector<Jugador>& jugadores) {
            Color(11); // Cian para bordes
            cout << "------------------------------------------" << endl;
            cout << left << setw(5) << "ID" << setw(20) << "NOMBRE" << "SALDO DISPONIBLE" << endl;
            cout << "------------------------------------------" << endl;
            Color(7); // Blanco

            for (size_t i = 0; i < jugadores.size(); i++) {
                cout << left << setw(5) << (i + 1)
                     << setw(20) << jugadores[i].GetNombre();
                
                Color(10); // Verde para el saldo
                cout << "S/ " << fixed << setprecision(2) << jugadores[i].GetSaldo() << endl;
                Color(7);
            }
            Color(11);
            cout << "------------------------------------------" << endl;
            Color(7);
        }

        int SeleccionarJugador() {
            int nNatural, idx;
            string opc;
            do {
                cout << "Indique el ID del jugador: ";
                cin >> nNatural;
                VerificarEntrada(nNatural);
                idx = nNatural - 1;

                if (idx >= 0 && idx < jugadores.size()){
                    return idx;
                }
                
                Color(12); cout << "ID invalido." << endl; Color(7);
                opc = PedirConfirmacion("Desea intentarlo otra vez?");
                if (opc == "N") return -1;
            } while (opc == "S");
            return -1;
        }

        double PedirApuesta(double saldo) {
            double s;
            if (saldo < 10) {
                Color(12);
                cout << "Saldo insuficiente (Min S/10). Recargue saldo." << endl;
                Color(7);
                return 0;
            }
            cout << "Apuesta: ";
            cin >> s;
            VerificarEntrada(s);
            if (s < 10 || s > saldo) {
                Color(12);
                cout << "Monto no valido." << endl;
                Color(7);
                return 0;
            }
            return s;
        }

        bool ExisteJugador(string nombre) {
            for (const auto& j : jugadores) {
                if (aMayusculas(j.GetNombre()) == aMayusculas(nombre)) return true;
            }
            return false;
        }

    public:
        Casino() {
            mazo.Llenar();
            mazo.Barajar();
        }

        // --- MÉTODOS DE GESTIÓN ---

        void RegistrarJugador() {
            string opc, nombre;
            double s;
            do {
                Color(11); cout << "------ Bienvenido al registro de los jugadores -----" << endl; Color(7);
                cout << "Nombre del jugador: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, nombre);

                if (ExisteJugador(nombre)) {
                    Color(12);
                    cout << "El nombre '" << nombre << "' ya existe." << endl;
                    Color(7);
                    opc = PedirConfirmacion("Quiere poner un nuevo nombre?");
                    LimpiarPantalla();
                    continue;
                }

                do {
                    cout << "Saldo inicial (min S/10): ";
                    cin >> s;
                    VerificarEntrada(s);
                    if (s < 10) {
                        Color(12); cout << "Minima recarga S/ 10." << endl; Color(7);
                    }
                } while (s < 10);

                jugadores.emplace_back(nombre, s);
                Color(10);
                cout << "-> Jugador '" << nombre << "' registrado con exito!" << endl;
                Color(7);
                Esperar(2.5);
                cout << "---------------" << endl;
                
                opc = PedirConfirmacion("Desea registrar a un nuevo jugador?");
                LimpiarPantalla();

            } while (opc == "S");
            Esperar(1);
        }

        void RecargarSaldo() {
            string opc;
            do {
                Color(11); cout << "------ Recargar saldo de los jugadores -----" << endl; Color(7);
                if (jugadores.empty()) {
                    cout << "No hay jugadores registrados." << endl;
                    cout << "Volviendo al menu..." << endl;
                    Esperar(3);
                    break;
                }
                
                MostrarListaJugadores(jugadores);
                int idx = SeleccionarJugador();
                if (idx == -1) break;

                double monto;
                cout << "Saldo actual: ";
                Color(10); cout << "S/ " << jugadores[idx].GetSaldo() << endl; Color(7);
                
                do {
                    cout << "Monto a recargar (min S/10): ";
                    cin >> monto;
                    VerificarEntrada(monto);
                } while (monto < 10);

                cout << "\nRecargado saldo. Por favor espere..." << endl;
                Esperar(2);
                jugadores[idx].Recargar(monto);
                
                Color(10);
                cout << "\nSaldo actualizado con exito :D." << endl;
                cout << "Nuevo saldo de '" << jugadores[idx].GetNombre() << "': S/ " << jugadores[idx].GetSaldo() << endl;
                Color(7);
                cout << "---------------" << endl;
                Esperar(1.5);
                
                opc = PedirConfirmacion("Desea recargar saldo a otro jugador?");
                LimpiarPantalla();
            } while (opc == "S");
        }

        void OrdenarRanking() {
            if (jugadores.empty()) { cout << "Sin jugadores." << endl; Esperar(2); return; }
            
            //Creamos una copia
            vector<Jugador> ranking = jugadores;
            int n = ranking.size();

            //MÉTODO CLÁSICO
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if (ranking[j].GetSaldo() < ranking[j + 1].GetSaldo()) {
                        Jugador temp = ranking[j];
                        ranking[j] = ranking[j + 1];
                        ranking[j + 1] = temp;
                    }
                }
            }

            cout << "Ordenando de mayor a menor saldo .." << endl;
            Esperar(1.5);
            LimpiarPantalla();

            Color(11); cout << "--- RANKING ---" << endl; Color(7);
            MostrarListaJugadores(ranking);
            cout << "\nEnter para salir...";
            cin.ignore(); cin.get();
        }

        void BuscarJugador() {
            if (jugadores.empty()) { cout << "Sin jugadores." << endl; Esperar(2); return; }
            
            string buscado;
            Color(11); cout << "--- BUSCADOR ---" << endl; Color(7);
            cout << "Nombre a buscar: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            getline(cin, buscado);

            bool encontrado = false;
            for (const auto& j : jugadores) {
                if (aMayusculas(j.GetNombre()) == aMayusculas(buscado)) {
                    Color(10);
                    cout << "------------------------------------------" << endl;
                    cout << "ENCONTRADO: " << j.GetNombre() << " - S/ " << fixed << setprecision(2) << j.GetSaldo() << endl;
                    cout << "------------------------------------------" << endl;
                    Color(7);
                    encontrado = true;
                    break;
                }
            }
            
            if (!encontrado) {
                Color(12); cout << "No encontrado." << endl; Color(7); 
            }

            cout << "\nEnter para salir...";
            cin.ignore(); cin.get();
            LimpiarPantalla();
        }

        // --- MÉTODOS DE JUEGO ---

        void JugarCartaMayor() {
            string opc;
            do {
                Color(11); cout << "----- Bienvenido al Juego de Carta Mayor -------" << endl; Color(7);
                if (jugadores.empty()) {
                    cout << "No hay jugadores registrados." << endl;
                    cout << "Volviendo al menu..." << endl;
                    Esperar(3);
                    break;
                }

                MostrarListaJugadores(jugadores);
                int idx = SeleccionarJugador();
                if (idx == -1) break;

                double apuesta = PedirApuesta(jugadores[idx].GetSaldo());
                if (apuesta == 0) {
                    opc = PedirConfirmacion("Reintentar?");
                    LimpiarPantalla(); continue;
                }

                if (mazo.Cantidad() < 2) { 
                    cout << "Baraja vacía. Re-barajando..." << endl;
                    mazo.Llenar(); mazo.Barajar(); Esperar(1.5); 
                }

                Carta cartaC = mazo.Robar();
                Carta cartaJ = mazo.Robar();

                cout << endl;
                cout << "Se estan repartiendo las cartas..." << endl;
                Esperar(1.5);

                cout << "Croupier:" << endl; cartaC.Dibujar();
                Esperar(1);
                cout << "Tu carta:" << endl; cartaJ.Dibujar();

                if (cartaJ.GetFuerza() > cartaC.GetFuerza()) {
                    Color(10); cout << "GANAS S/" << apuesta << "!!!" << endl; Color(7);
                    jugadores[idx].Ganar(apuesta);
                } else if (cartaJ.GetFuerza() == cartaC.GetFuerza()) {
                    Color(14); cout << "EMPATE." << endl; Color(7);
                } else {
                    Color(12); cout << "PIERDES S/" << apuesta << " :c" << endl; Color(7);
                    jugadores[idx].Perder(apuesta);
                }
                
                cout << "Nuevo saldo: "; Color(10); cout << jugadores[idx].GetSaldo() << endl; Color(7);
                opc = PedirConfirmacion("Jugar de nuevo?");
                LimpiarPantalla();
            } while (opc == "S");
        }

        void JugarRuleta() {
            string opc;
            do {
                Color(11); cout << "--------- Bienvenido al juego de La Ruleta ---------" << endl; Color(7);
                if (jugadores.empty()) {
                    cout << "No hay jugadores registrados." << endl;
                    cout << "Volviendo al menu..." << endl;
                    Esperar(3);
                    break;
                }

                MostrarListaJugadores(jugadores);
                int idx = SeleccionarJugador();
                if (idx == -1) break;

                double apuesta = PedirApuesta(jugadores[idx].GetSaldo());
                if (apuesta == 0) {
                    opc = PedirConfirmacion("Reintentar?");
                    LimpiarPantalla(); continue;
                }

                int tipoApuesta;
                cout << "Tipo de apuesta?" << endl;
                cout << "0: Par (x2)" << endl;
                cout << "1: Impar (x2)" << endl;
                cout << "2: Numero exacto (x10)" << endl;
                cin >> tipoApuesta;
                VerificarEntrada(tipoApuesta);

                while (tipoApuesta < 0 || tipoApuesta >= 3) {
                    Color(12); cout << "Opcion invalida." << endl; Color(7);
                    cin >> tipoApuesta;
                }

                int nApostado = -1;
                if (tipoApuesta == 2) {
                    cout << "Numero (0-36): "; cin >> nApostado;
                }

                cout << "Girando..." << endl; Esperar(2);
                int nRuleta = rand() % 37;
                Color(14); cout << "Salio el: " << nRuleta << endl; Color(7);

                bool gana = false;
                double multiplicador = 1;

                if (nRuleta != 0) {
                    if (tipoApuesta == 0 && nRuleta % 2 == 0) gana = true;
                    else if (tipoApuesta == 1 && nRuleta % 2 != 0) gana = true;
                    else if (tipoApuesta == 2 && nRuleta == nApostado) { gana = true; multiplicador = 10; }
                }

                if (gana) {
                    Color(10); cout << "GANASTE!" << endl; Color(7);
                    jugadores[idx].Ganar(apuesta * multiplicador);
                } else {
                    Color(12); cout << "PIERDES." << endl; Color(7);
                    jugadores[idx].Perder(apuesta);
                }

                cout << "--------------------" << endl;
                cout << "Su nuevo saldo es de: "; Color(10); cout << jugadores[idx].GetSaldo() << endl; Color(7);
                opc = PedirConfirmacion("Jugar de nuevo?");
                LimpiarPantalla();
            } while (opc == "S");
        }

        void JugarMundialito() {
            string opc;
            do {
                Color(11); cout << "------ Bienvenidos al Mundial de Clubes ------" << endl; Color(7);
                if (jugadores.empty()) {
                    cout << "No hay jugadores registrados." << endl;
                    cout << "Volviendo al menu..." << endl;
                    Esperar(3);
                    break;
                }

                MostrarListaJugadores(jugadores);
                int idx = SeleccionarJugador();
                if (idx == -1) break;

                double apuesta = PedirApuesta(jugadores[idx].GetSaldo());
                if (apuesta == 0) {
                    opc = PedirConfirmacion("Reintentar?");
                    LimpiarPantalla(); continue;
                }

                int modo;
                cout << "1.Rapido (8) | 2.Completo (16): ";
                cin >> modo;
                int nEquipos = (modo == 1) ? 8 : 16;

                vector<string> torneo = EQUIPOS_BASE; 
                
                // --- BARAJADO MANUAL ---
                int totalEquipos = torneo.size();
                for (int k = 0; k < 50; k++) {
                    int pos1 = rand() % totalEquipos;
                    int pos2 = rand() % totalEquipos;
                    
                    string temporal = torneo[pos1];
                    torneo[pos1] = torneo[pos2];
                    torneo[pos2] = temporal;
                }

                torneo.resize(nEquipos); 

                cout << "Equipos:" << endl;
                for(size_t i=0; i<torneo.size(); ++i) cout << (i+1) << "." << torneo[i] << endl;

                int eleccion;
                cout << "Tu equipo (Numero): "; cin >> eleccion;
                string miEquipo = torneo[eleccion - 1];

                while(torneo.size() > 1) {
                    vector<string> ganadores;
                    cout << "\n--- Ronda de " << torneo.size() << " ---" << endl;
                    for(size_t i=0; i<torneo.size(); i+=2) {
                        string winner = (rand()%2 == 0) ? torneo[i] : torneo[i+1];
                        cout << torneo[i] << " vs " << torneo[i+1] << " -> ";
                        
                        if(winner == miEquipo) Color(10);
                        else Color(7);
                        cout << winner << endl;
                        Color(7);

                        ganadores.push_back(winner);
                    }
                    torneo = ganadores;
                    Esperar(2);
                }

                cout << "\nCAMPEON: "; Color(14); cout << torneo[0] << endl; Color(7);
                
                if (torneo[0] == miEquipo) {
                    Color(10); cout << "CAMPEONES!! x2 Ganancia" << endl; Color(7);
                    jugadores[idx].Ganar(apuesta * 2);
                } else {
                    Color(12); cout << "Tu equipo '" << miEquipo << "' fue eliminado." << endl; Color(7);
                    jugadores[idx].Perder(apuesta);
                }
                opc = PedirConfirmacion("Jugar de nuevo?");
                LimpiarPantalla();
            } while (opc == "S");
        }

        void Ejecutar() {
            int opc;
            do {
                LimpiarPantalla();
                MostrarLogo();
                cout << "1. Registrar jugador" << endl;
                cout << "2. Recargar Saldo" << endl;
                cout << "3. Jugar Carta Mayor" << endl;
                cout << "4. La Ruleta" << endl;
                cout << "5. Mundial de Clubes" << endl;
                cout << "6. Ranking" << endl;
                cout << "7. Buscar Jugador" << endl;
                cout << "8. Salir" << endl;
                
                cin >> opc;
                VerificarEntrada(opc);

                LimpiarPantalla();
                switch (opc) {
                    case 1: RegistrarJugador(); break;
                    case 2: RecargarSaldo(); break;
                    case 3: JugarCartaMayor(); break;
                    case 4: JugarRuleta(); break;
                    case 5: JugarMundialito(); break;
                    case 6: OrdenarRanking(); break;
                    case 7: BuscarJugador(); break;
                    case 8: cout << "MUCHAS GRACIAS POR USAR! VUELVA PRONTO :D" << endl; break;
                    default: Color(12); cout << "Opcion invalida" << endl; Color(7); Esperar(1);
                }
            } while (opc != 8);
        }
};

// -------------------------------------------------------------------------
// --- MAIN ---
// -------------------------------------------------------------------------
int main() {
    SetConsoleOutputCP(437); 
    srand(time(NULL));

    Casino casinoBertila;
    casinoBertila.Ejecutar();

    return 0;
}