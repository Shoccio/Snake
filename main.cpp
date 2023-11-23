#include "Snake.h"
#include "LinkedList.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <random>
#include <string>

int Menu(sf::RenderWindow&, sf::Font&, int, int);
int Game(sf::RenderWindow&, sf::Font&, std::string&, int, int);
int HighScores(sf::RenderWindow&, sf::Font&, LinkedList&, int, int);
void HighScores_Sort(sf::RenderWindow&, sf::Font&, LinkedList&, int, int);
void HighScores_Search(sf::RenderWindow&, sf::Font&, LinkedList&, int, int);

int main()
{
	const int SCREEN_WIDTH = 500, SCREEN_HEIGHT = 500;
	sf::RenderWindow window(sf::VideoMode(SCREEN_HEIGHT, SCREEN_HEIGHT), "Snake");
	int choice = 0, score = 0;
	std::string name = "";

	std::ifstream iFile;
	std::ofstream oFile;
	LinkedList linkedList;

	sf::Font font;
	if (!font.loadFromFile(_SOLUTIONDIR + (std::string)"Fonts\\PixeloidMono-VGj6x.ttf"))
		return -1;

	linkedList.Load(iFile);

	do
	{
		choice = Menu(window, font, SCREEN_WIDTH, SCREEN_HEIGHT);

		if (choice == 0)
		{
			score = Game(window, font, name, SCREEN_WIDTH, SCREEN_HEIGHT);

			if (score == -1)
				break;

			linkedList.AddRecord(name, score);
			name.clear();
		}
		else if (choice == 1)
		{
			choice = HighScores(window, font, linkedList, SCREEN_WIDTH, SCREEN_HEIGHT);

			if (choice == -1)
				break;
		}

	} while (choice != 2);

	linkedList.Save(oFile);

	return 0;
}

int Menu(sf::RenderWindow& window, sf::Font& font, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	sf::Event event;
	int choice = 0;
	bool flag = false;

	sf::Text title;
	title.setFont(font);
	title.setString("Snake");
	title.setFillColor(sf::Color::White);
	title.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 50, (SCREEN_HEIGHT / 2) - 150));

	sf::Text start;
	start.setFont(font);
	start.setString("Start");
	start.setFillColor(sf::Color::White);
	start.setCharacterSize(20);
	start.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 35, (SCREEN_HEIGHT / 2) - 80));

	sf::Text highScore;
	highScore.setFont(font);
	highScore.setString("Scores");
	highScore.setFillColor(sf::Color::White);
	highScore.setCharacterSize(20);
	highScore.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 35, (SCREEN_HEIGHT / 2) - 40));

	sf::Text exit;
	exit.setFont(font);
	exit.setString("Exit");
	exit.setFillColor(sf::Color::White);
	exit.setCharacterSize(20);
	exit.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 35, (SCREEN_HEIGHT / 2)));

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return 2;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					choice--;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					choice++;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					flag = true;
			}
		}

		if (flag)
			break;

		if (choice > 2)
			choice = 2;
		else if (choice < 0)
			choice = 0;

		if (choice == 0)
		{
			start.setStyle(start.Bold);
			highScore.setStyle(highScore.Regular);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 1)
		{
			start.setStyle(start.Regular);
			highScore.setStyle(highScore.Bold);
			exit.setStyle(exit.Regular);
		}
		else if(choice == 2)
		{
			start.setStyle(start.Regular);
			highScore.setStyle(highScore.Regular);
			exit.setStyle(exit.Bold);
		}

		window.clear();
		window.draw(title);
		window.draw(start);
		window.draw(highScore);
		window.draw(exit);
		window.display();
	}

	return choice;
}

