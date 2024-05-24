#ifndef RANDOM_H
#define RANDOM_H

#include <random>
using namespace std;

class RNG
{
private:
    static int boardWidth;
    static int boardHeight;
    static random_device randomGenerator;

public:
    static void init(int boardWidth, int boardHeight);

    static int posX();
    static int posY();

    static int integer(int min, int max);
};

int RNG::boardWidth = 0;
int RNG::boardHeight = 0;
random_device RNG::randomGenerator;

void RNG::init(int width, int height)
{
    RNG::boardWidth = width;
    RNG::boardHeight = height;
}

int RNG::posX()
{
    uniform_int_distribution<int> randomX(0, boardWidth - 1);

    return randomX(randomGenerator);
}

int RNG::posY()
{
    uniform_int_distribution<int> randomX(0, boardHeight - 1);

    return randomX(randomGenerator);
}

int RNG::integer(int min, int max)
{
    uniform_int_distribution<int> randomX(min, max);

    return randomX(randomGenerator);
}

#endif