#pragma once
#include <fstream>
#include "SFML\Graphics.hpp"

class LinkedList
{
    private:
        struct Node
        {
            std::string name;
            int score;
            Node* next;

            Node(std::string, int);
        };

        Node* head;
        int size;
        bool sortedByName;
        bool sortedByScore;

        void SaveFile(std::ofstream&);
        void LoadFile(std::ifstream&);
        void DeleteList();
        void SortByName();
        void SearchTheName(sf::RenderWindow&, sf::Font&, std::string);
        void SortByScore();


    public:
        LinkedList() { this->head = NULL; this->size = 0; this->sortedByName = false; this->sortedByScore = true; };
        ~LinkedList();

        int GetSize() { return this->size; };
        void AddRecord(std::string, int);
        void ViewRecords(sf::RenderWindow&, sf::Font&);
        void Save(std::ofstream& oFile) { SaveFile(oFile); }
        void Load(std::ifstream& iFile) { LoadFile(iFile); }
        void SortName() { this->SortByName(); }
        void SortScore() { this->SortByScore(); }
        void SearchName(sf::RenderWindow& window, sf::Font& font, std::string name) { this->SearchTheName(window, font, name); }
};