int Game(sf::RenderWindow& window, sf::Font& font, std::string& name, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	sf::Event event;
	sf::Clock clock;
	sf::Keyboard::Key keyPressed = sf::Keyboard::W;
	int choice = 0;

	std::srand(std::time(0));


	sf::Image background;
	background.loadFromFile(_SOLUTIONDIR + (std::string)"Image\\background.jpg");
	
	sf::Texture backgrounds;
	backgrounds.loadFromImage(background);

	sf::Sprite back;
	back.setTexture(backgrounds);

	sf::Text title;
	title.setFont(font);
	title.setString("Snake");
	title.setCharacterSize(12);
	title.setFillColor(sf::Color::White);

	sf::Text score;
	score.setFont(font);
	score.setString("Score: ");
	score.setCharacterSize(12);
	score.setFillColor(sf::Color::White);
	score.move(sf::Vector2f(100.f, 0.f));

	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("Game Over");
	gameOverText.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 90, (SCREEN_HEIGHT / 2) - 120));

	sf::Text retry;
	retry.setFont(font);
	retry.setString("Retry");
	retry.setCharacterSize(20);
	retry.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 30, (SCREEN_HEIGHT / 2) - 50));
	retry.setStyle(retry.Bold);

	sf::Text exit;
	exit.setFont(font);
	exit.setString("Exit");
	exit.setCharacterSize(20);
	exit.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 30, (SCREEN_HEIGHT / 2) - 10));


	sf::Text actualScoreText;
	actualScoreText.setFont(font);
	actualScoreText.setCharacterSize(12);
	actualScoreText.setFillColor(sf::Color::White);
	actualScoreText.setPosition(sf::Vector2f(score.getPosition().x + 50.f, 0.f));
	int actualScore = 0;
	bool flag = false;


	sf::RectangleShape food(sf::Vector2f(8.f, 8.f));
	food.setOrigin(sf::Vector2f(4.f, 4.f));
	food.setFillColor(sf::Color::Red);
	//food.setOutlineColor(sf::Color::White);
	//food.setOutlineThickness(1);
	do
	{
		Snake snake = Snake();
		keyPressed = sf::Keyboard::W;
		snake.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
		actualScore = 0;
		choice = 0;
		flag = false;
		food.setPosition(snake.GetFood(food));
		actualScoreText.setString("0");

		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					return -1;

			}

			/*clock.restart();*/
			sf::Time elapsed1 = clock.getElapsedTime();
			std::cout << elapsed1.asSeconds() << std::endl;

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				keyPressed = sf::Keyboard::W;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				keyPressed = sf::Keyboard::A;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				keyPressed = sf::Keyboard::S;
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				keyPressed = sf::Keyboard::D;

			if (snake.Collision(food))
			{
				food.setPosition(snake.GetFood(food));

				snake.Grow();

				actualScore++;
				actualScoreText.setString(std::to_string(actualScore));
			}

			if (elapsed1.asSeconds() > .05f)
			{
				clock.restart();
				snake.Move(keyPressed);
			}

			if (snake.CollisionWall(SCREEN_WIDTH, SCREEN_HEIGHT) || snake.CollisionSnake())
				break;

			window.clear();

			window.draw(back);
			snake.Print(window);
			window.draw(food);

			window.draw(title);
			window.draw(score);
			window.draw(actualScoreText);

			window.display();
		}

		while (window.isOpen())
		{
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					return -1;

				if (event.type == sf::Event::KeyPressed)
				{

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					{
						retry.setStyle(retry.Bold);
						exit.setStyle(exit.Regular);
						choice = 0;
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					{
						retry.setStyle(retry.Regular);
						exit.setStyle(exit.Bold);
						choice = 1;
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
						flag = true;

				}

			}

			if (flag)
				break;

			window.clear();
			window.draw(back);

			snake.Print(window);
			window.draw(food);

			window.draw(title);
			window.draw(score);
			window.draw(actualScoreText);
			window.draw(gameOverText);
			window.draw(retry);
			window.draw(exit);

			window.display();

		}

	} while (choice != 1);

	gameOverText.setCharacterSize(30);
	gameOverText.setString("Enter Player Name");
	gameOverText.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 170, (SCREEN_HEIGHT / 2) - 120));
	flag = false;

	retry.setString(name);

	while (window.isOpen())
	{
		if (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				return -1;

			if (event.type == sf::Event::TextEntered)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !name.empty())
					name.pop_back();
				else
				{
					char character = event.text.unicode;

					if((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
						name += character;
				}

				retry.setString(name);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !name.empty())
					flag = true;
			}
		}

		if (flag)
			break;

		window.clear();
		window.draw(gameOverText);
		window.draw(retry);
		window.display();
	}

	return actualScore;
}

int HighScores(sf::RenderWindow& window, sf::Font& font, LinkedList& linkedList, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	int choice = 0;
	bool flag = false;
	sf::Event event;

	sf::Text display;
	sf::Text search;
	sf::Text exit;

	display.setFont(font);
	search.setFont(font);
	exit.setFont(font);

	display.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 100));
	search.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2) - 50));
	exit.setPosition(sf::Vector2f((SCREEN_WIDTH / 2) - 150, (SCREEN_HEIGHT / 2)));

	display.setString("Display Top 10");
	search.setString("Search Name");
	exit.setString("Exit");

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return -1;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					choice--;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					choice++;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					flag = true;
			}
		}

		if (flag)
			break;

		if (choice > 2)
			choice = 2;
		else if (choice < 0)
			choice = 0;

		if (choice == 0)
		{
			display.setStyle(display.Bold);
			search.setStyle(search.Regular);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 1)
		{
			display.setStyle(display.Regular);
			search.setStyle(search.Bold);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 2)
		{
			display.setStyle(display.Regular);
			search.setStyle(search.Regular);
			exit.setStyle(exit.Bold);
		}

		window.clear();
		window.draw(display);
		window.draw(search);
		window.draw(exit);
		window.display();
	}

	if (choice == 0)
		HighScores_Sort(window, font, linkedList, SCREEN_WIDTH, SCREEN_HEIGHT);
	else if (choice == 1)
		HighScores_Search(window, font, linkedList, SCREEN_WIDTH, SCREEN_HEIGHT);
	

	return 0;
}

