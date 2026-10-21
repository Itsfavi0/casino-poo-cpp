#pragma once

#include <string>

// Todo lo que depende del sistema operativo vive aqui. En Windows se usa la
// API de consola; en Linux/macOS, codigos de escape ANSI.
namespace casino {

enum class Color { Normal, Rojo, Verde, Amarillo, Cian };

// Configura la consola (UTF-8 en Windows). Llamar una vez al inicio.
void InicializarConsola();

void CambiarColor(Color color);

// Imprime el texto en el color indicado y vuelve al color normal.
void Imprimir(const std::string& texto, Color color);
void ImprimirLinea(const std::string& texto, Color color);

void LimpiarPantalla();

void Esperar(double segundos);

}  // namespace casino
