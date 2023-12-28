#include <iostream>
#include "Board.h"
#include <fstream>
#include <SFML/Graphics.hpp>
#include "HelperFunctions.h"
#include "TextureManager.h"
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

Board::Board(){ //default constructor

    this->numCol = 25;
    this->numRow = 16;
    this->numMines = 50;
    this->flagCount = 0;
    this->debugMode = false;
    this->mineCounter = 50;
    this->debugMode = false;
    this->paused = false;
    this->leaderboardOn = false;
    this->gameLost = false;
    this->gameWon = false;
    this->minutes = 0;
    this->seconds = 0;
    this->winTime = "";
}


Board::Board(const Board& board){ //copy assignment

    this->numCol = board.numCol;
    this->numRow = board.numRow;
    this->numMines = board.numMines;
    this->flagCount = board.flagCount;
    this->debugMode = board.debugMode;
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            this->tiles = board.tiles;
        }
    }
}

Board & Board::operator=(const Board& board){ //copy constructor

    this->numCol = board.numCol;
    this->numRow = board.numRow;
    this->numMines = board.numMines;
    this->flagCount = board.flagCount;
    this->debugMode = board.debugMode;
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            this->tiles = board.tiles;
        }
    }
    return *this;
}

Board::~Board(){ //destructor
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            delete tiles[i][j];
        }
    }
}

vector<int> Board::getBoardData(string filename){ //gets board data from config txt file

    vector<int> data;

    ifstream file(filename);

    if(!file.is_open()){
        cout << "Error";
        exit(0);
    }

    string token;
    for(int i = 0; i < 3; i++){
        file >> token;
        data.push_back(stoi(token));
    }

    return data;
}

//gets leaderboard data and updates it with winners time
string Board::getLeaderBoardData(string filename, string userName, string wintime, vector<string>& stringTimes,
multimap<int,string>& intRanks, vector<int>& timeTotals){

    string data;
    string rank;
    string time;
    string name;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error";
        exit(0);
    }
    string token;
    string token1;

    for(int i = 1; i < 6; i++) { //get defaults leaderboard.txt data
        file >> token;
        file >> token1;
        data += token;
        data += '.';
        data += '\t';
        for (int j = 0; j < token1.size(); j++) {
            if (token1[j] == ',') {
                time = token1.substr(0, j);
                name = token1.substr(j + 1, token1.size());
                stringTimes.push_back(time);
                data += time;
                data += '\t';
                data += name;
                data += '\n';
                data += '\n';
                int min0 = time[0] - '0';
                int min1 = time[1] - '0';
                int sec0 = time[2] - '0';
                int sec1 = time[3] - '0';
                int total = (((600*min0) + (60 * min1))) + ((sec0 * 10) + sec1);
                intRanks.insert({total,name});
            }
        }
    }

    auto iter = intRanks.begin();

    for (; iter != intRanks.end(); iter++) {
        timeTotals.push_back(iter->first);
    }

    string updatedData;
    if(gameWon && numLChecks == 1) { //if the user won, add their time, ensures it isn't added multiple times
        for (int i = 0; i < timeTotals.size(); i++) { // possible
            if (intWinTotal < timeTotals[i]) {
                timeTotals.insert(timeTotals.begin() + i, intWinTotal);
                stringTimes.insert(stringTimes.begin() + i, wintime);
                i = timeTotals.size() + 1;
            }
        }
        userName += '*';
        intRanks.insert({intWinTotal, userName});
    }

        for(int i = 0; i < 5; i++){
            updatedData += (i + 1) + '0';
            updatedData += '.';
            updatedData += '\t';
            updatedData += stringTimes[i];
            updatedData += '\t';
            int currentTotal = timeTotals[i];
            string currentName;
            auto iter = intRanks.begin();
            for(; iter != intRanks.end(); iter++){
                if(iter->first == currentTotal){
                    currentName = iter->second;
                }
            }
            updatedData += currentName;
            updatedData += '\n';
            updatedData += '\n';
        }

        data = updatedData;
        return data;
}