void HighScores_Sort(sf::RenderWindow& window, sf::Font& font, LinkedList& linkedList, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	int choice = 0;
	bool flag = false;

	sf::Event event;

	sf::Text name;
	sf::Text score;
	sf::Text exit;

	name.setFont(font);
	score.setFont(font);
	exit.setFont(font);

	name.setCharacterSize(15);
	score.setCharacterSize(15);
	exit.setCharacterSize(15);

	name.setString("Name");
	score.setString("Score");
	exit.setString("Exit");

	name.setPosition(100, 150);
	score.setPosition(100, 200);
	exit.setPosition(100, 250);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				std::exit(0);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					choice--;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					choice++;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					flag = true;
			}
		}

		if (flag)
			break;

		if (choice < 0)
			choice = 0;
		else if (choice > 2)
			choice = 2;

		if (choice == 0)
		{
			name.setStyle(name.Bold);
			score.setStyle(score.Regular);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 1)
		{
			name.setStyle(name.Regular);
			score.setStyle(score.Bold);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 2)
		{
			name.setStyle(name.Regular);
			score.setStyle(score.Regular);
			exit.setStyle(sf::Text::Bold);
		}

		window.clear();
		window.draw(name);
		window.draw(score);
		window.draw(exit);

		window.display();

	}

	if (choice == 0)
	{
		linkedList.SortName();
	}
	else if (choice == 1)
	{
		linkedList.SortScore();
	}
	else if (choice == 2)
	{
		return;
	}

	linkedList.ViewRecords(window, font);

}

void HighScores_Search(sf::RenderWindow& window, sf::Font& font, LinkedList& linkedList, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	int choice = 0;
	bool flag = false;
	std::string nameToSearch = "";

	sf::Event event;

	sf::Text name;
	sf::Text nameToBeSearch;
	sf::Text score;
	sf::Text exit;

	name.setFont(font);
	nameToBeSearch.setFont(font);
	score.setFont(font);
	exit.setFont(font);

	name.setCharacterSize(15);
	nameToBeSearch.setCharacterSize(15);
	score.setCharacterSize(15);
	exit.setCharacterSize(15);

	name.setString("Enter Name: ");
	score.setString("Search");
	exit.setString("Exit");

	name.setPosition(100, 150);
	nameToBeSearch.setPosition(225, 150);
	score.setPosition(100, 200);
	exit.setPosition(100, 250);

	while (window.isOpen())
	{
		flag = false;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				std::exit(0);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
					choice--;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
					choice++;
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					flag = true;
			}
		}



		if (flag)
		{
			if(choice != 0)
				break;
			else
			{
				flag = false;

				while (window.isOpen())
				{
					if (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							std::exit(0);

						if (event.type == sf::Event::TextEntered)
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !nameToSearch.empty())
								nameToSearch.pop_back();
							else
							{
								char character = event.text.unicode;

								if ((character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z'))
									nameToSearch += character;
							}

							nameToBeSearch.setString(nameToSearch);
						}

						if (event.type == sf::Event::KeyPressed && !nameToSearch.empty())
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
								flag = true;
						}
					}

					if (flag)
						break;

					window.clear();
					window.draw(name);
					window.draw(nameToBeSearch);
					window.draw(score);
					window.draw(exit);

					window.display();
				}
			}
		}

		if (choice < 0)
			choice = 0;
		else if (choice > 2)
			choice = 2;

		if (choice == 0)
		{
			name.setStyle(name.Bold);
			score.setStyle(score.Regular);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 1)
		{
			name.setStyle(name.Regular);
			score.setStyle(score.Bold);
			exit.setStyle(exit.Regular);
		}
		else if (choice == 2)
		{
			name.setStyle(name.Regular);
			score.setStyle(score.Regular);
			exit.setStyle(sf::Text::Bold);
		}

		window.clear();
		window.draw(name);
		window.draw(nameToBeSearch);
		window.draw(score);
		window.draw(exit);

		window.display();

	}

	if (choice == 1)
	{
		linkedList.SearchName(window, font, nameToSearch);
	}
	else if (choice == 2)
	{
		return;
	}

}