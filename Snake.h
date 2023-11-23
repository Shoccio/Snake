#pragma once
#include "SFML\Graphics.hpp"
#include <utility>
#include <vector>

class Snake
{
	int size;
	int borderSize;
	float perimeter;
	sf::Keyboard::Key lastKeyPressed;
	std::vector <sf::RectangleShape*> body;

	void MoveSnake(sf::Keyboard::Key, int);
	void MoveHead(sf::Keyboard::Key);
	void InitializePosition(int, int);
	void InitializePiece(sf::RectangleShape*&);
	void CorrectPiece(sf::FloatRect&);
	void PrintSnake(sf::RenderWindow&);
	bool CollisionDetection(sf::RectangleShape);
	bool CollisionWallDetection(int, int);
	bool CollisionSnakeDetection();
	sf::Vector2f GetFoodSnake(sf::RectangleShape);
	void GrowSnake();
	void DeleteSnake();

	public:
		Snake();
		~Snake() { this->DeleteSnake(); };
		void Move(sf::Keyboard::Key key) { this->MoveSnake(key, 0); }
		void Initialize(int width, int height) { this->InitializePosition(width, height); }
		void Print(sf::RenderWindow& window) { this->PrintSnake(window); };
		bool Collision(sf::RectangleShape food) { return this->CollisionDetection(food); }
		bool CollisionWall(int width, int height) { return this->CollisionWallDetection(width, height); }
		bool CollisionSnake() { return this->CollisionSnakeDetection(); }
		sf::Vector2f GetFood(sf::RectangleShape food) { return this->GetFoodSnake(food); }
		void Grow() { this->GrowSnake(); }

};

