#ifndef BOARD_H
#define BOARD_H

#include <string>

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
    string getBoard();
    bool isPositionValid(int posX, int PosY);

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

inline string Board::getBoard()
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

#endif