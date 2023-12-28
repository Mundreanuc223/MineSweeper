#include <iostream>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "HelperFunctions.h"
#include "TextureManager.h"
#include <string>
using namespace std;


string Window::DisplayWelcome(int width, int height) {

    sf::RenderWindow welcome(sf::VideoMode(width, height), "MineSweeper", sf::Style::Close);

    //Loads font
    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        std::cout << "can't load :(" << std::endl;
        exit(0);
    }

    //Creates Welcome text
    sf::Text welcomeText;
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);
    setText(welcomeText, width/2, (height / 2) - 150 );

    //Creates "Enter name" text
    sf::Text enterName;
    enterName.setString("Enter your name:");
    enterName.setFont(font);
    enterName.setCharacterSize(20);
    enterName.setStyle(sf::Text::Bold);
    enterName.setFillColor(sf::Color::White);
    setText(enterName, width/2, (height / 2) - 75 );

    //User typed name
    sf::Text typedName;
    typedName.setFont(font);
    typedName.setCharacterSize(18);
    typedName.setStyle(sf::Text::Bold);
    typedName.setFillColor(sf::Color::Yellow);
    setText(typedName, width / 2, (height / 2) - 45);

    //Variables for user typed name
    char letter;
    string name = "|";
    char cursor = '|';
    typedName.setString(name);


    while (welcome.isOpen()) { // while the welcome window is active, we do everything below here

        sf::Event event;

        while (welcome.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {// Click X on the window
                welcome.close();
                return "1";
            }

            if (event.type == sf::Event::KeyPressed) { // Getting user input

                if(event.key.code == sf::Keyboard::Enter) {  // event.key.code == ASCII values
                    if(name.length() > 1) {
                        welcome.close();
                        name.pop_back();//removes the cursor from the name
                        return name;
                    }
                }

                if(event.key.code == sf::Keyboard::Delete || event.key.code == sf::Keyboard::BackSpace){ //if user selects delete or backspace
                    if(name.length() == 1){ //if theres no more to delete, do nothing                    //it deletes the last character typed
                        continue;
                    }
                    else if(name.length() >= 2){
                        name.pop_back();
                        name.pop_back();
                        name.push_back(cursor);
                        typedName.setString(name);
                    }

                }
            }


            if(event.type == sf::Event::TextEntered){ //when user selects a key between a-z and A-Z
                if(isalpha(static_cast<char>(event.text.unicode))){
                    if(name.length() >= 11){
                        continue;
                    }
                    else{
                        if(name.length() == 1){ //capitalize first letter
                            letter = toupper(static_cast<char>(event.text.unicode));
                            name.pop_back();
                        }
                        else{ //lowercase every other letter
                            letter = tolower(static_cast<char>(event.text.unicode));
                            name.pop_back();
                        }
                        name.push_back(letter);
                        name.push_back(cursor);
                        typedName.setString(name);
                        setText(typedName, width / 2, (height / 2) - 45);
                    }
                }
            }
        }

        welcome.clear(sf::Color::Blue); // Set background color of the window
        welcome.draw(welcomeText); // have to draw each object for this specific window
        welcome.draw(enterName);
        welcome.draw(typedName);
        welcome.display();
    }
}








