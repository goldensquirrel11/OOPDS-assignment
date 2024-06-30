/*
 *
 * This file contains the RNG class that is responsible for generating all
 * random numbers that are used by this program.
 *
 * The code here contain functions that are able to generate random
 * X or Y positions on the board and also a general purpose integer
 * random number generator.
 * 
 * All class members are made static so that the random number generator
 * is accessible from any scope without needing to instantiate an object
 * of class RNG.
 *
 */

#ifndef RANDOM_H
#define RANDOM_H

#include <random>
using namespace std;

class RNG
{
private:
    static int boardWidth;
    static int boardHeight;

    /// @brief Non-deterministic random number generator
    static random_device randomGenerator;

public:
    /// @brief Initializes the board width & height for the posX & posY
    /// random number generators
    /// @param boardWidth 
    /// @param boardHeight 
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

/// @brief Generates a random X coordinate in the board
/// @returns int signifying a random X coordinate on the board
int RNG::posX()
{
    uniform_int_distribution<int> randomX(0, boardWidth - 1);

    return randomX(randomGenerator);
}

/// @brief Generates a random Y coordinate in the board
/// @returns int signifying a random Y coordinate on the board
int RNG::posY()
{
    uniform_int_distribution<int> randomX(0, boardHeight - 1);

    return randomX(randomGenerator);
}

/// @brief Generates a random integer value in the range [min, max]. min & max are inclusive
/// @param min minimum integer value to generate
/// @param max maximum integer value to generate
/// @return randomly generated int value
/// @example the function call integer(1,5) can generate any integer value from 1 to 5 including the values 1 and 5.
int RNG::integer(int min, int max)
{
    uniform_int_distribution<int> randomX(min, max);

    return randomX(randomGenerator);
}

#endif