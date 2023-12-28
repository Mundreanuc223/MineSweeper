#include <SFML/Graphics.hpp>
#include "HelperFunctions.h"
#include "TextureManager.h"
using namespace std;


void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

sf::Sprite imageLoader(string name){ //function that creates a sprite from an image file


    sf::Texture& nameTexture = TextureManager::getTexture(name);
    sf::Sprite nameSprite;
    nameSprite.setTexture(nameTexture);

    return nameSprite;

}


map<string, sf::Sprite> spriteHolder(){ //Function that creates a map of every sprite

    map<string,sf::Sprite> images;

    images.emplace("Pause",imageLoader("pause"));
    images.emplace("Play",imageLoader("play"));
    images.emplace("Debug",imageLoader("debug"));
    images.emplace("Digits",imageLoader("digits"));
    images.emplace("HappyFace",imageLoader("face_happy"));
    images.emplace("LoseFace",imageLoader("face_lose"));
    images.emplace("WinFace",imageLoader("face_win"));
    images.emplace("Flag",imageLoader("flag"));
    images.emplace("Leaderboard",imageLoader("leaderboard"));
    images.emplace("Mine",imageLoader("mine"));
    images.emplace("1",imageLoader("number_1"));
    images.emplace("2",imageLoader("number_2"));
    images.emplace("3",imageLoader("number_3"));
    images.emplace("4",imageLoader("number_4"));
    images.emplace("5",imageLoader("number_5"));
    images.emplace("6",imageLoader("number_6"));
    images.emplace("7",imageLoader("number_7"));
    images.emplace("8",imageLoader("number_8"));
    images.emplace("HiddenTile",imageLoader("tile_hidden"));
    images.emplace("RevealedTile",imageLoader("tile_revealed"));

    return images;
}