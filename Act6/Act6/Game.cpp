#include "Game.h"
#include "Box2DHelper.h"
#include <iostream>

// Constructor de la clase Game
Game::Game(int ancho, int alto, std::string titulo)
{
    // Inicializaci�n de la ventana y configuraci�n de propiedades
    wnd = new RenderWindow(VideoMode(ancho, alto), titulo);
    wnd->setVisible(true);
    fps = 60;
    wnd->setFramerateLimit(fps);
    frameTime = 1.0f / fps;
    SetZoom(); // Configuraci�n de la vista del juego
    InitPhysics(); // Inicializaci�n del motor de f�sica
}

// Bucle principal del juego
void Game::Loop()
{
    while (wnd->isOpen())
    {
        wnd->clear(clearColor); // Limpiar la ventana
        DoEvents(); // Procesar eventos de entrada
        CheckCollitions(); // Comprobar colisiones
        CannonRotation(); //Actualizo el ca�on 
        UpdatePhysics(); // Actualizar la simulaci�n f�sica
        DrawGame(); // Dibujar el juego
        wnd->display(); // Mostrar la ventana
    }
}

// Actualizaci�n de la simulaci�n f�sica
void Game::UpdatePhysics()
{
    phyWorld->Step(frameTime, 8, 8); // Simular el mundo f�sico
    phyWorld->ClearForces(); // Limpiar las fuerzas aplicadas a los cuerpos
    phyWorld->DebugDraw(); // Dibujar el mundo f�sico para depuraci�n
}

// Dibujo de los elementos del juego
void Game::DrawGame()
{
    // Dibujar el suelo
    sf::RectangleShape groundShape(sf::Vector2f(500, 5));
    groundShape.setFillColor(sf::Color::Red);
    groundShape.setPosition(0, 95);
    wnd->draw(groundShape);

    // Dibujar las paredes
    sf::RectangleShape leftWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    leftWallShape.setFillColor(sf::Color::Red);
    leftWallShape.setPosition(100, 0); // X = 100 para que comience donde termina el suelo
    wnd->draw(leftWallShape);

    sf::RectangleShape rightWallShape(sf::Vector2f(10, alto)); // Alto de la ventana
    rightWallShape.setFillColor(sf::Color::Red);
    rightWallShape.setPosition(90, 0); // X = 90 para que comience donde termina el suelo
    wnd->draw(rightWallShape);

    // Dibujar el ca�on (cuerpo de control)
    sf::RectangleShape cannonShape(sf::Vector2f(15.0f, 10.0f));
    cannonShape.setFillColor(sf::Color::Red);
    cannonShape.setPosition(controlBody->GetPosition().x, controlBody->GetPosition().y);
    cannonShape.setOrigin(7.5f, 5.0f); // Origen en la base del ca��n
    cannonShape.setRotation(controlBody->GetAngle() * 180 / b2_pi); // Box2D usa radianes, SFML grados
    wnd->draw(cannonShape);
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
            wnd->close(); // Cerrar la ventana si se presiona el bot�n de cerrar
            break;
        case Event::KeyPressed:

            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                Shoot();
            }
        }
    }
}

// Comprobaci�n de colisiones (a implementar m�s adelante)
void Game::CheckCollitions()
{
    // Implementaci�n de la comprobaci�n de colisiones
}

// Configuraci�n de la vista del juego
void Game::SetZoom()
{
    View camara;
    // Posicionamiento y tama�o de la vista
    camara.setSize(100.0f, 100.0f);
    camara.setCenter(50.0f, 50.0f);
    wnd->setView(camara); // Asignar la vista a la ventana
}

void Game::CannonRotation() {
    // Obtener posici�n del mouse en coordenadas del mundo
    Vector2i mousePixel = Mouse::getPosition(*wnd);
    Vector2f mouseWorld = wnd->mapPixelToCoords(mousePixel);

    // Obtener posici�n del ca��n en pixeles
    b2Vec2 cannonPosMeters = controlBody->GetPosition();
    Vector2f cannonPosPixels(cannonPosMeters.x, cannonPosMeters.y);

    // Calcular �ngulo hacia el mouse
    float dx = mouseWorld.x - cannonPosPixels.x;
    float dy = mouseWorld.y - cannonPosPixels.y;
    float angle = std::atan2(dy, dx); //Radianes

    // Aplicar rotaci�n
    controlBody->SetTransform(controlBody->GetPosition(), angle);
}

void Game::Shoot() {
    // Obtener el �ngulo actual del ca��n
    float angle = controlBody->GetAngle();   // �ngulo en radianes
    b2Vec2 cannonPos = controlBody->GetPosition();

    // longitud hasta la punta del ca��n (en metros)
    float length = 7.5f;

    // c�lculo manual
    float dx = std::cos(angle) * length;
    float dy = std::sin(angle) * length;
    b2Vec2 tipPos(cannonPos.x + dx,
        cannonPos.y + dy);

    // crear bala
    b2Body* bullet = Box2DHelper::CreateCircularDynamicBody(phyWorld, 0.5f, 1.0f, 0.2f, 0.1f);
    bullet->SetTransform(tipPos, angle);

    // velocidad inicial
    float speed = 50.0f;
    bullet->SetLinearVelocity(b2Vec2(std::cos(angle) * speed,
        std::sin(angle) * speed));
}

// Inicializaci�n del motor de f�sica y los cuerpos del mundo f�sico
void Game::InitPhysics()
{
    // Inicializar el mundo f�sico con la gravedad por defecto
    phyWorld = new b2World(b2Vec2(0.0f, 9.8f));

    // Crear un renderer de debug para visualizar el mundo f�sico
    debugRender = new SFMLRenderer(wnd);
    debugRender->SetFlags(UINT_MAX);
    phyWorld->SetDebugDraw(debugRender);

    // Crear el suelo y las paredes est�ticas del mundo f�sico
    b2Body* groundBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 100, 10);
    groundBody->SetTransform(b2Vec2(50.0f, 100.0f), 0.0f);
    b2Fixture* groundFixture = groundBody->GetFixtureList();
    groundFixture->SetFriction(0.5f);

    b2Body* leftWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    leftWallBody->SetTransform(b2Vec2(0.0f, 50.0f), 0.0f);
    b2Fixture* leftWallFixture = leftWallBody->GetFixtureList();
    leftWallFixture->SetRestitution(1.0f);

    b2Body* rightWallBody = Box2DHelper::CreateRectangularStaticBody(phyWorld, 10, 100);
    rightWallBody->SetTransform(b2Vec2(100.0f, 50.0f), 0.0f);
    b2Fixture* rightWallFixture = rightWallBody->GetFixtureList();
    rightWallFixture->SetRestitution(1.0f);

    // Crear un c�rculo que se controlar� con el teclado
    controlBody = Box2DHelper::CreateRectangularKinematicBody(phyWorld, 15, 10);
    controlBody->SetTransform(b2Vec2(10.0f, 50.0f), 0.0f);
}

// Destructor de la clase

Game::~Game(void)
{ }