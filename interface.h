#ifndef INTERFACE_H
#define INTERFACE_H

#include <fstream>
#include <iostream>
#include <string>

#include "deque.h"
#include "robot.h"

using namespace std;

/// @brief
/// @param character
/// @param numberOfTimes
/// @return a string containing of repeated character the indicated number of times
string repeatChar(const char character, const int numberOfTimes)
{
    string temp = "";
    for (int i = numberOfTimes; i > 0; i--)
    {
        temp += character;
    }

    return temp;
}

class Board
{
private:
    int width = 0;
    int height = 0;
    string boardBuffer = "";

    // Characters used in the board
    char EMPTY_SPACE = ' ';
    char CORNER = '+';
    char HORIZONTAL_BAR = '-';
    char VERTICAL_BAR = '|';

public:
    // Accessors
    string getBoard();

    // Modifiers
    void setWidth(int width);
    void setHeight(int height);
    void refresh();
};

inline string Board::getBoard()
{
    return boardBuffer;
}

inline void Board::setWidth(int width)
{
    this->width = width;
}

inline void Board::setHeight(int height)
{
    this->height = height;
}

// TODO: Rewrite this function once a working Robot Deque is available
inline void Board::refresh(/* pass in robot Queue */)
{
    boardBuffer = "";

    boardBuffer += CORNER;
    boardBuffer += repeatChar(HORIZONTAL_BAR, width);
    boardBuffer += CORNER;
    boardBuffer += '\n';

    for (int i = height; i > 0; i--)
    {
        boardBuffer += VERTICAL_BAR;
        boardBuffer += repeatChar(EMPTY_SPACE, width);
        boardBuffer += VERTICAL_BAR;
        boardBuffer += '\n';
    }

    boardBuffer += CORNER;
    boardBuffer += repeatChar(HORIZONTAL_BAR, width);
    boardBuffer += CORNER;
    boardBuffer += '\n';
}

class Game
{
private:
    int turn = 0;
    Board board;
    int turnLimit = 0;
    string displayBuffer = "";
    Deque<Robot> robotDeque;

    void readConfigFile(ifstream &configFile);

public:
    Game(ifstream &configFile);
};

inline void Game::readConfigFile(ifstream &configFile)
{
    // TODO: Read in values from config file and call appropriate initializers
    string input;

    // Read in board width & height
    getline(configFile, input);
    input = input.substr(9);
    board.setWidth(stoi(input.substr(0, input.find(' '))));
    board.setHeight(stoi(input.substr(input.find(' ') + 1)));

    // Initialize RNG
    RNG::init(board.getWidth(), board.getHeight());

    // Read in turn limit
    getline(configFile, input);
    turnLimit = stoi(input.substr(7));

    // Read in number of robots
    int robotCount = 0;
    getline(configFile, input);
    robotCount = stoi(input.substr(8));

    // Read in each robot
    string robotType;
    string robotName;
    int posX;
    int posY;

    for (int i = robotCount; i > 0; i--)
    {
        getline(configFile, robotType, ' ');
        getline(configFile, robotName, ' ');

        getline(configFile, input, ' ');
        if (input == "random")
            posX = RNG::posX();
        else
            posX = stoi(input);

        getline(configFile, input, '\n');
        if (input == "random")
            posY = RNG::posY();
        else
            posY = stoi(input);

        // TODO: Complete this if-else-if block when all robot classes have been defined
        if (robotType == "RoboCop")
        {
        }
        else if (robotType == "Terminator")
        {
        }
        else if (robotType == "TerminatorRoboCop")
        {
        }
        else if (robotType == "BlueThunder")
        {
        }
        else if (robotType == "Madbot")
        {
        }
        else if (robotType == "RoboTank")
        {
        }
        else if (robotType == "UltimateRobot")
        {
        }
        else
        {
            // TODO: Throw error for undefined robot type
        }
    }
}

#endif