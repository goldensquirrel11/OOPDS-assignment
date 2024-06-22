#ifndef BOARD_H
#define BOARD_H

#include <string>

#include "robotbase.h"

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
    static int width;
    static int height;
    string boardBuffer = "";

    // Characters used in the board
    char EMPTY_SPACE = ' ';
    char CORNER = '+';
    char HORIZONTAL_BAR = '-';
    char VERTICAL_BAR = '|';

public:
    // Accessors
    static int getWidth();
    static int getHeight();
    string getBoard() const;

    // Modifiers
    void setWidth(int width);
    void setHeight(int height);
    void refresh();
};

int Board::width;
int Board::height;

int Board::getWidth()
{
    return Board::width;
}

int Board::getHeight()
{
    return Board::height;
}

inline string Board::getBoard() const
{
    return boardBuffer;
}

inline void Board::setWidth(int width)
{
    Board::width = width;
}

inline void Board::setHeight(int height)
{
    Board::height = height;
}

inline void Board::refresh(/* pass in robot Queue */)
{
    boardBuffer = "";

    boardBuffer += CORNER;
    boardBuffer += repeatChar(HORIZONTAL_BAR, width);
    boardBuffer += CORNER;
    boardBuffer += '\n';

    int robotCount = Robot::robotDeque.size();

    struct Info
    {
        char firstChar;
        int positionX;
        int positionY;
    };

    /// For the sake of speed, arrays are used here to quickly look up
    /// the first letter of each robot name and robot position

    /// @brief array of all robot's info
    Info robotInfo[robotCount];

    // Reading in values from robotDeque
    for (int i = 0; i < robotCount; i++)
    {
        robotInfo[i].firstChar = Robot::robotDeque[i]->getName()[0];
        robotInfo[i].positionX = Robot::robotDeque[i]->getPositionX();
        robotInfo[i].positionY = Robot::robotDeque[i]->getPositionY();
    }

    for (int y = 0; y < height; y++)
    {
        boardBuffer += VERTICAL_BAR;

        for (int x = 0; x < width; x++)
        {
            for (int k = 0; k < robotCount; k++)
            {
                if (robotInfo[k].positionX == x && robotInfo[k].positionY == y)
                {
                    boardBuffer += robotInfo[k].firstChar;
                    break;
                }
                else if (k == robotCount - 1)
                {
                    boardBuffer += EMPTY_SPACE;
                }
            }
        }

        boardBuffer += VERTICAL_BAR;
        boardBuffer += '\n';
    }

    boardBuffer += CORNER;
    boardBuffer += repeatChar(HORIZONTAL_BAR, width);
    boardBuffer += CORNER;
    boardBuffer += '\n';
}

#endif