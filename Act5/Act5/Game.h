#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "SFMLRenderer.h"
#include <list>

using namespace sf;
class Game
{
private:
	// Propiedades de la ventana
	int alto;
	int ancho;
	RenderWindow* wnd;
	Color clearColor;

	// Objetos de box2d
	b2World* phyWorld;
	SFMLRenderer* debugRender;

	//tiempo de frame
	float frameTime;
	int fps;

	// Cuerpo de box2d
	b2Body* controlBody;
	b2Body* groundBody; //lo guardo como atributo para poder usar en el draw()

	float alphaAng = 45.0f * (b2_pi / 180.0f); //angulo de rotación en radianes

public:

	// Constructores, destructores e inicializadores
	Game(int ancho, int alto, std::string titulo);
	void CheckCollitions();
	void CreateEnemy(int x, int y);
	~Game(void);
	void InitPhysics();

	// Main game loop
	void Loop();
	void DrawGame();
	void UpdatePhysics();
	void DoEvents();
	void SetZoom();
};