void Board::createGrid(int row, int col, int mines){ //creates each tile, and each row of the board

    vector<Tile*> tiles1;
    for(int r = 0; r < row; r++) {
        tiles1.clear();
        for(int c = 0; c < col; c++) {
            Tile* newTile = new Tile;
            tiles1.push_back(newTile);
        }
        this->tiles.push_back(tiles1);
    }
}


void Board::setGridImage(){ //sets each tiles to hidden

    for(int i = 0; i < this->tiles.size(); i++){
        for(int j = 0; j < this->tiles[i].size(); j++){
            this->tiles[i][j]->sprite = imageLoader("tile_hidden");
        }
    }
}

void Board::setTilesPosition(){ //sets the position of each tile on the board

    int y = 0;
    for(int i = 0; i < this->tiles.size(); i++){
        int x = 0;
        for(int j = 0; j < this->tiles[i].size(); j++){
            this->tiles[i][j]->sprite.setPosition(x,y);
            this->tiles[i][j]->position.x = x;
            this->tiles[i][j]->position.y = y;
            x += 32;
        }
        y += 32;
    }
}

Board::Tile* Board::getTileClicked(sf::RenderWindow& window){ //clicking on a tile returns the tile
    sf::Vector2i mouse;
    mouse = sf::Mouse::getPosition(window);
    for(int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->sprite.getGlobalBounds().contains(window.mapPixelToCoords(mouse))){
                return this->tiles[i][j];
            }
        }
    }
}

void Board::drawBoard(sf::RenderWindow& window) { //draws every tile (the board) to the screen

    for (int i = 0; i < this->tiles.size(); i++) {
        for (int j = 0; j < this->tiles[i].size(); j++) {
            window.draw(this->tiles[i][j]->sprite);
        }
    }
}

void Board::setMines(){ //randomly sets the mines

    int i = 0;
    int seed = time(0);
    srand(seed);
    while(i < numMines){

        int x = rand() % numCol ;
        int y = rand() % numRow ;

        if(tiles[y][x]->mine == true){
            continue;
        }
        else{
            tiles[y][x]->mine = true;
            i++;
        }
    }
}

void Board::resetMines(){ //resets every mine to safe (used to reset mines)

    for(int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            tiles[i][j]->mine = false;
        }
    }
}

void Board::setMinesImage(){ //sets the image for evey mine tile

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->mine == true){
                tiles[i][j]->mineSprite = imageLoader("mine");
            }
        }
    }
}

void Board::setDebugMinesImage(){ //set the mine image used to display debug mode

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->mine == true){
                tiles[i][j]->debugMinesSprite = imageLoader("mine");
                tiles[i][j]->debugMinesSprite.setPosition(tiles[i][j]->position.x,tiles[i][j]->position.y);
            }
        }
    }
}


void Board::drawDebugMines(sf::RenderWindow& window){ //draws the debug mines to the screen

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->mine == true && debugMode) {
                window.draw(tiles[i][j]->debugMinesSprite);
            }
        }
    }
}

void Board::setPausedImage(){ //when paused, sets the tiles to revealed

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            tiles[i][j]->pausedSprite = imageLoader("tile_revealed");
            tiles[i][j]->pausedSprite.setPosition(tiles[i][j]->position.x,tiles[i][j]->position.y);
            }
        }
}

void Board::drawPaused(sf::RenderWindow& window){ //draws the paused tiles to the screen

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if((paused || leaderboardOn) && !gameWon && !gameLost){
                window.draw(tiles[i][j]->pausedSprite);
            }
        }
    }
}



