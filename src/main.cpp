#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Window.h"
#include "HelperFunctions.h"
#include "Board.h"
#include <vector>
#include <string>

 using namespace std;

 int main(){


    Window StartGame;
    Board board;

    vector<int> board_data;
    board_data = board.getBoardData("files/config.cfg"); //vector that contains num of  col,rows,mines

    board.numCol = board_data[0];
    board.numRow = board_data[1]; //sets board data
    board.numMines = board_data[2];
    board.mineCounter = board.numMines;


    int width = board.numCol * 32; //calculate width of windows
    int height = (board.numRow * 32); //calculate height of windows


    string name;
    name = StartGame.DisplayWelcome(width, height+100); //Displays Welcome Screen all functionalities

    if(name == "1"){ //User exited game
        return 0;
    }

    board.DisplayGame(width, height+100, name, board.numCol, board.numRow, board.numMines); //If user inputted a name, go on to the game


    return 0;
}
