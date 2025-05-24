#include "Game.h" // Incluye el archivo de encabezado de la clase Game
#include <tchar.h> // Incluye la biblioteca de caracteres de Windows

int _tmain(int argc, _TCHAR* argv[])
{
    // Crear el objeto de la clase Game
    Game* Juego;
    Juego = new Game(800, 600, "Esqueleto de Aplicación - MAVII");
    Juego->Loop(); // Ejecutar el bucle principal del juego

    return 0; // Retorna 0 indicando que el programa se ha ejecutado correctamente
}
