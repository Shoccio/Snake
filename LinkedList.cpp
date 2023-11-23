#include "LinkedList.h"
#include <iostream>
#include <string>
#include <stdlib.h>


LinkedList::Node::Node(std::string name, int score)
{
    this->name = name;
    this->score = score;
    this->next = NULL;
}

LinkedList::~LinkedList()
{
    DeleteList();
}

void LinkedList::DeleteList()
{
    if (this->head != NULL)
    {
        Node* temp = this->head;
        Node* temp2 = head->next;

        while (temp2 != NULL)
        {
            delete temp;

            temp = temp2;
            temp2 = temp2->next;
        }

        delete temp;
    }
}

void LinkedList::AddRecord(std::string name, int score)
{

    Node* temp = new Node(name, score);

    if (this->head == NULL)
        this->head = temp;
    else
    {
        Node* temp2 = this->head->next;
        Node* temp3 = this->head;

        while (temp2 != NULL)
        {
            temp2 = temp2->next;
            temp3 = temp3->next;
        }

        temp2 = temp;
        temp3->next = temp2;
    }

    this->size++;

}

void LinkedList::ViewRecords(sf::RenderWindow& window, sf::Font& font)
{
    if (this->head == NULL)
        std::cout << "Error! No record available!";
    else
    {
        Node* temp = this->head;
        std::vector<sf::Text> scores;
        std::vector<sf::Text> names;
        std::vector<sf::Text> rank;

        sf::Text scoreHeader;
        sf::Text nameHeader;
        sf::Text rankHeader;
        sf::Text enter;

        int rankCount = 1;

        rankHeader.setFont(font);
        scoreHeader.setFont(font);
        nameHeader.setFont(font);
        enter.setFont(font);

        rankHeader.setCharacterSize(15);
        scoreHeader.setCharacterSize(15);
        nameHeader.setCharacterSize(15);
        enter.setCharacterSize(15);

        rankHeader.setString("Rank");
        scoreHeader.setString("Score");
        nameHeader.setString("Name");
        enter.setString("Press Enter to Exit");

        rankHeader.setPosition(50, 20);
        scoreHeader.setPosition(400, 20);
        nameHeader.setPosition(200, 20);
        enter.setPosition(50, 470);

        sf::Event event;

        for (int i = 0; i < 10 && temp != NULL; i++)
        {
            scores.push_back(sf::Text());
            names.push_back(sf::Text());
            rank.push_back(sf::Text());

            scores[i].setFont(font);
            names[i].setFont(font);
            rank[i].setFont(font);

            scores[i].setCharacterSize(15);
            names[i].setCharacterSize(15);
            rank[i].setCharacterSize(15);

            scores[i].setString(std::to_string(temp->score));
            names[i].setString(temp->name);
            rank[i].setString(std::to_string(rankCount++));

            scores[i].setPosition(sf::Vector2f(400, 20 + 20 * (i + 1)));
            names[i].setPosition(sf::Vector2f(200, 20 + 20 * (i + 1)));
            rank[i].setPosition(sf::Vector2f(50, 20 + 20 * (i + 1)));

            scores[i].setFillColor(sf::Color::White);
            names[i].setFillColor(sf::Color::White);
            rank[i].setFillColor(sf::Color::White);

            temp = temp->next;
        }

        while (window.isOpen())
        {

            while (window.pollEvent(event))
            {
                if (event.type == event.Closed)
                    std::exit(0);

                if (event.type == event.KeyPressed)
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                        return;
                    
                }
            }

            window.clear();

            window.draw(rankHeader);
            window.draw(scoreHeader);
            window.draw(nameHeader);
            window.draw(enter);

            for (int i = 0; i < this->size; i++)
            {
                
                window.draw(names[i]);
                window.draw(scores[i]);
                window.draw(rank[i]);
            }

            window.display();
        }
    }
}

void LinkedList::SaveFile(std::ofstream& oFile)
{
    oFile.open(_SOLUTIONDIR + (std::string)"SaveFile\\saveFile.txt");

    if (!oFile)
        std::exit(0);
    Node* temp = this->head;

    while (temp != NULL)
    {
        oFile << temp->name << ' ' << temp->score << std::endl;

        temp = temp->next;
    }

    oFile.close();
}

void LinkedList::LoadFile(std::ifstream& iFile)
{
    std::string name = "";
    int score = 0;

    iFile.open(_SOLUTIONDIR + (std::string)"SaveFile\\saveFile.txt");

    if (!iFile)
        return;

    iFile >> name >> score;

    if (this->head == NULL)
    {
        Node* temp3 = new Node(name, score);

        iFile >> name >> score;

        this->size++;
        this->head = temp3;
    }

    Node* temp = this->head->next;
    Node* temp2 = this->head;


    while (!iFile.eof())
    {
        if (temp == NULL)
        {
            Node* temp3 = new Node(name, score);

            this->size++;

            temp = temp3;
            temp2->next = temp;
        }
        else
        {
            temp->name = name;
            temp->score = score;

            iFile >> name >> score;

            temp = temp->next;
            temp2 = temp2->next;
        }
    }

    iFile.close();
}

void LinkedList::SortByName()
{
    bool swap = false;
    
    do
    {
        Node* temp = this->head;
        Node* temp2 = this->head->next;
        swap = false;

        while (temp2 != NULL)
        {
            if (std::strcmp(temp->name.c_str(), temp2->name.c_str()) > 0)
            {
                std::string tempName = temp->name;
                int tempScore = temp->score;

                temp->name = temp2->name;
                temp->score = temp2->score;

                temp2->name = tempName;
                temp2->score = tempScore;
                swap = true;
            }

            temp = temp->next;
            temp2 = temp2->next;

        }


    } while (swap);

}

void LinkedList::SortByScore()
{
    bool swap = false;

    do
    {
        Node* temp = this->head;
        Node* temp2 = this->head->next;
        swap = false;

        while (temp2 != NULL)
        {
            if (temp->score < temp2->score)
            {
                std::string tempName = temp->name;
                int tempScore = temp->score;

                temp->name = temp2->name;
                temp->score = temp2->score;

                temp2->name = tempName;
                temp2->score = tempScore;
                swap = true;
            }

            temp = temp->next;
            temp2 = temp2->next;

        }


    } while (swap);

}

void LinkedList::SearchTheName(sf::RenderWindow& window, sf::Font& font, std::string name)
{
    LinkedList tempList;
    
    Node* temp = this->head;
    Node* temp2 = this->head->next;

    if (temp != NULL)
    {
        if (temp->name == name)
            tempList.AddRecord(temp->name, temp->score);

        temp = temp->next;
        temp2 = temp2->next;

        while (temp2 != NULL)
        {
            if (temp->name == name)
                tempList.AddRecord(temp->name, temp->score);

            temp = temp->next;
            temp2 = temp2->next;
        }
    }

    tempList.ViewRecords(window, font);
}