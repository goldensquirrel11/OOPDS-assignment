#ifndef INTERFACE_H
#define INTERFACE_H

#include <fstream>
#include <iostream>
#include <string>

#include "random.h"
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
    int getWidth();
    int getHeight();
    string getBoard();

    // Modifiers
    void setWidth(int width);
    void setHeight(int height);
    void refresh();
};

inline int Board::getWidth()
{
    return this->width;
}

inline int Board::getHeight()
{
    return this->height;
}

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
    static Deque<Robot*> robotDeque;
    static Deque<Robot*> reviveDeque;
    ofstream logFile;

    void readConfigFile(ifstream &configFile);

    // Exception Classes
    class LogFileOpeningError
    {
    };

public:
    Game(ifstream &configFile);

    void updateInterface();
};

Deque<Robot *> Game::robotDeque;
Deque<Robot *> Game::reviveDeque;

inline void Game::readConfigFile(ifstream &configFile)
{
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

inline Game::Game(ifstream &configFile)
{
    readConfigFile(configFile);

    // Open in default mode to clear log file
    logFile.open("game.log");

    if (!logFile.is_open())
    {
        throw LogFileOpeningError();
    }
    
    logFile.close();
    
    // Open log file in append mode
    logFile.open("game.log", ofstream::app);

    if (!logFile.is_open())
    {
        throw LogFileOpeningError();
    }

    updateInterface();
}

inline void Game::updateInterface()
{
    displayBuffer = "";

    board.refresh();

    ifstream interfaceTemplate;
    interfaceTemplate.open("interface.template");

    string input;

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    displayBuffer += to_string(turn);

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    displayBuffer += to_string(robotDeque.size());

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's name

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's type

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's kills to next evolution

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's looking range

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's moving range

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's can trample?

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's firing range

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    // TODO: Get current robot's status log

    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    displayBuffer += board.getBoard() + '\n';

    cout << displayBuffer;

    logFile << displayBuffer;
}

#endif