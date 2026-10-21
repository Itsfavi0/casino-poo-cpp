# 🎰 Casino POO

Casino de consola en C++ con tres juegos, gestión de jugadores y saldo. Empezó como el
proyecto final de mi curso de programación orientada a objetos y luego lo refactoricé
para aplicar buenas prácticas de C++ moderno.

```
Croupier:    Tu carta:
 _______      _______
|2      |    |A      |
|       |    |       |
|   ♠   |    |   ♥   |
|       |    |       |
|      2|    |      A|
|_______|    |_______|
GANAS S/ 20.00!!!
```

## Funcionalidades

- **Registro de jugadores** con saldo inicial (no permite nombres repetidos ni vacíos).
- **Recarga de saldo**, **ranking** por saldo y **buscador** por nombre o parte del nombre.
- **Juegos:**
  | Juego | Reglas | Pago |
  |---|---|---|
  | Carta Mayor | Tu carta contra la del croupier, el As es la más alta | 1 a 1 (empate devuelve la apuesta) |
  | La Ruleta | Ruleta europea 0-36: par, impar o número exacto | 1 a 1 / 10 a 1 |
  | Mundial de Clubes | Torneo de eliminación directa de 8 o 16 clubes | 2 a 1 si tu equipo sale campeón |

## Compilar y ejecutar

Requisitos: un compilador con C++14 (GCC, Clang o MSVC) y, opcionalmente, CMake 3.14 o superior.

**Con CMake:**

```bash
cmake -S . -B build
cmake --build build
./build/casino          # en Windows: build\Debug\casino.exe o build\casino.exe
ctest --test-dir build  # pruebas unitarias
```

**Solo con g++:**

```bash
g++ -std=c++14 -Isrc src/*.cpp src/juegos/*.cpp -o casino
```

Funciona en Windows, Linux y macOS. En Windows se recomienda Windows Terminal para ver
los símbolos de las cartas (♠ ♥ ♦ ♣).

## Estructura

```
src/
├── main.cpp
├── casino.h/.cpp        Menú principal, jugadores y flujo común de apuestas
├── jugador.h/.cpp       Jugador, ranking y búsqueda
├── carta.h/.cpp         Carta y dibujo ASCII
├── mazo.h/.cpp          Mazo de 52 cartas
├── juegos/
│   ├── juego.h          Interfaz abstracta de todos los juegos
│   ├── carta_mayor.*
│   ├── ruleta.*
│   └── mundialito.*
├── aleatorio.h/.cpp     Generador aleatorio (std::mt19937)
├── dinero.h/.cpp        Montos en céntimos y formato "S/ 12.50"
├── entrada.h/.cpp       Lectura validada del teclado
├── texto.h/.cpp         Utilidades de texto
└── consola.h/.cpp       Colores, limpiar pantalla y pausas (Windows / ANSI)
tests/
└── pruebas.cpp          Pruebas unitarias de la lógica
```

Para agregar un juego nuevo basta con crear una clase que herede de `Juego` e
implementar `GetNombre()` y `Jugar(apuesta)`; el casino se encarga de elegir el
jugador, validar la apuesta y actualizar el saldo.

## Qué mejoré respecto a la versión original

La versión original está en el primer commit del historial ("Proyecto original"): un
solo archivo de 680 líneas que solo compilaba en Windows.

**Bugs corregidos**
- Elegir un equipo fuera de rango en el Mundial de Clubes accedía fuera del vector.
- Escribir letras en ciertas preguntas de la ruleta dejaba `cin` en estado de error y el programa entraba en un bucle infinito.
- Apostar al número 0 en la ruleta nunca podía ganar.
- El modo del torneo y el número de la ruleta no se validaban.
- El buscador obligaba a presionar Enter dos veces.
- `toupper` con `char` provocaba comportamiento indefinido con letras como la ñ.
- Se podía registrar un jugador sin nombre.

**Diseño**
- De una clase `Casino` que hacía todo a módulos con una sola responsabilidad.
- Polimorfismo: los juegos implementan la interfaz `Juego`, y el código repetido de
  "elegir jugador y apostar" ahora existe una sola vez.
- El dinero se guarda en céntimos (`long long`) en vez de `double`, para evitar errores de redondeo.
- `std::mt19937` + `std::shuffle` en lugar de `rand() % n` (que no es uniforme).
- Toda la entrada se lee con `getline` y se valida en un solo lugar.
- El código específico de Windows quedó aislado en `consola.cpp`.
- Pruebas unitarias e integración continua en Windows, Linux y macOS con GitHub Actions.

## Licencia

[MIT](LICENSE)
