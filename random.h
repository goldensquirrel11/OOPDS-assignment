#ifndef RANDOM_H
#define RANDOM_H

#include <random>
using namespace std;

class RNG
{
private:
    int boardWidth;
    int boardHeight;
    random_device randomGenerator;
public:
    void init(int boardWidth, int boardHeight);

    int posX();
    int posY();

    int integer(int min, int max);
};

inline void RNG::init(int boardWidth, int boardHeight)
{
    this->boardWidth = boardWidth;
    this->boardHeight = boardHeight;
}

inline int RNG::posX()
{
    uniform_int_distribution<int> randomX(0, boardWidth - 1);

    return randomX(randomGenerator);
}

inline int RNG::posY()
{
    uniform_int_distribution<int> randomX(0, boardHeight - 1);

    return randomX(randomGenerator);
}

inline int RNG::integer(int min, int max)
{
    uniform_int_distribution<int> randomX(min, max);

    return randomX(randomGenerator);
}

RNG rng;

#endif