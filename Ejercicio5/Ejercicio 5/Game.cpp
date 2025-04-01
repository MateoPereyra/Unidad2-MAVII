#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicialización de la ventana y configuración de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuración de la vista del juego
    InitPhysics(); // Inicialización del motor de física
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        UpdatePhysics(); // Actualizar la simulación física
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }

}

// Actualización de la simulación física
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo físico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo físico para depuración
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    // Dibujar el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 10));
    groundShape.setOrigin(250, 5); // Centro el rectángulo
    groundShape.setFillColor(sf::Color::Red);
    {
        b2Vec2 pos = groundBody->GetPosition();
        float angle = groundBody->GetAngle();
        groundShape.setPosition(pos.x, pos.y);
        groundShape.setRotation(angle * 180.0f / b2_pi); // conversión a grados
    }
    wnd->draw(groundShape);


    // Caja
    sf::RectangleShape boxShape(sf::Vector2f(10, 10));
    boxShape.setOrigin(5, 5); // centro
    boxShape.setFillColor(sf::Color::Magenta);
    {
        b2Vec2 pos = boxBody->GetPosition();
        float angle = boxBody->GetAngle();
        boxShape.setPosition(pos.x, pos.y);
        boxShape.setRotation(angle * 180.0f / b2_pi);
    }
    wnd->draw(boxShape);
}

// Procesamiento de eventos de entrada
void Game::DoEvents()
{
    Event evt;
    while (wnd->pollEvent(evt))
    {
        switch (evt.type)
        {
        case Event::Closed:
            wnd->close(); // Cerrar la ventana si se presiona el botón de cerrar
            break;
        }
    }
}

// Comprobación de colisiones (a implementar más adelante)
void Game::CheckCollitions()
{
    // Implementación de la comprobación de colisiones
}

// Configuración de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tamaño de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

// Inicialización del motor de física y los cuerpos del mundo físico
void Game::InitPhysics()
{
    // Inicializar el mundo físico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo físico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo
    groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 500, 10);
    groundBody->SetTransform(b2Vec2(0.0f, 5.0f), 0.523599f); // Rotación de 30°

    // Creo la caja
    boxBody = Box2DHelper::CreateRectangularDynamicBody(phyWorld, 10, 10, 1.0f, 0.5f, 0.5f);
    boxBody->SetTransform(b2Vec2(0.0f, 0.0f), 0.0f);;
}

// Destructor de la clase

Game::~Game(void)
{ }