void Board::setNeighbors() { //establishes the neighboring tiles of each tile

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {

            if (i == 0) { //top row
                if (j == 0) {
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[1][1]);
                }
                if (j == numCol - 1) {
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][numCol - 2]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[1][numCol - 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[1][numCol - 2]);
                }
                if(j != 0 && j != numCol -1) {
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j - 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j + 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i + 1][j - 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i + 1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i + 1][j + 1]);
                }
            }


            if (i == numRow - 1) { //bottom row
                if (j == 0) {
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j + 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i - 1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i - 1][j + 1]);
                }

                if (j == numCol - 1) {
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j-1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j-1]);
                }
                if(j != 0 && j!= numCol - 1){
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j-1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j+1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j-1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j+1]);
                }
            }


            if (j == 0 && i != 0 && i != numRow - 1) { //left column
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i+1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j+1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j + 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i+1][j+1]);
            }

            if (j == numCol - 1 && i != 0 && i != numRow - 1) { //right column
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i+1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i-1][j-1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j-1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i+1][j-1]);
            }

            if(i != 0 && i != numRow-1 && j != 0 && j != numCol-1){
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i - 1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i - 1][j - 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i - 1][j + 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j - 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i][j + 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i + 1][j]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i + 1][j - 1]);
                    this->tiles[i][j]->adjacentTiles.push_back(this->tiles[i + 1][j + 1]);
                }
            }
    }
}

void Board::setIntImage(){ //sets the neighboring mines integer image

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {

            vector<Tile*> neighbors = tiles[i][j]->adjacentTiles;
            int mines = 0;
            for(int n = 0; n < neighbors.size(); n++){
                if(neighbors[n]->mine == true){
                    mines++;
                }
            }
            tiles[i][j]->nearMines = mines;
            string Smines = to_string(mines);
            if(mines != 0) {
                tiles[i][j]->intSprite = imageLoader("number_" + Smines);
            }
        }
    }
}


void Board::revealEmpty(Tile* tile, vector<sf::Sprite>& images) { //uses recursion to reveal neighbors, repeats on adjacent tiles with no neighboring mines

    if (!tile->mine && !tile->flag && !tile->revealed){
        tile->revealed = true;
        tile->sprite.setTexture(TextureManager::getTexture("tile_revealed"));
        tile->intSprite.setPosition(tile->position.x,tile->position.y);
        images.push_back(tile->intSprite);
        vector<Tile *> neighbors = tile->adjacentTiles;
        for(int n = 0; n < neighbors.size(); n++) {
            if (!neighbors[n]->mine && !neighbors[n]->flag && !neighbors[n]->revealed && tile->nearMines == 0) {
                revealEmpty(neighbors[n],images);
            }
        }
    }
}

void Board::resetTiles(){ //resets the attrivutes and sprites of each tile
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            tiles[i][j]->adjacentTiles.clear();
            tiles[i][j]->nearMines = 0;
            tiles[i][j]->intSprite = imageLoader("tile_revealed");
            tiles[i][j]->revealed = false;
            tiles[i][j]->flag = false;
            tiles[i][j]->flagSprite = imageLoader("tile_hidden");
            tiles[i][j]->mine = false;
            tiles[i][j]->debugMinesSprite = imageLoader("tile_hidden");
            flagCount = 0;
        }
    }
}

void Board::drawFlags(sf::RenderWindow& window){ //draws the flags to the window

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->flag) {
                window.draw(tiles[i][j]->flagSprite);
            }
        }
    }
}

void Board::drawMines(sf::RenderWindow& window){ //draws the mines to the window

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->mine && tiles[i][j]->revealed) {
                window.draw(tiles[i][j]->mineSprite);
            }
        }
    }
}

void Board::drawInts(sf::RenderWindow& window){ //draws the int image to the window

    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(!tiles[i][j]->mine && tiles[i][j]->revealed) {
                window.draw(tiles[i][j]->intSprite);
            }
        }
    }
}

void Board::revealMines(){ //reveals every mine if the game is lost

    if(gameLost){
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles[i].size(); j++){
                if(tiles[i][j]->mine) {
                    tiles[i][j]->sprite.setTexture(TextureManager::getTexture("tile_revealed"));
                    tiles[i][j]->revealed = true;
                    tiles[i][j]->mineSprite.setPosition(tiles[i][j]->position.x,tiles[i][j]->position.y);
                }
            }
        }
    }
}

void Board::checkWin(){ //checks if the user won the game

    int count = 0;
    int safeTiles = (numCol * numRow) - numMines;
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = 0; j < tiles[i].size(); j++) {
            if(tiles[i][j]->revealed && !tiles[i][j]->mine){
            count++;
            }
        }
    }
    if(count == safeTiles){
        gameWon = true;
        debugMode = false;
        leaderboardOn = true;
        numLChecks = 1;
    }
}

