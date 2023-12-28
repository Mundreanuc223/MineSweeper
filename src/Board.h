#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "HelperFunctions.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

class Board{

public:

    class Tile{

    public:

        bool mine = false;
        bool flag = false;
        bool hidden = true;
        bool revealed = false;
        int nearMines;
        sf::Vector2i position; //x y position
        sf::Sprite sprite;
        sf::Sprite mineSprite;
        sf::Sprite flagSprite;
        sf::Sprite intSprite;
        sf::Sprite debugMinesSprite;
        sf::Sprite pausedSprite;
        vector<Tile*> adjacentTiles;


    };

    int numCol;
    int numRow;
    int numMines;
    int flagCount = 0;
    int mineCounter;
    bool debugMode = false;
    bool paused = false;
    bool leaderboardOn = false;
    bool gameLost = false;
    bool gameWon = false;
    int minutes;
    int seconds;
    int total_time;
    int numLChecks;
    string winTime = "";
    string userName;
    vector<vector<Tile*>> tiles;
    int intWinTotal;
    vector<int> getBoardData(string filename);
    void DisplayGame(int width, int height, string playerName,int rows, int cols, int mines);
    void createGrid(int row, int col, int mines);
    void setTiles();
    void setBombs();
    void setGridImage();
    void setTilesPosition();
    void drawBoard(sf::RenderWindow& window);
    Tile* findTile(sf::Vector2i pos);
    Tile* getTileClicked(sf::RenderWindow& window);
    void setMines();
    void setMinesImage();
    void setDebugMinesImage();
    void debugMinesOff();
    void resetMines();
    void setNeighbors();
    int getNeighborCount(Tile* tile);
    void setIntImage();
    Board();
    ~Board();
    Board(const Board& board);
    Board &operator=(const Board& board);
    void resetBoard();
    void resetTiles();
    void revealEmpty(Tile* tile, vector<sf::Sprite>& images);
    void drawFlags(sf::RenderWindow& window);
    void drawMines(sf::RenderWindow& window);
    void drawDebugMines(sf::RenderWindow& window);
    void setPausedImage();
    void drawPaused(sf::RenderWindow& window);
    void drawInts(sf::RenderWindow& window);
    void revealMines();
    void checkWin();
    void winFlags();
    map<int, sf::Sprite> parseDigits(sf::Sprite digits);
    void displayLeaderBoard(sf::RenderWindow& window, string name, string winTime, vector<string>& stringTimes,
                            multimap<int,string>& intRanks, vector<int>& timeTotals);
    string getLeaderBoardData(string filename, string userName, string wintime,vector<string>& stringTimes,
                              multimap<int,string>& intRanks, vector<int>& timeTotals);





};




