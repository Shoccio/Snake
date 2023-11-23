#include "Snake.h"

Snake::Snake()
{
	this->size = 3;
	this->borderSize = 1;
	this->perimeter = 10.f;
	this->lastKeyPressed = sf::Keyboard::W;

	sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(this->perimeter, this->perimeter));
	sf::RectangleShape* shape2 = new sf::RectangleShape(sf::Vector2f(this->perimeter, this->perimeter));
	sf::RectangleShape* shape3 = new sf::RectangleShape(sf::Vector2f(this->perimeter, this->perimeter));

	this->InitializePiece(shape);
	this->InitializePiece(shape2);
	this->InitializePiece(shape3);

	this->body.push_back(shape);
	this->body.push_back(shape2);
	this->body.push_back(shape3);

	this->MoveSnake(this->lastKeyPressed, 0);

}

void Snake::InitializePiece(sf::RectangleShape*& shape)
{
	shape->setOrigin(sf::Vector2f(this->perimeter / 2.f, this->perimeter / 2.f));
	shape->setFillColor(sf::Color::Green);
	shape->setOutlineThickness(this->borderSize);
	shape->setOutlineColor(sf::Color::White);
}

void Snake::InitializePosition(int width, int height)
{
	this->body[0]->setPosition(sf::Vector2f(width / 2, height / 2 - 10));
	this->body[1]->setPosition(sf::Vector2f(width / 2, (height / 2)));
	this->body[2]->setPosition(sf::Vector2f(width / 2, (height / 2) + 10));
}

void Snake::PrintSnake(sf::RenderWindow& window)
{
	for (auto& body : this->body)
	{
		window.draw(*body);
	}
}

void Snake::MoveSnake(sf::Keyboard::Key key, int index)
{
	if (index >= this->size)
		return;

	MoveSnake(key, index + 1);

	//if (key == sf::Keyboard::W && sf::Keyboard::D == this->lastKeyPressed )
	//{
	//	if (index > 0)
	//	{
	//		if(this->body[0]->getPosition().x != this->body[this->size - 1]->getPosition().x)
	//	
	//		else
	//			this->body[index]->move(sf::Vector2f(0.f, -0.1f));
	//	}
	//	else
	//		this->body[index]->move(sf::Vector2f(0.f, -0.1f));
	//}
	//else if (key == sf::Keyboard::D)
	//{
		if (index > 0)
			this->body[index]->setPosition(this->body[index - 1]->getPosition());
		else
		{
			this->MoveHead(key);
		}
	//}
}

void Snake::MoveHead(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::W && this->lastKeyPressed != sf::Keyboard::S)
	{
		this->body[0]->move(sf::Vector2f(0.f, -this->perimeter));
		this->lastKeyPressed = key;
	}
	else if (key == sf::Keyboard::A && this->lastKeyPressed != sf::Keyboard::D)
	{
		this->body[0]->move(sf::Vector2f(-this->perimeter, 0.f));
		this->lastKeyPressed = key;
	}
	else if (key == sf::Keyboard::S && this->lastKeyPressed != sf::Keyboard::W)
	{
		this->body[0]->move(sf::Vector2f(0.f, this->perimeter));
		this->lastKeyPressed = key;
	}
	else if (key == sf::Keyboard::D && this->lastKeyPressed != sf::Keyboard::A)
	{
		this->body[0]->move(sf::Vector2f(this->perimeter, 0.f));
		this->lastKeyPressed = key;
	}
	else
		MoveHead(this->lastKeyPressed);
}

bool Snake::CollisionDetection(sf::RectangleShape food)
{
	sf::FloatRect headDim = this->body[0]->getGlobalBounds();
	sf::FloatRect foodDim = food.getGlobalBounds();

	return headDim.intersects(foodDim);
}

bool Snake::CollisionWallDetection(int width, int height)
{
	sf::FloatRect headDim = this->body[0]->getGlobalBounds();
	this->CorrectPiece(headDim);

	if (headDim.left < 0 || headDim.top < 0 || headDim.left + headDim.width > width || headDim.top + headDim.height > height)
		return true;

	return false;
}

bool Snake::CollisionSnakeDetection()
{
	sf::FloatRect headDim = this->body[0]->getGlobalBounds();
	this->CorrectPiece(headDim);
	bool flag = false;

	for (auto& piece : this->body)
	{
		if (!flag)
		{
			flag = true;
			continue;
		}

		sf::FloatRect pieceDim = piece->getGlobalBounds();
		this->CorrectPiece(pieceDim);

		if (headDim.intersects(pieceDim))
			return true;
	}

	return false;
}

sf::Vector2f Snake::GetFoodSnake(sf::RectangleShape food)
{
	sf::Vector2f pos;
	do
	{
		int x = 10 + (10 * (int)(rand() % 45));
		int y = 10 + (10 * (int)(rand() % 45));

		pos = sf::Vector2f((float)x, (float)y);

		food.setPosition(pos);
	}
	while (this->CollisionDetection(food));

	return pos;
}

void Snake::GrowSnake()
{
	sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(this->perimeter, this->perimeter));
	this->InitializePiece(shape);

	float x = this->body[size - 1]->getPosition().x - this->body[size - 2]->getPosition().x;
	float y = this->body[size - 1]->getPosition().y - this->body[size - 2]->getPosition().y;

	float xShape = this->body[size - 1]->getPosition().x - x;
	float yShape = this->body[size - 1]->getPosition().y - y;

	shape->setPosition(sf::Vector2f(xShape, yShape));
	this->size++;

	this->body.push_back(shape);
}

void Snake::CorrectPiece(sf::FloatRect& Dim)
{
	Dim.height -=  (1 + this->borderSize * 2);
	Dim.width -= (1 + this->borderSize * 2);
	Dim.left += this->borderSize;
	Dim.top += this->borderSize;
}

void Snake::DeleteSnake()
{
	for (auto& piece : this->body)
		delete piece;
}