void Board::winFlags(){ //places flags on every mine if the user wins

    if(gameWon){
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles[i].size(); j++) {
                if (tiles[i][j]->mine && !tiles[i][j]->revealed) {
                    tiles[i][j]->flag = true;
                    tiles[i][j]->flagSprite.setTexture(TextureManager::getTexture("flag"));
                    tiles[i][j]->flagSprite.setPosition(tiles[i][j]->position.x, tiles[i][j]->position.y);
                }
            }
        }
    }
}

map<int, sf::Sprite> Board::parseDigits(sf::Sprite digits){ //sets the digit rects - given in demo
    map<int, sf::Sprite> digitsMap;

    for(int i = 0; i < 10; i++){
        sf::IntRect rect(i*21,0,21,32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }
    sf::IntRect rect(10 * 21, 0, 21, 32);
    digits.setTextureRect(rect);
    sf::Sprite sprite = digits;
    digitsMap.emplace(-9, sprite);
    return digitsMap;
}


void Board::resetBoard(){ //resets the board for a new game
    resetMines();
    resetTiles();
    setGridImage();
    setTilesPosition();
    setMines();
    setMinesImage();
    setNeighbors();
    setIntImage();
    setDebugMinesImage();
    setPausedImage();
    gameLost = false;
    gameWon = false;
    mineCounter = numMines;
    seconds = 0;
    minutes = 0;
    numLChecks = 0;
}

//displays the leaderboard window
void Board::displayLeaderBoard(sf::RenderWindow& window ,string name, string winTime,vector<string>& stringTimes,
                               multimap<int,string>& intRanks, vector<int>& timeTotals){

    int width = numCol * 16;
    int height = (numRow * 16) + 50;

    sf::RenderWindow leaderboard(sf::VideoMode(width, height), "MineSweeper", sf::Style::Close);
    leaderboard.requestFocus();
    leaderboard.setPosition(sf::Vector2i (535 , 50));

    string data; //gets the leaderboard data
    data = getLeaderBoardData("files/leaderboard.txt", name, winTime, stringTimes, intRanks, timeTotals);

    sf::Font font;
    if (!font.loadFromFile("files/font.ttf")) {
        std::cout << "can't load :(" << std::endl;
        exit(0);
    }

    //Creates title text
    sf::Text titleText;
    titleText.setString("LEADERBOARD");
    titleText.setFont(font);
    titleText.setCharacterSize(20);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setFillColor(sf::Color::White);
    setText(titleText, width/2, (height / 2) - 120 );

    sf::Text rankingsText;
    rankingsText.setString(data);
    rankingsText.setFont(font);
    rankingsText.setCharacterSize(18);
    rankingsText.setStyle(sf::Text::Bold);
    rankingsText.setFillColor(sf::Color::White);
    setText(rankingsText, width/2, (height / 2) + 20 );


    while (leaderboard.isOpen()) { // while the welcome window is active, we do everything below here

        sf::Event event;

        while (leaderboard.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {// Click X on the window
                leaderboard.close();
                leaderboardOn = false;
            }

        }

        leaderboard.clear(sf::Color::Blue);
        leaderboard.draw(titleText);
        leaderboard.draw(rankingsText);
        leaderboard.display();
    }
}

//displays the actual game window itself
void Board::DisplayGame (int width, int height, string playerName, int cols, int rows, int mines) {

    sf::RenderWindow game(sf::VideoMode(width, height), "MineSweeper", sf::Style::Close);

    userName = playerName;
    numMines = mines;
    numRow = rows;
    numCol = cols;
    string name = playerName;
    map<string, sf::Sprite> images;
    vector<string> stringTimes;
    multimap<int,string> intRanks;
    vector<int> timeTotals;
    images = spriteHolder();

    auto start_time = chrono::high_resolution_clock::now(); //sets time variables
    auto pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

    sf::Sprite pause = images["Pause"]; //sets image sprites
    sf::Sprite play = images["Play"];
    sf::Sprite happyFace = images["HappyFace"];
    sf::Sprite debugButton = images["Debug"];
    sf::Sprite leaderboardButton = images["Leaderboard"];
    sf::Sprite leaderBoardPause = images["Play"];

    sf::Texture& digitsText = TextureManager::getTexture("digits");
    sf::Sprite digits;
    digits.setTexture(digitsText);
    map<int, sf::Sprite> digitsMap = parseDigits(digits);

    pause.setPosition( (cols * 32) - 240, 32 * (rows + 0.5)); //sets positions of buttons
    play.setPosition((cols * 32) - 240, 32 * (rows + 0.5));
    happyFace.setPosition( ((cols / 2.0) * 32) -32, 32 * (rows + 0.5));
    debugButton.setPosition((cols * 32)-304,32*(rows+0.5));
    leaderboardButton.setPosition((cols * 32) - 176, 32 * (rows + 0.5));
    leaderBoardPause.setPosition((cols * 32) - 240, 32 * (rows + 0.5));

    createGrid(rows,cols,mines); //calls functions to set up the board and window
    setGridImage();
    setTilesPosition();
    setMines();
    setMinesImage();
    setNeighbors();
    setIntImage();
    setDebugMinesImage();
    setPausedImage();


    vector<sf::Sprite> updateImages;

    while (game.isOpen()) { // while the welcome window is active, we do everything below here

        sf::Event event;

        while (game.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {// Click X on the window
                game.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePosition;
                mousePosition = sf::Mouse::getPosition(game);
                if (event.mouseButton.button == sf::Mouse::Left) {

                    //ensure click is on a tile
                    if(mousePosition.y <= rows * 32 - 0.5 && mousePosition.y > 0 && !paused && !gameLost && !gameLost) {
                        Tile* currentTile = getTileClicked(game);
                        if(getTileClicked(game)->flag){
                            continue;
                        }

                        //if tile clicked is a mine
                        if(getTileClicked(game)->mine){
                            getTileClicked(game)->sprite.setTexture(TextureManager::getTexture("tile_revealed"));
                            currentTile->mineSprite.setPosition(currentTile->position.x,currentTile->position.y);
                            currentTile->revealed = true;
                            happyFace.setTexture(TextureManager::getTexture("face_lose"));
                            gameLost = true;
                            revealMines();
                        }

                        //if tile clicked is not a mine
                        if(!getTileClicked(game)->mine) {
                            getTileClicked(game)->sprite.setTexture(TextureManager::getTexture("tile_revealed"));
                            revealEmpty(getTileClicked(game),updateImages);
                            getTileClicked(game)->revealed = true;
                            getTileClicked(game)->intSprite.setPosition(currentTile->position.x,currentTile->position.y);
                            updateImages.push_back(getTileClicked(game)->intSprite);
                            checkWin();
                            winFlags();
                            if(gameWon){
                                happyFace.setTexture(TextureManager::getTexture("face_win"));
                            }
                        }
                    }

                    //if debug button is clicked
                    if(debugButton.getGlobalBounds().contains(game.mapPixelToCoords(mousePosition)) && !paused && !gameLost && !gameWon){
                        debugMode = !debugMode;
                    }

                    //if leaderboard button is clicked
                    if(leaderboardButton.getGlobalBounds().contains(game.mapPixelToCoords(mousePosition))){
                        leaderboardOn = !leaderboardOn;
                        if(!paused) {
                            pauseTime = chrono::high_resolution_clock::now();
                        }
                        if(gameWon){
                            numLChecks++;
                        }
                    }

                    //if pause button is clicked
                    if(pause.getGlobalBounds().contains(game.mapPixelToCoords(mousePosition)) && !gameLost && !gameWon){ //pause and play button
                        paused = !paused;
                        if(paused){
                            pause.setTexture(TextureManager::getTexture("play"));
                            pauseTime = chrono::high_resolution_clock::now();
                        }
                        if(!paused){
                            pause.setTexture(TextureManager::getTexture("pause"));
                            auto unPausedTime = chrono::steady_clock::now();
                            elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
                        }
                    }

                    //if reset button is clicked
                    if(happyFace.getGlobalBounds().contains(game.mapPixelToCoords(mousePosition))){
                        resetBoard();
                        updateImages.clear();
                        happyFace.setTexture(TextureManager::getTexture("face_happy"));
                        start_time = chrono::high_resolution_clock::now();
                        pauseTime= chrono::high_resolution_clock ::now();
                        elapsed_paused_time = 0;
                        continue;
                    }
                }

                if(event.mouseButton.button == sf::Mouse::Right){

                    //ensure click is on a tile
                    if(mousePosition.y <= rows * 32 - 0.5 && mousePosition.y > 0 && !paused && !gameLost && !gameWon){
                        Tile* currentTile = getTileClicked(game);

                        //do nothing if tile is revealed
                        if(currentTile->revealed){
                            continue;
                        }
                        //if tile is already a flag
                        if(currentTile->flag){
                            currentTile->flagSprite.setTexture(TextureManager::getTexture("tile_hidden"));
                            currentTile->flag = false;
                            flagCount--;
                            mineCounter++;
                            continue;
                        }
                        //if tile is not revealed or a flag, put flag on it
                        if(!currentTile->revealed && !currentTile->flag) {
                            currentTile->flag = true;
                            currentTile->flagSprite.setTexture(TextureManager::getTexture("flag"));
                            currentTile->flagSprite.setPosition(currentTile->position.x, currentTile->position.y);
                            flagCount++;
                            mineCounter--;
                        }
                    }

                }
            }
        }


        game.clear(sf::Color::White); // Set background color of the window

        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        total_time = game_duration.count();

        if(!paused && !gameLost && !gameWon && !leaderboardOn) { //adjust time if not paused or game over or leaderboard on
            total_time = total_time - elapsed_paused_time; //
            minutes = total_time / 60;
            seconds = total_time % 60;
        }

        int minutes0 = minutes / 10 % 10; //minutes index 0
        int minutes1 = minutes % 10; // minutes index 1
        int seconds0 = seconds / 10 % 10; // seconds index 0
        int seconds1 = seconds % 10; // seconds index 1

        if(gameWon){ //calculate win time
            intWinTotal = (((600*minutes0) + (60 * minutes1))) + ((seconds0 * 10) + seconds1);
            winTime = to_string(minutes0) + to_string(minutes1) + ':' + to_string(seconds0) + to_string(seconds1);
        }

        int f1 = abs(mineCounter / 100 % 10);
        int f2 = abs(mineCounter / 10 % 10);
        int f3 = abs(mineCounter % 10);

        //draw digits for timer
        digitsMap[minutes0].setPosition((numCol * 32) - 97, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[minutes0]);
        digitsMap[minutes1].setPosition((numCol * 32) - 76, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[minutes1]);
        digitsMap[seconds0].setPosition((numCol * 32) - 54, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[seconds0]);
        digitsMap[seconds1].setPosition((numCol * 32) - 33, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[seconds1]);

        //draw digits for mine counter
        digitsMap[f1].setPosition(33, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[f1]);
        digitsMap[f2].setPosition(33 + 21, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[f2]);
        digitsMap[f3].setPosition(33 + 42, 32 * (numRow + 0.5) + 16);
        game.draw(digitsMap[f3]);

        //draw negative if mine counter goes negative
        if(mineCounter < 0){
            digitsMap[-9].setPosition(12, 32 * (numRow + 0.5) + 16);
            game.draw(digitsMap[-9]);
        }

        drawBoard(game);
        game.draw(pause);
        if(leaderboardOn && !gameWon && !gameLost){
            game.draw(leaderBoardPause);
        }
        game.draw(happyFace);
        game.draw(debugButton);
        game.draw(leaderboardButton);
        drawInts(game);
        drawFlags(game);
        drawMines(game);
        drawDebugMines(game);
        drawPaused(game);
        game.display();
        if(leaderboardOn){
            displayLeaderBoard(game,userName,winTime,stringTimes, intRanks, timeTotals);
            if(!paused) {
                auto unPausedTime = chrono::steady_clock::now();
                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
            }
        }
    }
}


