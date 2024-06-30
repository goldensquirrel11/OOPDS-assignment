/*
 *
 * This file contains the Game class that functions as the main controller
 * for the game's flow
 *
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <fstream>
#include <iostream>
#include <string>

#include "random.h"
#include "board.h"
#include "robot.h"

using namespace std;

class Game
{
private:
    /// @brief Current game turn
    int turn = 0;

    int turnLimit = 0;
    
    /// @brief The game board
    Board board;
    
    /// @brief Temporarily stores the interface as a string before
    /// outputting to the terminal
    string displayBuffer = "";

    /// @brief The file that is used to log all game output
    ofstream logFile;
    
    /// @brief The text file that stores a template of the interface
    ifstream interfaceTemplate;

    /// @brief Stores the game logs that will be displayed on every turn
    Log actionLog;

    void readConfigFile(ifstream &configFile);

    // Exception Classes
    
    /// @brief Exception that occurs when the log output file could not be opened
    class LogFileOpeningError
    {
    };

public:
    Game(ifstream &configFile);


    void nextTurn();
    void updateInterface();
    void revive();

    bool isValidState() const;
};

/// @brief Reads in values from the game's initial config file
/// @param configFile reference to the config file ifstream object
inline void Game::readConfigFile(ifstream &configFile)
{
    string input;

    // Read in board width & height
    getline(configFile, input);
    input = input.substr(9);
    board.setWidth(stoi(input.substr(0, input.find(' '))));
    board.setHeight(stoi(input.substr(input.find(' ') + 1)));

    // Initialize random number generator
    RNG::init(board.getWidth(), board.getHeight());

    // Read in turn limit
    getline(configFile, input);
    turnLimit = stoi(input.substr(7));

    // Read in number of robots
    int robotCount = 0;
    getline(configFile, input);
    robotCount = stoi(input.substr(8));

    string robotType;
    string robotName;
    int posX;
    int posY;

    // Read in each robot
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
        
        
        if (robotType == "RoboCop")
        {
            Robot::robotDeque.push_back(new RoboCop(robotName, posX, posY));
        }
        else if (robotType == "Terminator")
        {
            Robot::robotDeque.push_back(new Terminator(robotName, posX, posY));
        }
        else if (robotType == "TerminatorRoboCop")
        {
            Robot::robotDeque.push_back(new TerminatorRoboCop(robotName, posX, posY));
        }
        else if (robotType == "BlueThunder")
        {
            Robot::robotDeque.push_back(new BlueThunder(robotName, posX, posY));
        }
        else if (robotType == "Madbot")
        {
            Robot::robotDeque.push_back(new Madbot(robotName, posX, posY));
        }
        else if (robotType == "RoboTank")
        {
            Robot::robotDeque.push_back(new RoboTank(robotName, posX, posY));
        }
        else if (robotType == "UltimateRobot")
        {
            Robot::robotDeque.push_back(new UltimateRobot(robotName, posX, posY));
        }
        else
        {
            cout << "[ERROR] The robot type " << robotType << " could not be interpreted\n";
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
    
    interfaceTemplate.open("interface.template");
}

/// @brief Executes the next game turn
inline void Game::nextTurn()
{
    // Checking for any robots queued for a revive
    if (Robot::reviveDeque.size() > 0) {
        revive();
    }


    while (Robot::robotDeque.front()->getNextTurn() == turn)
    {
        Robot::robotDeque.front()->executeTurn();
        Robot::robotDeque.push_back(Robot::robotDeque.pop_front());
    }
    
    updateInterface();

    turn++;
}

/// @brief Updates the game display in the terminal
/// 
/// Reads interface.template and outputs game data to terminal following the template format.
/// This function will also log the output to the game.log file.
/// 
inline void Game::updateInterface()
{
    displayBuffer = "";

    board.refresh();

    string input;

    // Current Game Turn
    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;
    displayBuffer += to_string(turn);

    // Number of robots alive
    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;
    displayBuffer += to_string(Robot::robotDeque.size());

    // Number of robots waiting to revive
    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;
    displayBuffer += to_string(Robot::reviveDeque.size());
    
    // Robot actions taken this turn
    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;

    displayBuffer += actionLog.getLog();

    // Game board
    getline(interfaceTemplate, input, '\'');
    displayBuffer += input;
    displayBuffer += board.getBoard() + '\n';

    // Display to terminal
    cout << displayBuffer;

    // Record output in log file
    logFile << displayBuffer;

    // move the read position back to the beginning of the file
    interfaceTemplate.seekg(0, interfaceTemplate.beg);

    actionLog.resetLog();
}

/// @brief Revives the robot in front of the reviveDeque and places it in a
/// random position on the board
inline void Game::revive()
{
    Robot::reviveDeque.front()->setNextTurn(turn);

    int robotCount = Robot::robotDeque.size();
    
    
    // listing all invalid board positions in arrays for easy access
    int invalidXPositions[robotCount];
    int invalidYPositions[robotCount];

    for (int i=0; i < robotCount; i++) {
        invalidXPositions[i] = Robot::robotDeque[i]->getPositionX();
        invalidYPositions[i] = Robot::robotDeque[i]->getPositionY();
    }


    // Generating a valid revive position
    bool isValidRevivePosition = false;
    int reviveXPosition;
    int reviveYPosition;

    while (!isValidRevivePosition)
    {
        reviveXPosition = RNG::posX();
        reviveYPosition = RNG::posY();

        isValidRevivePosition = true;

        // checking if the generated position matches any of the invalid positions
        for (int i=0; i < robotCount; i++) {
            if (reviveXPosition == invalidXPositions[i] && reviveYPosition == invalidYPositions[i]) {
                isValidRevivePosition = false;
                break;
            }
        }
    }


    // setting the revive position
    Robot::reviveDeque.front()->updatePositionX(reviveXPosition);
    Robot::reviveDeque.front()->updatePositionY(reviveYPosition);

    Robot::robotDeque.push_back(Robot::reviveDeque.pop_front());

    Log::revive(Robot::robotDeque.back()->getName(), reviveXPosition, reviveYPosition);
}

/// @brief Determines whether the current game state is valid
/// @details The current game state is considered invalid when either:
///             1. the number of robots alive is 1 OR
///             2. when the turn limit has been reached.
///          In every other case, the game state is considered valid.
/// @return true if the game state is valid, false otherwise
inline bool Game::isValidState() const
{
    if (Robot::robotDeque.size() <= 1 || turn > turnLimit) {
        return false;
    }

    return true;
}

#endif