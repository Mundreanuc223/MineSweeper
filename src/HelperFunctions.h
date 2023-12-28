#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
# include <string>

using namespace std;

void setText(sf::Text &text, float x, float y);

sf::Sprite imageLoader(string name);

map<string, sf::Sprite> spriteHolder();


